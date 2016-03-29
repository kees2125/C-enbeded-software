/* ========================================================================
* [PROJECT]    SIR100
* [MODULE]     FIRSTSETUP
* [TITLE]      firstsetup source file
* [FILE]       firstsetup.c
* [VSN]        1.0
* [CREATED]    18022016
* [LASTCHNGD]  18022016
* [COPYRIGHT]  Copyright (C) Avans BV
* [PURPOSE]    Contains all functions for initializing and handling first time
				use of the system.
* ======================================================================== */

#define LOG_MODULE  LOG_DISPLAY_MODULE

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

#include "rtc.h"
#include <dev/board.h>
#include <stdio.h>
#include <io.h>
#include "EEPROMHandler.h"

/*-------------------------------------------------------------------------*/
/* local defines                                                           */
/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
/* local variable definitions                                              */
/*-------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------*/
/* local routines (prototyping)                                            */
/*-------------------------------------------------------------------------*/

/*@{*/

/*-------------------------------------------------------------------------*/
/*                         start of code                                   */
/*-------------------------------------------------------------------------*/


int initFirstTime()
{
	return readEEPROM();
}

void drawUTC()
{
	clearScreen();
	LcdBackLight(LCD_BACKLIGHT_ON);
	char UTC1[60];
	sprintf(UTC1, "UTC= %d", IMCconfig.UTC);
	printString("Uw tijdzone?");
	setCursorPos(1, 0);
	printString(UTC1);
}

void changeUtcUp()
{
	if(IMCconfig.UTC < 12)
	{
		IMCconfig.UTC++;
	}
}

void changeUtcDown()
{
	if(IMCconfig.UTC>-12)
	{
		IMCconfig.UTC--;
	}
}




