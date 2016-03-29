#ifndef _DEV_SBBIF1_H_
#define _DEV_SBBIF1_H_
/*
 * Copyright (C) 2007 by egnite Software GmbH. All rights reserved.
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

/*!
 * \file dev/sbbif1.h
 * \brief Serial bit banged interface 1.
 *
 * \verbatim
 *
 * $Log: sbbif1.h,v $
 * Revision 1.1  2007/04/12 09:07:54  haraldkipp
 * Configurable SPI added.
 *
 *
 * \endverbatim
 */

#include <cfg/arch/gpio.h>

/*!
 * \brief Maximum number of devices (chip selects).
 */
#ifndef SBBI1_MAX_DEVICES
#define SBBI1_MAX_DEVICES   4
#endif

#if defined(__AVR__)            /* MCU */
/*
 * AVR implementation.
 * ======================================
 */

#ifdef SBBI1_CS0_BIT

#if (SBBI1_CS0_AVRPORT == AVRPORTB)
#define SBBI1_CS0_SOD_REG PORTB
#define SBBI1_CS0_OE_REG  DDRB
#elif (SBBI1_CS0_AVRPORT == AVRPORTD)
#define SBBI1_CS0_SOD_REG PORTD
#define SBBI1_CS0_OE_REG  DDRD
#elif (SBBI1_CS0_AVRPORT == AVRPORTE)
#define SBBI1_CS0_SOD_REG PORTE
#define SBBI1_CS0_OE_REG  DDRE
#elif (SBBI1_CS0_AVRPORT == AVRPORTF)
#define SBBI1_CS0_SOD_REG PORTF
#define SBBI1_CS0_OE_REG  DDRF
#elif (SBBI1_CS0_AVRPORT == AVRPORTG)
#define SBBI1_CS0_SOD_REG PORTG
#define SBBI1_CS0_OE_REG  DDRG
#elif (SBBI1_CS0_AVRPORT == AVRPORTH)
#define SBBI1_CS0_SOD_REG PORTH
#define SBBI1_CS0_OE_REG  DDRH
#endif

/*! \brief Enable SBBI1 chip select 0 output. */
#define SBBI1_CS0_ENA()      sbi(SBBI1_CS0_OE_REG, SBBI1_CS0_BIT)
/*! \brief Set SBBI1 chip select 0 output low. */
#define SBBI1_CS0_CLR()      cbi(SBBI1_CS0_SOD_REG, SBBI1_CS0_BIT)
/*! \brief Set SBBI1 chip select 0 output high. */
#define SBBI1_CS0_SET()      sbi(SBBI1_CS0_SOD_REG, SBBI1_CS0_BIT)

#endif                          /* SBBI1_CS0_BIT */

#ifdef SBBI1_CS1_BIT

#if (SBBI1_CS1_AVRPORT == AVRPORTB)
#define SBBI1_CS1_SOD_REG PORTB
#define SBBI1_CS1_OE_REG  DDRB
#elif (SBBI1_CS1_AVRPORT == AVRPORTD)
#define SBBI1_CS1_SOD_REG PORTD
#define SBBI1_CS1_OE_REG  DDRD
#elif (SBBI1_CS1_AVRPORT == AVRPORTE)
#define SBBI1_CS1_SOD_REG PORTE
#define SBBI1_CS1_OE_REG  DDRE
#elif (SBBI1_CS1_AVRPORT == AVRPORTF)
#define SBBI1_CS1_SOD_REG PORTF
#define SBBI1_CS1_OE_REG  DDRF
#elif (SBBI1_CS1_AVRPORT == AVRPORTG)
#define SBBI1_CS1_SOD_REG PORTG
#define SBBI1_CS1_OE_REG  DDRG
#elif (SBBI1_CS1_AVRPORT == AVRPORTH)
#define SBBI1_CS1_SOD_REG PORTH
#define SBBI1_CS1_OE_REG  DDRH
#endif

/*! \brief Enable SBBI1 chip select 1 output. */
#define SBBI1_CS1_ENA()      sbi(SBBI1_CS1_OE_REG, SBBI1_CS1_BIT)
/*! \brief Set SBBI1 chip select 1 output low. */
#define SBBI1_CS1_CLR()      cbi(SBBI1_CS1_SOD_REG, SBBI1_CS1_BIT)
/*! \brief Set SBBI1 chip select 1 output high. */
#define SBBI1_CS1_SET()      sbi(SBBI1_CS1_SOD_REG, SBBI1_CS1_BIT)

#endif                          /* SBBI1_CS1_BIT */

#ifdef SBBI1_CS2_BIT

#if (SBBI1_CS2_AVRPORT == AVRPORTB)
#define SBBI1_CS2_SOD_REG PORTB
#define SBBI1_CS2_OE_REG  DDRB
#elif (SBBI1_CS2_AVRPORT == AVRPORTD)
#define SBBI1_CS2_SOD_REG PORTD
#define SBBI1_CS2_OE_REG  DDRD
#elif (SBBI1_CS2_AVRPORT == AVRPORTE)
#define SBBI1_CS2_SOD_REG PORTE
#define SBBI1_CS2_OE_REG  DDRE
#elif (SBBI1_CS2_AVRPORT == AVRPORTF)
#define SBBI1_CS2_SOD_REG PORTF
#define SBBI1_CS2_OE_REG  DDRF
#elif (SBBI1_CS2_AVRPORT == AVRPORTG)
#define SBBI1_CS2_SOD_REG PORTG
#define SBBI1_CS2_OE_REG  DDRG
#elif (SBBI1_CS2_AVRPORT == AVRPORTH)
#define SBBI1_CS2_SOD_REG PORTH
#define SBBI1_CS2_OE_REG  DDRH
#endif

