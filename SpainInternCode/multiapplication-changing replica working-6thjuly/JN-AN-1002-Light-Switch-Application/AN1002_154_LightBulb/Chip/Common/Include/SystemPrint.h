/****************************************************************************
 *
 * MODULE:             Printf Header
 *
 * COMPONENT:          SystemPrint.h,v
 *
 * VERSION:            Build_Release_030308_RC1
 *
 * REVISION:           1.2
 *
 * DATED:              2008/02/29 18:39:30
 *
 * STATUS:             Exp
 *
 * AUTHOR:             Wayne Ellis
 *
 * DESCRIPTION:
 * System Print Mechanism Header File
 *
 * LAST MODIFIED BY:   dclar
 *                     $Modtime: $
 *
 ****************************************************************************
 *
 *  (c) Copyright 2006 JENNIC Ltd
 *
 ****************************************************************************/

#ifndef SYSTEMPRINT_H_INCLUDED
#define SYSTEMPRINT_H_INCLUDED

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include <Utilities.h>
#ifdef PRINT_LCD
#include <LcdDriver.h>
#endif

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/* API Platform dependent DISPLAY MACRO'S */

/* UART PRINT option */
#if(defined PRINT_UART)
#define _PRINT_INIT()                                   vUTIL_UartInit()
#define _CLEAR()
#define _WRITE_TEXT_TO_CLEAR_LINE(string, row, col)     vUTIL_UartTextToNewLine(string)
#define _WRITE_TEXT_TO_LINE(string, row, col)           vUTIL_UartText(string)
#define _UPDATE_TEXT()
#define _CONVERT_NUM_TO_STR(num, str)                   vUTIL_NumToString(num, str)

/* LCD PRINT option */
#elif(defined PRINT_LCD)
#define _PRINT_INIT()                                   vLcdResetDefault()
#define _CLEAR()                                        vLcdClear()
#define _WRITE_TEXT_TO_CLEAR_LINE(string, row, col)     vLcdWriteTextToClearLine(string, row, col)
#define _WRITE_TEXT_TO_LINE(string, row, col)           vLcdWriteText(string, row, col)
#define _UPDATE_TEXT()                                  vLcdRefreshAll()
#define _CONVERT_NUM_TO_STR(num, str)                   vUTIL_NumToString(num, str)

/* No PRINT option */
#else
#define _PRINT_INIT()
#define _CLEAR()
#define _WRITE_TEXT_TO_CLEAR_LINE(string, row, col)
#define _WRITE_TEXT_TO_LINE(string, row, col)
#define _UPDATE_TEXT()
#define _CONVERT_NUM_TO_STR(num, str)

#endif
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif /* SYSTEMPRINT_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/


