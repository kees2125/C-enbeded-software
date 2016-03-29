/*
 * <MFS> Modified for Streamit
 * use PPP-stack from 3.9.6
 *
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
 * Portions are 
 * Copyright (c) 1989 by Carnegie Mellon University.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by Carnegie Mellon University.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
 * $Log: lcpin.c,v $
 * Revision 1.2  2003/08/14 15:19:15  haraldkipp
 * Echo support added.
 *
 * Revision 1.1.1.1  2003/05/09 14:41:34  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.2  2003/05/06 18:14:45  harald
 * Cleanup
 *
 * Revision 1.1  2003/03/31 14:53:27  harald
 * Prepare release 3.1
 *
 */

#include <cfg/os.h>
#include <cfg/ppp.h>

#include <net/if_var.h>
#include <dev/ppp.h>

#include <netinet/if_ppp.h>
#include <netinet/ppp_fsm.h>

#include <string.h>

#ifdef NUTDEBUG
#include <net/netdebug.h>
#endif

/* Only when using NutSleep */
#include <sys/timer.h>

/*!
 * \addtogroup xgLCP
 *
 */
/*@{*/

extern u_long new_magic;

/*
 * Received Configure-Request.
 */
static void LcpRxConfReq(NUTDEVICE * dev, u_char id, NETBUF * nb)
{
    PPPDCB *dcb = dev->dev_dcb;
    int rc = XCP_CONFACK;
    XCPOPT *xcpo;
    u_short xcpl;
    XCPOPT *xcpr;
    u_short xcps;
    u_short len = 0;
    u_short sval;
    u_short i;


    switch (dcb->dcb_lcp_state) {
    case PPPS_CLOSED:
        /*
         * Go away, we're closed. 
         */
        NutNetBufFree(nb);
        NutLcpOutput(dev, XCP_TERMACK, id, 0);
        return;

    case PPPS_CLOSING:
    case PPPS_STOPPING:
        /*
         * Silently ignore configuration requests while going down.
         */
        NutNetBufFree(nb);
        return;

    case PPPS_OPENED:
        /* 
         * Go down and restart negotiation.
         */
        IpcpLowerDown(dev);
        LcpTxConfReq(dev, ++dcb->dcb_reqid);
        break;

    case PPPS_STOPPED:
        /* 
         * Negotiation started by our peer.
         */
        LcpTxConfReq(dev, ++dcb->dcb_reqid);
        dcb->dcb_lcp_state = PPPS_REQSENT;
        break;
    }

    /*
     * Check if there is anything to reject.
     */
    xcpo = nb->nb_ap.vp;
    xcpl = nb->nb_ap.sz;
    xcpr = nb->nb_ap.vp;
    xcps = 0;
    while (xcpl >= 2) {
        /* Michel Hendriks */
        int reject = 1;

        len  = xcpo->xcpo_len;
        if(len > xcpl)
            len = xcpl;
        else {
            switch (xcpo->xcpo_type) {
            case LCP_MRU:
                if (len == 4) 
                    reject = 0;
                break;
            case LCP_MAGICNUMBER:
            case LCP_ASYNCMAP:
                if (len == 6)
                    reject = 0;
                break;
            case LCP_AUTHTYPE:
                if (len >= 4) 
                    reject = 0;
                break;
            }
        }


        if (reject) {
#ifdef NUTDEBUG
            if (__ppp_trf) {
                static prog_char dbgstr[] = "LcpReject xcpr=%p, xcpo=%p, len=%u, xlen=%u, xcpl=%u, xcps=%u\n";
                fprintf_P(__ppp_trs, dbgstr, xcpr, xcpo, len, xcpo->xcpo_len, xcpl, xcps);
            }
#endif
            if(xcpr != xcpo) {
                /*
                 * Move all remaining data to the front
                 */
                /* Michel Hendriks */
                char *dst = (char *)xcpr;
                char *src = (char *)xcpo;
                for(i = 0; i < xcpl; i++) {
                    *dst++ = *src++;
                }
                /*
                 * Move the original pointer by 
                 * the shifted amount
                 */
                xcpo -= xcpo - xcpr;
            }
            xcpr = (XCPOPT *)((char *)xcpr + len);
            xcps += len;
        }
        xcpl -= len;
        xcpo = (XCPOPT *)((char *)xcpo + len);
    }

    if(xcps) {
        nb->nb_ap.sz = xcps;
        rc = XCP_CONFREJ;
    }

    /*
     * Check if there is anything to negotiate.
     */
    else {
        xcpo = nb->nb_ap.vp;
        xcpl = nb->nb_ap.sz;
        xcpr = nb->nb_ap.vp;
        xcps = 0;
        while(xcpl >= 2) {
            len = 0;
            switch (xcpo->xcpo_type) {
            case LCP_MRU:
                if((sval = htons(xcpo->xcpo_.us)) < MIN_LCPMRU) {
                    len = 4;
                    xcpr->xcpo_.us = ntohs(MIN_LCPMRU);
                } else
                    dcb->dcb_rem_mru = sval;
                break;
            case LCP_ASYNCMAP:
                dcb->dcb_accm = ntohl(xcpo->xcpo_.ul);
                break;
            case LCP_AUTHTYPE: // Modified Eduardo
#ifdef CHAP_SUPPORT
                if ( (xcpo->xcpo_.us == htons(PPP_CHAP)) ) {
                    dcb->dcb_auth = PPP_CHAP;
                } else if(xcpo->xcpo_.us == htons(PPP_PAP)) {
                    dcb->dcb_auth = PPP_PAP;
                } else {
                    len = 4;
                    xcpr->xcpo_.us = htons(PPP_PAP);
                }
#else
                /*
                 * This extra sleep seems to be needed when using the combination
                 * GCC 3.4.5 and no CHAP support
                 *
                 * If this sleep is removed, PPP crashes when dialing in to a network
                 * that suggests to use CHAP
                 */
                NutSleep(63);
                if(xcpo->xcpo_.us != htons(PPP_PAP)) {
                    len = 4;
                    xcpr->xcpo_.us = htons(PPP_PAP);
                }
#endif /* #ifdef CHAP_SUPPORT */
                break;
            case LCP_MAGICNUMBER:
                if(xcpo->xcpo_.ul == dcb->dcb_loc_magic || 
                   xcpo->xcpo_.ul == dcb->dcb_neg_magic) {
                    dcb->dcb_rem_magic = new_magic;
                    len = 6;
                    xcpr->xcpo_.ul = dcb->dcb_rem_magic;
                }
                break;
            case LCP_PCOMPRESSION:
                dcb->dcb_compr |= PPP_PFC;
                break;
            case LCP_ACCOMPRESSION:
                dcb->dcb_compr |= PPP_ACFC;
                break;
            }

            if (len) {
                if(xcpr != xcpo) {
                    xcpr->xcpo_type = xcpo->xcpo_type;
                    xcpr->xcpo_len = len;
                }
                xcpr = (XCPOPT *)((char *)xcpr + len);
                xcps += len;
                len = 0;
            }
            if(len) {
                xcpl -= len;
                xcpo = (XCPOPT *) ((char *) xcpo + len);
            } else {
                   xcpl -= xcpo->xcpo_len;
                   xcpo = (XCPOPT *) ((char *) xcpo + xcpo->xcpo_len);
            }
        }
        if(xcps) {
            nb->nb_ap.sz = xcps;
            rc = XCP_CONFNAK;
        }
    }

    NutLcpOutput(dev, rc, id, nb);

    if (rc == XCP_CONFACK) {
        if (dcb->dcb_lcp_state == PPPS_ACKRCVD) {
            dcb->dcb_lcp_state = PPPS_OPENED;
            switch (dcb->dcb_auth) {
            case PPP_PAP:
                PapTxAuthReq(dev, ++dcb->dcb_reqid);
                break;
#ifdef CHAP_SUPPORT
            case PPP_CHAP:
                //ChapTxAuthReq(dev, ++dcb->dcb_reqid);
                break;
#endif /* #ifdef CHAP_SUPPORT */
            default:
                IpcpLowerUp(dev);
                break;
            }
        } else
            dcb->dcb_lcp_state = PPPS_ACKSENT;
        dcb->dcb_lcp_naks = 0;
    } else if (dcb->dcb_lcp_state != PPPS_ACKRCVD)
        dcb->dcb_lcp_state = PPPS_REQSENT;
}