/*! \brief Enable SBBI1 chip select 2 output. */
#define SBBI1_CS2_ENA()      sbi(SBBI1_CS2_OE_REG, SBBI1_CS2_BIT)
/*! \brief Set SBBI1 chip select 2 output low. */
#define SBBI1_CS2_CLR()      cbi(SBBI1_CS2_SOD_REG, SBBI1_CS2_BIT)
/*! \brief Set SBBI1 chip select 2 output high. */
#define SBBI1_CS2_SET()      sbi(SBBI1_CS2_SOD_REG, SBBI1_CS2_BIT)

#endif                          /* SBBI1_CS2_BIT */

#ifdef SBBI1_CS3_BIT

#if (SBBI1_CS3_AVRPORT == AVRPORTB)
#define SBBI1_CS3_SOD_REG PORTB
#define SBBI1_CS3_OE_REG  DDRB
#elif (SBBI1_CS3_AVRPORT == AVRPORTD)
#define SBBI1_CS3_SOD_REG PORTD
#define SBBI1_CS3_OE_REG  DDRD
#elif (SBBI1_CS3_AVRPORT == AVRPORTE)
#define SBBI1_CS3_SOD_REG PORTE
#define SBBI1_CS3_OE_REG  DDRE
#elif (SBBI1_CS3_AVRPORT == AVRPORTF)
#define SBBI1_CS3_SOD_REG PORTF
#define SBBI1_CS3_OE_REG  DDRF
#elif (SBBI1_CS3_AVRPORT == AVRPORTG)
#define SBBI1_CS3_SOD_REG PORTG
#define SBBI1_CS3_OE_REG  DDRG
#elif (SBBI1_CS3_AVRPORT == AVRPORTH)
#define SBBI1_CS3_SOD_REG PORTH
#define SBBI1_CS3_OE_REG  DDRH
#endif

/*! \brief Enable SBBI1 chip select 3 output. */
#define SBBI1_CS3_ENA()      sbi(SBBI1_CS3_OE_REG, SBBI1_CS3_BIT)
/*! \brief Set SBBI1 chip select 3 output low. */
#define SBBI1_CS3_CLR()      cbi(SBBI1_CS3_SOD_REG, SBBI1_CS3_BIT)
/*! \brief Set SBBI1 chip select 3 output high. */
#define SBBI1_CS3_SET()      sbi(SBBI1_CS3_SOD_REG, SBBI1_CS3_BIT)

#endif                          /* SBBI1_CS3_BIT */

#ifdef SBBI1_RST0_BIT

#if (SBBI1_RST0_AVRPORT == AVRPORTB)
#define SBBI1_RST0_SOD_REG PORTB
#define SBBI1_RST0_OE_REG  DDRB
#elif (SBBI1_RST0_AVRPORT == AVRPORTD)
#define SBBI1_RST0_SOD_REG PORTD
#define SBBI1_RST0_OE_REG  DDRD
#elif (SBBI1_RST0_AVRPORT == AVRPORTE)
#define SBBI1_RST0_SOD_REG PORTE
#define SBBI1_RST0_OE_REG  DDRE
#elif (SBBI1_RST0_AVRPORT == AVRPORTF)
#define SBBI1_RST0_SOD_REG PORTF
#define SBBI1_RST0_OE_REG  DDRF
#elif (SBBI1_RST0_AVRPORT == AVRPORTG)
#define SBBI1_RST0_SOD_REG PORTG
#define SBBI1_RST0_OE_REG  DDRG
#elif (SBBI1_RST0_AVRPORT == AVRPORTH)
#define SBBI1_RST0_SOD_REG PORTH
#define SBBI1_RST0_OE_REG  DDRH
#endif

/*! \brief Enable SBBI1 reset 0 output. */
#define SBBI1_RST0_ENA()      sbi(SBBI1_RST0_OE_REG, SBBI1_RST0_BIT)
/*! \brief Set SBBI1 reset 0 output low. */
#define SBBI1_RST0_CLR()      cbi(SBBI1_RST0_SOD_REG, SBBI1_RST0_BIT)
/*! \brief Set SBBI1 reset 0 output high. */
#define SBBI1_RST0_SET()      sbi(SBBI1_RST0_SOD_REG, SBBI1_RST0_BIT)

#endif                          /* SBBI1_RST0_BIT */

#ifdef SBBI1_RST1_BIT

#if (SBBI1_RST1_AVRPORT == AVRPORTB)
#define SBBI1_RST1_SOD_REG PORTB
#define SBBI1_RST1_OE_REG  DDRB
#elif (SBBI1_RST1_AVRPORT == AVRPORTD)
#define SBBI1_RST1_SOD_REG PORTD
#define SBBI1_RST1_OE_REG  DDRD
#elif (SBBI1_RST1_AVRPORT == AVRPORTE)
#define SBBI1_RST1_SOD_REG PORTE
#define SBBI1_RST1_OE_REG  DDRE
#elif (SBBI1_RST1_AVRPORT == AVRPORTF)
#define SBBI1_RST1_SOD_REG PORTF
#define SBBI1_RST1_OE_REG  DDRF
#elif (SBBI1_RST1_AVRPORT == AVRPORTG)
#define SBBI1_RST1_SOD_REG PORTG
#define SBBI1_RST1_OE_REG  DDRG
#elif (SBBI1_RST1_AVRPORT == AVRPORTH)
#define SBBI1_RST1_SOD_REG PORTH
#define SBBI1_RST1_OE_REG  DDRH
#endif

