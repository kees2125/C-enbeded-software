--
-- Copyright (C) 2004-2007 by egnite Software GmbH. All rights reserved.
--
-- Redistribution and use in source and binary forms, with or without
-- modification, are permitted provided that the following conditions
-- are met:
--
-- 1. Redistributions of source code must retain the above copyright
--    notice, this list of conditions and the following disclaimer.
-- 2. Redistributions in binary form must reproduce the above copyright
--    notice, this list of conditions and the following disclaimer in the
--    documentation and/or other materials provided with the distribution.
-- 3. Neither the name of the copyright holders nor the names of
--    contributors may be used to endorse or promote products derived
--    from this software without specific prior written permission.
--
-- THIS SOFTWARE IS PROVIDED BY EGNITE SOFTWARE GMBH AND CONTRIBUTORS
-- ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
-- LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
-- FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EGNITE
-- SOFTWARE GMBH OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
-- INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
-- BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
-- OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
-- AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
-- OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
-- THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
-- SUCH DAMAGE.
--
-- For additional information see http://www.ethernut.de/
--

-- ARM Architecture
--
-- $Log: arm.nut,v $
-- Revision 1.17  2007/02/15 16:04:34  haraldkipp
-- Configurable AT91 EMAC buffer usage and link timeout.
-- Port usage for HD44780 is now configurable. Data bits no longer need four
-- consecutive port bits.
-- Added support for ST7036 based displays. Tested with EA DOG-M LCDs.
-- Support for AT91 system controller interrupts added.
-- Support for AT91 periodic interval timer added.
-- Configurable SAM7 and SAM9 runtime initialization.
--
-- Revision 1.16  2006/10/08 16:41:34  haraldkipp
-- PHY address and power down bit are now configurable.
--
-- Revision 1.15  2006/09/29 12:34:59  haraldkipp
-- Basic AT91 SPI support added.
--
-- Revision 1.14  2006/09/08 16:47:24  haraldkipp
-- For some reason the SSC driver for SAM7X had not been included.
--
-- Revision 1.13  2006/09/07 09:03:43  haraldkipp
-- Corrected descriptions. SAM7X and SAM9260 are both using the same
-- Ethernet driver source. at91sam7x_emac.c will become obsolete.
-- SSC driver code added to SAM7X build.
--
-- Revision 1.12  2006/09/05 12:29:59  haraldkipp
-- SPI and MCI support added for SAM9260.
--
-- Revision 1.11  2006/08/31 19:04:08  haraldkipp
-- Added support for the AT91SAM9260 and Atmel's AT91SAM9260 Evaluation Kit.
--
-- Revision 1.10  2006/07/26 11:19:06  haraldkipp
-- Defining AT91_PLL_MAINCK will automatically determine SAM7X clock by
-- reading PLL settings.
-- Added MMC/SD-Card support for AT91SAM7X Evaluation Kit.
--
-- Revision 1.9  2006/07/05 08:02:17  haraldkipp
-- SAM7X interrupt and EMAC support added.
--
-- Revision 1.8  2006/06/28 17:22:34  haraldkipp
-- Make it compile for AT91SAM7X256.
--
-- Revision 1.7  2006/05/25 09:13:22  haraldkipp
-- Platform independent watchdog API added.
--
-- Revision 1.6  2006/04/07 12:24:12  haraldkipp
-- ARM driver for HD44780 LCD controller added.
--
-- Revision 1.5  2006/03/02 19:56:10  haraldkipp
-- First attempt to compile with ICCARM. All compile errors fixed, but not
-- a finished port yet. Many things are missing.
-- Added MCU specific hardware initialization routine. For the AT91 the
-- spurious interrupt handler has been added, which fixes SF 1440948.
--
-- Revision 1.4  2006/02/23 15:41:40  haraldkipp
-- Added support for AT91 watchdog timer.
--
-- Revision 1.3  2005/11/20 14:40:28  haraldkipp
-- Added interrupt driven UART driver for AT91.
--
-- Revision 1.2  2005/10/24 09:52:32  haraldkipp
-- New AT91 interrupt handler routines.
-- New DM9000E Ethernet driver for Ethernut 3 board.
--
-- Revision 1.1  2005/07/26 15:41:06  haraldkipp
-- All target dependent code is has been moved to a new library named
-- libnutarch. Each platform got its own script.
--
--

