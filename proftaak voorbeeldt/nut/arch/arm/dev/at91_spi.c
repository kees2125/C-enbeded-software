/*
 * Copyright (C) 2006 by egnite Software GmbH. All rights reserved.
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
 *
 */

/*
 * $Log: at91_spi.c,v $
 * Revision 1.2  2006/10/08 16:48:07  haraldkipp
 * Documentation fixed
 *
 * Revision 1.1  2006/09/29 12:34:59  haraldkipp
 * Basic AT91 SPI support added.
 *
 */
#include <stdio.h>

#include <cfg/arch.h>
#include <dev/board.h>
#include <dev/irqreg.h>

#include <sys/event.h>
#include <sys/timer.h>

#include <dev/at91_spi.h>

static HANDLE spi0_que;
static HANDLE spi1_que;

/*!
 * \brief SPI0 interrupt handler.
 *
 * Called when PDC transmission done.
 */
static void At91Spi0Interrupt(void *arg)
{
    NutEventPostFromIrq(&spi0_que);
}

/*!
 * \brief Initialize the first serial peripheral interface on the AT91 MCU.
 */
int At91Spi0Init(void)
{
    /* Enable SPI peripherals. */
    At91Spi0Enable();
    /* Enable SPI clock. */
    outr(PMC_PCER, _BV(SPI0_ID));

    /* Register and enable SPI0 interrupt handler. */
    NutRegisterIrqHandler(&sig_SPI0, At91Spi0Interrupt, 0);
    NutIrqEnable(&sig_SPI0);

    return At91SpiReset(SPI0_BASE);
}

/*!
 * \brief Initialize specified SPI0 chip selects on the AT91 MCU.
 *
 * \param mask Chip selects to use. Set bit 0 for NPCS0, bit 1
 *             for NPCS1 and so on.
 *
 * \return 0 on success or -1 if any of the specified chip selects is 
 *         not available.
 */
int At91Spi0InitChipSelects(u_int mask)
{
    if (mask & _BV(0)) {
#if defined(SPI0_CS0_PIN)
        outr(SPI0_CS0_PIO_BASE + SPI0_CS0_PSR_OFF, SPI0_CS0_PIN);
        outr(SPI0_CS0_PIO_BASE + PIO_PDR_OFF, SPI0_CS0_PIN);
        mask &= ~_BV(0);
#endif                          /* SPI0_CS0_PIN */
    }
    if (mask & _BV(1)) {
#if defined(SPI0_CS1_PIN)
        outr(SPI0_CS1_PIO_BASE + SPI0_CS1_PSR_OFF, SPI0_CS1_PIN);
        outr(SPI0_CS1_PIO_BASE + PIO_PDR_OFF, SPI0_CS1_PIN);
        mask &= ~_BV(1);
#endif                          /* SPI0_CS1_PIN */
    }
    if (mask & _BV(2)) {
#if defined(SPI0_CS2_PIN)
        outr(SPI0_CS2_PIO_BASE + SPI0_CS2_PSR_OFF, SPI0_CS2_PIN);
        outr(SPI0_CS2_PIO_BASE + PIO_PDR_OFF, SPI0_CS2_PIN);
        mask &= ~_BV(2);
#endif                          /* SPI0_CS2_PIN */
    }
    if (mask & _BV(3)) {
#if defined(SPI0_CS3_PIN)
        outr(SPI0_CS3_PIO_BASE + SPI0_CS3_PSR_OFF, SPI0_CS3_PIN);
        outr(SPI0_CS3_PIO_BASE + PIO_PDR_OFF, SPI0_CS3_PIN);
        mask &= ~_BV(3);
#endif                          /* SPI0_CS3_PIN */
    }
    return mask ? -1 : 0;
}

/*!
 * \brief Initialize the second serial peripheral interface on the AT91 MCU.
 */
int At91Spi0Enable(void)
{
    /* Enable SPI peripherals. */
    outr(SPI0_PIO_BASE + SPI0_PSR_OFF, SPI0_PINS);
    outr(SPI0_PIO_BASE + PIO_PDR_OFF, SPI0_PINS);

    return 0;
}

#if defined(SPI1_BASE)

/*!
 * \brief SPI1 interrupt handler.
 *
 * Called when PDC transmission done.
 */
