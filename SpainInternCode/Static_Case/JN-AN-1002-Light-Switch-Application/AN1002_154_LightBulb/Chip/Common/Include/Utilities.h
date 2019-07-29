/*****************************************************************************
 *
 * MODULE:              Utility functions for string handling
 *
 * COMPONENT:           Utilities.h,v
 *
 * VERSION:             Build_Release_030308_RC1
 *
 * REVISION:            1.3
 *
 * DATED:               2008/02/29 18:40:11
 *
 * STATUS:              Exp
 *
 * AUTHOR:              CJG
 *
 * DESCRIPTION:
 * Utility functions for string handling.
 *
 * LAST MODIFIED BY:    dclar
 *                      $Modtime: $
 *
 ****************************************************************************
 *
 *  (c) Copyright JENNIC Ltd 2006
 *
 ****************************************************************************/

#ifndef  NUM_TO_STRING_INCLUDED
#define  NUM_TO_STRING_INCLUDED

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include "jendefs.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
#ifdef CHIP_RELEASE_5121
typedef void *(*tprmemcpy)(void *s1, const void *s2, long unsigned int n);
typedef int (*tprmemcmp)(const void * cs, const void * ct, long unsigned int count);
typedef void *(*tprmemset)(void *s, int c, long unsigned int n);
#endif

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/* Number conversion */
PUBLIC void vUTIL_NumToString(uint32 u32Data, char *pcString);

/* UART printing */
PUBLIC void vUTIL_UartInit(void);
PUBLIC void vUTIL_UartText(char *pcString);
PUBLIC void vUTIL_UartTextToNewLine(char *pcString);

/* Standard memcpy, memset, memcmp but defined to be linked from ROM for
   JN5121 */
#ifdef CHIP_RELEASE_5121
#define memcpy ((tprmemcpy) 0xf9ec)
#define memcmp ((tprmemcmp) 0xfb04)
#define memset ((tprmemset) 0xfb5c)
#else
PUBLIC void *memcpy(void *s1, const void *s2, long unsigned int n);
PUBLIC int   memcmp(const void * cs, const void * ct, long unsigned int count);
PUBLIC void *memset(void *s, int c, long unsigned int n);
#endif

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* NUM_TO_STRING_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

