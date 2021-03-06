/*
 * Copyright (C) 2003-2005 by egnite Software GmbH. All rights reserved.
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
 */

/*
 * $Log: config.c,v $
 * Revision 1.1  2005/11/03 15:14:27  haraldkipp
 * First import.
 *
 */

#include <avr/io.h>
#include "config.h"
#include "debug.h"

CONFNET confnet = {
    0,
    { 'E', 'T', 'H', 'E', 'R', 'N', 'U', 'T', 0 },
    { 0x00, 0x06, 0x98, 0x00, 0x00, 0x00 },
    0, 0, 0, 0
};

CONFBOOT confboot;

#ifdef EE_CONFNET

static unsigned char ConfigGet(unsigned int loc)
{
    while (EECR & 0x02);
    EEAR = loc;
    EECR |= 0x01;
    return EEDR;
}

static void ConfigRead(unsigned int loc, void *buf, size_t n)
{
    unsigned char *cp = buf;

    while (n--) {
        *cp++ = ConfigGet(loc);
        loc++;
    }
}

#endif

void BootConfigRead(void)
{
#ifdef EE_CONFNET
    DEBUG("CONFNET ");
    if (ConfigGet(EE_CONFNET) == sizeof(CONFNET)) {
        DEBUG("OK\n");
        ConfigRead(EE_CONFNET, &confnet, sizeof(CONFNET));
#ifdef EE_CONFBOOT
        DEBUG("CONFBOOT ");
        if (ConfigGet(EE_CONFBOOT) == sizeof(CONFBOOT)) {
            DEBUG(" OK\n");
            ConfigRead(EE_CONFBOOT, &confboot, sizeof(CONFBOOT));
        }
        else {
            DEBUG("No\n");
        }
#endif
    }
    else {
        DEBUG("No\n");
    }
#endif
}

