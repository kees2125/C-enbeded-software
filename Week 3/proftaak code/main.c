/*! \mainpage SIR firmware documentation
 *
 *  \section intro Introduction
 *  A collection of HTML-files has been generated using the documentation in the sourcefiles to
 *  allow the developer to browse through the technical documentation of this project.
 *  \par
 *  \note these HTML files are automatically generated (using DoxyGen) and all modifications in the
 *  documentation should be done via the sourcefiles.
 */

/*! \file
 *  COPYRIGHT (C) STREAMIT BV 2010
 *  \date 19 december 2003
 */
 
 
 

#define LOG_MODULE  LOG_MAIN_MODULE

/*--------------------------------------------------------------------------*/
/*  Include files                                                           */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include <sys/thread.h>
#include <sys/timer.h>
#include <sys/version.h>
#include <dev/irqreg.h>

#include "system.h"
#include "portio.h"
#include "display.h"
#include "remcon.h"
#include "keyboard.h"
#include "led.h"
#include "log.h"
#include "uart0driver.h"
#include "mmc.h"
#include "watchdog.h"
#include "flash.h"
#include "spidrv.h"

#include <time.h>
#include "rtc.h"


/*-------------------------------------------------------------------------*/
/* global variable definitions                                             */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/* local variable definitions                                              */
/*-------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------*/
/* local routines (prototyping)                                            */
/*-------------------------------------------------------------------------*/
static void SysMainBeatInterrupt(void*);
static void SysControlMainBeat(u_char);
void showString(char*,int);

/*-------------------------------------------------------------------------*/
/* Stack check variables placed in .noinit section                         */
/*-------------------------------------------------------------------------*/

/*!
 * \addtogroup System
 */

/*@{*/


/*-------------------------------------------------------------------------*/
/*                         start of code                                   */
/*-------------------------------------------------------------------------*/


/* 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様� */
/*!
 * \brief ISR MainBeat Timer Interrupt (Timer 2 for Mega128, Timer 0 for Mega256).
 *
 * This routine is automatically called during system
 * initialization.
 *
 * resolution of this Timer ISR is 4,448 msecs
 *
 * \param *p not used (might be used to pass parms from the ISR)
 */
/* 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様� */
static void SysMainBeatInterrupt(void *p)
{

    /*
     *  scan for valid keys AND check if a MMCard is inserted or removed
     */
    KbScan();
    CardCheckCard();
}


/* 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様� */
/*!
 * \brief Initialise Digital IO
 *  init inputs to '0', outputs to '1' (DDRxn='0' or '1')
 *
 *  Pull-ups are enabled when the pin is set to input (DDRxn='0') and then a '1'
 *  is written to the pin (PORTxn='1')
 */
/* 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様� */
void SysInitIO(void)
{
    /*
     *  Port B:     VS1011, MMC CS/WP, SPI
     *  output:     all, except b3 (SPI Master In)
     *  input:      SPI Master In
     *  pull-up:    none
     */
    outp(0xF7, DDRB);

    /*
     *  Port C:     Address bus
     */

    /*
     *  Port D:     LCD_data, Keypad Col 2 & Col 3, SDA & SCL (TWI)
     *  output:     Keyboard colums 2 & 3
     *  input:      LCD_data, SDA, SCL (TWI)
     *  pull-up:    LCD_data, SDA & SCL
     */
    outp(0x0C, DDRD);
    outp((inp(PORTD) & 0x0C) | 0xF3, PORTD);

    /*
     *  Port E:     CS Flash, VS1011 (DREQ), RTL8019, LCD BL/Enable, IR, USB Rx/Tx
     *  output:     CS Flash, LCD BL/Enable, USB Tx
     *  input:      VS1011 (DREQ), RTL8019, IR
     *  pull-up:    USB Rx
     */
    outp(0x8E, DDRE);
    outp((inp(PORTE) & 0x8E) | 0x01, PORTE);

    /*
     *  Port F:     Keyboard_Rows, JTAG-connector, LED, LCD RS/RW, MCC-detect
     *  output:     LCD RS/RW, LED
     *  input:      Keyboard_Rows, MCC-detect
     *  pull-up:    Keyboard_Rows, MCC-detect
     *  note:       Key row 0 & 1 are shared with JTAG TCK/TMS. Cannot be used concurrent
     */
#ifndef USE_JTAG
    sbi(JTAG_REG, JTD); // disable JTAG interface to be able to use all key-rows
    sbi(JTAG_REG, JTD); // do it 2 times - according to requirements ATMEGA128 datasheet: see page 256
#endif //USE_JTAG

    outp(0x0E, DDRF);
    outp((inp(PORTF) & 0x0E) | 0xF1, PORTF);

    /*
     *  Port G:     Keyboard_cols, Bus_control
     *  output:     Keyboard_cols
     *  input:      Bus Control (internal control)
     *  pull-up:    none
     */
    outp(0x18, DDRG);
}

