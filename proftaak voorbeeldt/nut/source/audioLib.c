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
#include <stdio.h>
#include <string.h>

#include <sys/heap.h>

//#pragma text:appcode

#include "vs10xx.h"
#include "system.h"
#include "log.h"

#include "audioLib.h"

#define OK			1
#define NOK			0

THREAD(StreamPlayer, arg);
int soundVolume = -1;
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
void audioInit()
{
	VsPlayerInit();
}
void setAudioVolume(int vol)
{
	VsSetVolume(vol, vol);
	soundVolume = vol;
}

int getVolume()
{
	return soundVolume;
}
void playTone(int tone, int time)
{
	VsBeepStart(tone);
	NutSleep(10);
}
void playStream(FILE *stream)
{
	NutThreadCreate("Bg", StreamPlayer, stream, 512);
	printf("Play thread created. Device is playing stream now !\n");
}

THREAD(StreamPlayer, arg)
{
	FILE *stream = (FILE *)arg;
	size_t rbytes = 0;
	char *mp3buf;
	int result = NOK;
	int nrBytesRead = 0;
	unsigned char iflag;

	//
	// Init MP3 buffer. NutSegBuf is een globale, systeem buffer
	//
	if (0 != NutSegBufInit(8192))
	{
		// Reset global buffer
		iflag = VsPlayerInterrupts(0);
		NutSegBufReset();
		VsPlayerInterrupts(iflag);

		result = OK;
	}

	// Init the Vs1003b hardware
	if (OK == result)
	{
		if (-1 == VsPlayerInit())
		{
			if (-1 == VsPlayerReset(0))
			{
				result = NOK;
			}
		}
	}

	for (;;)
	{
		/*
		* Query number of byte available in MP3 buffer.
		*/
		iflag = VsPlayerInterrupts(0);
		mp3buf = NutSegBufWriteRequest(&rbytes);
		VsPlayerInterrupts(iflag);

		// Bij de eerste keer: als player niet draait maak player wakker (kickit)
		if (VS_STATUS_RUNNING != VsGetStatus())
		{
			if (rbytes < 1024)
			{
				printf("VsPlayerKick()\n");
				VsPlayerKick();
			}
		}

		while (rbytes)
		{
			// Copy rbytes (van 1 byte) van stream naar mp3buf.
			nrBytesRead = fread(mp3buf, 1, rbytes, stream);

			if (nrBytesRead > 0)
			{
				iflag = VsPlayerInterrupts(0);
				mp3buf = NutSegBufWriteCommit(nrBytesRead);
				VsPlayerInterrupts(iflag);
				if (nrBytesRead < rbytes && nrBytesRead < 512)
				{
					NutSleep(250);
				}
			}
			else
			{
				break;
			}
			rbytes -= nrBytesRead;

			if (nrBytesRead <= 0)
			{
				break;
			}
		}
	}
}

/*static int PlayMp3File(char *path)
{
	int fd;
	size_t rbytes;
	u_char *mp3buf;
	int got;
	u_char ief;

	
	// Open the MP3 file.
	
	printf("Play %s: ", path);
	if ((fd = _open(path, _O_RDONLY | _O_BINARY)) == -1) {
		printf("Error %d\n", errno);
		return -1;
	}
	puts("OK");

	
	//Reset decoder buffer.
	
	printf("[B.RST]");
	ief = VsPlayerInterrupts(0);
	NutSegBufReset();
	VsPlayerInterrupts(ief);

	for (;;) {
		
		 //Query number of byte available in MP3 buffer.
		
		ief = VsPlayerInterrupts(0);
		mp3buf = NutSegBufWriteRequest(&rbytes);
		VsPlayerInterrupts(ief);

		
		// Read data directly into the MP3 buffer.
		
		if (rbytes) {
			printf("[B.RD%d]", rbytes);
			if ((got = _read(fd, mp3buf, rbytes)) > 0) {
				printf("[B.CMT%d]", got);
				ief = VsPlayerInterrupts(0);
				mp3buf = NutSegBufWriteCommit(got);
				VsPlayerInterrupts(ief);
			}
			else {
				printf("[EOF]");
				break;
			}
		}

		
		 //If the player is not running, kick it.
		
		if (VsGetStatus() != VS_STATUS_RUNNING) {
			printf("[P.KICK]");
			VsPlayerKick();
		}

		//Allow background threads to take over.
		
		NutThreadYield();
	}

	_close(fd);

	
 //Flush decoder and wait until finished.
	
	printf("[P.FLUSH]");
	//VsPlayerFlush();
	while (VsGetStatus() != VS_STATUS_EMPTY) {
		NutSleep(1);
	}

	
 //Reset the decoder.
	
	printf("[P.RST]");
	VsPlayerReset(0);

	printf("\nDone, %u bytes free\n", NutHeapAvailable());
	return 0;
}*/
void pauze()
{
	VsBeepStop();
}
void play()
{

}