static void At91Spi1Interrupt(void *arg)
{
    NutEventPostFromIrq(&spi1_que);
}

/*!
 * \brief Initialize the second serial peripheral interface on the AT91 MCU.
 */
int At91Spi1Init(void)
{
    /* Enable SPI peripherals. */
    At91Spi1Enable();
    /* Enable SPI clock. */
    outr(PMC_PCER, _BV(SPI1_ID));

    /* Register and enable SPI1 interrupt handler. */
    NutRegisterIrqHandler(&sig_SPI1, At91Spi1Interrupt, 0);
    NutIrqEnable(&sig_SPI1);

    return At91SpiReset(SPI1_BASE);
}

/*!
 * \brief Initialize specified SPI1 chip selects on the AT91 MCU.
 *
 * \param mask Chip selects to use. Set bit 0 for NPCS0, bit 1
 *             for NPCS1 and so on.
 *
 * \return 0 on success or -1 if any of the specified chip selects is 
 *         not available.
 */
int At91Spi1InitChipSelects(u_int mask)
{
#if defined(SPI1_CS0_PIN)
    if (mask & _BV(0)) {
        outr(SPI1_CS0_PIO_BASE + SPI1_CS0_PSR_OFF, SPI1_CS0_PIN);
        outr(SPI1_CS0_PIO_BASE + PIO_PDR_OFF, SPI1_CS0_PIN);
        mask &= ~_BV(0);
    }
#endif                          /* SPI1_CS0_PIN */
#if defined(SPI1_CS1_PIN)
    if (mask & _BV(1)) {
        outr(SPI1_CS1_PIO_BASE + SPI1_CS1_PSR_OFF, SPI1_CS1_PIN);
        outr(SPI1_CS1_PIO_BASE + PIO_PDR_OFF, SPI1_CS1_PIN);
        mask &= ~_BV(1);
    }
#endif                          /* SPI1_CS1_PIN */
#if defined(SPI1_CS2_PIN)
    if (mask & _BV(2)) {
        outr(SPI1_CS2_PIO_BASE + SPI1_CS2_PSR_OFF, SPI1_CS2_PIN);
        outr(SPI1_CS2_PIO_BASE + PIO_PDR_OFF, SPI1_CS2_PIN);
        mask &= ~_BV(2);
    }
#endif                          /* SPI1_CS2_PIN */
#if defined(SPI1_CS3_PIN)
    if (mask & _BV(3)) {
        outr(SPI1_CS3_PIO_BASE + SPI1_CS3_PSR_OFF, SPI1_CS3_PIN);
        outr(SPI1_CS3_PIO_BASE + PIO_PDR_OFF, SPI1_CS3_PIN);
        mask &= ~_BV(3);
    }
#endif                          /* SPI1_CS3_PIN */
    return mask ? -1 : 0;
}

/*!
 * \brief Initialize the second serial peripheral interface on the AT91 MCU.
 */
int At91Spi1Enable(void)
{
    /* Enable SPI peripherals. */
    outr(SPI1_PIO_BASE + SPI1_PSR_OFF, SPI1_PINS);
    outr(SPI1_PIO_BASE + PIO_PDR_OFF, SPI1_PINS);

    return 0;
}

#endif                          /* SPI1_BASE */

/*!
 * \brief Initialize serial peripheral interface on the AT91 MCU.
 *
 * \param base    Interface base address, either SPI_BASE, SPI0_BASE 
 *                or SPI1_BASE.
 *
 * \return 0 on success or -1 if SPI is not available.
 */
int At91SpiInit(u_int base)
{
    int rc = -1;

    /* 
     * Enable PIO lines and clock. 
     */
    if (base == SPI0_BASE) {
        rc = At91Spi0Init();
    }
#if defined(SPI1_BASE)
    if (base == SPI1_BASE) {
        rc = At91Spi1Init();
    }
#endif
    return rc;
}

int At91SpiEnable(u_int base)
{
    outr(base + SPI_CR_OFF, SPI_SPIEN);

    return 0;
}

int At91SpiDisable(u_int base)
{
    outr(base + SPI_CR_OFF, SPI_SPIDIS);

    return 0;
}