/* 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様� */
/*!
 * \brief Starts or stops the 4.44 msec mainbeat of the system
 * \param OnOff indicates if the mainbeat needs to start or to stop
 */
/* 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様� */
static void SysControlMainBeat(u_char OnOff)
{
    int nError = 0;

    if (OnOff==ON)
    {
        nError = NutRegisterIrqHandler(&OVERFLOW_SIGNAL, SysMainBeatInterrupt, NULL);
        if (nError == 0)
        {
            init_8_bit_timer();
        }
    }
    else
    {
        // disable overflow interrupt
        disable_8_bit_timer_ovfl_int();
    }
}

/* 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様� */
/*!
 * \brief Main entry of the SIR firmware
 *
 * All the initialisations before entering the for(;;) loop are done BEFORE
 * the first key is ever pressed. So when entering the Setup (POWER + VOLMIN) some
 * initialisatons need to be done again when leaving the Setup because new values
 * might be current now
 *
 * \return \b never returns
 */
/* 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様� */
int main(void)
{
	int t = 0;
	int x = 0;
	int timer=0;
	tm a;
	tm test = {30,30,10,1,1,95,1,1,1};
	int result=0;
	int boolTime=0;
	
	
	/* 
	 * Kroeske: time struct uit nut/os time.h (http://www.ethernut.de/api/time_8h-source.html)
	 *
	 */
	tm gmt;
	/*
	 * Kroeske: Ook kan 'struct _tm gmt' Zie bovenstaande link
	 */
	
    /*
     *  First disable the watchdog
     */
    WatchDogDisable();

    NutDelay(100);

    SysInitIO();
	
	SPIinit();
    
	LedInit();
	
	LcdLowLevelInit();

    Uart0DriverInit();
    Uart0DriverStart();
	LogInit();
	LogMsg_P(LOG_INFO, PSTR("Hello World"));

    CardInit();

	/*
	 * Kroeske: sources in rtc.c en rtc.h
	 */
    X12Init();
    if (X12RtcGetClock(&gmt) == 0)
    {
		LogMsg_P(LOG_INFO, PSTR("RTC time [%02d:%02d:%02d]"), gmt.tm_hour, gmt.tm_min, gmt.tm_sec );
    }


    if (At45dbInit()==AT45DB041B)
    {
        // ......
    }


    RcInit();
    
	KbInit();

    SysControlMainBeat(ON);             // enable 4.4 msecs hartbeat interrupt

    /*
     * Increase our priority so we can feed the watchdog.
     */
    NutThreadSetPriority(1);

	/* Enable global interrupts */
	sei();
	
    for (;;)
    {
		if(KbGetKey()==KEY_01)
		{
			X12RtcGetClock(&a);
			char hours[15];
			sprintf(hours, "%d", a.tm_hour);
			
			char minutes[15];
			sprintf(minutes, "%d",a.tm_min);
			char seconds[15];
			sprintf(seconds,"%d", a.tm_sec);
			char stringtest[80];
			strcpy(stringtest, hours);
			strcat(stringtest, ":");
			strcat(stringtest, minutes);
			strcat(stringtest, ":");
			strcat(stringtest, seconds);
			LogMsg_P(LOG_INFO, PSTR("TIME: [%d] [%d] [%d]"),a.tm_hour,a.tm_min,a.tm_sec);
			LogMsg_P(LOG_INFO, PSTR("TIME: [%d] [%d] [%d]"),test.tm_hour,test.tm_min,test.tm_sec);
			
			showString(stringtest,0);
			
		}
		if(KbGetKey()==KEY_02)
		{
			LcdBackLight(LCD_BACKLIGHT_ON);
			result = X12RtcSetClock(&test);
			//result = NutRtcSetTime(test);
			boolTime=1;
			char tresult[15];
			sprintf(tresult,"%d",result);
			//showString(tresult,0);
			LogMsg_P(LOG_INFO, PSTR("init: [%d]"),result);
			//result =NutRtcGetTime(&a);
			//LogMsg_P(LOG_INFO, PSTR("done: [%d]"),result);
			//result =X12RtcGetClock(&a);
			
			// char str[80];
			// strcpy(str, "these ");
			// strcat(str, "strings ");
			// strcat(str, "are ");
			// strcat(str, "concatenated.");
			
			
			//showString(":",0);
			//showString(minutes,0);
			//showString(":",0);
			//showString(seconds,0);
			
		}
		
		if(KbGetKey()==KEY_03)
		{	LcdBackLight(LCD_BACKLIGHT_ON);
			timer=1;
			LedControl(LED_TOGGLE);
		}
		
			
			
		if(KbGetKey() == KEY_04)
		{
			//LcdChar('O');
			//LcdChar('K');
			//setLowerLine();
			showString("Test String",0);
			showString("Test String",1);
			LedControl(LED_TOGGLE);
		}
		
		if(KbGetKey() == KEY_05)
		{
			
			
			
			
			
			
			
			
		}

		
        NutSleep(100);
		if( !((t++)%15) )
		{
			
			if(boolTime==1)
			{
				LedControl(LED_TOGGLE);
				X12RtcGetClock(&a);
				char hours[15];
				sprintf(hours, "%d", a.tm_hour);
			
				char minutes[15];
				sprintf(minutes, "%d",a.tm_min);
				char seconds[15];
				sprintf(seconds,"%d", a.tm_sec);
				char stringtest[80];
				strcpy(stringtest, hours);
				strcat(stringtest, ":");
				strcat(stringtest, minutes);
				strcat(stringtest, ":");
				strcat(stringtest, seconds);
				//LogMsg_P(LOG_INFO, PSTR("TIME: [%d] [%d] [%d]"),a.tm_hour,a.tm_min,a.tm_sec);
				LogMsg_P(LOG_INFO, PSTR("TIME: [%d] [%d] [%d]"),test.tm_hour,test.tm_min,test.tm_sec);
				
				showString(stringtest,0);
			}
			
			
			//LogMsg_P(LOG_INFO, PSTR("Yes!, I'm alive ... [%d]"),t);
			
			
			//LogMsg_P(LOG_INFO, PSTR("Yes!, I'm alive ... [%c]"),"ok");
			//LcdChar('9');	
			
				
			if(timer>0)
			{
				timer++;
			}
			
			if(timer==4)
			{
				timer=0;
				LcdBackLight(LCD_BACKLIGHT_ON);
			}
			
			//LcdChar(KbGetKey());
			
			//LedControl(LED_TOGGLE);
		
			if( x )
			{
				//LcdBackLight(LCD_BACKLIGHT_ON);
				//x = 0;
				
					
					
			}
			else
			{
				//LcdBackLight(LCD_BACKLIGHT_OFF);
				//x = 1;
				
			}
		}
		
        WatchDogRestart();
    }

    return(0);      // never reached, but 'main()' returns a non-void, so.....
}

void showString(char* string, int line)
{
	int i;
	
	if(line<2)
	setLine(line);
	clearDisplay();
	for(i=0; i < strlen(string);i++)
	{
		LcdChar(string[i]);
	}
	
}
/* ---------- end of module ------------------------------------------------ */

/*@}*/
