#define LOG_MODULE  LOG_DISPLAY_MODULE

#include <dev/board.h>
#include <dev/debug.h>
#include <dev/nicrtl.h>
#include <sys/confnet.h>
#include <sys/timer.h>
#include <sys/thread.h>

#include <arpa/inet.h>
#include <net/route.h>
#include <pro/dhcp.h>
#include <pro/sntp.h>

#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <time.h>
#include "EEPROMHandler.h"
#include "dateTime.h"
#include "ntp.h"
#include <rtc.h>

/*-------------------------------------------------------------------------*/
/* local variable definitions                                              */


/*-------------------------------------------------------------------------*/
/* local routines (prototyping)                                            */
/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
/*                         start of code                                   */
/*-------------------------------------------------------------------------*/
unsigned long baud = 115200;
time_t ntp_time = 0;
tm *ntp_datetime;
uint32_t timeserver = 0;


void initNTP()
{	
	printf("\ninitialising internet time in different thread\n");
	_timezone = -IMCconfig.UTC * 60 * 60;
}


void RegisterDevice()
{
	printf("Registering ethernet port. \n");
	if (NutRegisterDevice(&DEV_ETHER, 0, 0)) {
		printf("Registering  failed.");
	}
	else if (NutDhcpIfConfig(DEV_ETHER_NAME, NULL, 0))
	{
		printf("Connected");
	}
	printf("registering donge");
	printf("Receive time from: pool.ntp.org");
	timeserver = inet_addr("213.154.229.24");
}

void RetrieveTime()
{
	tm test;
	for (;;) {
		if (NutSNTPGetTime(&timeserver, &ntp_time) == 0) {
			break;
		} else {
			NutSleep(200);
			puts("Time Error");
		}
	}
	ntp_datetime = localtime(&ntp_time);
	tm RTCdate={ntp_datetime->tm_sec,  ntp_datetime->tm_min,  ntp_datetime->tm_hour, ntp_datetime->tm_mday,  ntp_datetime->tm_mon,  ntp_datetime->tm_year,1,1,1};

	setDateTime(&RTCdate);
	isSynchronizedTimer = 60;
	getDateTime(&test);

	printf(" RTC time check = %d : %d : %d \n ",  test.tm_hour, test.tm_min, test.tm_sec);
}

void PrintTime()
{
	//Don't know what is more efficient, next line in this func or in retrievetime...
	ntp_datetime = localtime(&ntp_time);
	printf("received time from server\n");
	printf("NTP time is: %02d:%02d:%02d\n", ntp_datetime->tm_hour, ntp_datetime->tm_min, ntp_datetime->tm_sec);
	printf("NTP date is: %02d.%02d.%04d\n", ntp_datetime->tm_mday, (ntp_datetime->tm_mon + 1), (ntp_datetime->tm_year + 1900));
}