/*! \brief Enable SBBI1 reset 1 output. */
#define SBBI1_RST1_ENA()      sbi(SBBI1_RST1_OE_REG, SBBI1_RST1_BIT)
/*! \brief Set SBBI1 reset 1 output low. */
#define SBBI1_RST1_CLR()      cbi(SBBI1_RST1_SOD_REG, SBBI1_RST1_BIT)
/*! \brief Set SBBI1 reset 1 output high. */
#define SBBI1_RST1_SET()      sbi(SBBI1_RST1_SOD_REG, SBBI1_RST1_BIT)

#endif                          /* SBBI1_RST1_BIT */

#ifdef SBBI1_RST2_BIT

#if (SBBI1_RST2_AVRPORT == AVRPORTB)
#define SBBI1_RST2_SOD_REG PORTB
#define SBBI1_RST2_OE_REG  DDRB
#elif (SBBI1_RST2_AVRPORT == AVRPORTD)
#define SBBI1_RST2_SOD_REG PORTD
#define SBBI1_RST2_OE_REG  DDRD
#elif (SBBI1_RST2_AVRPORT == AVRPORTE)
#define SBBI1_RST2_SOD_REG PORTE
#define SBBI1_RST2_OE_REG  DDRE
#elif (SBBI1_RST2_AVRPORT == AVRPORTF)
#define SBBI1_RST2_SOD_REG PORTF
#define SBBI1_RST2_OE_REG  DDRF
#elif (SBBI1_RST2_AVRPORT == AVRPORTG)
#define SBBI1_RST2_SOD_REG PORTG
#define SBBI1_RST2_OE_REG  DDRG
#elif (SBBI1_RST2_AVRPORT == AVRPORTH)
#define SBBI1_RST2_SOD_REG PORTH
#define SBBI1_RST2_OE_REG  DDRH
#endif

/*! \brief Enable SBBI1 reset 2 output. */
#define SBBI1_RST2_ENA()      sbi(SBBI1_RST2_OE_REG, SBBI1_RST2_BIT)
/*! \brief Set SBBI1 chip reset 2 output low. */
#define SBBI1_RST2_CLR()      cbi(SBBI1_RST2_SOD_REG, SBBI1_RST2_BIT)
/*! \brief Set SBBI1 chip reset 2 output high. */
#define SBBI1_RST2_SET()      sbi(SBBI1_RST2_SOD_REG, SBBI1_RST2_BIT)

#endif                          /* SBBI1_RST2_BIT */

#ifdef SBBI1_RST3_BIT

#if (SBBI1_RST3_AVRPORT == AVRPORTB)
#define SBBI1_RST3_SOD_REG PORTB
#define SBBI1_RST3_OE_REG  DDRB
#elif (SBBI1_RST3_AVRPORT == AVRPORTD)
#define SBBI1_RST3_SOD_REG PORTD
#define SBBI1_RST3_OE_REG  DDRD
#elif (SBBI1_RST3_AVRPORT == AVRPORTE)
#define SBBI1_RST3_SOD_REG PORTE
#define SBBI1_RST3_OE_REG  DDRE
#elif (SBBI1_RST3_AVRPORT == AVRPORTF)
#define SBBI1_RST3_SOD_REG PORTF
#define SBBI1_RST3_OE_REG  DDRF
#elif (SBBI1_RST3_AVRPORT == AVRPORTG)
#define SBBI1_RST3_SOD_REG PORTG
#define SBBI1_RST3_OE_REG  DDRG
#elif (SBBI1_RST3_AVRPORT == AVRPORTH)
#define SBBI1_RST3_SOD_REG PORTH
#define SBBI1_RST3_OE_REG  DDRH
#endif

/*! \brief Enable SBBI1 reset 3 output. */
#define SBBI1_RST3_ENA()      sbi(SBBI1_RST3_OE_REG, SBBI1_RST3_BIT)
/*! \brief Set SBBI1 reset 3 output low. */
#define SBBI1_RST3_CLR()      cbi(SBBI1_RST3_SOD_REG, SBBI1_RST3_BIT)
/*! \brief Set SBBI1 reset 3 output high. */
#define SBBI1_RST3_SET()      sbi(SBBI1_RST3_SOD_REG, SBBI1_RST3_BIT)

#endif                          /* SBBI1_RST3_BIT */

#ifdef SBBI1_SCK_BIT

#if (SBBI1_SCK_AVRPORT == AVRPORTB)
#define SBBI1_SCK_SOD_REG PORTB
#define SBBI1_SCK_OE_REG  DDRB
#elif (SBBI1_SCK_AVRPORT == AVRPORTD)
#define SBBI1_SCK_SOD_REG PORTD
#define SBBI1_SCK_OE_REG  DDRD
#elif (SBBI1_SCK_AVRPORT == AVRPORTE)
#define SBBI1_SCK_SOD_REG PORTE
#define SBBI1_SCK_OE_REG  DDRE
#elif (SBBI1_SCK_AVRPORT == AVRPORTF)
#define SBBI1_SCK_SOD_REG PORTF
#define SBBI1_SCK_OE_REG  DDRF
#elif (SBBI1_SCK_AVRPORT == AVRPORTG)
#define SBBI1_SCK_SOD_REG PORTG
#define SBBI1_SCK_OE_REG  DDRG
#elif (SBBI1_SCK_AVRPORT == AVRPORTH)
#define SBBI1_SCK_SOD_REG PORTH
#define SBBI1_SCK_OE_REG  DDRH
#endif

/*! \brief Enable SBBI1 clock output. */
#define SBBI1_SCK_ENA()      sbi(SBBI1_SCK_OE_REG, SBBI1_SCK_BIT)
/*! \brief Set SBBI1 clock output low. */
#define SBBI1_SCK_CLR()      cbi(SBBI1_SCK_SOD_REG, SBBI1_SCK_BIT)
/*! \brief Set SBBI1 clock output high. */
#define SBBI1_SCK_SET()      sbi(SBBI1_SCK_SOD_REG, SBBI1_SCK_BIT)

