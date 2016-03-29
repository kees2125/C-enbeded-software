#include "time.h"
#ifndef _DateTime_H
#define _DateTime_H
#include <time.h>
/*-------------------------------------------------------------------------*/
/* global defines                                                          */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/* typedefs & structs                                                      */
/*-------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*  Global variables                                                        */
/*--------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/* export global routines (interface)                                      */
/*-------------------------------------------------------------------------*/

void dateTimeInit(void);
void setDateTime(tm *td);
void getDateTime(tm *td);
//extern void setTimeZone(int modifier);
//extern int getTimeZone();
void getDateTimeSTR(void);
void drawDateTime(void);
void drawDateTimeComplete(void);
int isSynchronizedTimer;
#endif
