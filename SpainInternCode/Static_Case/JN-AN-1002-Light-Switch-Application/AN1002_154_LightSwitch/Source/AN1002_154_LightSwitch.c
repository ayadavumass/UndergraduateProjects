/****************************************************************************
 *
 * MODULE:             lightswitch.c
 *
 * COMPONENT:          $RCSfile: LightSwitch.c,v $
 *
 * VERSION:            $Name:  $
 *
 * REVISION:           $Revision: 1.2 $
 *
 * DATED:              $Date: 2008/07/24 15:22:56 $
 *
 * STATUS:             $State: Exp $
 *
 * AUTHOR:
 *
 * DESCRIPTION:
 * Performs the function of a simple switch in a light bulb switching system.
 * To be run on an Endpoint demonstration board, uses switch SW2 as the light
 * switch. After a power on reset the light switch scans for a network to
 * join. Once it has found the coordinator it joins the network and then
 * enters sleep mode until SW2 is pressed. At this point is wakes up and
 * transmits a data frame to the coordinator indicating that the light bulb
 * should be toggled. Sleep mode is entered again once the data frame
 * transmission is complete.
 *
 * CHANGE HISTORY:
 *
 * $Log: LightSwitch.c,v $
 * Revision 1.2  2008/07/24 15:22:56
 * includes modification for scanning higher channels in high traffic environments(sbarf)
 *
 * Revision 1.1  2007/10/25 15:41:01
 * Up data to latest SDK
 *
 * Revision 1.1  2006/08/15 13:22:38
 * Initial version
 *
 *
 *
 * LAST MODIFIED BY:   $Author:  $
 *                     $Modtime: $
 *
 *
 ****************************************************************************
 *
 * This software is owned by Jennic and/or its supplier and is protected
 * under applicable copyright laws. All rights are reserved. We grant You,
 * and any third parties, a license to use this software solely and
 * exclusively on Jennic products. You, and any third parties must reproduce
 * the copyright and warranty notice and any other legend of ownership on each
 * copy or partial copy of the software.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS". JENNIC MAKES NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
 * ACCURACY OR LACK OF NEGLIGENCE. JENNIC SHALL NOT, IN ANY CIRCUMSTANCES,
 * BE LIABLE FOR ANY DAMAGES, INCLUDING, BUT NOT LIMITED TO, SPECIAL,
 * INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER.
 *
 * Copyright Jennic Ltd 2009. All rights reserved
 *
 ****************************************************************************/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include <AppHardwareApi.h>
#include "AppQueueApi.h"
#include <mac_sap.h>
#include <mac_pib.h>
#include <string.h>
#include "Button.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/* Defines the channels to scan. Each bit represents one channel. All channels
   in the channels (11-26) in the 2.4GHz band are scanned. */
#define SCAN_CHANNELS 0x07FFF800UL
/* Network parameters */
#define PAN_ID                         0x0401U
#define COORD_ADDR                     0x0502U

/* NV RAM addresses used to store data through sleep cycles */
//#define NVRAM_ADR_DEVICE_PARAMETERS     20
#define NVRAM_ADR_DEVICE_SHORT_ADR      20
#define NVRAM_ADR_NETWORK_CHAN          21
#define NVRAM_ADR_DEVICE_STATE          22

/* Light switch device data */
#define LIGHT_SWITCH_NODE_ADDR_BASE    0x1000U

/*Enables software to build for JN5121******************************/
#ifdef BUILD_JN5121
 #define JN5121_BUILD
 //Light Switch Control
 #define LIGHT_SWITCH_INPUT_MASK         0x00000800UL
#else
 //Light Switch Control
 #define LIGHT_SWITCH_INPUT_MASK         BUTTON_1_PIN
#endif

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/* State machine states */
typedef enum
{
    E_STATE_OFF,
    E_STATE_SCANNING,
    E_STATE_ASSOCIATING,
    E_STATE_RUNNING,
} teState;