nutarch_arm =
{
    --
    -- Runtime Initialization
    --
    {
        name = "nutarch_arm_cstartup",
        brief = "ARM-GCC Startup",
        sources = { "arm/init/crt$(LDNAME).S" },
        targets = { "arm/init/crt$(LDNAME).o" },
        -- ICCARM: FIXME!
        requires = { "TOOL_CC_ARM", "TOOL_GCC" },
        options =
        {
            {
                macro = "IRQ_STACK_SIZE",
                brief = "IRQ Stack Size",
                description = "Number of bytes reserved for interrupt stack\n"..
                              "Default is 512 (128 words).",
                flavor = "booldata",
                file = "include/cfg/memory.h"
            },
            {
                macro = "FIQ_STACK_SIZE",
                brief = "FIQ Stack Size",
                description = "Number of bytes reserved for fast interrupt stack\n"..
                              "Default is 256 (64 words).",
                flavor = "booldata",
                file = "include/cfg/memory.h"
            },
            {
                macro = "ABT_STACK_SIZE",
                brief = "ABT Stack Size",
                description = "Number of bytes reserved for abort exception stack\n"..
                              "Default is 128 (32 words).",
                flavor = "booldata",
                file = "include/cfg/memory.h"
            },
            {
                macro = "UND_STACK_SIZE",
                brief = "UND Stack Size",
                description = "Number of bytes reserved for undefined exception stack\n"..
                              "Default is 128 (32 words).",
                flavor = "booldata",
                file = "include/cfg/memory.h"
            },
            {
                macro = "PLL_DIV_VAL",
                brief = "PLL Divider",
                description = "Make sure to read the datasheet before modifying this value.\n\n"..
                              "The main clock (crystal) will be divided by this value "..
                              "and multiplied by the PLL multiplier value plus 1 to "..
                              "generate the PLL output frequency.\n\n"..
                              "Default values are\n"..
                              "SAM7X: 14\n"..
                              "SAM9260: 14\n",
                requires = { "HW_PLL_AT91" },
                flavor = "booldata",
                file = "include/cfg/clock.h"
            },
            {
                macro = "PLL_MUL_VAL",
                brief = "PLL Multiplier",
                description = "Make sure to read the datasheet before modifying this value.\n\n"..
                              "The main clock (crystal) will be divided by the PLL "..
                              "divider value and multiplied by the specified multiplier "..
                              " value plus 1 to generate the PLL output frequency.\n\n"..
                              "Default values are\n"..
                              "SAM7X: 72\n"..
                              "SAM9260: 72\n",
                requires = { "HW_PLL_AT91" },
                flavor = "booldata",
                file = "include/cfg/clock.h"
            },
            {
                macro = "MASTER_CLOCK_PRES",
                brief = "Master Clock Prescaler",
                description = "The selected clock will be divided by this value to generate "..
                              "the master clock. Possible values are 1, 2 (default), 4, 8, 16, 32 or 64.",
                flavor = "booldata",
                file = "include/cfg/clock.h"
            },
        },
    },
    
    --
    -- Context Switching
    --
    {
        name = "nutarch_arm_context",
        brief = "Context Switching",
        provides = { "NUT_CONTEXT_SWITCH" },
        requires = { "HW_MCU_ARM", "TOOL_GCC" },
        sources = { "arm/os/context.c" },
    },
    
    --
    -- System Timer Hardware
    --
    {
        name = "nutarch_ostimer_at91",
        brief = "System Timer (AT91)",
        requires = { "HW_TIMER_AT91" },
        provides = { "NUT_OSTIMER_DEV" },
        sources = { "arm/dev/ostimer_at91.c" },
        options =
        {
            {
                macro = "AT91_PLL_MAINCK",
                brief = "AT91 Main Clock",
                description = "Frequency of the external crystal. If this option is "..
                              "enabled and NUT_CPU_FREQ is not enabled, then the "..
                              "specified value will be used together with "..
                              "the PLL register settings to determine the CPU master clock. "..
                              "Otherwise you must specify NUT_CPU_FREQ.",
                requires = { "HW_PLL_AT91" },
                flavor = "booldata",
                file = "include/cfg/clock.h"
            },
            {
                macro = "NUT_TICK_AT91PIT",
                brief = "Use PIT",
                description = "If selected, the AT91 periodic interval timer is used "..
                              "to generate system ticks. Otherwise timer 0 is used. ",
                requires = { "HW_PIT_AT91" },
                flavor = "booldata",
                file = "include/cfg/clock.h"
            },
        },
    },
    {
        name = "nutarch_ostimer_gba",
        brief = "System Timer (GBA)",
        requires = { "HW_TIMER_GBA" },
        provides = { "NUT_OSTIMER_DEV" },
        sources = { "arm/dev/ostimer_gba.c" },
    },
    {
        name = "nutarch_wdt_at91",
        brief = "Watchdog Timer (AT91)",
        requires = { "HW_WDOG_AT91" },
        sources = { "arm/dev/wdt_at91.c" },
    },

    --
    -- Interrupt handling.
    --
    {
        name = "nutarch_arm_irqgba",
        brief = "Interrupt Handler (GBA)",
        requires = { "HW_MCU_GBA" },
        provides = { "DEV_IRQ_GBA" },
        sources = { "arm/dev/ihndlr_gba.c" },
    },
    {
        name = "nutarch_arm_irqat91x40",
        brief = "Interrupt Handler (AT91R40XX)",
        requires = { "HW_MCU_AT91R40008" },
        provides = { "DEV_IRQ_AT91" },
        sources =
        {
            "arm/dev/ih_at91fiq.c",
            "arm/dev/ih_at91irq0.c",
            "arm/dev/ih_at91irq1.c",
            "arm/dev/ih_at91irq2.c",
            "arm/dev/ih_at91pio.c",
            "arm/dev/ih_at91swirq.c",
            "arm/dev/ih_at91tc0.c",
            "arm/dev/ih_at91tc1.c",
            "arm/dev/ih_at91tc2.c",
            "arm/dev/ih_at91uart0.c",
            "arm/dev/ih_at91uart1.c",
            "arm/dev/ih_at91wdi.c"
        },
    },
    {
        name = "nutarch_arm_irqat91sam7x",
        brief = "Interrupt Handler (SAM7X)",
        requires = { "HW_MCU_AT91SAM7X" },
        provides = { "DEV_IRQ_AT91" },
        sources =
        {
            "arm/dev/ih_at91fiq.c",
            "arm/dev/ih_at91sys.c",
            "arm/dev/ih_at91irq0.c",
            "arm/dev/ih_at91irq1.c",
            "arm/dev/ih_at91irq2.c",
            "arm/dev/ih_at91pio.c",
            "arm/dev/ih_at91ssc.c",
            "arm/dev/ih_at91swirq.c",
            "arm/dev/ih_at91tc0.c",
            "arm/dev/ih_at91tc1.c",
            "arm/dev/ih_at91tc2.c",
            "arm/dev/ih_at91uart0.c",
            "arm/dev/ih_at91uart1.c",
            "arm/dev/ih_at91emac.c",
            "arm/dev/ih_at91wdi.c"
        },
    },
    {
        name = "nutarch_arm_irqat91sam926x",
        brief = "Interrupt Handler (SAM926X)",
        requires = { "HW_MCU_AT91SAM9260" },
        provides = { "DEV_IRQ_AT91" },
        sources =
        {
            "arm/dev/ih_at91emac.c",
            "arm/dev/ih_at91fiq.c",
            "arm/dev/ih_at91irq0.c",
            "arm/dev/ih_at91irq1.c",
            "arm/dev/ih_at91irq2.c",
            "arm/dev/ih_at91pio.c",
            "arm/dev/ih_at91spi0.c",
            "arm/dev/ih_at91spi1.c",
            "arm/dev/ih_at91ssc.c",
            "arm/dev/ih_at91swirq.c",
            "arm/dev/ih_at91tc0.c",
            "arm/dev/ih_at91tc1.c",
            "arm/dev/ih_at91tc2.c",
            "arm/dev/ih_at91uart0.c",
            "arm/dev/ih_at91uart1.c",
        },
    },

    --
    -- Device Drivers
    --
    {
        name = "nutarch_arm_debug",
        brief = "UART Debug Output (AT91)",
        requires = { "HW_UART_AT91" },
        provides = { "DEV_UART", "DEV_FILE", "DEV_WRITE" },
        sources = { "arm/dev/debug_at91.c" }
    },
    {
        name = "nutarch_arm_usart0",
        brief = "USART0 Driver",
        description = "Hardware specific USART driver. Implements hardware "..
                      "functions for the generic driver framework.",
        requires = { "HW_UART_AT91", "DEV_IRQ_AT91", "NUT_EVENT", "CRT_HEAPMEM" },
        provides = { "DEV_UART_SPECIFIC" },
        sources = { "arm/dev/usart0at91.c" },
    },
    {
        name = "nutarch_arm_usart1",
        brief = "USART1 Driver",
        description = "Hardware specific USART driver. Implements hardware "..
                      "functions for the generic driver framework.",
        requires = { "HW_UART_AT91", "DEV_IRQ_AT91", "NUT_EVENT", "CRT_HEAPMEM" },
        provides = { "DEV_UART_SPECIFIC" },
        sources = { "arm/dev/usart1at91.c" },
    },
    {
        name = "nutarch_at91_hd44780",
        brief = "HD44780 LCD Driver (AT91)",
        description = "Only 4 bit interfaces are currently supported.\n"..
                      "Tested on the AT91R40008 with 2x16 character LCD.",
        requires = { "HW_MCU_AT91" },
        provides = { "DEV_FILE", "DEV_WRITE" },
        sources = { "arm/dev/hd44780_at91.c" },
        options =
        {
            {
                macro = "LCD_ROWS",
                brief = "Rows",
                description = "The number of available display rows, either 1, 2 or 4.",
                default = "2",
                flavor = "integer",
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_COLS",
                brief = "Columns",
                description = "The number of available display colums, either 8, 16, 20 or 40.",
                default = "16",
                flavor = "integer",
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_SHORT_DELAY",
                brief = "Short Delay",
                description = "The number of dummy loops executed after LCD enable goes up.",
                default = "10",
                flavor = "integer",
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_LONG_DELAY",
                brief = "Long Delay",
                description = "The number of loops executed after sending a command to the LCD "..
                              "controller. If a R/W line is speicifed, then the driver will queries "..
                              "the LCD status and terminates the loop as soon as the LCD busy "..
                              "flag has been cleared.",
                default = "1000",
                flavor = "integer",
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_DATA_LSB",
                brief = "Least Significant Data Bit",
                description = "Bit number of the least significant data bit. The remaining "..
                              "data bits must be connected to the following port bits.",
                type = "enumerated",
                flavor = "booldata",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "LCD_DATA_BIT0",
                brief = "Data Bit 0",
                description = "Port bit connected to LCD data bit 0.\n"..
                              "Not used if LCD_DATA_LSB is defined.\n",
                type = "enumerated",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "LCD_DATA_BIT1",
                brief = "Data Bit 1",
                type = "enumerated",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "LCD_DATA_BIT2",
                brief = "Data Bit 2",
                type = "enumerated",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "LCD_DATA_BIT3",
                brief = "Data Bit 3",
                type = "enumerated",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "LCD_ENABLE_BIT",
                brief = "Enable Bit",
                description = "Port bit of the LCD enable line. "..
                              "This line must be exclusively reserved.",
                default = "4",
                type = "enumerated",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "LCD_REGSEL_BIT",
                brief = "Register Select Bit",
                description = "Port bit of the LCD register select line. "..
                              "May be shared.",
                default = "7",
                type = "enumerated",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "LCD_RW_BIT",
                brief = "Read/Write Bit",
                description = "Optional port bit of the LCD register select line. "..
                              "If not specified, the driver will use the maximum delay. "..
                              "May be shared.",
                type = "enumerated",
                flavor = "booldata",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
        },
    },
    {
        name = "nutarch_at91_st7036",
        brief = "ST7036 LCD Driver (AT91)",
        description = "Bit banging SPI mode\n"..
                      "Tested on Ethernut 3 with 2x16 character LCD.",
        requires = { "HW_MCU_AT91" },
        provides = { "DEV_FILE", "DEV_WRITE" },
        sources = { "arm/dev/st7036_at91.c" },
        options =
        {
            {
                macro = "LCD_SHORT_DELAY",
                brief = "Short Delay",
                description = "The number of dummy loops executed after LCD enable goes up.",
                default = "10",
                flavor = "integer",
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_LONG_DELAY",
                brief = "Long Delay",
                description = "The number of loops executed after sending a command to the LCD "..
                              "controller. If a R/W line is speicifed, then the driver will queries "..
                              "the LCD status and terminates the loop as soon as the LCD busy "..
                              "flag has been cleared.",
                default = "1000",
                flavor = "integer",
                file = "include/cfg/lcd.h"
            },
            {
                macro = "LCD_PIO_ID",
                brief = "GPIO Register ID",
                description = "All signals must be connected to the same GPIO port.",
                type = "enumerated",
                choices = at91_pio_id_choice,
                flavor = "integer",
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "LCD_CS_BIT",
                brief = "LCD CS Bit",
                description = "Bit number of the chip select line.",
                type = "enumerated",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "LCD_CLK_BIT",
                brief = "LCD CLK Bit",
                description = "Bit number of the clock line.",
                type = "enumerated",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "LCD_MOSI_BIT",
                brief = "LCD MOSI Bit",
                description = "Bit number of the data line.",
                type = "enumerated",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "LCD_RS_BIT",
                brief = "LCD RS Bit",
                description = "Bit number of the register select line.",
                type = "enumerated",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
        }
        
    },
    {
        name = "nutarch_gba_debug",
        brief = "LCD Debug Output (GBA)",
        requires = { "HW_LCD_GBA" },
        provides = { "DEV_UART", "DEV_FILE", "DEV_WRITE" },
        sources = { "arm/dev/debug_gba.c" }
    },   
    {
        name = "nutarch_arm_ax88796",
        brief = "AX88796 Driver (AT91)",
        description = "LAN driver for Asix 88796. AT91 only.",
        requires = { "HW_MCU_AT91R40008", "NUT_EVENT", "NUT_TIMER" },
        provides = { "NET_PHY" },
        sources = { "arm/dev/ax88796.c" },
    },     
    {
        name = "nutarch_arm_dm9000e",
        brief = "DM9000E Driver (AT91)",
        description = "LAN driver for Davicom DM9000E. AT91 only.",
        requires = { "HW_MCU_AT91R40008", "NUT_EVENT", "NUT_TIMER" },
        provides = { "NET_PHY" },
        sources = { "arm/dev/dm9000e.c" },
    },     
    {
        name = "nutarch_arm_at91_emac",
        brief = "AT91 EMAC Driver",
        description = "LAN driver for AT91SAM7X and AT91SAM9260.",
        requires = { "HW_EMAC_AT91", "NUT_EVENT", "NUT_TIMER" },
        provides = { "NET_PHY" },
        sources = { "arm/dev/at91_emac.c" },
        options =
        {
            {
                macro = "NIC_PHY_ADDR",
                brief = "PHY Address",
                description = "Default is 31 for SAM7X and 8 for SAM9260 boards.\n\n",
                flavor = "integer",
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "PHY_PWRDN_BIT",
                brief = "PHY Power Down Bit",
                description = "For the SAM7X default is 18.\n\n",
                provides = { "PHY_PWRDN_CONTROL" },
                flavor = "booldata",
                type = "enumerated",
                choices = mcu_32bit_choice,
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "PHY_PWRDN_NEGPOL",
                brief = "PHY Power Down Polarity",
                description = "Select this, if the PHY is powered down by a low signal.",
                requires = { "PHY_PWRDN_CONTROL" },
                flavor = "boolean",
                file = "include/cfg/arch/armpio.h"
            },
            {
                macro = "NUT_THREAD_NICRXSTACK",
                brief = "Receiver Thread Stack",
                description = "Number of bytes to be allocated for the stack of the NIC receive thread.",
                default = "768",
                type = "integer",
                file = "include/cfg/dev.h"
            },
            {
                macro = "EMAC_RX_BUFFERS",
                brief = "Receive Buffers",
                description = "Number of 128 byte receive buffers.\n"..
                              "Increase to handle high traffic situations.\n"..
                              "Decrease to handle low memory situations.\n"..
                              "Default is 32.\n",
                flavor = "booldata",
                type = "integer",
                file = "include/cfg/dev.h"
            },
            {
                macro = "EMAC_TX_BUFSIZ",
                brief = "Transmit Buffer Size",
                description = "The driver will allocate two transmit buffers.\n"..
                              "Can be decreased in low memory situations. Be aware, "..
                              "that this may break your network application. Do not "..
                              "change this without exactly knowing the consequences.\n"..
                              "Default is 1536.\n",
                flavor = "booldata",
                type = "integer",
                file = "include/cfg/dev.h"
            },
            {
                macro = "EMAC_LINK_LOOPS",
                brief = "Link Polling Loops",
                description = "This simple implementation runs a dumb polling loop "..
                              "while waiting for the Ethernet link status.\n"..
                              "If you experience link problems, increasing this value "..
                              "may help.\n"..
                              "Default is 10000.\n",
                flavor = "booldata",
                type = "integer",
                file = "include/cfg/dev.h"
            },
        }
    },
    {
        name = "nutarch_arm_spimmc_at91",
        brief = "AT91 SPI MMC Access",
        description = "Low level MMC interface for AT91.",
        requires = { "HW_SPI_AT91" },
        provides = { "DEV_MMCLL" },
        sources = { "arm/dev/spimmc_at91.c" },
    },     
    {
        name = "nutarch_arm_mci_at91",
        brief = "AT91 MCI Device",
        description = "MCI based block device for AT91SAM9260.",
        requires = { "HW_MCI_AT91", "HW_MCU_AT91SAM9260" },
        provides = { "DEV_BLOCK" },
        sources = { "arm/dev/at91_mci.c" },
    },     
    
    --
    -- Special Functions
    --
    {
        name = "nutarch__arm_init",
        brief = "AT91 Initialization",
        description = "Contains spurious interrupt handler.",
        requires = { "HW_MCU_AT91" },
        sources = { "arm/dev/at91init.c" },
    },
    {
        name = "nutarch__arm_at91spi",
        brief = "AT91 SPI Support",
        description = "Preliminary SPI routines.",
        requires = { "HW_SPI_AT91", "HW_PDC_AT91" },
        sources = { "arm/dev/at91_spi.c" },
    },
    {
        name = "nutarch__arm_at91efc",
        brief = "AT91 Embedded Flash",
        description = "Routines for reading and writing embedded flash memory.",
        requires = { "HW_MCU_AT91SAM7X" },
        sources = { "arm/dev/at91_efc.c" },
        options =
        {
            {
                macro = "NUT_CONFIG_AT91EFC",
                brief = "System Configuration",
                description = "If enabled, Nut/OS and Nut/Net configurations will "..
                              "be stored in on-chip flash memory.",
                provides = { "HW_FLASH_PARAM_SECTOR" },
                flavor = "boolean",
                file = "include/cfg/eeprom.h"
            },
        },
    },
}

