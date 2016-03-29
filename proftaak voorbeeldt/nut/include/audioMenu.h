/* ========================================================================
* [PROJECT]    SIR100
* [MODULE]     Lcd
* [TITLE]      lcd header file
* [FILE]       lcd.h
* [VSN]        1.0
* [CREATED]    18022016
* [LASTCHNGD]  18022016
* [COPYRIGHT]  Copyright (C) Avans BV 2010
* [PURPOSE]    API and gobal defines for display module
* ======================================================================== */

#ifndef _AudioMenu_H
#define _AudioMenu_H
#define KEY_01         1
#define KEY_02         2
#define KEY_03         3
#define KEY_04         4
#define KEY_05         5
#define KEY_ALT        6

#define KEY_ESC        7
#define KEY_UP         8
#define KEY_OK         9
#define KEY_LEFT       10
#define KEY_DOWN       11
#define KEY_RIGHT      12

#define KEY_POWER      13
#define KEY_SETUP      14
#define KEY_LCD        15       // virtual key, generated when '1' is pressed and hold for > 2 secs

void(*audioMenu)(int);
extern void initAudioMenu(void);
extern int exitReady(void);




#endif /* _Lcd_H */
/*  ����  End Of File  �������� �������������������������������������������� */