/* All application data with scope within the entire file is kept here,
   including all stored node data */
typedef struct
{
    struct
    {
        teState eState;
        uint8   u8Channel;
        uint16  u16ShortAddr;
    } sSystem;
} tsDeviceData;

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void vInitSystem(void);
PRIVATE void vInitEndpoint(void);
PRIVATE void vStartScan(uint32 u32ChannelstoScan);
PRIVATE void vStartAssociate(void);
PRIVATE void vProcessEventQueues(void);
PRIVATE void vProcessIncomingMlme(MAC_MlmeDcfmInd_s *psMlmeInd);
PRIVATE void vProcessIncomingData(MAC_McpsDcfmInd_s *psMcpsInd);
PRIVATE void vProcessIncomingHwEvent(AppQApiHwInd_s *psAHI_Ind);
PRIVATE void vHandleActiveScanResponse(MAC_MlmeDcfmInd_s *psMlmeInd);
PRIVATE void vHandleAssociateResponse(MAC_MlmeDcfmInd_s *psMlmeInd);
PRIVATE void vTxDataFrame(void);
PRIVATE void vProcessLightSwitch(void);
PRIVATE void vLightSwitchSleep(void);
PRIVATE void vRestore(void);

PRIVATE void vPrintString(char *pcMessage);
PRIVATE void vInitUart(void);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/* Handles from the MAC */
PRIVATE void *s_pvMac;
PRIVATE MAC_Pib_s *s_psMacPib;

PRIVATE tsDeviceData sDeviceData;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: AppColdStart
 *
 * DESCRIPTION:
 * Runs application.
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * Entry point for a power on reset or wake from sleep mode.
 ****************************************************************************/
PUBLIC void AppColdStart(void)
{
	/* Disable watchdog if enabled by default */
	#ifdef WATCHDOG_ENABLED
	vAHI_WatchdogStop();
	#endif

    vInitSystem();

    if (u8AHI_PowerStatus() & 0x01) /* If this is a wake from sleep mode */
    {
        vPrintString("wkup\r\n");
        /* Must have occured due to the light switch being pressed so
           restore network and device parameters before processing light
           switch */
        vRestore();
        vProcessLightSwitch();
    }
    else /* else it must be a power on reset */
    {
        vPrintString("pwr on\r\n");
        /* Initialise software elements */
        vInitEndpoint();

        /* Perform scan to find light bulb (coordinator) */
        vStartScan(SCAN_CHANNELS);
    }

    while (1)
    {
        /* Run main loop. */
        vProcessEventQueues();

    }
}

/****************************************************************************
 *
 * NAME: AppWarmStart
 *
 * DESCRIPTION:
 * Entry point for a wake from sleep mode with the memory contents held. We
 * are not using this mode and so should never get here.
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.eState
 *
 * NOTES:
 * None.
 ****************************************************************************/
PUBLIC void AppWarmStart(void)
{
    /* Should never get here but just in case... */
    AppColdStart();
}

/****************************************************************************
 *
 * NAME: vInitSystem
 *
 * DESCRIPTION:
 * Initialise the device hardware and stack interface.
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PRIVATE void vInitSystem(void)
{
    /* Initialise stack and hardware interfaces. We aren't using callbacks
       at all, just monitoring the upward queues in a loop */
    u32AHI_Init();
    u32AppQApiInit(NULL, NULL, NULL);

    /* Set up the MAC handles. Must be called AFTER u32AppQApiInit() */
    s_pvMac = pvAppApiGetMacHandle();
    s_psMacPib = MAC_psPibGetHandle(s_pvMac);

    vInitUart();

    /* Set Pan ID in PIB (also sets match register in hardware) */
    MAC_vPibSetPanId(s_pvMac, PAN_ID);
    /*initialise flash JN5139xx */
    #ifdef JN5121_BUILD
    //nothing to initialise
 //   #else
    #elif defined BUILD_JN5139
    //initialise the flash
    bAHI_FlashInit(E_FL_CHIP_AUTO, NULL);
    #endif
}

