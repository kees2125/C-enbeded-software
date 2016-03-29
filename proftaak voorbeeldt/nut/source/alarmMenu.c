#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/timer.h>
#include <sys/event.h>
#include <sys/thread.h>
#include <sys/heap.h>

#include "system.h"
#include "portio.h"
#include "display.h"
#include "log.h"
#include "firstsetup.h"
#include "led.h"
#include "lcd.h"
#include <keyboard.h>


#include "rtc.h"
#include <dev/board.h>
#include <stdio.h>
#include <io.h>
#include "EEPROMHandler.h"
#include <time.h>
#include "dateTime.h"
#include "alarm.h"

char* getDay(int);

int steplayer=0;
int xlayer=0;
int ylayer=0;
int selectedAlarm=0;
int selectedDay=0;
int selectedHour=0;
int selectedMin=0;


char* getDay(int value)
{
	switch(value)
	{
		case 0:
			return "ZO";
		case 1:
			return "MA";
		case 2:
			return "DI";
		case 3:
			return "WO";
		case 4:
			return "DO";
		case 5:
			return "VR";
		case 6:
			return "ZA";
		default:
			return "ERR";
	}
}


void drawAlarm(void)
{
	
	switch(steplayer)
	{
		case 0:
			
			setCursorPos(0,0);
			if(IMCconfig.alarm1.isSet==0)
			{
				printString("1. LEEG ");
			}
			else
			{
				printString("1.");
				printString(getDay(IMCconfig.alarm1.dateTime.tm_wday));
				char time[50];
				sprintf(time," %d:%d ",IMCconfig.alarm1.dateTime.tm_hour,IMCconfig.alarm1.dateTime.tm_min );
				printString(time);
				
				if(IMCconfig.alarm1.status==1)
				{
					printString("AAN ");
				}
				else
				{
					printString("UIT ");
				}
			}
			if(ylayer==0)
			{
				//printString("*");
				printCustomCharacter(0);
			}
			setCursorPos(1,0);		
			
			if(IMCconfig.alarm2.isSet==0)
			{
				printString("2. LEEG ");
			}
			else
			{
				printString("2.");
				printString(getDay(IMCconfig.alarm2.dateTime.tm_wday));
				char time2[50];
				sprintf(time2," %02d:%02d ",IMCconfig.alarm2.dateTime.tm_hour,IMCconfig.alarm2.dateTime.tm_min );
				printString(time2);
				
				if(IMCconfig.alarm2.status==1)
				{
					printString("AAN ");
				}
				else
				{
					printString("UIT ");
				}
			}
			
			if(ylayer==1)
			{
				//printString("*");
				printCustomCharacter(0);
			}
			break;
		case 1:
		if(selectedAlarm==1)
		{
			setCursorPos(0,0);
			printString("Dag: ");
			setCursorPos(1,0);
			printString(getDay(selectedDay));
			
			if(ylayer==0)
			{
				printString(" *");
			}
			
			
		}
		break;
		
		case 2:
			setCursorPos(0,0);
			printString("Uur: ");
			setCursorPos(1,0);
			char hour[50];
			sprintf(hour,"%d",selectedHour);
			printString(hour);
			
			if(ylayer==0)
			{
				printString(" *");
			}
		break;
		
		case 3:
			setCursorPos(0,0);
			printString("Min: ");
			setCursorPos(1,0);
			char min[50];
			sprintf(min,"%d",selectedMin);
			printString(min);
			
			if(ylayer==0)
			{
				printString(" *");
			}
			break;
		case 4:
			setCursorPos(0,0);
			printString("Beeping ");
			if(ylayer==0)
			{
				printCustomCharacter(0);
			}
			setCursorPos(1,0);
			printString("Stream");
			if(ylayer==1)
			{
				printCustomCharacter(0);
			}
			
			
		
	}
	
}



void alarmInput(int input)
{
	tm localTime;
	switch(input)
	{
		case KEY_OK:
			switch(steplayer)
			{
				case 0:
				if(ylayer!=0)
				{
					selectedAlarm=ylayer;
				}
				
				break;
				case 1:
				ylayer=0;
				break;
				case 3:
				ylayer=0;
				break;
				case 4:
				//PUT IT HERE TIM! :)
				//EEPROM AND RTC UPDATE

					localTime.tm_min = selectedMin;
					localTime.tm_hour = selectedHour;
					localTime.tm_wday = selectedDay;
					if(ylayer==0)
					{
						setAlarm(&IMCconfig.alarm2, localTime, on, beep, "OK", 50);
						printf("beep\n");
					}
					else{
						setAlarm(&IMCconfig.alarm2, localTime, on, stream, "OK", 50);
						printf("stream\n");
					}
					
					setVariableAlarm();

				selectedDay=0;
				selectedHour=0;
				selectedMin=0;
				xlayer=0;
				ylayer=0;
				steplayer=-1;
				break;
				
			}
			if(steplayer < 6 && selectedAlarm > 0)
			steplayer++;
			break;
		
		
		case KEY_LEFT:
			switch(steplayer)
			{
				case 0:
				if(ylayer!=0)
				{
					IMCconfig.alarm2.status=0;
				}
				case 1:
					if(selectedDay>0)
					{
						selectedDay--;
					}
				break;
				case 2:
					if(selectedHour>0)
					selectedHour--;
				break;
				case 3:
					if(selectedMin>0)
					selectedMin--;
				break;
			}
			break;
		case KEY_RIGHT:
			switch(steplayer)
			{
				case 0:
				if(ylayer!=0)
				{
					IMCconfig.alarm2.status=1;
				}
				break;
				case 1:
					if(selectedDay<6)
					{
						selectedDay++;
					}
				break;
				case 2:
					if(selectedHour<24)
					selectedHour++;
				break;
				case 3:
					if(selectedMin<60)
					selectedMin++;
				break;
				
			}
			
			break;
		case KEY_UP:
			if(steplayer== 0 || steplayer==4)
			ylayer=0;
		
			break;
		case KEY_DOWN:
			if(steplayer==0|| steplayer==4)
			ylayer=1;
		
			break;
		default:
			break;
	}
}