#if defined(SBBI1_MOSI_BIT)

#if (SBBI1_MOSI_AVRPORT == AVRPORTB)
#define SBBI1_MOSI_SOD_REG PORTB
#define SBBI1_MOSI_OE_REG  DDRB
#elif (SBBI1_MOSI_AVRPORT == AVRPORTD)
#define SBBI1_MOSI_SOD_REG PORTD
#define SBBI1_MOSI_OE_REG  DDRD
#elif (SBBI1_MOSI_AVRPORT == AVRPORTE)
#define SBBI1_MOSI_SOD_REG PORTE
#define SBBI1_MOSI_OE_REG  DDRE
#elif (SBBI1_MOSI_AVRPORT == AVRPORTF)
#define SBBI1_MOSI_SOD_REG PORTF
#define SBBI1_MOSI_OE_REG  DDRF
#elif (SBBI1_MOSI_AVRPORT == AVRPORTG)
#define SBBI1_MOSI_SOD_REG PORTG
#define SBBI1_MOSI_OE_REG  DDRG
#elif (SBBI1_MOSI_AVRPORT == AVRPORTH)
#define SBBI1_MOSI_SOD_REG PORTH
#define SBBI1_MOSI_OE_REG  DDRH
#endif

/*! \brief Enable SBBI1 data output. */
#define SBBI1_MOSI_ENA()      sbi(SBBI1_MOSI_OE_REG, SBBI1_MOSI_BIT)
/*! \brief Set SBBI1 data output low. */
#define SBBI1_MOSI_CLR()      cbi(SBBI1_MOSI_SOD_REG, SBBI1_MOSI_BIT)
/*! \brief Set SBBI1 data output high. */
#define SBBI1_MOSI_SET()      sbi(SBBI1_MOSI_SOD_REG, SBBI1_MOSI_BIT)

#else                           /* SBBI1_MOSI_BIT */

#define SBBI1_MOSI_ENA()
#define SBBI1_MOSI_CLR()
#define SBBI1_MOSI_SET()

#endif                          /* SBBI1_MOSI_BIT */

#if defined(SBBI1_MISO_BIT)

#if (SBBI1_MISO_AVRPORT == AVRPORTB)
#define SBBI1_MISO_PDS_REG PINB
#define SBBI1_MISO_PUE_REG PORTB
#define SBBI1_MISO_OE_REG  DDRB
#elif (SBBI1_MISO_AVRPORT == AVRPORTD)
#define SBBI1_MISO_PDS_REG PIND
#define SBBI1_MISO_PUE_REG PORTD
#define SBBI1_MISO_OE_REG  DDRD
#elif (SBBI1_MISO_AVRPORT == AVRPORTE)
#define SBBI1_MISO_PDS_REG PINE
#define SBBI1_MISO_PUE_REG PORTE
#define SBBI1_MISO_OE_REG  DDRE
#elif (SBBI1_MISO_AVRPORT == AVRPORTF)
#define SBBI1_MISO_PDS_REG PINF
#define SBBI1_MISO_PUE_REG PORTF
#define SBBI1_MISO_OE_REG  DDRF
#elif (SBBI1_MISO_AVRPORT == AVRPORTG)
#define SBBI1_MISO_PDS_REG PING
#define SBBI1_MISO_PUE_REG PORTG
#define SBBI1_MISO_OE_REG  DDRG
#elif (SBBI1_MISO_AVRPORT == AVRPORTH)
#define SBBI1_MISO_PDS_REG PINH
#define SBBI1_MISO_PUE_REG PORTH
#define SBBI1_MISO_OE_REG  DDRH
#endif

/*! \brief Enable SBBI1 data input. */
#define SBBI1_MISO_ENA() \
    cbi(SBBI1_MISO_OE_REG, SBBI1_MISO_BIT); \
    sbi(SBBI1_MISO_PUE_REG, SBBI1_MISO_BIT)
/*! \brief Query SBBI1 data input. */
#define SBBI1_MISO_TST()    ((inb(SBBI1_MISO_PDS_REG) & _BV(SBBI1_MISO_BIT)) == _BV(SBBI1_MISO_BIT))

#else                           /* SBBI1_MISO_BIT */

#define SBBI1_MISO_ENA()
#define SBBI1_MISO_TST()   0

#endif                          /* SBBI1_MISO_BIT */

#endif                          /* SBBI1_SCK_BIT */

#else                           /* MCU */
/*
 * AT91 implementation.
 * ======================================
 */

#ifdef SBBI1_CS0_BIT

#if !defined(SBBI1_CS0_PIO_ID)
#define SBBI1_CS0_PE_REG        PIO_PER
#define SBBI1_CS0_OE_REG        PIO_OER
#define SBBI1_CS0_COD_REG       PIO_CODR
#define SBBI1_CS0_SOD_REG       PIO_SODR
#elif SBBI1_CS0_PIO_ID == PIOA_ID
#define SBBI1_CS0_PE_REG        PIOA_PER
#define SBBI1_CS0_OE_REG        PIOA_OER
#define SBBI1_CS0_COD_REG       PIOA_CODR
#define SBBI1_CS0_SOD_REG       PIOA_SODR
#elif SBBI1_CS0_PIO_ID == PIOB_ID
#define SBBI1_CS0_PE_REG        PIOB_PER
#define SBBI1_CS0_OE_REG        PIOB_OER
#define SBBI1_CS0_COD_REG       PIOB_CODR
#define SBBI1_CS0_SOD_REG       PIOB_SODR
#elif SBBI1_CS0_PIO_ID == PIOC_ID
#define SBBI1_CS0_PE_REG        PIOC_PER
#define SBBI1_CS0_OE_REG        PIOC_OER
#define SBBI1_CS0_COD_REG       PIOC_CODR
#define SBBI1_CS0_SOD_REG       PIOC_SODR
#endif