/****************************************************************************
 *
 * NAME: vInitEndpoint
 *
 * DESCRIPTION:
 * Initialise light bulb specific parameters.
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PRIVATE void vInitEndpoint(void)
{
    /* Set defaults for software */
    sDeviceData.sSystem.eState = E_STATE_OFF;
    sDeviceData.sSystem.u16ShortAddr = 0xffff;

    /* Set light switch button IO line to inputs */
    vAHI_DioSetDirection(LIGHT_SWITCH_INPUT_MASK, 0);

    /* Enable wakeup on negative edge of switch input */
    vAHI_DioInterruptEdge(0, LIGHT_SWITCH_INPUT_MASK);
    vAHI_DioInterruptEnable(LIGHT_SWITCH_INPUT_MASK, 0);
}

/****************************************************************************
 *
 * NAME: vLightSwitchSleep
 *
 * DESCRIPTION:
 * Put the light switch device into low power sleep mode. Before this is done
 * the current state of the application and details of the network it is a
 * part of are stored in NVRAM so that they are not lost when the main RAM is
 * powered down.
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PRIVATE void vLightSwitchSleep(void)
{
    //Sleep mode
    vPrintString("Sleep \r\n");
    #ifdef JN5121_BUILD

    /* Store network parameters and device state before going to sleep */
    //Used for JN5121
    vAHI_NvRamWrite(NVRAM_ADR_DEVICE_SHORT_ADR,sDeviceData.sSystem.u16ShortAddr);
    vAHI_NvRamWrite(NVRAM_ADR_NETWORK_CHAN,sDeviceData.sSystem.u8Channel);
    vAHI_NvRamWrite(NVRAM_ADR_DEVICE_STATE,sDeviceData.sSystem.eState);

    /* Go to sleep without holding contents of RAM to acheive lowest power */
    vAHI_MemoryHold(FALSE);
    vAHI_PowerDown(FALSE);

    #elif defined BUILD_JN5139

		/* Store network parameters and device state before going to sleep */
		//Used for JN5139
		//Erase the Flash
	    bAHI_FlashErase();
	    bAHI_FlashProgram(NVRAM_ADR_DEVICE_SHORT_ADR,7,(uint8 *) &sDeviceData.sSystem);
		//Select sleep Mode
	    vAHI_Sleep(E_AHI_SLEEP_OSCOFF_RAMOFF);

	#else
		// For JN5147 option sleep RAM On

		//Select sleep Mode
		vAHI_MemoryHold(TRUE); //Check if necessary before RAMON call below
		vAHI_Sleep(E_AHI_SLEEP_OSCOFF_RAMON);
    #endif
}

/****************************************************************************
 *
 * NAME: vRestore
 *
 * DESCRIPTION:
 * Restore application state and network parameter from NVRAM.
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PRIVATE void vRestore(void)
{
    //Restore
    vPrintString("Restore \r\n");

    #ifdef JN5121_BUILD
    /* Restore device state and network parameters from NVRAM */
    sDeviceData.sSystem.u16ShortAddr = (uint16)u32AHI_NvRamRead(NVRAM_ADR_DEVICE_SHORT_ADR);
    sDeviceData.sSystem.u8Channel = (uint8)u32AHI_NvRamRead(NVRAM_ADR_NETWORK_CHAN);
    sDeviceData.sSystem.eState = (uint16)u32AHI_NvRamRead(NVRAM_ADR_DEVICE_STATE);

    #elif defined BUILD_JN5139
		/* Restore device state and network parameters */
	    bAHI_FlashRead(NVRAM_ADR_DEVICE_SHORT_ADR,7,(uint8 *) &sDeviceData.sSystem);

	#else
		 // For JN5147 RAM is ON so no need to restore
		/* Restore device state and network parameters */
    #endif

}