/*!
 * \brief Reset serial peripheral interface on the AT91 MCU.
 *
 * \param base    Interface base address, either SPI_BASE, SPI0_BASE 
 *                or SPI1_BASE.
 *
 * \return 0 on success or -1 if SPI is not available.
 */
int At91SpiReset(u_int base)
{
    int rc = -1;

    /* Disable SPI. */
    At91SpiDisable(base);

    /* Reset SPI. */
    outr(base + SPI_CR_OFF, SPI_SWRST);

    /* Set SPI to master mode, fixed peripheral at no chip select, fault detection disabled. */
    outr(base + SPI_MR_OFF, (90 < SPI_DLYBCS_LSB) | SPI_PCS | SPI_MODFDIS | SPI_MSTR);

    /* Enable SPI. */
    At91SpiEnable(base);

    return rc;
}

/*!
 * \brief Initialize specified SPI chip selects on the AT91 MCU.
 *
 * \param base Interface base address, either SPI_BASE, SPI0_BASE 
 *             or SPI1_BASE.
 * \param mask Chip selects to use. Set bit 0 for NPCS0, bit 1
 *             for NPCS1 and so on.
 *
 * \return 0 on success or -1 if any of the specified chip selects is 
 *         not available.
 */
int At91SpiInitChipSelects(u_int base, u_int mask)
{
    int rc = -1;

    /* Init chip select lines for SPI 0. */
    if (base == SPI0_BASE) {
        rc = At91Spi0InitChipSelects(mask);
    }
    /* Init chip select lines for SPI 1. */
#if defined(SPI1_BASE)
    if (base == SPI1_BASE) {
        rc = At91Spi1InitChipSelects(mask);
    }
#endif
    return rc;
}

/*!
 * \brief Configure the SPI rate.
 *
 * \param base SPI register base.
 * \param cs   Chip select line.
 * \param rate Baudrate. The maximum is MCK/1 and the minimum is MCK/255.
 *             If the specified rate is above the maximum or below the
 *             minimum, the maximum or minimum value resp. will be set.
 */
int At91SpiSetRate(u_int base, u_int cs, u_long rate)
{
    int rc = 0;
    u_int divider;

    /* The SPI clock is driven by the master clock. */
    divider = (u_int) At91GetMasterClock();
    /* Calculate the SPI clock divider. Avoid rounding errors. */
    divider += (u_int) (rate / 2);
    divider /= rate;
    /* A divider value of 0 is not allowed. */
    if (divider < 1) {
        divider = 1;
    }
    /* The divider value maximum is 255. */
    else if (divider > 255) {
        divider = 255;
    }
    switch (cs) {
    case 0:
        outr(base + SPI_CSR0_OFF, (inr(base + SPI_CSR0_OFF) & ~SPI_SCBR) | (divider << SPI_SCBR_LSB));
        break;
    case 1:
        outr(base + SPI_CSR1_OFF, (inr(base + SPI_CSR1_OFF) & ~SPI_SCBR) | (divider << SPI_SCBR_LSB));
        break;
    case 2:
        outr(base + SPI_CSR2_OFF, (inr(base + SPI_CSR2_OFF) & ~SPI_SCBR) | (divider << SPI_SCBR_LSB));
        break;
    case 3:
        outr(base + SPI_CSR3_OFF, (inr(base + SPI_CSR3_OFF) & ~SPI_SCBR) | (divider << SPI_SCBR_LSB));
        break;
    default:
        rc = -1;
        break;
    }
    return 0;
}

u_long At91SpiGetModeFlags(u_int base, u_int cs)
{
    u_long rc = SPIMF_MFDETECT;
    u_int mv = inr(base + SPI_MR_OFF);

    if (mv & SPI_MSTR) {
        rc |= SPI_MSTR;
    }
    if (mv & SPI_PCSDEC) {
        rc |= SPIMF_MASTER;
    }
    if (mv & SPI_MODFDIS) {
        rc &= ~SPIMF_MFDETECT;
    }
    if (mv & SPI_LLB) {
        rc |= SPIMF_LOOPBACK;
    }

    mv = inr(base + SPI_CSR0_OFF + cs * 4);
    if (mv & SPI_CPOL) {
        if (mv & SPI_NCPHA) {
            rc |= SPIMF_SCKIAHI;
        } else {
            rc |= SPIMF_SCKIAHI | SPIMF_CAPRISE;
        }
    } else if (mv & SPI_NCPHA) {
        rc |= SPIMF_CAPRISE;
    }
    return rc;
}