/*! \brief Enable SBBI1 chip select 0 output. */
#define SBBI1_CS0_ENA() \
    outr(SBBI1_CS0_PE_REG, _BV(SBBI1_CS0_BIT)); \
    outr(SBBI1_CS0_OE_REG, _BV(SBBI1_CS0_BIT))
/*! \brief Set SBBI1 chip select 0 output low. */
#define SBBI1_CS0_CLR()   outr(SBBI1_CS0_COD_REG, _BV(SBBI1_CS0_BIT))
/*! \brief Set SBBI1 chip select 0 output high. */
#define SBBI1_CS0_SET()   outr(SBBI1_CS0_SOD_REG, _BV(SBBI1_CS0_BIT))

#endif                          /* SBBI1_CS0_BIT */

#ifdef SBBI1_CS1_BIT

#if !defined(SBBI1_CS1_PIO_ID)
#define SBBI1_CS1_PE_REG        PIO_PER
#define SBBI1_CS1_OE_REG        PIO_OER
#define SBBI1_CS1_COD_REG       PIO_CODR
#define SBBI1_CS1_SOD_REG       PIO_SODR
#elif SBBI1_CS1_PIO_ID == PIOA_ID
#define SBBI1_CS1_PE_REG        PIOA_PER
#define SBBI1_CS1_OE_REG        PIOA_OER
#define SBBI1_CS1_COD_REG       PIOA_CODR
#define SBBI1_CS1_SOD_REG       PIOA_SODR
#elif SBBI1_CS1_PIO_ID == PIOB_ID
#define SBBI1_CS1_PE_REG        PIOB_PER
#define SBBI1_CS1_OE_REG        PIOB_OER
#define SBBI1_CS1_COD_REG       PIOB_CODR
#define SBBI1_CS1_SOD_REG       PIOB_SODR
#elif SBBI1_CS1_PIO_ID == PIOC_ID
#define SBBI1_CS1_PE_REG        PIOC_PER
#define SBBI1_CS1_OE_REG        PIOC_OER
#define SBBI1_CS1_COD_REG       PIOC_CODR
#define SBBI1_CS1_SOD_REG       PIOC_SODR
#endif

/*! \brief Enable SBBI1 chip select output. */
#define SBBI1_CS1_ENA() \
    outr(SBBI1_CS1_PE_REG, _BV(SBBI1_CS1_BIT)); \
    outr(SBBI1_CS1_OE_REG, _BV(SBBI1_CS1_BIT))
/*! \brief Set SBBI1 chip select output low. */
#define SBBI1_CS1_CLR()   outr(SBBI1_CS1_COD_REG, _BV(SBBI1_CS1_BIT))
/*! \brief Set SBBI1 chip select output high. */
#define SBBI1_CS1_SET()   outr(SBBI1_CS1_SOD_REG, _BV(SBBI1_CS1_BIT))

#endif                          /* SBBI1_CS1_BIT */

#ifdef SBBI1_CS2_BIT

#if !defined(SBBI1_CS2_PIO_ID)
#define SBBI1_CS2_PE_REG        PIO_PER
#define SBBI1_CS2_OE_REG        PIO_OER
#define SBBI1_CS2_COD_REG       PIO_CODR
#define SBBI1_CS2_SOD_REG       PIO_SODR
#elif SBBI1_CS2_PIO_ID == PIOA_ID
#define SBBI1_CS2_PE_REG        PIOA_PER
#define SBBI1_CS2_OE_REG        PIOA_OER
#define SBBI1_CS2_COD_REG       PIOA_CODR
#define SBBI1_CS2_SOD_REG       PIOA_SODR
#elif SBBI1_CS2_PIO_ID == PIOB_ID
#define SBBI1_CS2_PE_REG        PIOB_PER
#define SBBI1_CS2_OE_REG        PIOB_OER
#define SBBI1_CS2_COD_REG       PIOB_CODR
#define SBBI1_CS2_SOD_REG       PIOB_SODR
#elif SBBI1_CS2_PIO_ID == PIOC_ID
#define SBBI1_CS2_PE_REG        PIOC_PER
#define SBBI1_CS2_OE_REG        PIOC_OER
#define SBBI1_CS2_COD_REG       PIOC_CODR
#define SBBI1_CS2_SOD_REG       PIOC_SODR
#endif

/*! \brief Enable SBBI1 chip select output. */
#define SBBI1_CS2_ENA() \
    outr(SBBI1_CS2_PE_REG, _BV(SBBI1_CS2_BIT)); \
    outr(SBBI1_CS2_OE_REG, _BV(SBBI1_CS2_BIT))
/*! \brief Set SBBI1 chip select output low. */
#define SBBI1_CS2_CLR()   outr(SBBI1_CS2_COD_REG, _BV(SBBI1_CS2_BIT))
/*! \brief Set SBBI1 chip select output high. */
#define SBBI1_CS2_SET()   outr(SBBI1_CS2_SOD_REG, _BV(SBBI1_CS2_BIT))

#endif                          /* SBBI1_CS2_BIT */

#ifdef SBBI1_CS3_BIT

