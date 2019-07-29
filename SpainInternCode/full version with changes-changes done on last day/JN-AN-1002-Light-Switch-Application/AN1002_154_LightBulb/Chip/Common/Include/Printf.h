/****************************************************************************
 *
 * MODULE:             Printf Header
 *
 * COMPONENT:          Printf.h,v
 *
 * VERSION:            Build_Release_030308_RC1
 *
 * REVISION:           1.2
 *
 * DATED:              2008/02/29 18:19:35
 *
 * STATUS:             Exp
 *
 * AUTHOR:             LJM
 *
 * DESCRIPTION:
 * Printf header file
 *
 * CHANGE HISTORY:
 *
 * Printf.h,v
 * Revision 1.2  2008/02/29 18:19:35  dclar
 * dos2unix
 *
 * Revision 1.1  2006/12/07 15:56:25  imorr
 * Initial version
 *
 * Revision 1.4  2006/02/23 16:12:06  we1
 * added __cplusplus check and vUART_printInit() function
 *
 * Revision 1.3  2006/02/23 12:25:19  we1
 * added bWaitForKey on UART init
 *
 * Revision 1.2  2006/02/03 14:34:53  we1
 * added UART functions required for Printf operation
 *
 * Revision 1.1  2006/01/30 13:48:14  we1
 * first check in
 *
 * Revision 1.1  2006/01/13 09:54:48  lmitch
 * Initial revision
 *
 *
 *
 *
 * LAST MODIFIED BY:   dclar
 *                     $Modtime: $
 *
 ****************************************************************************
 *
 *  (c) Copyright 2004 JENNIC Ltd
 *
 ****************************************************************************/

#ifndef PRINTF_H_INCLUDED
#define PRINTF_H_INCLUDED

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void vUART_printInit(void);

PUBLIC void vInitPrintf(void (*fp)(char c));
PUBLIC void vPrintf(const char *fmt, ...);

PUBLIC void vPutC(unsigned char c);
PUBLIC void vUART_Init(bool bWaitForKey);

#if defined __cplusplus
}
#endif

#endif /* PRINTF_H_INCLUDED */


