/*
 * <MFS> Modified for Streamit
 * use PPP-stack from 3.9.6
 *
 * Copyright (C) 2001-2003 by egnite Software GmbH. All rights reserved.
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
 * $Log: pppsm.c,v $
 * Revision 1.2  2003/08/14 15:14:19  haraldkipp
 * Do not increment ID when resending.
 * Added authentication retries.
 *
 * Revision 1.1.1.1  2003/05/09 14:41:37  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.2  2003/05/06 18:18:37  harald
 * PPP hack for simple UART support, functions reordered.
 *
 * Revision 1.1  2003/03/31 14:53:28  harald
 * Prepare release 3.1
 *
 */

#include <string.h>
#include <io.h>
#include <fcntl.h>

#include <cfg/os.h>
#include <cfg/ppp.h>
#include <dev/uart.h>

#include <sys/thread.h>
#include <sys/heap.h>
#include <sys/event.h>
#include <sys/timer.h>

#include <net/if_var.h>
#include <dev/ppp.h>

#include <netinet/if_ppp.h>
#include <netinet/ppp_fsm.h>

#ifdef NUTDEBUG
#include <net/netdebug.h>
#endif

/* Ugly hack to get simple UART drivers working. */
extern u_char ppp_hackup;
extern void NutPppEnable(NUTDEVICE *dev);

/*!
 * \addtogroup xgPPP
 */
/*@{*/

u_long new_magic = 0x12345678;

/*! \fn NutPppSm(void *arg)
 * \brief PPP state machine timeout thread.
 *
 * Handles timeouts for LCP and IPCP.
 */
THREAD(NutPppSm, arg)
{
    NUTDEVICE *dev = arg;
    PPPDCB *dcb = dev->dev_dcb;
    u_char retries;

    for (;;) {
        NutSleep(5000);
        new_magic++;

        retries = dcb->dcb_retries;

        /*
         * LCP timeouts.
         */
        switch (dcb->dcb_lcp_state) {
        case PPPS_CLOSING:
        case PPPS_STOPPING:

#ifdef NUTDEBUG
            if (__ppp_trf) {
                static prog_char dbgstr[] = "LCP timeout\n";
                fputs_P(dbgstr, __ppp_trs);
            }
#endif
            if(retries < 9) {
                if (retries) {
                    NutLcpOutput(dev, XCP_TERMREQ, dcb->dcb_reqid, 0);
                }
                dcb->dcb_retries = retries + 1;
            }
            else
                dcb->dcb_lcp_state = (dcb->dcb_lcp_state == PPPS_CLOSING) ? PPPS_CLOSED : PPPS_STOPPED;
            break;

        case PPPS_REQSENT:
        case PPPS_ACKSENT:

#ifdef NUTDEBUG
            if (__ppp_trf) {
                static prog_char dbgstr[] = "LCP timeout\n";
                fputs_P(dbgstr, __ppp_trs);
            }
#endif
            if(retries < 9) {
                if(retries)
                    LcpTxConfReq(dev, dcb->dcb_reqid);
                dcb->dcb_retries = retries + 1;
            }
            else
                dcb->dcb_lcp_state = PPPS_STOPPED;
            break;
        }

        /*
         * Authentication timeouts.
         */
        switch (dcb->dcb_auth_state) {
        case PAPCS_AUTHREQ:
#ifdef NUTDEBUG
            if (__ppp_trf) {
                static prog_char dbgstr[] = "Authentication timeout\n";
                fputs_P(dbgstr, __ppp_trs);
            }
#endif
            if(retries < 9) {
                if(retries)
                    PapTxAuthReq(dev, dcb->dcb_reqid);
                dcb->dcb_retries = retries + 1;
            } else
                dcb->dcb_lcp_state = PPPS_STOPPED;
            break;
#ifdef CHAP_SUPPORT
        case CHAPCS_AUTHREQ:
            if (retries < 9) {
                if (retries) {
                    //ChapTxAuthReq(dev, dcb->dcb_reqid);
                }
                dcb->dcb_retries = retries + 1;
            } else
                dcb->dcb_lcp_state = PPPS_STOPPED;
            break;
#endif /* #ifdef CHAP_SUPPORT */
        }

        /*
         * IPCP timeouts.
         */
        switch (dcb->dcb_ipcp_state) {
        case PPPS_CLOSING:
        case PPPS_STOPPING:
#ifdef NUTDEBUG
            if (__ppp_trf) {
                static prog_char dbgstr[] = "IPCP timeout\n";
                fputs_P(dbgstr, __ppp_trs);
            }
#endif
            if(retries < 9) {
                if (retries)
                    NutIpcpOutput(dev, XCP_TERMREQ, dcb->dcb_reqid, 0);
                dcb->dcb_retries = retries + 1;
            } else
                dcb->dcb_ipcp_state = (dcb->dcb_ipcp_state == PPPS_CLOSING) ? PPPS_CLOSED : PPPS_STOPPED;
            break;

        case PPPS_REQSENT:
        case PPPS_ACKSENT:
#ifdef NUTDEBUG
            if (__ppp_trf) {
                static prog_char dbgstr[] = "IPCP timeout\n";
                fputs_P(dbgstr, __ppp_trs);
            }
#endif
            if(retries < 9) {
                if (retries)
                    IpcpTxConfReq(dev, dcb->dcb_reqid);
                dcb->dcb_retries = retries + 1;
            } else
                dcb->dcb_ipcp_state = PPPS_STOPPED;
            break;
        }
    }
}