#if !defined(SBBI1_CS3_PIO_ID)
#define SBBI1_CS3_PE_REG        PIO_PER
#define SBBI1_CS3_OE_REG        PIO_OER
#define SBBI1_CS3_COD_REG       PIO_CODR
#define SBBI1_CS3_SOD_REG       PIO_SODR
#elif SBBI1_CS3_PIO_ID == PIOA_ID
#define SBBI1_CS3_PE_REG        PIOA_PER
#define SBBI1_CS3_OE_REG        PIOA_OER
#define SBBI1_CS3_COD_REG       PIOA_CODR
#define SBBI1_CS3_SOD_REG       PIOA_SODR
#elif SBBI1_CS3_PIO_ID == PIOB_ID
#define SBBI1_CS3_PE_REG        PIOB_PER
#define SBBI1_CS3_OE_REG        PIOB_OER
#define SBBI1_CS3_COD_REG       PIOB_CODR
#define SBBI1_CS3_SOD_REG       PIOB_SODR
#elif SBBI1_CS3_PIO_ID == PIOC_ID
#define SBBI1_CS3_PE_REG        PIOC_PER
#define SBBI1_CS3_OE_REG        PIOC_OER
#define SBBI1_CS3_COD_REG       PIOC_CODR
#define SBBI1_CS3_SOD_REG       PIOC_SODR
#endif

/*! \brief Enable SBBI1 chip select output. */
#define SBBI1_CS3_ENA() \
    outr(SBBI1_CS3_PE_REG, _BV(SBBI1_CS3_BIT)); \
    outr(SBBI1_CS3_OE_REG, _BV(SBBI1_CS3_BIT))
/*! \brief Set SBBI1 chip select output low. */
#define SBBI1_CS3_CLR()   outr(SBBI1_CS3_COD_REG, _BV(SBBI1_CS3_BIT))
/*! \brief Set SBBI1 chip select output high. */
#define SBBI1_CS3_SET()   outr(SBBI1_CS3_SOD_REG, _BV(SBBI1_CS3_BIT))

#endif                          /* SBBI1_CS3_BIT */

#ifdef SBBI1_RST0_BIT

#if !defined(SBBI1_RST0_PIO_ID)
#define SBBI1_RST0_PE_REG      PIO_PER
#define SBBI1_RST0_OE_REG      PIO_OER
#define SBBI1_RST0_COD_REG     PIO_CODR
#define SBBI1_RST0_SOD_REG     PIO_SODR
#elif SBBI1_RST0_PIO_ID == PIOA_ID
#define SBBI1_RST0_PE_REG      PIOA_PER
#define SBBI1_RST0_OE_REG      PIOA_OER
#define SBBI1_RST0_COD_REG     PIOA_CODR
#define SBBI1_RST0_SOD_REG     PIOA_SODR
#elif SBBI1_RST0_PIO_ID == PIOB_ID
#define SBBI1_RST0_PE_REG      PIOB_PER
#define SBBI1_RST0_OE_REG      PIOB_OER
#define SBBI1_RST0_COD_REG     PIOB_CODR
#define SBBI1_RST0_SOD_REG     PIOB_SODR
#elif SBBI1_RST0_PIO_ID == PIOC_ID
#define SBBI1_RST0_PE_REG      PIOC_PER
#define SBBI1_RST0_OE_REG      PIOC_OER
#define SBBI1_RST0_COD_REG     PIOC_CODR
#define SBBI1_RST0_SOD_REG     PIOC_SODR
#endif

/*! \brief Enable SBBI1 reset output. */
#define SBBI1_RST0_ENA() \
    outr(SBBI1_RST0_PE_REG, _BV(SBBI1_RST0_BIT)); \
    outr(SBBI1_RST0_OE_REG, _BV(SBBI1_RST0_BIT))
/*! \brief Set SBBI1 reset output low. */
#define SBBI1_RST0_CLR()   outr(SBBI1_RST0_COD_REG, _BV(SBBI1_RST0_BIT))
/*! \brief Set SBBI1 reset output high. */
#define SBBI1_RST0_SET()   outr(SBBI1_RST0_SOD_REG, _BV(SBBI1_RST0_BIT))

#endif                          /* SBBI1_RST0_BIT */

#ifdef SBBI1_RST1_BIT

#if !defined(SBBI1_RST1_PIO_ID)
#define SBBI1_RST1_PE_REG      PIO_PER
#define SBBI1_RST1_OE_REG      PIO_OER
#define SBBI1_RST1_COD_REG     PIO_CODR
#define SBBI1_RST1_SOD_REG     PIO_SODR
#elif SBBI1_RST1_PIO_ID == PIOA_ID
#define SBBI1_RST1_PE_REG      PIOA_PER
#define SBBI1_RST1_OE_REG      PIOA_OER
#define SBBI1_RST1_COD_REG     PIOA_CODR
#define SBBI1_RST1_SOD_REG     PIOA_SODR
#elif SBBI1_RST1_PIO_ID == PIOB_ID
#define SBBI1_RST1_PE_REG      PIOB_PER
#define SBBI1_RST1_OE_REG      PIOB_OER
#define SBBI1_RST1_COD_REG     PIOB_CODR
#define SBBI1_RST1_SOD_REG     PIOB_SODR
#elif SBBI1_RST1_PIO_ID == PIOC_ID
#define SBBI1_RST1_PE_REG      PIOC_PER
#define SBBI1_RST1_OE_REG      PIOC_OER
#define SBBI1_RST1_COD_REG     PIOC_CODR
#define SBBI1_RST1_SOD_REG     PIOC_SODR
#endif

/*! \brief Enable SBBI1 reset output. */
#define SBBI1_RST1_ENA() \
    outr(SBBI1_RST1_PE_REG, _BV(SBBI1_RST1_BIT)); \
    outr(SBBI1_RST1_OE_REG, _BV(SBBI1_RST1_BIT))