/****************************************************************************
 *
 * NAME: vProcessLightSwitch
 *
 * DESCRIPTION:
 * Process the light switch input. This routine gets called after a wakeup
 * event has been generated by the light switch input being taken low. By the
 * time we arrive at this point the application has been loaded from external
 * FLASH. This means that in order to debounce the switch all we have to do is
 * check that it is still low. If it is then we send a data frame to the light
 * bulb indicating that it should be toggled, sleep mode is entered when we
 * get a confirmation that this frame has been successfully transmitted.
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PRIVATE void vProcessLightSwitch(void)
{
    uint32 u32DioStatus;

    /* Read wake status of IO lines to clear bits and prevent multiple interrupts */
    u32DioStatus = u32AHI_DioInterruptStatus();

    if ((u32AHI_DioReadInput() & LIGHT_SWITCH_INPUT_MASK) == 0)
    {
        /* If light switch input is still low after the time taken to load
           the application from external serial FLASH then we can assume it
           is not a result of switch bounce and toggle the light bulb by
           sending a data frame to the coordinator */
        vTxDataFrame();
    }
    else
    {
        /* Switch not low (could be a switch bounce) so just return to sleep */
        vLightSwitchSleep();
    }
}

/****************************************************************************
 *
 * NAME: vProcessEventQueues
 *
 * DESCRIPTION:
 * Check each of the three event queues and process and items found.
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PRIVATE void vProcessEventQueues(void)
{
    MAC_MlmeDcfmInd_s *psMlmeInd;
    MAC_McpsDcfmInd_s *psMcpsInd;
    AppQApiHwInd_s    *psAHI_Ind;

    /* Check for anything on the MCPS upward queue */
    do
    {
        psMcpsInd = psAppQApiReadMcpsInd();
        if (psMcpsInd != NULL)
        {
            vProcessIncomingData(psMcpsInd);
            vAppQApiReturnMcpsIndBuffer(psMcpsInd);
        }
    } while (psMcpsInd != NULL);

    /* Check for anything on the MLME upward queue */
    do
    {
        psMlmeInd = psAppQApiReadMlmeInd();
        if (psMlmeInd != NULL)
        {
            vProcessIncomingMlme(psMlmeInd);
            vAppQApiReturnMlmeIndBuffer(psMlmeInd);
        }
    } while (psMlmeInd != NULL);

    /* Check for anything on the AHI upward queue */
    do
    {
        psAHI_Ind = psAppQApiReadHwInd();
        if (psAHI_Ind != NULL)
        {
            vProcessIncomingHwEvent(psAHI_Ind);
            vAppQApiReturnHwIndBuffer(psAHI_Ind);
        }
    } while (psAHI_Ind != NULL);
}

/****************************************************************************
 *
 * NAME: vProcessIncomingMlme
 *
 * DESCRIPTION:
 * Process any incoming managment events from the stack.
 *
 * PARAMETERS:      Name            RW  Usage
 *                  psMlmeInd
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PRIVATE void vProcessIncomingMlme(MAC_MlmeDcfmInd_s *psMlmeInd)
{
    /* We respond to several MLME indications and confirmations, depending
       on mode */
    switch (psMlmeInd->u8Type)
    {
    /* Deferred confirmation that the scan is complete */
    case MAC_MLME_DCFM_SCAN:
        vPrintString("scan cmplt\r\n");
        /* Only respond to this if scanning */
        if (sDeviceData.sSystem.eState == E_STATE_SCANNING)
        {
            vHandleActiveScanResponse(psMlmeInd);
        }
        break;

    /* Deferred confirmation that the association process is complete */
    case MAC_MLME_DCFM_ASSOCIATE:
        vPrintString("assc resp\r\n");
        /* Only respond to this if associating */
        if (sDeviceData.sSystem.eState == E_STATE_ASSOCIATING)
        {
            vHandleAssociateResponse(psMlmeInd);
        }
        break;

    default:
        break;
    }
}