/*!
 * \brief Trigger LCP open event.
 *
 * Enable the link to come up. Typically triggered by the upper layer,
 * when it is enabled.
 *
 * \param dev Pointer to the NUTDEVICE structure of the PPP device.
 *
 */
void LcpOpen(NUTDEVICE *dev)
{
    PPPDCB *dcb = dev->dev_dcb;

#ifdef NUTDEBUG
    if (__ppp_trf) {
        static prog_char dbgstr[] = "[LCP-OPEN]\n";
        fputs_P(dbgstr, __ppp_trs);
    }
#endif

    switch (dcb->dcb_lcp_state) {
    case PPPS_INITIAL:
        /*
         * The LCP layer and the lower layer are down. Enable the LCP
         * layer. Link negotiation will start as soon as the lower
         * layer comes up.
         */
        dcb->dcb_lcp_state = PPPS_STARTING;
        break;

    case PPPS_CLOSED:
        /*
         * The LCP layer is down and the lower layer is up. Start
         * link negotiation by sending out a request.
         */
        LcpTxConfReq(dev, ++dcb->dcb_reqid);
        dcb->dcb_lcp_state = PPPS_REQSENT;
        break;

    case PPPS_CLOSING:
        /*
         * The LCP layer is going down.
         */
        dcb->dcb_lcp_state = PPPS_STOPPING;
        break;
    }
}

/*!
 * \brief Trigger LCP close event.
 *
 * Disable the link.
 *
 * \param dev Pointer to the NUTDEVICE structure of the PPP device.
 */
void LcpClose(NUTDEVICE *dev)
{
    PPPDCB *dcb = dev->dev_dcb;

#ifdef NUTDEBUG
    if (__ppp_trf) {
        static prog_char dbgstr[] = "[LCP-CLOSE]\n";
        fputs_P(dbgstr, __ppp_trs);
    }
#endif

    switch (dcb->dcb_lcp_state) {
    case PPPS_STARTING:
        /*
         * The LCP layer has been enabled, but the lower layer is still
         * down. Disable the link layer.
         */
        dcb->dcb_lcp_state = PPPS_INITIAL;
        break;

    case PPPS_STOPPED:
        dcb->dcb_lcp_state = PPPS_CLOSED;
        break;

    case PPPS_STOPPING:
        dcb->dcb_lcp_state = PPPS_CLOSING;
        break;

    case PPPS_REQSENT:
    case PPPS_ACKRCVD:
    case PPPS_ACKSENT:
    case PPPS_OPENED:
        /*
         * The LCP layer and the lower layer are up. Inform the upper
         * layer that we are going down and send out a termination
         * request.
         */
        dcb->dcb_lcp_state = PPPS_CLOSING;
        IpcpLowerDown(dev);
        NutLcpOutput(dev, XCP_TERMREQ, dcb->dcb_reqid, 0);
        break;
    }
}

/*!
 * \brief Trigger LCP lower up event.
 *
 * \param dev Pointer to the NUTDEVICE structure of the PPP device.
 *
 */
