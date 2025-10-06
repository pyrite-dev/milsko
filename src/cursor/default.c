/* $Id$ */
#include <Mw/Milsko.h>

/**
 * Copyright notice:
 *   Copyright (c) Digital Equipment Corporation,1988. All Rights Reserved.
 */
/**
 *  Permission to use, copy, modify, and distribute this software and
 *  its documentation for any purpose and without fee is hereby granted,
 *  provided that the above copyright notices appear in all copies and
 *  that both those copyright notices and this permission notice appear
 *  in supporting documentation, and that the name
 *  Digital Equipment Corporation not be used in advertising or
 *  publicity pertaining to distribution of the software without
 *  specific, written prior permission.  Digital
 *  Equipment Corporation makes no representations about the suitability
 *  of this software for any purpose.  It is provided "as is" without
 *  express or implied warranty.
 *  DIGITAL EQUIPMENT CORPORATION DISCLAIMS ALL
 *  WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 *  DIGITAL EQUIPMENT CORPORATION BE LIABLE FOR ANY SPECIAL,
 *  INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 *  RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 *  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 *  CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
MwCursor MwCursorDefault = {
    16, 16, 0, -15, {
			0,     /* ................ */
			24576, /* .##............. */
			30720, /* .####........... */
			15872, /* ..#####......... */
			16256, /* ..#######....... */
			8160,  /* ...########..... */
			8184,  /* ...##########... */
			3968,  /* ....#####....... */
			4032,  /* ....######...... */
			1760,  /* .....##.###..... */
			1648,  /* .....##..###.... */
			568,   /* ......#...###... */
			540,   /* ......#....###.. */
			14,    /* ............###. */
			4,     /* .............#.. */
			0      /* ................ */
		    }};
MwCursor MwCursorDefaultMask = {
    16, 16, 0, -15, {
			57344, /* ###............. */
			63488, /* #####........... */
			65024, /* #######......... */
			32640, /* .########....... */
			32736, /* .##########..... */
			16376, /* ..###########... */
			16380, /* ..############.. */
			8184,  /* ...##########... */
			8160,  /* ...########..... */
			4080,  /* ....########.... */
			4088,  /* ....#########... */
			1916,  /* .....###.#####.. */
			1854,  /* .....###..#####. */
			543,   /* ......#....##### */
			14,    /* ............###. */
			4      /* .............#.. */
		    }};
