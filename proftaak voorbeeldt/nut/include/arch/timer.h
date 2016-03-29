#ifndef _ARCH_TIMER_H_
#define _ARCH_TIMER_H_

/*
 * <MFS> Modified for Streamit
 * Added timer modifications from Harald Kipp, 2008/07/29 + new NutDelay implementation
 *
 * Copyright (C) 2001-2005 by egnite Software GmbH. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY EGNITE SOFTWARE GMBH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EGNITE
 * SOFTWARE GMBH OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 */

/*
 * $Log: timer.h,v $
 * Revision 1.2  2008/07/08 08:25:05  haraldkipp
 * NutDelay is no more architecture specific.
 * Number of loops per millisecond is configurable or will be automatically
 * determined.
 * A new function NutMicroDelay provides shorter delays.
 *
 * Revision 1.1  2005/07/26 18:42:19  haraldkipp
 * First check in
 *
 */

#if defined(__AVR__)
#include <arch/avr/timer.h>
#elif defined(__arm__)
#include <arch/arm/timer.h>
#elif defined(__H8300H__) || defined(__H8300S__)
#include <arch/h8300h/timer.h>
#elif defined(__m68k__)
#include <arch/m68k/timer.h>
#elif defined(__linux__) || defined(__APPLE__) || defined(__CYGWIN__)
#include <arch/unix/timer.h>
#endif

__BEGIN_DECLS
/* Prototypes */

/*
 * Architecture dependent functions.
 */
extern void NutRegisterTimer(void (*handler) (void *));
extern u_long NutGetCpuClock(void);
extern u_long NutGetTickClock(void);
extern u_long NutTimerMillisToTicks(u_long ms);

__END_DECLS
/* End of prototypes */

#endif
