#ifndef _DEV_CY2239X_H_
#define	_DEV_CY2239X_H_

/*
 * Copyright (C) 2005 by egnite Software GmbH. All rights reserved.
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
 * $Log: cy2239x.h,v $
 * Revision 1.2  2006/01/05 16:53:26  haraldkipp
 * Several new functions added to query and modify the clock settings.
 *
 * Revision 1.1  2005/10/24 11:41:39  haraldkipp
 * Initial check-in.
 *
 */

/*!
 * \addtogroup xgCy2239x
 */
/*@{*/

#include <sys/types.h>

/*! \brief Clock A output.
 *
 * On Ethernut 3 this is output drives the Ethernet Controller,
 * if R3 is mounted (default).
 */
#define CY2239X_CLKA    0

/*! \brief Clock B output.
 *
 * On Ethernut 3 this is output drives the GCK1 input of the CPLD,
 * if R6 is mounted (default).
 */
#define CY2239X_CLKB    1

/*! \brief Clock C output.
 *
 * On Ethernut 3 this is output drives the CPU, if R5 is mounted
 * (default).
 */
#define CY2239X_CLKC    2

/*! \brief Clock D output.
 *
 * On Ethernut 3 this is output drives the GCK3 input of the CPLD,
 * if R4 is mounted and R104 is not mounted (default).
 */
#define CY2239X_CLKD    3

/*! \brief Clock E output.
 *
 * On Ethernut 3 this is output drives the GCK3 input of the CPLD,
 * if R104 is mounted and R4 is not mounted. By default R104 is not 
 * mounted, but R4 is.
 */
#define CY2239X_CLKE    4


/*! \brief Reference clock identifier. */
#define CY2239X_REF     0

/*! \brief PLL1 identifier.
 *
 * The output of PLL1 can be connected to any output divider.
 * Clock E output is fixed to PLL1.
 */
#define CY2239X_PLL1    1

/*! \brief PLL2 identifier.
 *
 * The output of PLL2 can be connected to the output divider
 * of clock A, B, C or D.
 */
#define CY2239X_PLL2    2

/*! \brief PLL3 identifier.
 *
 * The output of PLL3 can be connected to the output divider
 * of clock A, B, C or D.
 */
#define CY2239X_PLL3    3

/*@}*/

__BEGIN_DECLS
/* Prototypes */

extern u_long Cy2239xGetFreq(int clk, int fctrl);

extern int Cy2239xGetPll(int clk);
extern int Cy2239xSetPll(int clk, int pll);

extern int Cy2239xGetDivider(int clk, int fctrl);
extern int Cy2239xSetDivider(int clk, int sel, int val);

extern int Cy2239xPllEnable(int pll, int fctrl, int ena);

extern u_long Cy2239xPllGetFreq(int pll, int fctrl);
extern int Cy2239xPllSetFreq(int pll, int fctrl, u_int pval, u_int poff, u_int qval, u_int fval);

__END_DECLS
/* End of prototypes */
#endif
