#ifndef _DEV_IRQREG_ARM_H_
#define _DEV_IRQREG_ARM_H_

/*
 * Copyright (C) 2001-2004 by egnite Software GmbH. All rights reserved.
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
 * $Log: irqreg_arm.h,v $
 * Revision 1.3  2005/04/05 17:52:40  haraldkipp
 * Much better implementation of GBA interrupt registration.
 *
 * Revision 1.2  2004/09/08 10:52:31  haraldkipp
 * Tyou's support for the SAMSUNG S3C45
 *
 * Revision 1.1  2004/03/16 16:48:28  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 *
 */
#ifdef S3C4510B
#include "s3c4510b_irqreg.h"

#elif defined(MCU_GBA)

extern IRQ_HANDLER sig_VBLANK;
extern IRQ_HANDLER sig_HBLANK;
extern IRQ_HANDLER sig_VCOUNT;
extern IRQ_HANDLER sig_TMR0;
extern IRQ_HANDLER sig_TMR1;
extern IRQ_HANDLER sig_TMR2;
extern IRQ_HANDLER sig_TMR3;
extern IRQ_HANDLER sig_SIO;
extern IRQ_HANDLER sig_DMA0;
extern IRQ_HANDLER sig_DMA1;
extern IRQ_HANDLER sig_DMA2;
extern IRQ_HANDLER sig_DMA3;
extern IRQ_HANDLER sig_KEYPAD;
extern IRQ_HANDLER sig_GAMEPAK;

extern void InitIrqHandler(void);

#elif defined(MCU_AT91R40008)

#else
#warning "No MCU defined"
#endif

#endif