void LcpLowerUp(NUTDEVICE *dev)
{
    PPPDCB *dcb = dev->dev_dcb;

#ifdef NUTDEBUG
    if (__ppp_trf) {
        static prog_char dbgstr[] = "[LCP-LOWERUP]\n";
        fputs_P(dbgstr, __ppp_trs);
    }
#endif

    switch (dcb->dcb_lcp_state) {
    case PPPS_INITIAL:
        /*
         * The LCP layer is still disabled.
         */
        dcb->dcb_lcp_state = PPPS_CLOSED;
        break;

    case PPPS_STARTING:
        /*
         * The LCP layer is enabled. Send a configuration request.
         */
        LcpTxConfReq(dev, ++dcb->dcb_reqid);
        dcb->dcb_lcp_state = PPPS_REQSENT;
        break;
    }
}

/*!
 * \brief Trigger LCP lower down event.
 *
 * \param dev Pointer to the NUTDEVICE structure of the PPP device.
 *
 */
void LcpLowerDown(NUTDEVICE *dev)
{
    PPPDCB *dcb = dev->dev_dcb;

#ifdef NUTDEBUG
    if (__ppp_trf) {
        static prog_char dbgstr[] = "[LCP-LOWERDOWN]\n";
        fputs_P(dbgstr, __ppp_trs);
    }
#endif

    switch (dcb->dcb_lcp_state) {
    case PPPS_CLOSED:
        dcb->dcb_lcp_state = PPPS_INITIAL;
#ifdef NUTDEBUG
        /* Michel Hendriks */
        /* Note to myself: we are really down here */
        if (__ppp_trf) {
            static prog_char dbgstr[] = "End of PPP\n";
            fputs_P(dbgstr, __ppp_trs);
        }
#endif
        break;

    case PPPS_STOPPED:
        dcb->dcb_lcp_state = PPPS_STARTING;
        break;

    case PPPS_CLOSING:
        dcb->dcb_lcp_state = PPPS_INITIAL;
        break;

    case PPPS_STOPPING:
    case PPPS_REQSENT:
    case PPPS_ACKRCVD:
    case PPPS_ACKSENT:
        dcb->dcb_lcp_state = PPPS_STARTING;
        break;

    case PPPS_OPENED:
        IpcpLowerDown(dev);
        dcb->dcb_lcp_state = PPPS_STARTING;
        break;
    }
}

/*!
 * \brief Trigger IPCP open event.
 *
 * Link is allowed to come up.
 * \param dev Pointer to the NUTDEVICE structure of the PPP device.
 *
 */
void IpcpOpen(NUTDEVICE *dev)
{
    PPPDCB *dcb = dev->dev_dcb;

#ifdef NUTDEBUG
    if (__ppp_trf) {
        static prog_char dbgstr[] = "[IPCP-OPEN]\n";
        fputs_P(dbgstr, __ppp_trs);
    }
#endif

    switch (dcb->dcb_ipcp_state) {
    case PPPS_INITIAL:
        /*
         * The IPCP layer and the lower layer are down. Enable the
         * IPCP layer and the lower layer.
         */
        dcb->dcb_ipcp_state = PPPS_STARTING;
        LcpOpen(dev);
        break;

    case PPPS_CLOSED:
        IpcpTxConfReq(dev, ++dcb->dcb_reqid);
        dcb->dcb_ipcp_state = PPPS_REQSENT;
        break;

    case PPPS_CLOSING:
        dcb->dcb_ipcp_state = PPPS_STOPPING;
        break;
    }
}

/*!
 * \brief Trigger IPCP close event.
 *
 * Disable the link.
 *
 * Cancel timeouts and either initiate close or possibly go directly to
 * the PPPS_CLOSED state.
 *
 * \param dev Pointer to the NUTDEVICE structure of the PPP device.
 */
