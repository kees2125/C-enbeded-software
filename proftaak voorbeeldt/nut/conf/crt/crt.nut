--
-- Copyright (C) 2004 by egnite Software GmbH. All rights reserved.
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

-- Operating system functions
--
-- $Log: crt.nut,v $
-- Revision 1.8  2005/08/05 11:25:01  olereinhardt
-- Added seek.c, tell.c
--
-- Revision 1.7  2005/06/26 11:33:31  chaac
-- Added forgotten kbhit.c to build process.
--
-- Revision 1.6  2004/11/24 14:48:34  haraldkipp
-- crt/crt.nut
--
-- Revision 1.5  2004/09/07 19:09:53  haraldkipp
-- Split program space I/O
--
-- Revision 1.4  2004/08/18 16:05:38  haraldkipp
-- Use consistent directory structure
--
-- Revision 1.3  2004/08/18 13:46:09  haraldkipp
-- Fine with avr-gcc
--
-- Revision 1.2  2004/08/03 15:09:31  haraldkipp
-- Another change of everything
--
-- Revision 1.1  2004/06/07 16:35:53  haraldkipp
-- First release
--
--

nutcrt =
{
    --
    -- Low level I/O
    --
    {
        name = "nutcrt_file",
        brief = "Low level file I/O",
        requires = { "DEV_FILE" },
        provides = { "CRT_FILE" },
        sources = 
        { 
            "close.c",
            "clrerr.c",
            "ioctl.c",
            "open.c",
        }
    },
    {
        name = "nutcrt_read",
        brief = "Low level file read",
        requires = { "DEV_READ", "CRT_FILE" },
        provides = { "CRT_READ" },
        sources = 
        { 
            "getf.c",
            "read.c",
        }
    },
    {
        name = "nutcrt_write",
        brief = "Low Level Write",
        requires = { "DEV_WRITE", "CRT_FILE" },
        provides = { "CRT_WRITE" },
        sources = 
        { 
            "putf.c",
            "write.c"
        }
    },
    {
        name = "nutcrt_write_p",
        brief = "Low Level Write (PGM-Space)",
        requires = { "HW_MCU_AVR", "DEV_WRITE", "CRT_FILE" },
        provides = { "CRT_WRITE_P" },
        sources = 
        { 
            "write_p.c"
        }
    },

    --
    -- Stream I/O
    --
    {
        name = "nutcrt_stream",
        brief = "File Streams",
        provides = { "CRT_STREAM" },
        sources = 
        { 
            "fclose.c",
            "fcloseall.c",
            "fdopen.c",
            "feof.c",
            "ferror.c",
            "fflush.c",
            "filelength.c",
            "fileno.c",
            "flushall.c",
            "fmode.c",
            "fopen.c",
            "fpurge.c",
            "freopen.c",
            "fseek.c",
            "ftell.c",
            "seek.c",
            "tell.c"
        },
        options = 
        {
            {
                macro = "STDIO_FLOATING_POINT",
                brief = "Floating point",
                description = "Enables floating point support for standard input/output.", 
                flavor = "boolean",
                file = "include/cfg/crt.h"
            },
            {
                macro = "FOPEN_MAX",
                brief = "Max. Open Streams",
                description = "Maximum number of concurrently opened streams.\n\n"..
                              "Default is 8",
                type = "integer",
                flavor = "booldata",
                file = "include/cfg/crt.h"
            }
        }

    },
    {
        name = "nutcrt_istream",
        brief = "File Stream Input",
        provides = { "CRT_STREAM_READ" },
        sources = 
        { 
            "fgetc.c",
            "fgets.c",
            "fread.c",
            "fscanf.c",
            "getc.c",
            "getchar.c",
            "gets.c",
            "kbhit.c",
            "scanf.c",
            "ungetc.c",
            "vfscanf.c"
        }
    },
    {
        name = "nutcrt_istream_p",
        brief = "File Stream Input (PGM-Space)",
        requires = { "HW_MCU_AVR" },
        provides = { "CRT_STREAM_READ_P" },
        sources = 
        { 
            "fscanf_p.c",
            "scanf_p.c",
            "vfscanf_p.c"
        }
    },
    {
        name = "nutcrt_ostream",
        brief = "File Stream Output",
        provides = { "CRT_STREAM_WRITE" },
        sources = 
        { 
            "fprintf.c",
            "fputc.c",
            "fputs.c",
            "fwrite.c",
            "printf.c",
            "putc.c",
            "putchar.c",
            "puts.c",
            "vfprintf.c"
        }
    },
    {
        name = "nutcrt_ostream_p",
        brief = "File Stream Output (PGM-Space)",
        requires = { "HW_MCU_AVR" },
        provides = { "CRT_STREAM_WRITE_P" },
        sources = 
        { 
            "fprintf_p.c",
            "fputs_p.c",
            "fwrite_p.c",
            "printf_p.c",
            "puts_p.c",
            "vfprintf_p.c"
        }
    },

    --
    -- String I/O
    --
    {
        name = "nutcrt_fstrio",
        brief = "Formatted String I/O",
        sources = 
        { 
            "sprintf.c",
            "sscanf.c",
            "vsprintf.c",
            "vsscanf.c"
        }
    },
    {
        name = "nutcrt_fstrio_p",
        brief = "Formatted String I/O (PGM-Space)",
        requires = { "HW_MCU_AVR" },
        sources = 
        { 
            "sprintf_p.c",
            "sscanf_p.c",
            "vsprintf_p.c",
            "vsscanf_p.c"
        }
    },

    --
    -- Date and time
    --
    {
        name = "nutcrt_time",
        brief = "Time and date",
        provides = { "CRT_DATETIME" },
        sources = 
        { 
            "gmtime.c",
            "localtim.c",
            "mktime.c",
            "time.c",
            "tzset.c"
        }
    },

    --
    -- Date and time
    --
    {
        name = "nutcrt_errno",
        brief = "Global errno",
        sources = { "errno.c" }
    },

    --
    -- Heap memory
    --
    {
        name = "nutcrt_malloc",
        brief = "Heap memory",
        requires = { "NUT_HEAPMEM" },
        provides = { "CRT_HEAPMEM" },
        sources = { "malloc.c" }
    },

    --
    -- Misc
    --
    {
        name = "nutcrt_strtok_r",
        brief = "Recursive strtok",
        requires = { "TOOL_ICC" },
        sources = { "strtok_r.c" }
    }
}