/*!
 * \brief Configure the SPI operation mode.
 *
 * \param base SPI register base.
 * \param cs   Chip select line.
 * \param mode Any of the following
 *              - SPIMF_MASTER   Master mode.
 *              - SPIMF_PCSDEC   Decoded chip selects.
 *              - SPIMF_MFDETECT Mode fault detection.
 *              - SPIMF_LOOPBACK Loopback mode.
 *              - SPIMF_SCKIAHI  Clock is high when inactive.
 *              - SPIMF_CAPRISE  Data cpatured on rising edge.
 *              - SPIMF_KEEPCS   Chip select remains active after transfer.
 */
int At91SpiSetModeFlags(u_int base, u_int cs, u_long mode)
{
    u_int mv;

    mv = inr(base + SPI_MR_OFF) & ~(SPI_MSTR | SPI_PCSDEC | SPI_MODFDIS | SPI_LLB);
    if (mode & SPIMF_MASTER) {
        mv |= SPI_MSTR;
    }
    if (mode & SPIMF_PCSDEC) {
        mv |= SPI_PCSDEC;
    }
    if (mode & SPIMF_MFDETECT) {
        mv &= ~SPI_MODFDIS;
    }
    if (mode & SPIMF_LOOPBACK) {
        mv |= SPI_LLB;
    }
    outr(base + SPI_MR_OFF, mv);

    mv = inr(base + SPI_CSR0_OFF + cs * 4) & ~(SPI_CPOL | SPI_NCPHA | SPI_CSAAT);
    if (mode & SPIMF_SCKIAHI) {
        if (mode & SPIMF_CAPRISE) {
            mv |= SPI_CPOL;
        } else {
            mv |= SPI_CPOL | SPI_NCPHA;
        }
    } else {
        if (mode & SPIMF_CAPRISE) {
            mv |= SPI_NCPHA;
        }
    }
    if (mode & SPIMF_KEEPCS) {
        mv |= SPI_CSAAT;
    }
    outr(base + SPI_CSR0_OFF + cs * 4, mv);

    if (At91SpiGetModeFlags(base, cs) != mode) {
        return -1;
    }
    return 0;
}

u_int At91SpiGetBits(u_int base, u_int cs)
{
    u_int rc;

    switch (inr(base + SPI_CSR0_OFF + cs * 4) & SPI_BITS) {
    case SPI_BITS_9:
        rc = 9;
        break;
    case SPI_BITS_10:
        rc = 10;
        break;
    case SPI_BITS_11:
        rc = 11;
        break;
    case SPI_BITS_12:
        rc = 12;
        break;
    case SPI_BITS_13:
        rc = 13;
        break;
    case SPI_BITS_14:
        rc = 14;
        break;
    case SPI_BITS_15:
        rc = 15;
        break;
    case SPI_BITS_16:
        rc = 16;
        break;
    default:
        rc = 8;
        break;
    }
    return rc;
}

int At91SpiSetBits(u_int base, u_int cs, u_int bits)
{
    u_int mv;

    mv = inr(base + SPI_CSR0_OFF + cs * 4) & ~SPI_BITS;
    switch (bits) {
    case 9:
        mv |= SPI_BITS_9;
        break;
    case 10:
        mv |= SPI_BITS_10;
        break;
    case 11:
        mv |= SPI_BITS_11;
        break;
    case 12:
        mv |= SPI_BITS_12;
        break;
    case 13:
        mv |= SPI_BITS_13;
        break;
    case 14:
        mv |= SPI_BITS_14;
        break;
    case 15:
        mv |= SPI_BITS_15;
        break;
    case 16:
        mv |= SPI_BITS_16;
        break;
    default:
        mv |= SPI_BITS_8;
        break;
    }
    outr(base + SPI_CSR0_OFF + cs * 4, mv);

    if (At91SpiGetBits(base, cs) != bits) {
        return -1;
    }
    return 0;
}

