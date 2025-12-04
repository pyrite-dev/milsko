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
MwCursor MwCursorArrow = {
    16, 16, -15, -15, {
			  0,	 /* ................ */
			  6,	 /* .............##. */
			  30,	 /* ...........####. */
			  124,	 /* .........#####.. */
			  508,	 /* .......#######.. */
			  2040,	 /* .....########... */
			  8184,	 /* ...##########... */
			  496,	 /* .......#####.... */
			  1008,	 /* ......######.... */
			  1888,	 /* .....###.##..... */
			  3680,	 /* ....###..##..... */
			  7232,	 /* ...###...#...... */
			  14400, /* ..###....#...... */
			  28672, /* .###............ */
			  8192,	 /* ..#............. */
			  0	 /* ................ */
		      }};
MwCursor MwCursorArrowMask = {
    16, 16, -15, -15, {
			  7,	 /* .............### */
			  31,	 /* ...........##### */
			  127,	 /* .........####### */
			  510,	 /* .......########. */
			  2046,	 /* .....##########. */
			  8188,	 /* ...###########.. */
			  16380, /* ..############.. */
			  8184,	 /* ...##########... */
			  2040,	 /* .....########... */
			  4080,	 /* ....########.... */
			  8176,	 /* ...#########.... */
			  16096, /* ..#####.###..... */
			  31968, /* .#####..###..... */
			  63552, /* #####....#...... */
			  28672, /* .###............ */
			  8192	 /* ..#............. */
		      }};
