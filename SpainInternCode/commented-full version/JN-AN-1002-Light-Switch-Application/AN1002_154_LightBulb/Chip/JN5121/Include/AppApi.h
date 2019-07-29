/*****************************************************************************
 *
 * MODULE:              Application API header
 *
 * COMPONENT:           AppApi.h,v
 *
 * VERSION:             Build_Release_030308_RC1
 *
 * REVISION:            1.3
 *
 * DATED:               2007/03/09 15:34:05
 *
 * STATUS:              Exp
 *
 * AUTHOR:              CJG
 *
 * DESCRIPTION:
 * Access functions and structures used by the application to interact with
 * the Jennic 802.15.4 stack.
 *
 * LAST MODIFIED BY:    cjg
 *                      $Modtime: $
 *
 ****************************************************************************
 *
 *  (c) Copyright JENNIC Ltd 2006
 *
 ****************************************************************************/

/**
 * @defgroup g_app_sap Application MAC Service Access Point (SAP)
 */

#ifndef  APP_API_H_INCLUDED
#define  APP_API_H_INCLUDED

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <mac_sap.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

#ifndef AHI_DEVICE_ENUM
#define AHI_DEVICE_ENUM

/* Device types, used to identify interrupt source */
typedef enum
{
    E_AHI_DEVICE_SYSCTRL = 2, /* System controller */
    E_AHI_DEVICE_BBC,         /* Baseband controller */
    E_AHI_DEVICE_AES,         /* Encryption engine */
    E_AHI_DEVICE_PHYCTRL,     /* Phy controller */
    E_AHI_DEVICE_UART0,       /* UART 0 */
    E_AHI_DEVICE_UART1,       /* UART 1 */
    E_AHI_DEVICE_TIMER0,      /* Timer 0 */
    E_AHI_DEVICE_TIMER1,      /* Timer 1 */
    E_AHI_DEVICE_SI,          /* Serial Interface (2 wire) */
    E_AHI_DEVICE_SPIM,        /* SPI master */
    E_AHI_DEVICE_INTPER,      /* Intelligent peripheral */
    E_AHI_DEVICE_ANALOGUE     /* Analogue peripherals */
} teAHI_Device;

/* Individual interrupts */
typedef enum
{
    E_AHI_SYSCTRL_WK0 = 26,   /* Wake timer 0 */
    E_AHI_SYSCTRL_WK1 = 27    /* Wake timer 1 */
} teAHI_Item;

#endif /* !AHI_DEVICE_ENUM */

typedef enum
{
    E_PHY_ENTERED_TX,
    E_PHY_ENTERED_RX,
    E_PHY_ENTERED_IDLE
} tePhyEnteredState;

/**
 * @ingroup g_app_sap
 * @brief Get Buffer routine type
 *
 * Type of Get Buffer callback routine
 */
typedef MAC_DcfmIndHdr_s * (*PR_GET_BUFFER)(void *);

/**
 * @ingroup g_app_sap
 * @brief Post routine type
 *
 * Type of Post callback routine
 */
typedef void (*PR_POST_CALLBACK)(void *, MAC_DcfmIndHdr_s *);

/**
 * @ingroup g_app_sap
 * @brief Hardware Interrupt routine type
 *
 * Type of Hardware Interrupt callback routine
 */
typedef void (*PR_HWINT_CALLBACK)(uint32 u32DeviceId, uint32 u32ItemBitmap);
typedef void (*PR_PHY_CALLBACK)(tePhyEnteredState);

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/* Typedefs used as function prototypes for functions in ROM */
typedef void * (*tprpvAppApiGetMacHandle)(void);
typedef PHY_Enum_e (*tpreAppApiPlmeGet)(PHY_PibAttr_e ePhyPibAttribute,
                                        uint32 *pu32PhyPibValue);
typedef void (*tprvAppApiSetPhyCallback)(PR_PHY_CALLBACK prPhyCallback);

/* Functions mapped to addresses in ROM, found from lookup table at base
   of ROM */
#define pvAppApiGetMacHandle  ((tprpvAppApiGetMacHandle) (*(uint32 *)0x0c))
#define eAppApiPlmeGet        ((tpreAppApiPlmeGet)       (*(uint32 *)0x1c))
#define vAppApiSetPhyCallback ((tprvAppApiSetPhyCallback)(*(uint32 *)0x20))

/* Functions that are defined in library file */
PUBLIC uint32
u32AppApiInit(PR_GET_BUFFER prMlmeGetBuffer,
              PR_POST_CALLBACK prMlmeCallback,
              void *pvMlmeParam,
              PR_GET_BUFFER prMcpsGetBuffer,
              PR_POST_CALLBACK prMcpsCallback,
              void *pvMcpsParam);
PUBLIC void
vAppApiMlmeRequest(MAC_MlmeReqRsp_s *psMlmeReqRsp,
                   MAC_MlmeSyncCfm_s *psMlmeSyncCfm);
PUBLIC void
vAppApiMcpsRequest(MAC_McpsReqRsp_s *psMcpsReqRsp,
                   MAC_McpsSyncCfm_s *psMcpsSyncCfm);
PUBLIC PHY_Enum_e eAppApiPlmeSet(PHY_PibAttr_e ePhyPibAttribute,
                                 uint32 u32PhyPibValue);
PUBLIC void
vAppApiSaveMacSettings(void);
PUBLIC void
vAppApiRestoreMacSettings(void);
PUBLIC void *
pvAppApiGetMacAddrLocation(void);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* APP_API_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