/*
 * Configure-Ack received.
 * Never called in INITIAL or STARTING phase.
 */
static void LcpRxConfAck(NUTDEVICE * dev, u_char id, NETBUF * nb)
{
    PPPDCB *dcb = dev->dev_dcb;
    XCPOPT *xcpo;
    u_short xcpl;

    /*
     * Check if this is a valid ack.
     */
    if (id == dcb->dcb_reqid && dcb->dcb_acked == 0) {
        dcb->dcb_acked = 1;
        xcpo = nb->nb_ap.vp;
        xcpl = nb->nb_ap.sz;
        while(xcpl >= 2) {
            switch (xcpo->xcpo_type) {
            case LCP_MRU:
                if(htons(xcpo->xcpo_.us) != 1500)
                    dcb->dcb_acked = 0;
                break;
            case LCP_ASYNCMAP:
                //if(ntohl(xcpo->xcpo_.ul) != )
                //    dcb->dcb_acked = 0;
                break;
            case LCP_AUTHTYPE:
                if(htons(xcpo->xcpo_.us) != dcb->dcb_auth)
                    dcb->dcb_acked = 0;
                break;
            case LCP_MAGICNUMBER:
                if(xcpo->xcpo_.ul == dcb->dcb_neg_magic) {
                    dcb->dcb_loc_magic = dcb->dcb_neg_magic;
                } else {
                    dcb->dcb_acked = 0;
                }
                break;
            case LCP_PCOMPRESSION:
                dcb->dcb_acked = 0;
                break;
            case LCP_ACCOMPRESSION:
                dcb->dcb_acked = 0;
                break;
            }
            xcpl -= xcpo->xcpo_len;
            xcpo = (XCPOPT *)((char *)xcpo + xcpo->xcpo_len);
        }
    }

    /*
     * We don't need the NETBUF any more.
     */
    NutNetBufFree(nb);

    /*
     * Ignore invalid acks.
     */
    if (dcb->dcb_acked == 0)
        return;

    switch (dcb->dcb_lcp_state) {
    case PPPS_CLOSED:
    case PPPS_STOPPED:
        /*
         * Go away, we're closed. 
         */
        NutLcpOutput(dev, XCP_TERMACK, id, 0);
        break;

    case PPPS_REQSENT:
        dcb->dcb_lcp_state = PPPS_ACKRCVD;
        dcb->dcb_retries = 0;
        break;

    case PPPS_ACKRCVD:
        LcpTxConfReq(dev, ++dcb->dcb_reqid);
        dcb->dcb_lcp_state = PPPS_REQSENT;
        break;

    case PPPS_ACKSENT:
        /*
         * ACK sent and ACK received.
         */
        dcb->dcb_lcp_state = PPPS_OPENED;

        switch (dcb->dcb_auth) {
        case PPP_PAP:
            PapTxAuthReq(dev, ++dcb->dcb_reqid);
            break;
#ifdef CHAP_SUPPORT
        case PPP_CHAP:
            //ChapTxAuthReq(dev, ++dcb->dcb_reqid);
            break;
#endif /* #ifdef CHAP_SUPPORT */
        default:
            IpcpLowerUp(dev);
            break;
        }
        break;

    case PPPS_OPENED:
        /* 
         * Go down and restart negotiation.
         */
        IpcpLowerDown(dev);
        LcpTxConfReq(dev, ++dcb->dcb_reqid);
        dcb->dcb_lcp_state = PPPS_REQSENT;
        break;
    }
}

