/*
 * <MFS> Modified for Streamit
 * use PPP-stack from 3.9.6
 *
 * Copyright (C) 2002 by Call Direct Cellular Solutions Pty. Ltd. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY CALL DIRECT CELLULAR SOLUTIONS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL CALL DIRECT
 * CELLULAR SOLUTIONS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.calldirect.com.au/
 * -
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
 *
 * -
 */

/*
 * $Log: pppin.c,v $
 * Revision 1.3  2003/08/14 15:15:28  haraldkipp
 * Unsuccessful try to fix ICCAVR bug
 *
 * Revision 1.2  2003/07/13 19:05:22  haraldkipp
 * Debug output corrected.
 *
 * Revision 1.1.1.1  2003/05/09 14:41:36  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.2  2003/05/06 18:17:11  harald
 * Separate PPP debug module added.
 *
 * Revision 1.1  2003/03/31 14:53:28  harald
 * Prepare release 3.1
 *
 */

#include <cfg/os.h>
#include <cfg/ppp.h>
#include <dev/ppp.h>

#include <netinet/in.h> // added Johan 14-11
#include <netinet/if_ppp.h>
#include <netinet/ppp_fsm.h>
#include <netinet/ip.h>
#include <net/ppp.h>
#include <sys/types.h>
#include <sys/timer.h>

#ifdef NUTDEBUG
#include <net/netdebug.h>
#endif

/*!
 * \addtogroup xgPPP
 */
/*@{*/


/*!
 * \brief Handle incoming PPP frames.
 *
 * Splits the PPP frame into the data link and the network part.
 * Then the frame is routed to the proper handler, based on the
 * type field in the header.
 *
 * \note This routine is called by the device driver on incoming
 *       PPP frames. Applications typically do not call this function.
 *
 * \param dev Identifies the device that received the frame.
 * \param nb  Pointer to a network buffer structure containing
 *            the PPP frame.
 */
void NutPppInput(NUTDEVICE * dev, NETBUF * nb)
{
    PPPHDR *ph = (PPPHDR *) nb->nb_dl.vp;
    PPPDCB *dcb = dev->dev_dcb;
    u_short protocol;

#ifdef NUTDEBUG
    if (__ppp_trf) {
        fputs("\nPPP>", __ppp_trs);
        NutDumpPpp(__ppp_trs, nb);
    }
#elif defined(__IMAGECRAFT__)
    /*
     * No idea what this is, but ICCAVR fails if this call isn't there.
     */
    NutSleep(100);
#endif

    /*
     * Chop off the PPP header.
     */
    nb->nb_nw.vp = ph + 1;
    nb->nb_nw.sz = nb->nb_dl.sz - sizeof(PPPHDR);
    nb->nb_dl.sz = sizeof(PPPHDR);

    protocol = ntohs(ph->prot_type);

    /*
     * Toss all non-LCP packets unless LCP is OPEN.
     */
    if (protocol != PPP_LCP && dcb->dcb_lcp_state != PPPS_OPENED) {
        NutNetBufFree(nb);
        return;
    }

    /*
     * Until we get past the authentication phase, toss all packets
     * except LCP, LQR and authentication packets.
     */
    if ( (dcb->dcb_auth_state != PAPCS_OPEN) &&
#ifdef CHAP_SUPPORT
         (dcb->dcb_auth_state != CHAPCS_OPEN) &&
#endif /* #ifdef CHAP_SUPPORT */
         !(protocol == PPP_LCP || protocol == PPP_LQR || protocol == PPP_PAP || protocol == PPP_CHAP)) {
        NutNetBufFree(nb);
        return;
    }

    /*
     * Route frame to the proper handler.
     */
    switch (protocol) {
    case PPP_IP:
        /* Internet protocol. */
        if(dcb->dcb_ipcp_state == PPPS_OPENED)
            NutIpInput(dev, nb);
        else
            NutNetBufFree(nb);
        break;

    case PPP_LCP:
        /* Link control protocol. */
        NutLcpInput(dev, nb);
        break;

    case PPP_IPCP:
        /* IP control protocol. */
        NutIpcpInput(dev, nb);
        break;

    case PPP_PAP:
        /* Password authentication protocol. */
        NutPapInput(dev, nb);
        break;

#ifdef CHAP_SUPPORT
    case PPP_CHAP:
        NutChapInput(dev, nb);
        break;
#endif

    default:
        LcpTxProtRej(dev, protocol, nb);
        break;
    }
}

/*@}*/