void IpcpClose(NUTDEVICE *dev)
{
    PPPDCB *dcb = dev->dev_dcb;

#ifdef NUTDEBUG
    if (__ppp_trf) {
        static prog_char dbgstr[] = "[IPCP-CLOSE]\n";
        fputs_P(dbgstr, __ppp_trs);
    }
#endif

    switch (dcb->dcb_ipcp_state) {
    case PPPS_STARTING:
        /*
         * The IPCP layer has been enabled, but the lower layer is still
         * down. Disable the network layer.
         */
        dcb->dcb_ipcp_state = PPPS_INITIAL;
        break;

    case PPPS_STOPPED:
        dcb->dcb_ipcp_state = PPPS_CLOSED;
        break;

    case PPPS_STOPPING:
        dcb->dcb_ipcp_state = PPPS_CLOSING;
        break;

    case PPPS_REQSENT:
    case PPPS_ACKRCVD:
    case PPPS_ACKSENT:
    case PPPS_OPENED:
        /*
         * The IPCP layer and the lower layer are up. Inform the upper
         * layer that we are going down and send out a termination
         * request.
         */
        NutEventPost(&dcb->dcb_state_chg); /* mhe: in newer versions the post is done _after_ the state change */
        NutIpcpOutput(dev, XCP_TERMREQ, dcb->dcb_reqid, 0);
        dcb->dcb_ipcp_state = PPPS_CLOSING;
        break;
    }
}

/*
 * The lower layer is up.
 * \param dev Pointer to the NUTDEVICE structure of the PPP device.
 *
 */
void IpcpLowerUp(NUTDEVICE *dev)
{
    PPPDCB *dcb = dev->dev_dcb;

#ifdef NUTDEBUG
    if (__ppp_trf) {
        static prog_char dbgstr[] = "[IPCP-LOWERUP]\n";
        fputs_P(dbgstr, __ppp_trs);
    }
#endif

    switch (dcb->dcb_ipcp_state) {
    case PPPS_INITIAL:
        dcb->dcb_ipcp_state = PPPS_CLOSED;
        break;

    case PPPS_STARTING:
        IpcpTxConfReq(dev, ++dcb->dcb_reqid);
        dcb->dcb_ipcp_state = PPPS_REQSENT;
        break;
    }
}

/*
 * The link layer is down.
 *
 * Cancel all timeouts and inform upper layers.
 * \param dev Pointer to the NUTDEVICE structure of the PPP device.
 *
 */
void IpcpLowerDown(NUTDEVICE *dev)
{
    PPPDCB *dcb = dev->dev_dcb;

#ifdef NUTDEBUG
    if (__ppp_trf) {
        static prog_char dbgstr[] = "[IPCP-LOWERDOWN]\n";
        fputs_P(dbgstr, __ppp_trs);
    }
#endif

    switch (dcb->dcb_ipcp_state) {
    case PPPS_CLOSED:
        dcb->dcb_ipcp_state = PPPS_INITIAL;
        _ioctl(dcb->dcb_fd, HDLC_SETIFNET, &dev);
        /* Ugly hack to get simple UART drivers working. */
        ppp_hackup = 0;
        break;

    case PPPS_STOPPED:
        dcb->dcb_ipcp_state = PPPS_STARTING;
        break;

    case PPPS_CLOSING:
        dcb->dcb_ipcp_state = PPPS_INITIAL;
        break;

    case PPPS_STOPPING:
    case PPPS_REQSENT:
    case PPPS_ACKRCVD:
    case PPPS_ACKSENT:
        dcb->dcb_ipcp_state = PPPS_STARTING;
        break;

    case PPPS_OPENED:
        NutEventPost(&dcb->dcb_state_chg); /* mhe: in newer versions the post is done _after_ the state change */
        dcb->dcb_ipcp_state = PPPS_STARTING;
        break;
    }
}


/*!
 * \brief Perform PPP control functions.
 *
 * \param dev  Identifies the device that receives the device-control
 *             function.
 * \param req  Requested control function. May be set to one of the
 *             following constants:
 *             - LCP_OPEN
 *             - LCP_CLOSE
 *             - LCP_LOWERUP
 *             - LCP_LOWERDOWN
 *             Any other function will be passed to the physical driver.
 *
 * \param conf Points to a buffer that contains any data required for
 *             the given control function or receives data from that
 *             function.
 * \return 0 on success, -1 otherwise.
 *
 */