u_int At91SpiGetSckDelay(u_int base, u_int cs)
{
    return (inr(base + SPI_CSR0_OFF + cs * 4) & ~SPI_DLYBS) >> SPI_DLYBS_LSB;
}

int At91SpiSetSckDelay(u_int base, u_int cs, u_int dly)
{
    u_int csr = base + SPI_CSR0_OFF + cs * 4;

    outr(csr, (inr(csr) & ~SPI_DLYBS) | ((dly << SPI_DLYBS_LSB) & SPI_DLYBS));

    if (At91SpiGetSckDelay(base, cs) != dly) {
        return -1;
    }
    return 0;
}

u_int At91SpiGetTxDelay(u_int base, u_int cs)
{
    return (inr(base + SPI_CSR0_OFF + cs * 4) & ~SPI_DLYBCT) >> SPI_DLYBCT_LSB;
}

int At91SpiSetTxDelay(u_int base, u_int cs, u_int dly)
{
    u_int csr = base + SPI_CSR0_OFF + cs * 4;

    outr(csr, (inr(csr) & ~SPI_DLYBCT) | ((dly << SPI_DLYBCT_LSB) & SPI_DLYBCT));

    if (At91SpiGetTxDelay(base, cs) != dly) {
        return -1;
    }
    return 0;
}

u_int At91SpiGetCsDelay(u_int base)
{
    return (inr(base + SPI_MR_OFF) & ~SPI_DLYBCS) >> SPI_DLYBCS_LSB;
}

int At91SpiSetCsDelay(u_int base, u_int dly)
{
    outr(base + SPI_MR_OFF, (inr(base + SPI_MR_OFF) & ~SPI_DLYBCS) | ((dly << SPI_DLYBCS_LSB) & SPI_DLYBCS));

    if (At91SpiGetCsDelay(base) != dly) {
        return -1;
    }
    return 0;
}

int At91SpiTransfer2(u_int base, u_int cs, CONST void *txbuf, void *rxbuf, int xlen, CONST void *txnbuf, void *rxnbuf, int xnlen)
{
    int rc = -1;
    u_int flags;
    u_int sr;

    outr(base + PERIPH_PTCR_OFF, PDC_TXTDIS | PDC_RXTDIS);

    flags = inr(base + SPI_MR_OFF) & ~SPI_PCS;
    switch (cs) {
    case 0:
        flags |= SPI_PCS_0;
        break;
    case 1:
        flags |= SPI_PCS_1;
        break;
    case 2:
        flags |= SPI_PCS_2;
        break;
    case 3:
        flags |= SPI_PCS_3;
        break;
    }
    outr(base + SPI_MR_OFF, flags);

    /* Set transmit pointer and counter. */
    outr(base + PERIPH_TPR_OFF, (u_int) txbuf);
    outr(base + PERIPH_TCR_OFF, (u_int) xlen);
    /* Set receive pointer and counter. */
    outr(base + PERIPH_RPR_OFF, (u_int) rxbuf);
    outr(base + PERIPH_RCR_OFF, (u_int) xlen);

    /* Set transmit pointer and counter. */
    outr(base + PERIPH_TNPR_OFF, (u_int) txnbuf);
    outr(base + PERIPH_TNCR_OFF, (u_int) xnlen);

    /* Set receive pointer and counter. */
    outr(base + PERIPH_RNPR_OFF, (u_int) rxnbuf);
    outr(base + PERIPH_RNCR_OFF, (u_int) xnlen);

    outr(base + SPI_IDR_OFF, (u_int) - 1);
    outr(base + SPI_IER_OFF, SPI_RXBUFF);
    outr(base + PERIPH_PTCR_OFF, PDC_TXTEN | PDC_RXTEN);

    while (((sr = inr(base + SPI_SR_OFF)) & SPI_RXBUFF) == 0) {
        if (base == SPI0_BASE) {
            if ((rc = NutEventWait(&spi0_que, 500)) != 0) {
                printf("[TO]");
                break;
            }
        }
#if defined(SPI1_BASE)
        else if (base == SPI1_BASE) {
            if ((rc = NutEventWait(&spi1_que, 500)) != 0) {
                break;
            }
        }
#endif
    }
    outr(base + PERIPH_PTCR_OFF, PDC_TXTDIS | PDC_RXTDIS);

    return rc;
}
