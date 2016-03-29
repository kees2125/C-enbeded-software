#include <stdio.h>
#include <string.h>

#include "menu.h"
#include "firstsetup.h"
#include "EEPROMHandler.h"
#include "dateTime.h"
#include "keyboard.h"
#include "alarmMenu.h"
#include "lcd.h"
#include "audioMenu.h"

/*-------------------------------------------------------------------------*/
/* local variable definitions                                              */
/*-------------------------------------------------------------------------*/

enum { alarmeringEnum, settingsEnum } scrollMenuIndex;

/*-------------------------------------------------------------------------*/
/* local routines (prototyping)                                            */
/*-------------------------------------------------------------------------*/
void getMenuItem(void);
void goIntoScrollMenu(void);
void scrollMenuLeft(void);
void scrollMenuRight(void);


//setup menu
void setupMenu(int input);

//klok
void klokMenu(int input);

//alarm is afgegaan
void alarmGaatAfMenu(int input);
void snoozeMenu(int input);

//scrollmenu
void scrollMenu(int input);
void alarmeringMenu(int input);
void settingsMenu(int input);
void audioMenuItem(int input);
int updateM = 0;
void reset(void);
/*-------------------------------------------------------------------------*/
/*                         start of code                                   */
/*-------------------------------------------------------------------------*/

void initMenu(void)
{
	int isClear;
	scrollMenuIndex = alarmeringEnum;

	printf("checking memory for timezone\n");
	isClear = readEEPROM();
	if(isClear == 0)
	{
		printf("Timezone found\n");
		stateMenu = klokMenu;
		dateTimeInit();
	}else
	{
		printf("No Timezone found on EEPROM\n");
		stateMenu = setupMenu;
	}
	clearScreen();
	getMenuItem();
	initAudioMenu();
}

void getMenuItem()
{
	if(stateMenu == setupMenu)
	{
		drawUTC();
	}
	else if(stateMenu == klokMenu)
	{
		drawDateTime();
	}
	else if(stateMenu == scrollMenu)
	{
		if(scrollMenuIndex == alarmeringEnum)
		{
			printString("Alarmering");
		}
		else if(scrollMenuIndex == settingsEnum)
		{
			printString("Settings");
		}
	}
	else if(stateMenu == alarmeringMenu){
		//printString("You are in Alarmering");
		drawAlarm();
	}
	else if(stateMenu == settingsMenu){
		printString("Audio Settings");
	}
}

void setupMenu(int input)
{
	updateM = 1;
	switch(input)
	{
	case KEY_OK:
		saveEEPROM();
		dateTimeInit();
		stateMenu = klokMenu;
		break;
	case KEY_UP:
		changeUtcUp();
		break;
	case KEY_DOWN:
		changeUtcDown();
		break;
	default:
		updateM = 0;
		break;
	}
	if (updateM == 1)
	{
		clearScreen();
		getMenuItem();
	}
};

//klok
void klokMenu(int input)
{
	updateM = 1;
	switch(input)
	{
	case KEY_OK:
		stateMenu = scrollMenu;
		clearScreen();
		break;
	case KEY_UP:
		changeUtcUp();
		break;
	case KEY_DOWN:
		changeUtcDown();
		break;
	default:
		updateM = 0;
		break;
	}
	if (updateM == 1)
	{
		getMenuItem();
	}
};

//alarm is afgegaan
void alarmGaatAfMenu(int input){}
void snoozeMenu(int input){}

//menu
void scrollMenu(int input)
{
	updateM = 1;
	switch(input)
	{
	case KEY_OK:
		goIntoScrollMenu();
		break;
	case KEY_LEFT:
		scrollMenuLeft();
		break;
	case KEY_RIGHT:
		scrollMenuRight();
		break;
	case KEY_ESC:
		stateMenu = klokMenu;
		drawDateTimeComplete();
		updateM = 0;
		break;
	default:
		updateM = 0;
		break;
	}
	if (updateM == 1)
	{
		clearScreen();
		getMenuItem();
	}
}

void goIntoScrollMenu()
{
	
	switch(scrollMenuIndex)
	{
		case alarmeringEnum:
			stateMenu = alarmeringMenu;
		break;
		case settingsEnum:
			stateMenu = settingsMenu;
		break;
	}
}

void scrollMenuRight()
{

	if (scrollMenuIndex < 1)
	{
		scrollMenuIndex++;
	}
	else
	{
		scrollMenuIndex--;
	}
	//scrollMenuIndex++;
	//if(scrollMenuIndex > 1){ scrollMenuIndex = 0;}
}

void scrollMenuLeft()
{
	if (scrollMenuIndex > 0)
	{
		scrollMenuIndex--;
	}
	else
	{
		scrollMenuIndex++;
	}
	//scrollMenuIndex--;
	//if(scrollMenuIndex < 0){ scrollMenuIndex = 1;}
}

void alarmeringMenu(int input)
{
	alarmInput(input);
	switch(input)
	{
		case KEY_ESC:
			updateM = 1;
			stateMenu = scrollMenu;
		break;
		default:
			updateM = 0;
		break;
	}
	
	clearScreen();
	getMenuItem();
	
}
void settingsMenu(int input)
{
	updateM = 1;
	switch(input)
	{
	case KEY_OK:
		stateMenu = audioMenuItem;
		audioMenu(1000); //calles for a first refresh
		updateM = 0;
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case KEY_ESC:
		stateMenu = scrollMenu;
		break;
	default:
		updateM = 0;
		break;
	}
	if (updateM == 1)
	{
		clearScreen();
		getMenuItem();
	}
}
void audioMenuItem(int input)
{
	audioMenu(input);
	if (input == KEY_ESC && exitReady() == 1)
	{
		stateMenu = settingsMenu;
		stateMenu(KEY_RIGHT);
	}
}

void refresh()
{
	if(stateMenu == klokMenu){
		getMenuItem();
	}
}
void reset()
{
	stateMenu = setupMenu;
}