int NutPppIOCtl(NUTDEVICE * dev, int req, void *conf)
{
    int rc = 0;

#ifdef NUTDEBUG
        if (__ppp_trf) {
            static prog_char dbgstr[] = "NutPppIOCtl\n";
            fputs_P(dbgstr, __ppp_trs);
        }
#endif

    switch (req) {
    case LCP_OPEN:
        LcpOpen(dev);
        break;

    case LCP_CLOSE:
        LcpClose(dev);
        break;

    case LCP_LOWERUP:
        LcpLowerUp(dev);
        break;

    case LCP_LOWERDOWN:
        LcpLowerDown(dev);
        break;

    default:
        {
#ifdef NUTDEBUG
            if (__ppp_trf) {
                static prog_char dbgstr[] = "NutPppIOCtl to uart\n";
                fputs_P(dbgstr, __ppp_trs);
            }
#endif
            //PPPDCB *dcb = dev->dev_dcb;
            rc = _ioctl(((PPPDCB *)(dev->dev_dcb))->dcb_fd, req, conf);
            //rc = (*dcb->dcb_pdev->dev_ioctl)(dcb->dcb_pdev, req, conf);
        }
        break;
    }
    return rc;
}

/*
 * \brief Enable the link layer to come up.
 *
 * The underlying hardware driver should have established a physical
 * connection before calling this function.
 *
 * \param name Physical device name optionally followed by username
 *             and password, each separated by a slash.
 *
 */
NUTFILE *NutPppOpen(NUTDEVICE * dev, CONST char *name, int mode, int acc)
{
    NUTFILE *fp;
    u_char i;
    char *cp;
    char *sp;
    char pdn[9];
    PPPDCB *dcb = dev->dev_dcb;

    memset(dcb, 0, sizeof(PPPDCB));

    /*
     * Determine the physical device.
     */
    for(cp = (char *)name, i = 0; *cp && *cp != '/' && i < sizeof(pdn) - 1; i++)
        pdn[i] = *cp++;
    pdn[i] = 0;

    if((dcb->dcb_fd = _open(pdn, _O_RDWR | _O_BINARY)) == -1)
        return NUTFILE_EOF;
    //if((dcb->dcb_pdev = NutDeviceLookup(pdn)) == 0)
    //    return NUTFILE_EOF;

    /*
     * Allocate a file structure to return.
     */
    if ((fp = NutHeapAlloc(sizeof(NUTFILE))) == 0)
        return NUTFILE_EOF;
    fp->nf_next = 0;
    fp->nf_dev = dev;
    fp->nf_fcb = 0;

    /*
     * Parse user name and password.
     */
    if(*cp == '/') {
        for(sp = ++cp, i = 0; *sp && *sp != '/'; sp++, i++);
        if(i) {
            dcb->dcb_user = NutHeapAlloc(i + 1);
            for(sp = (char*)dcb->dcb_user; *cp && *cp != '/'; )
                *sp++ = *cp++;
            *sp = 0;
        }
        if(*cp == '/') {
            for(sp = ++cp, i = 0; *sp && *sp != '/'; sp++, i++);
            if(i) {
                dcb->dcb_pass = NutHeapAlloc(i + 1);
                for(sp = (char*)dcb->dcb_pass; *cp && *cp != '/'; )
                    *sp++ = *cp++;
                *sp = 0;
            }
        }
    }

    /*
     * Enable all layers to come up.
     */
    IpcpOpen(dev);

    return fp;
}


/*
 * Start closing connection.
 */
int NutPppClose(NUTFILE *fp)
{
    PPPDCB *dcb = fp->nf_dev->dev_dcb;

#ifdef NUTDEBUG
    if (__ppp_trf) {
        static prog_char dbgstr[] = "NutPppClose\n";
        fputs_P(dbgstr, __ppp_trs);
    }
#endif

    IpcpClose(fp->nf_dev);
//    _close(dcb->dcb_fd);  // Solves memory leak bug Johan

    if(dcb->dcb_user)
        NutHeapFree(dcb->dcb_user);
    if(dcb->dcb_pass)
        NutHeapFree(dcb->dcb_pass);
    NutHeapFree(fp);

    return 0;
}

/*!
 * \brief Initialize the PPP state machine.
 *
 */
int NutPppInit(NUTDEVICE *dev)
{
    /* Michel Hendriks. Check for already running thread */
    /*
     * Start the timer thread if not already running.
     */
    if (GetThreadByName("pppsm") == NULL)
        NutThreadCreate("pppsm", NutPppSm, dev, 512);
    NutPppEnable(dev);

    return 0;
}

/*@}*/
