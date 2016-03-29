#include "dateTime.h"
#include "time.h"
#include <dev/board.h>
#include <dev/debug.h>
#include <dev/nicrtl.h>
#include <sys/confnet.h>
#include <sys/timer.h>
#include <arpa/inet.h>
#include <net/route.h>
#include <pro/dhcp.h>
#include <pro/sntp.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <time.h>
#include "rtc.h"
#include "EEPROMHandler.h"
#include "ntp.h"
#include "lcd.h"
int min_Old;
int hour_Old;
char str_time[40];
char str_date[40];
void dateTimeInit()
{
	initNTP();
	isSynchronizedTimer = 0;
	//synchronize local to internet time
}

void setDateTime(struct _tm *td)
{
	X12RtcSetClock(td);
}

void getDateTime(struct _tm *td)
{
	X12RtcGetClock(td);
}

// void setTimeZone(int modifier)
// {

// }

// int getTimeZone()
// {

// }

void drawDateTime()
{
	struct _tm current_time;
	char temp[2];
	
	getDateTime(&current_time);
	if(isSynchronizedTimer>0)
	{
		setCursorPos(0, 15);
		printCustomCharacter(3);
	}
	else
	{
		setCursorPos(0, 15);
		printString(" ");
	}
	if (hour_Old != current_time.tm_hour)
	{
		hour_Old = current_time.tm_hour;
		min_Old = current_time.tm_min;
		sprintf(str_time, "    %02d:%02d:%02d", current_time.tm_hour, current_time.tm_min, current_time.tm_sec, isSynchronizedTimer);
		sprintf(str_date, "   %02d-%02d-%04d", current_time.tm_mday, current_time.tm_mon + 1, current_time.tm_year + 1900);
		setCursorPos(0, 0);
		printString(str_time);
		setCursorPos(1, 0);
		printString(str_date);
	}
	else if (min_Old != current_time.tm_min)
	{
		min_Old = current_time.tm_min;
		sprintf(temp, "%02d", current_time.tm_min);
		setCursorPos(0, 7);
		printString(temp);
		sprintf(temp, "%02d", current_time.tm_sec);
		setCursorPos(0, 10);
		printString(temp);

	}
	else
	{
		sprintf(temp, "%02d", current_time.tm_sec);
		setCursorPos(0, 10);
		printString(temp);
	}
}
void drawDateTimeComplete()
{
	struct _tm current_time;
	char temp[2];

	getDateTime(&current_time);
	if (isSynchronizedTimer>0)
	{
		setCursorPos(0, 15);
		printCustomCharacter(3);
	}
	else
	{
		setCursorPos(0, 15);
		printString(" ");
	}
	hour_Old = current_time.tm_hour;
	min_Old = current_time.tm_min;
	sprintf(str_time, "    %02d:%02d:%02d", current_time.tm_hour, current_time.tm_min, current_time.tm_sec, isSynchronizedTimer);
	sprintf(str_date, "   %02d-%02d-%04d", current_time.tm_mday, current_time.tm_mon + 1, current_time.tm_year + 1900);
	setCursorPos(0, 0);
	printString(str_time);
	setCursorPos(1, 0);
	printString(str_date);

}