/*
 * Configure-Nak or Configure-Reject received.
 */
static void LcpRxConfNakRej(NUTDEVICE * dev, u_char id, NETBUF * nb)
{
    PPPDCB *dcb = dev->dev_dcb;
    int ret = 0;

    XCPOPT *xcpo;
    u_short xcpl;

    /*
     * Ignore, if we are not expecting this id.
     */
    if (id != dcb->dcb_reqid || dcb->dcb_acked) {
        NutNetBufFree(nb);
        return;
    }

    /*
     * TODO: Process acked options.
     */
    dcb->dcb_acked = 1;

    xcpo = nb->nb_ap.vp;
    xcpl = nb->nb_ap.sz;
    while(xcpl >= 2) {
        xcpl -= xcpo->xcpo_len;
        xcpo = (XCPOPT *)((char *)xcpo + xcpo->xcpo_len);
    }

    NutNetBufFree(nb);

    switch (dcb->dcb_lcp_state) {
    case PPPS_CLOSED:
    case PPPS_STOPPED:
        /*
         * Go away, we're closed. 
         */
        NutLcpOutput(dev, XCP_TERMACK, id, 0);
        break;

    case PPPS_REQSENT:
    case PPPS_ACKSENT:
        /* They didn't agree to what we wanted - try another request */
        if (ret < 0)
            dcb->dcb_lcp_state = PPPS_STOPPED;       /* kludge for stopping CCP */
        else
            LcpTxConfReq(dev, ++dcb->dcb_reqid);
        break;

    case PPPS_ACKRCVD:
        /* Got a Nak/reject when we had already had an Ack?? oh well... */
        LcpTxConfReq(dev, ++dcb->dcb_reqid);
        dcb->dcb_lcp_state = PPPS_REQSENT;
        break;

    case PPPS_OPENED:
        /* 
         * Go down and restart negotiation.
         */
        IpcpLowerDown(dev);
        LcpTxConfReq(dev, ++dcb->dcb_reqid);
        dcb->dcb_lcp_state = PPPS_REQSENT;
        break;
    }
}