/****************************************************************************
 *
 * NAME: vProcessIncomingData
 *
 * DESCRIPTION:
 * Process incoming data events from the stack.
 *
 * PARAMETERS:      Name            RW  Usage
 *                  psMcpsInd
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PRIVATE void vProcessIncomingData(MAC_McpsDcfmInd_s *psMcpsInd)
{
    switch (psMcpsInd->u8Type)
    {
        /* Deferred confirmation that data frame has been transmitted, or not */
        case MAC_MCPS_DCFM_DATA:
            vPrintString("data in\r\n");
            /* Only respond if the light bulb application is running */
            if (sDeviceData.sSystem.eState == E_STATE_RUNNING)
            {
                /* Check that the frame transmission was successful */
                if (psMcpsInd->uParam.sDcfmData.u8Status == MAC_ENUM_SUCCESS)
                {
                    /* Frame transmission complete, go to sleep until next event */
                    vLightSwitchSleep();
                }
            }
        break;
    }
}

/****************************************************************************
 *
 * NAME: vProcessIncomingHwEvent
 *
 * DESCRIPTION:
 * Process any hardware events.
 *
 * PARAMETERS:      Name            RW  Usage
 *                  psAHI_Ind
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PRIVATE void vProcessIncomingHwEvent(AppQApiHwInd_s *psAHI_Ind)
{
    /* No hardware events generated to nothing to be done here */
}

/****************************************************************************
 *
 * NAME: vStartScan
 *
 * DESCRIPTION:
 * Start a scan to search for a network to join.
 *
 * PARAMETERS:      Name            RW  Usage
 *                  u32ChannelstoScan
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * updated to solve not scanning high channels in high traffic problem (sbarf)
 ****************************************************************************/
PRIVATE void vStartScan(uint32 u32ChannelstoScan)
{
    MAC_MlmeReqRsp_s  sMlmeReqRsp;
    MAC_MlmeSyncCfm_s sMlmeSyncCfm;

    sDeviceData.sSystem.eState = E_STATE_SCANNING;

    /* Request scan */
    sMlmeReqRsp.u8Type = MAC_MLME_REQ_SCAN;
    sMlmeReqRsp.u8ParamLength = sizeof(MAC_MlmeReqScan_s);
    sMlmeReqRsp.uParam.sReqScan.u8ScanType = MAC_MLME_SCAN_TYPE_ACTIVE;
    sMlmeReqRsp.uParam.sReqScan.u32ScanChannels = u32ChannelstoScan;
    sMlmeReqRsp.uParam.sReqScan.u8ScanDuration = 3;
    vAppApiMlmeRequest(&sMlmeReqRsp, &sMlmeSyncCfm);

    /* Check immediate response */
    if (sMlmeSyncCfm.u8Status != MAC_MLME_CFM_DEFERRED)
    {
        /* Unexpected result: scan request should result in a deferred
           confirmation (i.e. we will receive it later) */
    }
}

/****************************************************************************
 *
 * NAME: vHandleActiveScanResponse
 *
 * DESCRIPTION:
 * Handle the reponse generated by the stack as a result of the network scan.
 *
 * PARAMETERS:      Name            RW  Usage
 *                  psMlmeInd
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PRIVATE void vHandleActiveScanResponse(MAC_MlmeDcfmInd_s *psMlmeInd)
{
    MAC_PanDescr_s *psPanDesc;
    int i;

    /* Make sure it is what we're after */
    if ((psMlmeInd->uParam.sDcfmScan.u8Status == MAC_ENUM_SUCCESS)
        && (psMlmeInd->uParam.sDcfmScan.u8ScanType == MAC_MLME_SCAN_TYPE_ACTIVE))
    {
        /* Determine which, if any, network contains demo coordinator.
           Algorithm for determining which network to connect to is
           beyond the scope of 802.15.4, and we use a simple approach
           of matching the required PAN ID and short address, both of
           which we already know */

        i = 0;
        while (i < psMlmeInd->uParam.sDcfmScan.u8ResultListSize)
        {
            psPanDesc = &psMlmeInd->uParam.sDcfmScan.uList.asPanDescr[i];

            if ( (psPanDesc->sCoord.u16PanId == PAN_ID)
                 && (psPanDesc->sCoord.u8AddrMode == 2)
                 && (psPanDesc->sCoord.uAddr.u16Short == COORD_ADDR))
            {
                /* Matched so start to synchronise and associate */
                sDeviceData.sSystem.u8Channel = psPanDesc->u8LogicalChan;
                vStartAssociate();
                return;
            }
            i++;
        }


    }

   //sbarf: updated section 24_7_08
   // if there are unscanned channels remaining, scan them
    if(psMlmeInd->uParam.sDcfmScan.u32UnscannedChannels != 0)
	                         vStartScan(psMlmeInd->uParam.sDcfmScan.u32UnscannedChannels);

    else
        /* Failed to find coordinator: keep trying */
	vStartScan(SCAN_CHANNELS);

}

