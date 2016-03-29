#ifndef _menu_H
#define _menu_H
/*
*  Copyright STREAMIT BV, 2010.
*
*  Project             : IMCAPP-Ti2.3
*  Module              : menu
*  File name 			 $Workfile: menu.h  $
*  Last Save 			 $Date: 2016/02/25 12:15:00  $
*            			 $Revision: 0.1  $
*  Creation Date       : 2016/02/25 12::15:00
*
*  Description         : Menu for radio
*
*/
/*--------------------------------------------------------------------------*/
/*  Include files                                                           */
/*--------------------------------------------------------------------------*/
#include "time.h"	
/*--------------------------------------------------------------------------*/
/*  Constant definitions                                                    */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*  Type declarations                                                       */
/*--------------------------------------------------------------------------*/
typedef enum 
{ 
	testmenu,
	menu 
}MENU;

typedef enum 
{ 
	toon,
	muziek
}MODE;

typedef enum 
{
	aan,
	uit,
	snooze,
	actief
}STATUS;

typedef enum 
{ 
	Maandag,
	Dinsdag,
	Woensdag,
	Donderdag,
	Vrijdag,
	Zaterdag,
	Zondag 
}DAGEN;

typedef struct 
{
	int AlarmID;
	DAGEN dag;
	struct _tm tijd;
	STATUS status;
	MODE mode;
}ALARM;

/*--------------------------------------------------------------------------*/
/*  Global variables                                                        */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*  Global functions                                                        */
/*--------------------------------------------------------------------------*/
//initfunction
void initMenu(void);

/**function pointer*/
void (*stateMenu)(int);

void refresh(void);

void reset();

#endif
