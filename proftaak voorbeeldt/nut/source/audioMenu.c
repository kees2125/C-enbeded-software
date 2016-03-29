/*
*  Copyright STREAMIT BV, 2010.
*
*  Project             : SIR
*  Module              : Util
*  File name  $Workfile: Util.c  $
*       Last Save $Date: 2006/05/11 9:53:22  $
*             $Revision: 0.1  $
*  Creation Date       : 2006/05/11 9:53:22
*
*  Description         : Utility functions for the SIR project
*
*/


/*--------------------------------------------------------------------------*/
/*  Include files                                                           */
/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include "lcd.h"
#include "audioMenu.h"
#include "StreamWork.h"


#define OK			1
#define NOK			0

void volumeMenu(int key);
void bassMenu(int key);
void leftRightMenu(int key);
void audioMenuState(int key);

int mVolume = 0;
int menuState = 0;
int bassOn = 0;
int leftSelect = 0;
int update = 0;
/*--------------------------------------------------------------------------*/
/*  Constant definitions                                                    */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*  Type declarations                                                       */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*  Local variables                                                         */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*  Global variables                                                        */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*  Local functions                                                         */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*  Global functions                                                        */
/*--------------------------------------------------------------------------*/
void initAudioMenu()
{
	audioMenu = audioMenuState;
}
void audioMenuState(int key)
{
	update = 1;
	
	
	switch (key)
	{
	case KEY_OK:
		switch (menuState)
		{
		case 0:
			audioMenu = volumeMenu;
			volumeMenu(1000);
			break;
		case 1:
			audioMenu = bassMenu;
			bassMenu(1000);
			break;
		case 2:
			audioMenu = leftRightMenu;
			leftRightMenu(1000);
			break;
		default:
			break;
		}
		break;
	case KEY_LEFT:
		if (menuState > 0)
		{
			menuState--;
		}
		break;
	case KEY_RIGHT:
		if (menuState < 2)
		{
			menuState++;
		}
		break;
	default:
		update = 0;
		break;
	}
	if (update == 1 || key == 1000)
	{
		clearScreen();
		switch (menuState)
		{
		case 0:
			printString("Volume");
			break;
		case 1:
			printString("Bass/Tremmor");
			break;
		case 2:
			printString("L/R Balance");
			break;
		default:
			break;
		}
		setCursorPos(0, 14);
		printCustomCharacter(0);
		printCustomCharacter(1);
	}
}
void volumeMenu(int key)
{
	update = 1;
	int i;
	switch (key)
	{
	case KEY_OK:
		audioMenu = audioMenuState;
		audioMenuState(1000);
		break;
	case KEY_LEFT:
		if (mVolume < 250)
		{
			mVolume += 25;
			setGVolume(mVolume);
		}
		break;
	case KEY_RIGHT:
		if (mVolume > 0)
		{
			mVolume -= 25;
			setGVolume(mVolume);
		}
		break;
	default:
		update = 0;
		break;
	}
	if (update == 1|| key == 1000)
	{
		clearScreen();
		setCursorPos(0, 0);
		printString("Volume:");
		setCursorPos(1, 0);
		for (i = mVolume; i < 250; i += 25)
		{
			printCustomCharacter(2);
		}
		setCursorPos(0, 14);
		printCustomCharacter(0);
		printCustomCharacter(1);
	}
}
void bassMenu(int key)
{
	update = 1;
	switch (key)
	{
	case KEY_OK:
		audioMenu = audioMenuState;
		audioMenuState(1000);
		break;
	case KEY_LEFT:
		bassOn = 0;
		setBassEnhancer(bassOn);
		break;
	case KEY_RIGHT:
		bassOn = 1;
		setBassEnhancer(bassOn);
		break;
	default:
		update = 0;
		break;
	}
	if (update || key == 1000)
	{
		clearScreen();
		if (bassOn == 1)
		{
			printString("Bass/Tr: ON");
		}
		else
		{
			printString("Bass/Tr: OFF");
		}
	}
}
void leftRightMenu(int key)
{
	int l;
	int r;
	char temp[40];
	update = 1;
	getLeftRightBalance(&l, &r);
	switch (key)
	{
	case KEY_OK:
		audioMenu = audioMenuState;
		audioMenuState(1000);
		break;
	case KEY_LEFT:
		leftSelect = 1;
		break;
	case KEY_RIGHT:
		leftSelect = 0;
		break;
	case KEY_UP:
		if (leftSelect == 1)
		{
			if (l<256)
			l++;
		}
		else
		{
			if (r<256)
			r++;
		}
		setLeftRightBalance(l, r);
		break;
	case KEY_DOWN:
		if (leftSelect == 1)
		{
			if (l > 1)
			{
				l--;
			}
		}
		else
		{
			if (r > 1)
			{
				r--;
			}
		}
		setLeftRightBalance(l, r);
		break;
	default:
		update = 0;
		break;
	}
	if (update == 1 || key == 1000)
	{
		clearScreen();
		setCursorPos(0, 0);
		printf("%d%d", l, r);
		sprintf(temp, "Left:%d Right:%d", (unsigned char)l, (unsigned char)r);
		printf("%s", temp);
		if (leftSelect == 1)
		{
			printString("L</R");
			setCursorPos(1, 0);
			printString(temp);
		}
		else
		{
			printString("L/R<");
			setCursorPos(1, 0);
			printString(temp);
		}
	}

}
int exitReady()
{
	if (audioMenu == audioMenuState)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}