/*
 * Terminate-Request received.
 */
static void LcpRxTermReq(NUTDEVICE * dev, u_char id, NETBUF * nb)
{
    PPPDCB *dcb = dev->dev_dcb;

    NutNetBufFree(nb);

#ifdef NUTDEBUG
    if (__ppp_trf) {
        static prog_char dbgstr[] = "LcpRxTermReq, state = %u\n";
        fprintf_P(__ppp_trs, dbgstr, dcb->dcb_lcp_state);
    }
#endif
    switch (dcb->dcb_lcp_state) {
    case PPPS_ACKRCVD:
    case PPPS_ACKSENT:
        dcb->dcb_lcp_state = PPPS_REQSENT;   /* Start over but keep trying */
        break;

    case PPPS_OPENED:
        IpcpLowerDown(dev);
        dcb->dcb_lcp_state = PPPS_STOPPING;
        break;
    }
#ifdef NUTDEBUG
    if (__ppp_trf) {
        static prog_char dbgstr[] = "LcpRxTermReq, new state = %u\n";
        fprintf_P(__ppp_trs, dbgstr, dcb->dcb_lcp_state);
    }
#endif
    NutLcpOutput(dev, XCP_TERMACK, id, 0);
}

/*
 * Terminate-Ack received.
 */
static void LcpRxTermAck(NUTDEVICE * dev, u_char id, NETBUF * nb)
{
    PPPDCB *dcb = dev->dev_dcb;

    NutNetBufFree(nb);

#ifdef NUTDEBUG
    if (__ppp_trf) {
        static prog_char dbgstr[] = "LcpRxTermAck, state = %u\n";
        fprintf_P(__ppp_trs, dbgstr, dcb->dcb_lcp_state);
    }
#endif
    switch (dcb->dcb_lcp_state) {
    case PPPS_CLOSING:
        dcb->dcb_lcp_state = PPPS_CLOSED;
        break;
    case PPPS_STOPPING:
        dcb->dcb_lcp_state = PPPS_STOPPED;
        break;

    case PPPS_ACKRCVD:
        dcb->dcb_lcp_state = PPPS_REQSENT;
        break;

    case PPPS_OPENED:
        IpcpLowerDown(dev);
        LcpTxConfReq(dev, ++dcb->dcb_reqid);
        break;
    }
#ifdef NUTDEBUG
    if (__ppp_trf) {
        static prog_char dbgstr[] = "LcpRxTermAck, new state = %u\n";
        fprintf_P(__ppp_trs, dbgstr, dcb->dcb_lcp_state);
    }
#endif
}

/*
 * Peer doesn't speak this protocol.
 *
 * Treat this as a catastrophic error (RXJ-).
 */