/****************************************************************************
 *
 * NAME: vStartAssociate
 *
 * DESCRIPTION:
 * Start the association process with the network coordinator.
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * Assumes that a network has been found during the network scan.
 ****************************************************************************/
PRIVATE void vStartAssociate(void)
{
    MAC_MlmeReqRsp_s  sMlmeReqRsp;
    MAC_MlmeSyncCfm_s sMlmeSyncCfm;

    sDeviceData.sSystem.eState = E_STATE_ASSOCIATING;

    /* Create associate request. We know short address and PAN ID of
       coordinator as this is preset and we have checked that received
       beacon matched this */
    sMlmeReqRsp.u8Type = MAC_MLME_REQ_ASSOCIATE;
    sMlmeReqRsp.u8ParamLength = sizeof(MAC_MlmeReqAssociate_s);
    sMlmeReqRsp.uParam.sReqAssociate.u8LogicalChan = sDeviceData.sSystem.u8Channel;
    sMlmeReqRsp.uParam.sReqAssociate.u8Capability = 0x80; /* We want short address, other features off */
    sMlmeReqRsp.uParam.sReqAssociate.u8SecurityEnable = FALSE;
    sMlmeReqRsp.uParam.sReqAssociate.sCoord.u8AddrMode = 2;
    sMlmeReqRsp.uParam.sReqAssociate.sCoord.u16PanId = PAN_ID;
    sMlmeReqRsp.uParam.sReqAssociate.sCoord.uAddr.u16Short = COORD_ADDR;

    /* Put in associate request and check immediate confirm. Should be
       deferred, in which case response is handled by event handler */
    vAppApiMlmeRequest(&sMlmeReqRsp, &sMlmeSyncCfm);
    if (sMlmeSyncCfm.u8Status != MAC_MLME_CFM_DEFERRED)
    {
        /* Unexpected result */
    }
}

/****************************************************************************
 *
 * NAME: vHandleAssociateResponse
 *
 * DESCRIPTION:
 * Handle the response generated by the stack as a result of the associate
 * start request.
 *
 * PARAMETERS:      Name            RW  Usage
 *                  psMlmeInd
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PRIVATE void vHandleAssociateResponse(MAC_MlmeDcfmInd_s *psMlmeInd)
{
    /* If successfully associated with network coordinator */
    if (psMlmeInd->uParam.sDcfmAssociate.u8Status == MAC_ENUM_SUCCESS)
    {
        /* Store short address that we have been assigned */
        sDeviceData.sSystem.u16ShortAddr = psMlmeInd->uParam.sDcfmAssociate.u16AssocShortAddr;
        /* We are now in the running state */
        sDeviceData.sSystem.eState = E_STATE_RUNNING;
        /* Go to sleep until light switch is pressed */
        vLightSwitchSleep();
    }
    else
    {
        /* Try, try again */
        vStartAssociate();
    }
}

