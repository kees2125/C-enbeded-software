	/*! \file
	*  COPYRIGHT (C) STREAMIT BV 2010
	*  \date 19 december 2003
	*/

	#define LOG_MODULE  LOG_MAIN_MODULE

	/*--------------------------------------------------------------------------*/
	/*  Include files                                                           */
	/*--------------------------------------------------------------------------*/
	#include <dev/board.h>
	#include <dev/debug.h>
	#include <dev/urom.h>

	#include <sys/version.h>
	#include <sys/heap.h>
	#include <sys/event.h>
	#include <sys/timer.h>
	#include <sys/thread.h>
	#include <sys/bankmem.h>

	#include <stdlib.h>
	#include <string.h>
	#include <stdio.h>
	#include <io.h>
	#include <fcntl.h>
	#include <errno.h>

	#include <sys/version.h>
	#include <dev/irqreg.h>
	#include <time.h>

	#include "lcd.h"
	#include "audioMenu.h"
	#include "vs10xx.h"
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
	#include "ntp.h"
	#include "firstsetup.h"
	#include "EEPROMHandler.h"
	#include "rtc.h"
	#include "menu.h"
	#include "dateTime.h"
	#include "StreamWork.h"


	#include <dev/debug.h>
	#ifdef ETHERNUT2
	#include <dev/lanc111.h>
	#else
	#include <dev/nicrtl.h>
	#endif
	#include <dev/vs1001k.h>

	#include <sys/version.h>
	#include <sys/confnet.h>
	#include <sys/heap.h>
	#include <sys/bankmem.h>
	#include <sys/thread.h>
	#include <sys/timer.h>

	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <net/route.h>
	#include <netinet/tcp.h>

	#include <pro/dhcp.h>

	/*-------------------------------------------------------------------------*/
	/* local variable definitions                                              */
	/*-------------------------------------------------------------------------*/
	FILE *fp;
	int alarmOn=0;
	int old = -10;
	int volume = 10;

	/*-------------------------------------------------------------------------*/
	/* local routines (prototyping)                                            */
	/*-------------------------------------------------------------------------*/
	static void SysMainBeatInterrupt(void*);
	static void SysControlMainBeat(u_char);
	static void buttonHandler(int keyvalue);

	/*-------------------------------------------------------------------------*/
	/* Stack check variables placed in .noinit section                         */
	/*-------------------------------------------------------------------------*/

	/*-------------------------------------------------------------------------*/
	/*                         start of code                                   */
	/*-------------------------------------------------------------------------*/


	/* ??????????????????????????????????????????????????????????????????????? */
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
	/* ??????????????????????????????????????????????????????????????????????? */
	static void SysMainBeatInterrupt(void *p)
	{
		/*
		*  scan for valid keys, key is pressed AND check if a MMCard is inserted or removed
		*/
		KbScan();
		CardCheckCard();
	}


	/* ??????????????????????????????????????????????????????????????????????? */
	/*!
	* \brief Initialise Digital IO
	*  init inputs to '0', outputs to '1' (DDRxn='0' or '1')
	*
	*  Pull-ups are enabled when the pin is set to input (DDRxn='0') and then a '1'
	*  is written to the pin (PORTxn='1')
	*/
	/* ??????????????????????????????????????????????????????????????????????? */
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

	/* ??????????????????????????????????????????????????????????????????????? */
	/*!
	* \brief Starts or stops the 4.44 msec mainbeat of the system
	* \param OnOff indicates if the mainbeat needs to start or to stop
	*/
	/* ??????????????????????????????????????????????????????????????????????? */
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

	THREAD(AlarmBeep,arg)
	{
		printf("beeping started");
		//while(alarmOn==1)
		for(;;)
		{
			NutSleep(500);
			//alarmBeep();
			VsBeepStart(100);
			NutSleep(500);
			VsBeepStop();
		}
		printf	("beep done");
		
		for(;;)
		{
			NutSleep(1000);
		}
	}

	THREAD(StreamMusic, arg)
	{
		printf("thread started");
		startStream();
		for (;;) {
			NutSleep(100);

		}
	}

	THREAD(ButtonHandlerThread, arg)
	{
		for (;;) {
			NutSleep(100);
			buttonHandler(KbGetKey());
		}
	}

	THREAD(AlarmCheckerThread, arg)
	{
		for (;;) {
			NutSleep(5000);
			if(CheckAlarm1())
			{
				printf("Alarm should go off\n");
				startStream();
				NutSleep(61000);
				NutThreadExit();
				printf("Exit thread and started new one...\n");
				NutThreadCreate("Bl", AlarmCheckerThread, NULL, 512);
				break;
			}else{
				printf("Alarm should not go off\n");
			}
			if(CheckAlarm2())
			{
				printf("Alarm should go off\n");
				if(IMCconfig.alarm2.mode ==0)
				{
					NutThreadCreate("Bq", AlarmBeep, NULL, 512);
				}
				else{
					startStream();
				}
				
				NutSleep(61000);
				NutThreadExit();
				printf("Exit thread and started new one...\n");
				NutThreadCreate("Bl", AlarmCheckerThread, NULL, 512);
				break;
			}else{
				printf("Alarm should not go off\n");
			}
		}
	}



	THREAD(TimeSyncThread, arg)
	{
		
		puts("\ntime started\n");
		RegisterDevice();
		puts("shit registered\n");
		for (;;) {
			RetrieveTime();
			PrintTime();
			NutSleep(10000);
		}
	}
	THREAD(RefreshSceenThread, arg)
	{
		for (;;) {
			NutSleep(1000);
			refresh();
			if(isSynchronizedTimer > 0){
				isSynchronizedTimer--;
			}
		}
	}
	
	THREAD(SleepMode, arg)
	{
		struct _tm current_Time;
		getDateTime(&current_Time);
		int temp = current_Time.tm_min;
		int volume = 0;
		setGVolume(volume);
		NutThreadCreate("fs", StreamMusic, NULL, 512);
		for (;;) {
			
			getDateTime(&current_Time);
			if (temp > 45)
			{
				if (current_Time.tm_min >= temp - 45 && current_Time.tm_min < 45)
				{
					stopStream();
					break;
				}
			}
			else
			{
				if (current_Time.tm_min >= (temp + 15))
				{
					stopStream();
					break;
				}
			}
			NutSleep(20000);
			volume+=1;
			setGVolume(volume);
			printf("test3 min:%d", current_Time.tm_min);


		}
	}

	void buttonHandler(int keyvalue)
	{
		if(old != keyvalue){
			//printf("%d",keyvalue);
			stateMenu(keyvalue);
			old = keyvalue;
			switch (keyvalue) {
			case KEY_01:
				NutThreadCreate("Bs", SleepMode, NULL, 512);
				break;
			case KEY_02:
				
				break;
			case KEY_03:
				alarmOn = 1;
				printf("DEBUG: Beep on\n");
				NutThreadCreate("Bq", AlarmBeep,NULL,512);
				break;
			case KEY_04:
				break;
			case KEY_05:
				break;
			case KEY_ALT:
				printf("Stop alarm");
				alarmOn=0;
				stopStream();
				break;
			case KEY_ESC:
				break;
			case KEY_UP:
				break;
			case KEY_OK:
				break;
			case KEY_LEFT:
				break;
			case KEY_DOWN:
				break;
			case KEY_RIGHT:
				break;
			case KEY_RESET:       // powerbutton +alt key
				printf("reset");
				resetEEPROM();
				reset();
				break;
			default:
				break;
			}
		}
	}

	/* ??????????????????????????????????????????????????????????????????????? */
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
	/* ??????????????????????????????????????????????????????????????????????? */

	int main(void) {
		//audioInit();
		//setVolume(254);
		WatchDogDisable();		//disable watchdog
		NutDelay(100);			//wait for it to disable
		SysInitIO();			//initialise input and output
		SPIinit();				//initialise SPI-registers (speed, mode)
		LedInit();				//initialise led
		initLCD();				//initialise lcd
		Uart0DriverInit();		//initialise Universal asynchronous receiver/transmitter  
		Uart0DriverStart();		//start Uart
		printf("\n\n\n\nHardware initialization done\n\n");
		LogInit();				//initialise ability to log
		CardInit();				//initialise cardreader
		printf("\n----------------------------------------------START OF PROGRAM------------------------------------------------------------\n");

		if (NutSegBufInit(8192) == 0) {
			puts("NutSegBufInit: Fatal error");
		}
		
		/*
		* Initialize the MP3 decoder hardware.
		*/
		if (VsPlayerInit() || VsPlayerReset(0)) {
			puts("VsPlayer: Fatal error");
		}
		
		if(X12Init() == -1){
			printf("error initializing RTC");	//initialise X12RTC 
		}				
		tm gmt;
		if (X12RtcGetClock(&gmt) == 0) {
			LogMsg_P(LOG_INFO, PSTR("local storage time [%02d:%02d:%02d] (in RTC)\n"), gmt.tm_hour, gmt.tm_min, gmt.tm_sec);
		}

		if (At45dbInit() == AT45DB041B) {
			// ......
		}

		RcInit();					//Initialise the Remote Control module
		KbInit();					//Initialise keyboard
		initMenu();					//Initialise menu
		NutThreadCreate("Bh", ButtonHandlerThread, NULL, 512);	//create thread to handle button presses
		printf("\nInitialization done.\n");
		initAlarms();
		SysControlMainBeat(ON); 	//enable 4.4 msecs heartbeat interrupt
		printf("Heartbeat on.\n");
		NutThreadSetPriority(1);	//Increase our priority so we can feed the watchdog.
		sei();						//enable global interrupts
		NutThreadCreate("Bg", TimeSyncThread, NULL, 512);
		NutThreadCreate("Bq", RefreshSceenThread, NULL, 512);
		NutThreadCreate("Bl", AlarmCheckerThread, NULL, 512);
		printf("\nEntering main loop.\n");
		LcdBackLight(LCD_BACKLIGHT_ON);
		for(;;){
			NutSleep(100);
			WatchDogRestart();			//restart watchdog
		}
		
		return(0);
	}
