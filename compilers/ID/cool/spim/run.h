/* SPIM S20 MIPS simulator.
   Execute SPIM instructions.

   Copyright (C) 1990-2000 by James Larus (larus@cs.wisc.edu).
   ALL RIGHTS RESERVED.

   SPIM is distributed under the following conditions:

     You may make copies of SPIM for your own use and modify those copies.

     All copies of SPIM must retain my name and copyright notice.

     You may not sell SPIM or distributed SPIM in conjunction with a
     commerical product or service without the expressed written consent of
     James Larus.

   THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
   PURPOSE. */


/* $Header: /project/cs/aiken/aa/projects/SourceRepository/cool/cool/spim/run.h,v 1.2 2003/04/13 20:40:42 matth Exp $
*/


/* Exported functions: */

#ifdef __STDC__
int run_spim (mem_addr initial_PC, register int steps, int display);
#else
int run_spim ();
#endif