void LcpRxProtRej(NUTDEVICE *dev)
{
    PPPDCB *dcb = dev->dev_dcb;

    switch (dcb->dcb_lcp_state) {
    case PPPS_CLOSING:
    case PPPS_CLOSED:
        dcb->dcb_lcp_state = PPPS_CLOSED;
        break;

    case PPPS_STOPPING:
    case PPPS_REQSENT:
    case PPPS_ACKRCVD:
    case PPPS_ACKSENT:
    case PPPS_STOPPED:
        dcb->dcb_lcp_state = PPPS_STOPPED;
        break;

    case PPPS_OPENED:
        IpcpLowerDown(dev);
        NutIpcpOutput(dev, XCP_TERMREQ, dcb->dcb_reqid, 0);
        dcb->dcb_lcp_state = PPPS_STOPPING;
        break;
    }
}

/*
 * Received a Code-Reject.
 */
static void LcpRxCodeRej(NUTDEVICE * dev, u_char id, NETBUF * nb)
{
    PPPDCB *dcb = dev->dev_dcb;

    NutNetBufFree(nb);

    if (dcb->dcb_lcp_state == PPPS_ACKRCVD)
        dcb->dcb_lcp_state = PPPS_REQSENT;
}

/*
 * Received an Echo-Request.
 */
static void LcpRxEchoReq(NUTDEVICE * dev, u_char id, NETBUF * nb)
{
    PPPDCB *dcb = dev->dev_dcb;

    if (dcb->dcb_lcp_state != PPPS_OPENED) {
        NutNetBufFree(nb);
    } else {
        /* Use local magic number. */
        memcpy(nb->nb_ap.vp, &dcb->dcb_loc_magic, sizeof(u_long));
        NutLcpOutput(dev, LCP_ERP, id, nb);
    }
}



/*!
 * \brief Handle incoming LCP packets.
 *
 * Packets not destined to us or packets with unsupported 
 * address type or item length are silently discarded.
 *
 * \note This routine is called by the Ethernet layer on
 *       incoming ARP packets. Applications typically do
 *       not call this function.
 *
 * \param dev Identifies the device that received the packet.
 * \param nb  Pointer to a network buffer structure containing 
 *            the ARP packet.
 */
void NutLcpInput(NUTDEVICE * dev, NETBUF * nb)
{
    XCPHDR *lcp;
    PPPDCB *dcb = dev->dev_dcb;
    u_short len;

    /*
     * Discard packets with illegal lengths.
     */
    if (nb->nb_nw.sz < sizeof(XCPHDR)) {
        NutNetBufFree(nb);
        return;
    }
    lcp = (XCPHDR *) nb->nb_nw.vp;
    if ((len = htons(lcp->xch_len)) < sizeof(XCPHDR) || len > nb->nb_nw.sz) {
        NutNetBufFree(nb);
        return;
    }

    /*
     * Discard all packets while we are in initial or starting state.
     */
    if (dcb->dcb_lcp_state == PPPS_INITIAL || dcb->dcb_lcp_state == PPPS_STARTING) {
        NutNetBufFree(nb);
        return;
    }

    /*
     * Split the LCP packet.
     */
    nb->nb_ap.vp = lcp + 1;
    nb->nb_ap.sz = htons(lcp->xch_len) - sizeof(XCPHDR);

    /* Michel Hendriks */
    nb->nb_nw.sz = sizeof(XCPHDR);

    /*
     * Action depends on code.
     */
    switch (lcp->xch_code) {
    case XCP_CONFREQ:
        LcpRxConfReq(dev, lcp->xch_id, nb);
        break;

    case XCP_CONFACK:
        LcpRxConfAck(dev, lcp->xch_id, nb);
        break;

    case XCP_CONFNAK:
    case XCP_CONFREJ:
        LcpRxConfNakRej(dev, lcp->xch_id, nb);
        break;

    case XCP_TERMREQ:
        LcpRxTermReq(dev, lcp->xch_id, nb);
        break;

    case XCP_TERMACK:
        LcpRxTermAck(dev, lcp->xch_id, nb);
        break;

    case XCP_CODEREJ:
        LcpRxCodeRej(dev, lcp->xch_id, nb);
        break;

    case LCP_ERQ:
        LcpRxEchoReq(dev, lcp->xch_id, nb);
        break;

    case LCP_ERP:
    case LCP_DRQ:
        /* Silently ignore echo responses and discard requests. */
        NutNetBufFree(nb);
        break;

    default:
        /*
         * TODO: Send code reject.
         */
        NutNetBufFree(nb);
        break;
    }
}

/*@}*/
