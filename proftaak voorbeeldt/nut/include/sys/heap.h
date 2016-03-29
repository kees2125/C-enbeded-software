#ifndef _SYS_HEAP_H_
#define _SYS_HEAP_H_

/*
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
 * Portions Copyright (C) 2000 David J. Hudson <dave@humbug.demon.co.uk>
 *
 * This file is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You can redistribute this file and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software Foundation;
 * either version 2 of the License, or (at your discretion) any later version.
 * See the accompanying file "copying-gpl.txt" for more details.
 *
 * As a special exception to the GPL, permission is granted for additional
 * uses of the text contained in this file.  See the accompanying file
 * "copying-liquorice.txt" for details.
 */

/*
 * $Log: heap.h,v $
 * Revision 1.4  2006/10/06 23:19:08  hwmaier
 * Added include statement for cfg/memory.h so the macro NUTMEM_STACKHEAP is found when compiling for the AT90CAN128.
 *
 * Revision 1.3  2006/09/29 12:26:14  haraldkipp
 * All code should use dedicated stack allocation routines. For targets
 * allocating stack from the normal heap the API calls are remapped by
 * preprocessor macros.
 * Stack allocation code moved from thread module to heap module.
 * Adding static attribute to variable 'available' will avoid interference
 * with application code. The ugly format macros had been replaced by
 * converting all platform specific sizes to unsigned integers.
 *
 * Revision 1.2  2004/03/16 16:48:44  haraldkipp
 * Added Jan Dubiec's H8/300 port.
 *
 * Revision 1.1.1.1  2003/05/09 14:41:20  haraldkipp
 * Initial using 3.2.1
 *
 * Revision 1.7  2003/02/04 18:00:52  harald
 * Version 3 released
 *
 * Revision 1.6  2002/06/26 17:29:28  harald
 * First pre-release with 2.4 stack
 *
 */

#include <cfg/memory.h>
#include <sys/types.h>

/*!
 * \file sys/heap.h
 * \brief Heap management definitions.
 */

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \struct _HEAPNODE heap.h sys/heap.h
 * \brief Heap memory node information structure.
 */
/*!
 * \typedef HEAPNODE
 * \brief Heap memory node type.
 */
typedef struct _HEAPNODE {
    size_t hn_size;             /*!< \brief Size of this node. */
    struct _HEAPNODE *hn_next;  /*!< \brief Link to next free node. */
} HEAPNODE;

extern HEAPNODE* volatile heapFreeList;


/*!
 * \brief Allocation threshold.
 *
 * Might be increased to avoid creating
 * too many small nodes.
 */
#define ALLOC_THRESHOLD 6

extern void *NutHeapAlloc(size_t size);
extern void *NutHeapAllocClear(size_t size);
extern int NutHeapFree(void *block);
extern void NutHeapAdd(void *addr, size_t size);
extern size_t NutHeapAvailable(void);

#if defined (NUTMEM_STACKHEAP)

/* Dedicated stack memory. */
extern void *NutStackAlloc(size_t size);
extern int NutStackFree(void *block);
extern void NutStackAdd(void *addr, size_t size);

#else /* !NUTMEM_STACKHEAP */

/* Thread stacks resides in normal heap. */
#define NutStackAlloc(size)     NutHeapAlloc(size)
#define NutStackFree(block)     NutHeapFree(block)

#endif /* !NUTMEM_STACKHEAP */

#ifdef __cplusplus
}
#endif

#endif