/*! \brief Set SBBI1 reset output low. */
#define SBBI1_RST1_CLR()   outr(SBBI1_RST1_COD_REG, _BV(SBBI1_RST1_BIT))
/*! \brief Set SBBI1 reset output high. */
#define SBBI1_RST1_SET()   outr(SBBI1_RST1_SOD_REG, _BV(SBBI1_RST1_BIT))

#endif                          /* SBBI1_RST1_BIT */

#ifdef SBBI1_RST2_BIT

#if !defined(SBBI1_RST2_PIO_ID)
#define SBBI1_RST2_PE_REG      PIO_PER
#define SBBI1_RST2_OE_REG      PIO_OER
#define SBBI1_RST2_COD_REG     PIO_CODR
#define SBBI1_RST2_SOD_REG     PIO_SODR
#elif SBBI1_RST2_PIO_ID == PIOA_ID
#define SBBI1_RST2_PE_REG      PIOA_PER
#define SBBI1_RST2_OE_REG      PIOA_OER
#define SBBI1_RST2_COD_REG     PIOA_CODR
#define SBBI1_RST2_SOD_REG     PIOA_SODR
#elif SBBI1_RST2_PIO_ID == PIOB_ID
#define SBBI1_RST2_PE_REG      PIOB_PER
#define SBBI1_RST2_OE_REG      PIOB_OER
#define SBBI1_RST2_COD_REG     PIOB_CODR
#define SBBI1_RST2_SOD_REG     PIOB_SODR
#elif SBBI1_RST2_PIO_ID == PIOC_ID
#define SBBI1_RST2_PE_REG      PIOC_PER
#define SBBI1_RST2_OE_REG      PIOC_OER
#define SBBI1_RST2_COD_REG     PIOC_CODR
#define SBBI1_RST2_SOD_REG     PIOC_SODR
#endif

/*! \brief Enable SBBI1 reset output. */
#define SBBI1_RST2_ENA() \
    outr(SBBI1_RST2_PE_REG, _BV(SBBI1_RST2_BIT)); \
    outr(SBBI1_RST2_OE_REG, _BV(SBBI1_RST2_BIT))
/*! \brief Set SBBI1 reset output low. */
#define SBBI1_RST2_CLR()   outr(SBBI1_RST2_COD_REG, _BV(SBBI1_RST2_BIT))
/*! \brief Set SBBI1 reset output high. */
#define SBBI1_RST2_SET()   outr(SBBI1_RST2_SOD_REG, _BV(SBBI1_RST2_BIT))

#endif                          /* SBBI1_RST2_BIT */

#ifdef SBBI1_RST3_BIT

#if !defined(SBBI1_RST3_PIO_ID)
#define SBBI1_RST3_PE_REG      PIO_PER
#define SBBI1_RST3_OE_REG      PIO_OER
#define SBBI1_RST3_COD_REG     PIO_CODR
#define SBBI1_RST3_SOD_REG     PIO_SODR
#elif SBBI1_RST3_PIO_ID == PIOA_ID
#define SBBI1_RST3_PE_REG      PIOA_PER
#define SBBI1_RST3_OE_REG      PIOA_OER
#define SBBI1_RST3_COD_REG     PIOA_CODR
#define SBBI1_RST3_SOD_REG     PIOA_SODR
#elif SBBI1_RST3_PIO_ID == PIOB_ID
#define SBBI1_RST3_PE_REG      PIOB_PER
#define SBBI1_RST3_OE_REG      PIOB_OER
#define SBBI1_RST3_COD_REG     PIOB_CODR
#define SBBI1_RST3_SOD_REG     PIOB_SODR
#elif SBBI1_RST3_PIO_ID == PIOC_ID
#define SBBI1_RST3_PE_REG      PIOC_PER
#define SBBI1_RST3_OE_REG      PIOC_OER
#define SBBI1_RST3_COD_REG     PIOC_CODR
#define SBBI1_RST3_SOD_REG     PIOC_SODR
#endif

/*! \brief Enable SBBI1 reset output. */
#define SBBI1_RST3_ENA() \
    outr(SBBI1_RST3_PE_REG, _BV(SBBI1_RST3_BIT)); \
    outr(SBBI1_RST3_OE_REG, _BV(SBBI1_RST3_BIT))
/*! \brief Set SBBI1 reset output low. */
#define SBBI1_RST3_CLR()   outr(SBBI1_RST3_COD_REG, _BV(SBBI1_RST3_BIT))
/*! \brief Set SBBI1 reset output high. */
#define SBBI1_RST3_SET()   outr(SBBI1_RST3_SOD_REG, _BV(SBBI1_RST3_BIT))

#endif                          /* SBBI1_RST3_BIT */

#ifdef SBBI1_SCK_BIT

#if !defined(SBBI1_SCK_PIO_ID)
#define SBBI1_SCK_PE_REG        PIO_PER
#define SBBI1_SCK_OE_REG        PIO_OER
#define SBBI1_SCK_COD_REG       PIO_CODR
#define SBBI1_SCK_SOD_REG       PIO_SODR
#elif SBBI1_SCK_PIO_ID == PIOA_ID
#define SBBI1_SCK_PE_REG        PIOA_PER
#define SBBI1_SCK_OE_REG        PIOA_OER
#define SBBI1_SCK_COD_REG       PIOA_CODR
#define SBBI1_SCK_SOD_REG       PIOA_SODR
#elif SBBI1_SCK_PIO_ID == PIOB_ID
#define SBBI1_SCK_PE_REG        PIOB_PER
#define SBBI1_SCK_OE_REG        PIOB_OER
#define SBBI1_SCK_COD_REG       PIOB_CODR
#define SBBI1_SCK_SOD_REG       PIOB_SODR
#elif SBBI1_SCK_PIO_ID == PIOC_ID
#define SBBI1_SCK_PE_REG        PIOC_PER
#define SBBI1_SCK_OE_REG        PIOC_OER
#define SBBI1_SCK_COD_REG       PIOC_CODR
#define SBBI1_SCK_SOD_REG       PIOC_SODR
#endif

