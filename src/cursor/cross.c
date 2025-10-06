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
MwCursor MwCursorCross = {
    13, 12, -6, -6, {7175, /* ###.......### */
		     4617, /* #..#.....#..# */
		     2322, /* .#..#...#..#. */
		     1188, /* ..#..#.#..#.. */
		     584,  /* ...#..#..#... */
		     272,  /* ....#...#.... */
		     272,  /* ....#...#.... */
		     584,  /* ...#..#..#... */
		     1188, /* ..#..#.#..#.. */
		     2322, /* .#..#...#..#. */
		     4617, /* #..#.....#..# */
		     7175, /* ###.......### */
		     0, 0, 0, 0}};
MwCursor MwCursorCrossMask = {
    13, 12, -6, -6, {7175, /* ###.......### */
		     7695, /* ####.....#### */
		     3870, /* .####...####. */
		     1980, /* ..####.####.. */
		     1016, /* ...#######... */
		     496,  /* ....#####.... */
		     496,  /* ....#####.... */
		     1016, /* ...#######... */
		     1980, /* ..####.####.. */
		     3870, /* .####...####. */
		     7695, /* ####.....#### */
		     7175, /* ###.......### */
		     0, 0, 0, 0}};