/****************************************************************************
 *
 * NAME: vTxDataFrame
 *
 * DESCRIPTION:
 * Transmit a single data frame to the network coordinator. The payload can
 * be set to anything at all. The coordinator will toggle the state of the
 * light bulb upon reception of a data frame from a light switch that is part
 * of the network regardless of the contents of the payload.
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PRIVATE void vTxDataFrame(void)
{
    MAC_McpsReqRsp_s  sMcpsReqRsp;
    MAC_McpsSyncCfm_s sMcpsSyncCfm;
    uint8 *pu8Payload;
    //char *pu8Payload;

    /* Create frame transmission request */
    sMcpsReqRsp.u8Type = MAC_MCPS_REQ_DATA;
    sMcpsReqRsp.u8ParamLength = sizeof(MAC_McpsReqData_s);
    /* Set handle so we can match confirmation to request */
    sMcpsReqRsp.uParam.sReqData.u8Handle = 1;
    /* Use short address for source */
    sMcpsReqRsp.uParam.sReqData.sFrame.sSrcAddr.u8AddrMode = 2;
    sMcpsReqRsp.uParam.sReqData.sFrame.sSrcAddr.u16PanId = PAN_ID;
    sMcpsReqRsp.uParam.sReqData.sFrame.sSrcAddr.uAddr.u16Short = sDeviceData.sSystem.u16ShortAddr;
    /* Use short address for destination */
    sMcpsReqRsp.uParam.sReqData.sFrame.sDstAddr.u8AddrMode = 2;
    sMcpsReqRsp.uParam.sReqData.sFrame.sDstAddr.u16PanId = PAN_ID;
    sMcpsReqRsp.uParam.sReqData.sFrame.sDstAddr.uAddr.u16Short = COORD_ADDR;
    /* Frame requires ack but not security, indirect transmit or GTS */
    sMcpsReqRsp.uParam.sReqData.sFrame.u8TxOptions = MAC_TX_OPTION_ACK;
    sMcpsReqRsp.uParam.sReqData.sFrame.u8SduLength = 8;
    pu8Payload = sMcpsReqRsp.uParam.sReqData.sFrame.au8Sdu;
    /* Set payload data to anything at all */
    pu8Payload[0] = 0;
    pu8Payload[1] = 1;
    pu8Payload[2] = 2;
    pu8Payload[3] = 3;
    pu8Payload[4] = 4;
    pu8Payload[5] = 5;

    /* Request transmit */
    vAppApiMcpsRequest(&sMcpsReqRsp, &sMcpsSyncCfm);
}

/****************************************************************************
 *
 * NAME: vInitUart
 *
 * DESCRIPTION:
 * Initialise the UART
 *
 * RETURNS:
 * None.
 *
 ****************************************************************************/
PRIVATE void vInitUart(void)
{
    /* Enable UART 0: 19200-8-N-1 */
    vAHI_UartEnable(E_AHI_UART_0);
    vAHI_UartReset(E_AHI_UART_0, TRUE, TRUE);
    vAHI_UartReset(E_AHI_UART_0, FALSE, FALSE);

    vAHI_UartSetClockDivisor(E_AHI_UART_0, E_AHI_UART_RATE_19200);
    vAHI_UartSetControl(E_AHI_UART_0, FALSE, FALSE, E_AHI_UART_WORD_LEN_8, TRUE, FALSE);
    vAHI_UartSetInterrupt(E_AHI_UART_0, FALSE, FALSE, FALSE, TRUE, E_AHI_UART_FIFO_LEVEL_1);

    vPrintString("uart on\r\n");

}

/****************************************************************************
 *
 * NAME: vPrintString
 *
 * DESCRIPTION:
 * Sends a string to the circular buffer ready for printing to the UART.
 *
 * PARAMETERS:      Name            RW  Usage
 *                  pcMessage       R   Null-terminated message to send
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
 PRIVATE void vPrintString(char *pcMessage)
 {
     while (*pcMessage)
     {
         while ((u8AHI_UartReadLineStatus(0) & 0x20) == 0);
         vAHI_UartWriteData(0, *pcMessage);
         pcMessage++;
     }
 }

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