/*! \brief Enable SBBI1 clock output. */
#define SBBI1_SCK_ENA() \
    outr(SBBI1_SCK_PE_REG, _BV(SBBI1_SCK_BIT)); \
    outr(SBBI1_SCK_OE_REG, _BV(SBBI1_SCK_BIT))
/*! \brief Set SBBI1 clock output low. */
#define SBBI1_SCK_CLR()     outr(SBBI1_SCK_COD_REG, _BV(SBBI1_SCK_BIT))
/*! \brief Set SBBI1 clock output high. */
#define SBBI1_SCK_SET()     outr(SBBI1_SCK_SOD_REG, _BV(SBBI1_SCK_BIT))

#ifdef SBBI1_MOSI_BIT

#if !defined(SBBI1_MOSI_PIO_ID)
#define SBBI1_MOSI_PE_REG       PIO_PER
#define SBBI1_MOSI_OE_REG       PIO_OER
#define SBBI1_MOSI_COD_REG      PIO_CODR
#define SBBI1_MOSI_SOD_REG      PIO_SODR
#elif SBBI1_MOSI_PIO_ID == PIOA_ID
#define SBBI1_MOSI_PE_REG       PIOA_PER
#define SBBI1_MOSI_OE_REG       PIOA_OER
#define SBBI1_MOSI_COD_REG      PIOA_CODR
#define SBBI1_MOSI_SOD_REG      PIOA_SODR
#elif SBBI1_MOSI_PIO_ID == PIOB_ID
#define SBBI1_MOSI_PE_REG       PIOB_PER
#define SBBI1_MOSI_OE_REG       PIOB_OER
#define SBBI1_MOSI_COD_REG      PIOB_CODR
#define SBBI1_MOSI_SOD_REG      PIOB_SODR
#elif SBBI1_MOSI_PIO_ID == PIOC_ID
#define SBBI1_MOSI_PE_REG       PIOC_PER
#define SBBI1_MOSI_OE_REG       PIOC_OER
#define SBBI1_MOSI_COD_REG      PIOC_CODR
#define SBBI1_MOSI_SOD_REG      PIOC_SODR
#endif

/*! \brief Enable SBBI1 data output. */
#define SBBI1_MOSI_ENA() \
    outr(SBBI1_MOSI_PE_REG, _BV(SBBI1_MOSI_BIT)); \
    outr(SBBI1_MOSI_OE_REG, _BV(SBBI1_MOSI_BIT))
/*! \brief Set SBBI1 data output low. */
#define SBBI1_MOSI_CLR()    outr(SBBI1_MOSI_COD_REG, _BV(SBBI1_MOSI_BIT))
/*! \brief Set SBBI1 data output high. */
#define SBBI1_MOSI_SET()    outr(SBBI1_MOSI_SOD_REG, _BV(SBBI1_MOSI_BIT))

#else                           /* SBBI1_MOSI_BIT */

#define SBBI1_MOSI_ENA()
#define SBBI1_MOSI_CLR()
#define SBBI1_MOSI_SET()

#endif                          /* SBBI1_MOSI_BIT */

#ifdef SBBI1_MISO_BIT

#if !defined(SBBI1_MISO_PIO_ID)
#define SBBI1_MISO_PE_REG       PIO_PER
#define SBBI1_MISO_OD_REG       PIO_ODR
#define SBBI1_MISO_PDS_REG      PIO_PDSR
#elif SBBI1_MISO_PIO_ID == PIOA_ID
#define SBBI1_MISO_PE_REG       PIOA_PER
#define SBBI1_MISO_OD_REG       PIOA_ODR
#define SBBI1_MISO_PDS_REG      PIOA_PDSR
#elif SBBI1_MISO_PIO_ID == PIOB_ID
#define SBBI1_MISO_PE_REG       PIOB_PER
#define SBBI1_MISO_OD_REG       PIOB_ODR
#define SBBI1_MISO_PDS_REG      PIOB_PDSR
#elif SBBI1_MISO_PIO_ID == PIOC_ID
#define SBBI1_MISO_PE_REG       PIOC_PER
#define SBBI1_MISO_OD_REG       PIOC_ODR
#define SBBI1_MISO_PDS_REG      PIOC_PDSR
#endif

/*! \brief Enable SBBI1 data input. */
#define SBBI1_MISO_ENA() \
    outr(SBBI1_MISO_PE_REG, _BV(SBBI1_MISO_BIT)); \
    outr(SBBI1_MISO_OD_REG, _BV(SBBI1_MISO_BIT))
/*! \brief Query SBBI1 data input. */
#define SBBI1_MISO_TST()    ((inr(SBBI1_MISO_PDS_REG) & _BV(SBBI1_MISO_BIT)) == _BV(SBBI1_MISO_BIT))

#else                           /* SBBI1_MISO_BIT */

#define SBBI1_MISO_ENA()
#define SBBI1_MISO_TST()   0

#endif                          /* SBBI1_MISO_BIT */

#endif                          /* SBBI1_SCK_BIT */

#endif                          /* MCU */

#define SBBI1_INIT() \
{ \
    SBBI1_SCK_CLR(); \
    SBBI1_SCK_ENA(); \
    SBBI1_MOSI_CLR(); \
    SBBI1_MOSI_ENA(); \
    SBBI1_MISO_ENA(); \
}

#endif
