/****************************************************************************
 *
 * MODULE:             lightbulb.c
 *
 * COMPONENT:          $RCSfile: LightBulb.c,v $
 *
 * VERSION:            $Name:  $
 *
 * REVISION:           $Revision: 1.1 $
 *
 * DATED:              $Date: 2008/05/09 13:24:20 $
 *
 * STATUS:             $State: Exp $
 *
 * AUTHOR:
 *
 * DESCRIPTION:
 * Performs the function of the bulb in a light bulb switching system. Runs
 * on the controller board of the evaluation kit and forms the PAN coordinator
 * of the system. Once at least once light switch has joined the network the
 * then coordinator waits for a data frame to arrive. Upon reception the state
 * of the bulb (four LEDs on the controller board) is toggled.
 *
 * CHANGE HISTORY:
 *
 * $Log: LightBulb.c,v $
 * Revision 1.1  2008/05/09 13:24:20
 * Changed for JN-SW-4032-JN5147-SDK-Patch-RC2beta release
 *
 * Revision 1.2  2006/08/15 13:56:05
 * Updated so that extended address of associating nodes is stored correctly
 *
 * Revision 1.1  2006/08/15 13:22:52
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
#include <AppQueueApi.h>
#include <mac_sap.h>
#include <mac_pib.h>
#include <Utilities.h>
#include<stdlib.h>
#include "Button.h"

#include <..\..\..\Jenie\Include\Jenie.h>
#include <..\..\..\Jenie\Include\JPI.h>
#include <..\..\..\Chip\Common\Include\Printf.h>
#include<..\..\md5-c\global.h>
#include<..\..\md5-c\md5.h>
#include "LedControl.h"


/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/* Topology Dimension */
#define DIMX                           200
#define DIMY                           200

/* Light bulb LED control */
#define LED1                           0
#define LED2                           1
#define LED3                           2
#define LED4                           3

#define OFF                            0
#define ON                             1
/* Network parameters */
#define PAN_ID                         0x0401U
#define COORD_ADDR                     0x0521U
#define MAX_PAYLOAD                    118
#define DEBUG                          1
#define MAX_NEIGHBOURS                 10
#define MAX_STORAGE                    50
#define NO_REPLICA                     4
#define NO_NODES                       20
#define OPR_MODE                       0   //  0 => <Timer 1=> Button

#define Run_Time                       60    //mins


/* Light switch device data */
#define MAX_LIGHT_SWITCHES             2
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

/* System states with respect to screen display being shown */
typedef enum
{
    E_STATE_INIT,
    E_STATE_START_COORDINATOR,
    E_STATE_RUNNING_LIGHT_BULB_APP

} teState;

/* Used to track an association between extended address and short address */
typedef struct
{
    uint32 u32ExtAddrLo;
    uint32 u32ExtAddrHi;
    uint16 u16ShortAddr;
} tsAssocNodes;

/*Data Type Enumeration */
typedef enum
{
    TEMP,
    HUMIDITY,
    LIGHT_INTENSITY

} DATA_TYPE;

typedef enum
{
    PUT,
    GET,
    GET_RESP,
    REPLICATION

} PACKET_TYPE;

typedef enum
{
    PRODUCER,
    CONSUMER,
    REPLICA,
    NOTHING

} NODE_TYPE;

/* All application data with scope within the entire file is kept here,
   including all stored node data, GUI settings and current state */
typedef struct
{
    struct
    {
        tsAssocNodes asAssocNodes[MAX_LIGHT_SWITCHES];
        uint8        u8AssociatedNodes;
    } sNode;

    struct
    {
        teState eState;
        uint8   u8Channel;
    } sSystem;
} tsCoordData;


/**************************************
Structure for making routuing link list
Its for neighbours of the node
***************************************/
typedef struct{
    int Neighbour_ID;
    bool END;

}tsNext_Hop;
typedef struct{
    struct
    {
        uint16 X;
        uint16 Y;
    }G_Loc;
    struct
    {
        uint16 Node_Shrt_Addr;
    }Address;


}tsNode_Info;


/*************************
Routing Header Structure

*************************/
typedef struct{
    uint8 Version;
    uint8 ToS;
    uint8 TTL;
    uint8 Proto;
    struct
    {
        uint16 SrcX;
        uint16 SrcY;
    }Src_Info;

    struct
    {
        uint16 DstX;
        uint16 DstY;
    }Dst_Info;
}tsRouting_Header;



typedef struct {
    uint16 Time_Stamp;
    uint8 Value;
}tsPUTData;

/*****************************************
Structure for storing data of PUT Operation
******************************************/


typedef struct {

    uint16 CoordX;
    uint16 CoordY;
   uint16 Time_Stamp;
    uint16 Value;
}tsData_Storage;

/*****************************************
DCS Header
******************************************/
typedef struct{
    uint8 OpCode;
    uint8 Flags;
    uint16 AppID;
    uint8 Data_Type;
    uint8 Len;
    uint8 Rep_Num;
    }tsDCS_Header;



/**************************
Payload = Routing + DCS
***************************/
typedef struct{
    tsRouting_Header Routing_Header;
    tsDCS_Header DCS_Header;
}tsHeaders;




/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
/* Handles from the MAC */
PRIVATE void *s_pvMac;
PRIVATE MAC_Pib_s *s_psMacPib;

PRIVATE tsCoordData sCoordData;
PRIVATE bool_t bLightBulbState;
PRIVATE bool_t bToggle = TRUE;

/*Head Of link list*/
PRIVATE tsNext_Hop Next_Hop_Head[MAX_NEIGHBOURS];
PRIVATE tsData_Storage TEMP_Storage[MAX_STORAGE];
PRIVATE tsData_Storage HUMIDITY_Storage[MAX_STORAGE];
PRIVATE tsData_Storage LIGHT_Storage[MAX_STORAGE];
PRIVATE uint8 PayLoad[MAX_PAYLOAD];
PRIVATE tsData_Storage Rep_Data[10];

PRIVATE int TEMP_Counter=0;
PRIVATE int HUMIDITY_Counter=0;
PRIVATE int LIGHT_Counter=0;
PRIVATE bool Temp_Replica=FALSE;
PRIVATE NODE_TYPE Node_Type;
PRIVATE int No_Of_GET=1;
PRIVATE int No_Of_PUT=1;
PRIVATE uint16 Current_Time=0;
PRIVATE char *Temp[10]={"TEM00","TEM10","TEM20","TEM30","TEM40","TEM50","TEM60","TEM70","TEM80","TEM90"};
PRIVATE uint16 HashX,HashY;
PRIVATE uint16 Array_HashX[NO_REPLICA];
PRIVATE uint16 Array_HashY[NO_REPLICA];
PRIVATE uint16 Replica_Num;
PRIVATE uint16 Seq_Num=0;
PRIVATE uint16 Replication_Forwarding_Sets[NO_REPLICA][10];    //  -1 means the end of the list
PRIVATE uint16 Count_Forwarding_Sets[NO_REPLICA];
PRIVATE tsNode_Info Node_Info[NO_NODES];
PRIVATE int Own_ID;
PRIVATE bool START = FALSE;
PRIVATE int Msg_Sent=0;
PRIVATE int Msg_Recv=0;
PRIVATE int Msg_Gen=0;
PRIVATE int Msg_Con=0;

PRIVATE int Time_Counter=0;
PRIVATE int PRODUCER_Counter=0;
PRIVATE int CONSUMER_Counter=0;
PRIVATE int Bytes_Recv=0;
PRIVATE int Bytes_Sent=0;

















/*Node Coordinates*/
PRIVATE uint16 Source_X,Source_Y;

/*Routing Header*/
PRIVATE tsRouting_Header Routing_Header;
PRIVATE tsDCS_Header DCS_Header;

/* Payload=Routing + DCS */
PRIVATE tsHeaders Headers;


extern PUBLIC uint32 u32AppQApiInit(PR_QIND_CALLBACK prMlmeCallback,
                             PR_QIND_CALLBACK prMcpsCallback,
                             PR_HWQINT_CALLBACK prHwCallback);

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/
PRIVATE void vInitSystem(void);
PRIVATE bool_t bStartCoordinator(void);
PRIVATE void vHandleNodeAssociation(MAC_MlmeDcfmInd_s *psMlmeInd);
PRIVATE void vProcessEventQueues(void);
PRIVATE void vProcessIncomingMlme(MAC_MlmeDcfmInd_s *psMlmeInd);
PRIVATE void vProcessIncomingData(MAC_McpsDcfmInd_s *psMcpsInd);
PRIVATE void vProcessIncomingHwEvent(AppQApiHwInd_s *psAHI_Ind);
PRIVATE void vLightBulb(bool_t bState);
PRIVATE void vLightBulbInit(void);
PRIVATE void vTransmitDataPacket(uint8 pu8Data[], uint8 u8Len, uint16 u16DestAdr);

PRIVATE void vPrintString(unsigned char *pcMessage);
PRIVATE void vInitUart(void);
PRIVATE void vTimerConfig(void);
PRIVATE void vTimer0ISR(uint32 u32DeviceId, uint32 u32ItemBitmap);
PRIVATE void vDebug(char *pcMessage);
PRIVATE void vDisplayHex8(uint8 u32Data);
PRIVATE void vDisplayHex16(uint16 u32Data);
PRIVATE void Topology(uint16 ShorAddress);
PRIVATE void Read_Sensors(void);
PRIVATE void Display(void);
PRIVATE void Closest_Replica(uint16 SrcX,uint16 SrcY);
PRIVATE void Send_Replication_Message();
PRIVATE void Calculate_Hashes(uint16 No_Replicas);
PRIVATE uint16 Calculate_NextHopAddress(uint16 SrcX,uint16 SrcY,uint16 DstX,uint16 DstY);
PRIVATE void Replication_Algo(uint8 Rep_Num);
PRIVATE void Perform_Replication(uint8 Len,uint8 Get_Root_Node);
PRIVATE void Send_Msg(char *Char_Data_Type,NODE_TYPE Node_Type1);
PRIVATE void Free_Space();


void Read_Sensors(void)
{
    uint16 u16LightSensor,u16TempSensor,u16HumidSensor;
    // u16LightSensor = u16ALSreadChannelResult();
     vHTSstartReadTemp();
     u16TempSensor=u16HTSreadTempResult();
     vHTSstartReadHumidity();
     u16HumidSensor=u16HTSreadHumidityResult();
    //vPrintString("Light Sensor ");
    //vDisplayHex16(u16LightSensor);
    vPrintString("Temp Sensor ");
    vDisplayHex16(u16TempSensor);
    vPrintString("Humid Sensor ");
    vDisplayHex16(u16HumidSensor);
}
/*Displaying Routing and DCS Header Before Transmission*/
void Display()
{
    vPrintString("\tRouting Header \r\n");

    vPrintString("\tSrcX ");
    vDisplayHex16(Routing_Header.Src_Info.SrcX);
     vPrintString("\tSrcY ");
    vDisplayHex16(Routing_Header.Src_Info.SrcY);
     vPrintString("\tDstX ");
    vDisplayHex16(Routing_Header.Dst_Info.DstX);
     vPrintString("\tDstY ");
    vDisplayHex16(Routing_Header.Dst_Info.DstY);

    vPrintString("\tDCS Header \r\n");

    vPrintString("\tOpCode ");
    vDisplayHex16(DCS_Header.OpCode);
     vPrintString("\tFlags ");
    vDisplayHex16(DCS_Header.Flags);
     vPrintString("\tAppID ");
    vDisplayHex16(DCS_Header.AppID);
     vPrintString("\tData_Type ");
    vDisplayHex16(DCS_Header.Data_Type);
    vPrintString("\tLen ");
    vDisplayHex16(DCS_Header.Len);
}


/***********************************
Makes Topology takes nodes Short Address
as input,Topology is Hard Coded
Makes the two linklist 1. Neighbours
                       2. All Nodes
***********************************/


PRIVATE void Topology(uint16 ShorAddress)
{

    vPrintString("ShortAddress ");
vDisplayHex16( ShorAddress);

    Node_Info[0].Address.Node_Shrt_Addr=0x0502U;
    Node_Info[0].G_Loc.X=1;
    Node_Info[0].G_Loc.Y=1;

    Node_Info[1].Address.Node_Shrt_Addr=0x0503U;
    Node_Info[1].G_Loc.X=50;
    Node_Info[1].G_Loc.Y=1;

    Node_Info[2].Address.Node_Shrt_Addr=0x0504U;
    Node_Info[2].G_Loc.X=100;
    Node_Info[2].G_Loc.Y=1;

    Node_Info[3].Address.Node_Shrt_Addr=0x0505U;
    Node_Info[3].G_Loc.X=150;
    Node_Info[3].G_Loc.Y=1;

    Node_Info[4].Address.Node_Shrt_Addr=0x0506U;
    Node_Info[4].G_Loc.X=190;
    Node_Info[4].G_Loc.Y=1;

    Node_Info[5].Address.Node_Shrt_Addr=0x0507U;
    Node_Info[5].G_Loc.X=1;
    Node_Info[5].G_Loc.Y=60;

    Node_Info[6].Address.Node_Shrt_Addr=0x0508U;
    Node_Info[6].G_Loc.X=50;
    Node_Info[6].G_Loc.Y=60;

    Node_Info[7].Address.Node_Shrt_Addr=0x0509U;
    Node_Info[7].G_Loc.X=100;
    Node_Info[7].G_Loc.Y=60;

    Node_Info[8].Address.Node_Shrt_Addr=0x0510U;
    Node_Info[8].G_Loc.X=150;
    Node_Info[8].G_Loc.Y=60;

    Node_Info[9].Address.Node_Shrt_Addr=0x0511U;
    Node_Info[9].G_Loc.X=190;
    Node_Info[9].G_Loc.Y=60;

    Node_Info[10].Address.Node_Shrt_Addr=0x0512U;
    Node_Info[10].G_Loc.X=1;
    Node_Info[10].G_Loc.Y=120;

    Node_Info[11].Address.Node_Shrt_Addr=0x0513U;
    Node_Info[11].G_Loc.X=50;
    Node_Info[11].G_Loc.Y=120;

    Node_Info[12].Address.Node_Shrt_Addr=0x0514U;
    Node_Info[12].G_Loc.X=100;
    Node_Info[12].G_Loc.Y=120;

    Node_Info[13].Address.Node_Shrt_Addr=0x0515U;
    Node_Info[13].G_Loc.X=150;
    Node_Info[13].G_Loc.Y=120;

    Node_Info[14].Address.Node_Shrt_Addr=0x0516U;
    Node_Info[14].G_Loc.X=190;
    Node_Info[14].G_Loc.Y=120;

    Node_Info[15].Address.Node_Shrt_Addr=0x0517U;
    Node_Info[15].G_Loc.X=1;
    Node_Info[15].G_Loc.Y=190;

    Node_Info[16].Address.Node_Shrt_Addr=0x0518U;
    Node_Info[16].G_Loc.X=50;
    Node_Info[16].G_Loc.Y=190;

    Node_Info[17].Address.Node_Shrt_Addr=0x0519U;
    Node_Info[17].G_Loc.X=100;
    Node_Info[17].G_Loc.Y=190;

    Node_Info[18].Address.Node_Shrt_Addr=0x0520U;
    Node_Info[18].G_Loc.X=150;
    Node_Info[18].G_Loc.Y=190;

    Node_Info[19].Address.Node_Shrt_Addr=0x0521U;
    Node_Info[19].G_Loc.X=190;
    Node_Info[19].G_Loc.Y=190;

    Node_Type=NOTHING;



    if(ShorAddress==0x0502U)
     {
        Next_Hop_Head[0].Neighbour_ID=5;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=1;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=6;
         Next_Hop_Head[2].END=TRUE;

         Own_ID=0;

         Node_Type=CONSUMER;



     }else if(ShorAddress==0x0503U)
     {
         Next_Hop_Head[0].Neighbour_ID=0;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=2;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=6;
         Next_Hop_Head[2].END=TRUE;

         Own_ID=1;

         Node_Type=CONSUMER;




     }else if(ShorAddress==0x0504U)
     {
         Next_Hop_Head[0].Neighbour_ID=1;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=3;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=7;
         Next_Hop_Head[2].END=TRUE;

         Own_ID=2;

         Node_Type=CONSUMER;



     }else if(ShorAddress==0x0505U)
     {
         Next_Hop_Head[0].Neighbour_ID=2;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=4;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=8;
         Next_Hop_Head[2].END=TRUE;

         Own_ID=3;


     }else if(ShorAddress==0x0506U)
     {
         Next_Hop_Head[0].Neighbour_ID=3;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=9;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=8;
         Next_Hop_Head[2].END=TRUE;

         Own_ID=4;
          Node_Type=CONSUMER;


     }else if(ShorAddress==0x0507U)
     {
        Next_Hop_Head[0].Neighbour_ID=0;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=6;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=11;
         Next_Hop_Head[2].END=FALSE;

         Next_Hop_Head[3].Neighbour_ID=10;
         Next_Hop_Head[3].END=TRUE;

         Own_ID=5;


     }else if(ShorAddress==0x0508U)
     {
         Next_Hop_Head[0].Neighbour_ID=5;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=0;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=1;
         Next_Hop_Head[2].END=FALSE;

         Next_Hop_Head[3].Neighbour_ID=7;
         Next_Hop_Head[3].END=FALSE;

         Next_Hop_Head[4].Neighbour_ID=12;
         Next_Hop_Head[4].END=FALSE;

         Next_Hop_Head[5].Neighbour_ID=11;
         Next_Hop_Head[5].END=TRUE;

         Own_ID=6;


     }else if(ShorAddress==0x0509U)
     {
        Next_Hop_Head[0].Neighbour_ID=6;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=2;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=8;
         Next_Hop_Head[2].END=FALSE;

         Next_Hop_Head[3].Neighbour_ID=12;
         Next_Hop_Head[3].END=TRUE;

         Own_ID=7;



     }else if(ShorAddress==0x0510U)
     {
         Next_Hop_Head[0].Neighbour_ID=7;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=3;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=4;
         Next_Hop_Head[2].END=FALSE;

         Next_Hop_Head[3].Neighbour_ID=9;
         Next_Hop_Head[3].END=FALSE;

         Next_Hop_Head[4].Neighbour_ID=13;
         Next_Hop_Head[4].END=FALSE;

         Next_Hop_Head[5].Neighbour_ID=12;
         Next_Hop_Head[5].END=TRUE;

         Own_ID=8;


     }else if(ShorAddress==0x0511U)
     {
        Next_Hop_Head[0].Neighbour_ID=8;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=4;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=14;
         Next_Hop_Head[2].END=FALSE;

         Next_Hop_Head[3].Neighbour_ID=13;
         Next_Hop_Head[3].END=TRUE;

         Own_ID=9;




     }else if(ShorAddress==0x0512U)
     {
         Next_Hop_Head[0].Neighbour_ID=5;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=11;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=15;
         Next_Hop_Head[2].END=TRUE;

         Own_ID=10;


     }else if(ShorAddress==0x0513U)
     {
         Next_Hop_Head[0].Neighbour_ID=10;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=5;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=6;
         Next_Hop_Head[2].END=FALSE;

         Next_Hop_Head[3].Neighbour_ID=12;
         Next_Hop_Head[3].END=FALSE;

         Next_Hop_Head[4].Neighbour_ID=17;
         Next_Hop_Head[4].END=FALSE;

         Next_Hop_Head[5].Neighbour_ID=16;
         Next_Hop_Head[5].END=TRUE;

         Own_ID=11;



     }else if(ShorAddress==0x0514U)
     {
        Next_Hop_Head[0].Neighbour_ID=11;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=6;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=7;
         Next_Hop_Head[2].END=FALSE;

         Next_Hop_Head[3].Neighbour_ID=8;
         Next_Hop_Head[3].END=FALSE;

         Next_Hop_Head[4].Neighbour_ID=13;
         Next_Hop_Head[4].END=FALSE;

         Next_Hop_Head[5].Neighbour_ID=17;
         Next_Hop_Head[5].END=TRUE;

         Own_ID=12;


     }else if(ShorAddress==0x0515U)
     {
         Next_Hop_Head[0].Neighbour_ID=12;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=8;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=9;
         Next_Hop_Head[2].END=FALSE;

         Next_Hop_Head[3].Neighbour_ID=14;
         Next_Hop_Head[3].END=FALSE;

         Next_Hop_Head[4].Neighbour_ID=18;
         Next_Hop_Head[4].END=FALSE;

         Next_Hop_Head[5].Neighbour_ID=17;
         Next_Hop_Head[5].END=TRUE;

         Own_ID=13;

          Node_Type=CONSUMER;


     }else if(ShorAddress==0x0516U)
     {
         Next_Hop_Head[0].Neighbour_ID=13;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=9;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=19;
         Next_Hop_Head[2].END=TRUE;

         Own_ID=14;

         Node_Type=CONSUMER;


     }else if(ShorAddress==0x0517U)
     {


         Next_Hop_Head[0].Neighbour_ID=10;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=16;
         Next_Hop_Head[1].END=TRUE;

         Own_ID=15;

          Node_Type=NOTHING;


     }else if(ShorAddress==0x0518U)
     {
         Next_Hop_Head[0].Neighbour_ID=15;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=11;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=17;
         Next_Hop_Head[2].END=TRUE;

         Own_ID=16;

         Node_Type=CONSUMER;


     }else if(ShorAddress==0x0519U)
     {
         Next_Hop_Head[0].Neighbour_ID=16;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=11;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=12;
         Next_Hop_Head[2].END=FALSE;

          Next_Hop_Head[3].Neighbour_ID=13;
         Next_Hop_Head[3].END=FALSE;

          Next_Hop_Head[4].Neighbour_ID=18;
         Next_Hop_Head[4].END=TRUE;

         Own_ID=17;


     }else if(ShorAddress==0x0520U)
     {
         Next_Hop_Head[0].Neighbour_ID=17;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=13;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=19;
         Next_Hop_Head[2].END=TRUE;

         Own_ID=18;
         Node_Type=CONSUMER;




     }else if(ShorAddress==0x0521U)
     {
         Next_Hop_Head[0].Neighbour_ID=14;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=18;
         Next_Hop_Head[1].END=TRUE;

         Own_ID=19;

         Node_Type=NOTHING;
    }

    Source_X=Node_Info[Own_ID].G_Loc.X;
    Source_Y=Node_Info[Own_ID].G_Loc.Y;


      int i=0;
      for (i=0;i<NO_REPLICA;i++)
      {
          Replication_Forwarding_Sets[i][0]=-1;
          Count_Forwarding_Sets[i]=0;
      }


}

/***************************************
Calculate All The Hashes X And Y Takes paramenter as No of Replica for Hashes are to be Generated
****************************************/
PRIVATE void Calculate_Hashes(uint16 No_Replicas)
{



   int i=0;
   uint16 *p;


   for( i=0;i<No_Replicas;i++)
   {
       a:

  p=(uint16 *)MDString(Temp[i],5);

 // uint16 *p=(uint16 *)MDString("TEMP0");
  vPrintString(Temp[i]);
  vPrintString("\r\n");



    Array_HashX[i]=p[0];
    Array_HashY[i]=p[1];
    Array_HashX[i]=Array_HashX[i]%DIMX;
    Array_HashY[i]=Array_HashY[i]%DIMY;
    if(Array_HashX[i]==0&&Array_HashY[i]==0)
    goto a;

    vPrintString("Array_Hash X ");
    vDisplayHex16(Array_HashX[i]);
    vPrintString("Array_Hash Y ");
    vDisplayHex16(Array_HashY[i]);
    }
}


PRIVATE void vTimerConfig(void)
{
    Time_Counter=0;
/* set up timer 0 for count */
    vAHI_TimerEnable(E_AHI_TIMER_0,
                     16,      // prescalar value 2^16
                     FALSE,
                     TRUE,
                     FALSE);
    vAHI_TimerClockSelect(E_AHI_TIMER_0,
                          FALSE,
                          TRUE);    // gate input pin active high
    vAHI_TimerStartRepeat(E_AHI_TIMER_0,
                          0x0000,       // null value
                        //  122);         // number of pulses to count
                        244);  // 244 is 1 sec         // number of pulses to count

    /* register Timer0 interrupt */
    vAHI_Timer0RegisterCallback(vTimer0ISR);
}

PRIVATE void vTimer0ISR(uint32 u32DeviceId, uint32 u32ItemBitmap)
{
    Time_Counter=Time_Counter+1;

    /*Current_Time++;
    vPrintString("Current Time\r\n");
    vDisplayHex16(Current_Time);*/



  //  vPrintString("\n\rinterrupt from timer0");

   //  vTransmitDataPacket(NULL, NULL, 0x0503U);


   //DCS_Header.AppID=HUMIDITY;
   if(!START&&Time_Counter>=60&&OPR_MODE==0&&Time_Counter<(Run_Time+1)*60)
   {

       if(!START)
            {
                vPrintString("Timer Starting \r\n");

            vLightBulbInit();
            START=TRUE;
            }

   }

   else if(Time_Counter>=(Run_Time+1)*60&&START&&OPR_MODE==0)
   {

       START=FALSE;
       vPrintString("Nodes have automatically stopped you can check the results \r\n");

    }else if(OPR_MODE==0&&START)
            {
                PRODUCER_Counter=PRODUCER_Counter+1;
                CONSUMER_Counter=CONSUMER_Counter+1;

                char * Char_Data_Type="TEMP";

            DCS_Header.AppID=TEMP;
           // if(Node_Type==CONSUMER&&CONSUMER_Counter==2)
         if(CONSUMER_Counter==15)
            {
                CONSUMER_Counter=0;
                Send_Msg(Char_Data_Type,CONSUMER);

            }

            if(PRODUCER_Counter==45)
            {
                PRODUCER_Counter=0;
                Send_Msg(Char_Data_Type,PRODUCER);
            }


            }
}

/********
Displaying Functions in HexaDecimal
*******/


PRIVATE void vDebug(char *pcMessage)
{

    while (*pcMessage)

    {

        while ((u8AHI_UartReadLineStatus(0) & 0x20) == 0);

        vAHI_UartWriteData(0, *pcMessage);

        pcMessage++;

    }

}

PRIVATE void vDisplayHex8(uint8 u32Data)

{

    char acValue[9];

    char *pcString = acValue;

    uint8 u8Nybble;

    int i, j;



    j = 0;

    for (i = 28; i >= 0; i -= 4)

    {

        u8Nybble = (uint8)((u32Data >> i) & 0x0f);

        u8Nybble += 0x30;

        if (u8Nybble > 0x39)

            u8Nybble += 7;



        *pcString = u8Nybble;

        pcString++;

    }

    *pcString = '\0';


vDebug("\t");
    vDebug(acValue);
    vDebug("\r\n");

}

PRIVATE void vDisplayHex16(uint16 u32Data)

{

    char acValue[9];

    char *pcString = acValue;

    uint8 u8Nybble;

    int i, j;



    j = 0;

    for (i = 28; i >= 0; i -= 4)

    {

        u8Nybble = (uint8)((u32Data >> i) & 0x0f);

        u8Nybble += 0x30;

        if (u8Nybble > 0x39)

            u8Nybble += 7;



        *pcString = u8Nybble;

        pcString++;

    }

    *pcString = '\0';


 vDebug("\t");
    vDebug(acValue);
    vDebug("\r\n");

}



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


	/* General initialisation: set up MAC API */
    vInitSystem();

    // Uart cannot be used for both debug and print
       vInitUart();


    sCoordData.sSystem.eState = E_STATE_INIT;
    Topology(COORD_ADDR);
    Calculate_Hashes(NO_REPLICA);
vPrintString("Topology Initiated \r\n ");
 vLightBulbInit();



   vTimerConfig();

    while (1)
    {

        vProcessEventQueues(); /* Process event(s) posted by interrupt(s) */


        switch (sCoordData.sSystem.eState)
        {
        case E_STATE_INIT:
            sCoordData.sSystem.u8Channel = 0;
            sCoordData.sSystem.eState = E_STATE_START_COORDINATOR;
            break;

        case E_STATE_START_COORDINATOR:
            if(bStartCoordinator())
            {
                vLedControl(LED3, TRUE);
                sCoordData.sSystem.eState = E_STATE_RUNNING_LIGHT_BULB_APP;
            }
            break;

        case E_STATE_RUNNING_LIGHT_BULB_APP:

            vProcessEventQueues(); /* Process event(s) posted by interrupt(s) */

            break;
        }
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
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PUBLIC void AppWarmStart(void)
{
    AppColdStart();
}

/****************************************************************************
 *
 * NAME: vInitSystem
 *
 * DESCRIPTION:
 * Initialises stack and hardware, sets non-default values in the 802.15.4
 * PIB.
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

    sCoordData.sNode.u8AssociatedNodes = 0;

    /* Set Pan ID and short address in PIB (also sets match registers in hardware) */
    MAC_vPibSetPanId(s_pvMac, PAN_ID);
    MAC_vPibSetShortAddr(s_pvMac, COORD_ADDR);

    s_psMacPib->bAssociationPermit = 1;
    MAC_vPibSetRxOnWhenIdle(s_pvMac, TRUE, FALSE);




    /* Set SW1(dio9) to input and LED1(dio14) to output */

    vAHI_DioSetDirection(E_AHI_DIO9_INT, 0);  // For Temp
    vAHI_DioSetDirection(E_AHI_DIO10_INT, 0); // For Humid
    vAHI_DioSetDirection(E_AHI_DIO11_INT, 0);  // For Query


   // vAHI_DioSetDirection(0, E_AHI_DIO14_INT);
  //  vAHI_DioSetDirection(0, E_AHI_DIO15_INT);

    /* enable pullups on all inputs/unused pins  */
   // vAHI_DioSetPullup(!E_AHI_DIO14_INT, E_AHI_DIO14_INT);

    /* turn LED off */
   // vAHI_DioSetOutput(E_AHI_DIO14_INT, 0);

    /* enable pullups on all inputs/unused pins  */
   // vAHI_DioSetPullup(!E_AHI_DIO15_INT, E_AHI_DIO15_INT);

    /* turn LED off */
   // vAHI_DioSetOutput(E_AHI_DIO15_INT, 0);

    /* set interrupt for DIO9 to occur on button release - rising edge */
    vAHI_DioInterruptEdge(E_AHI_DIO9_INT, 0);
    vAHI_DioInterruptEdge(E_AHI_DIO10_INT, 0);
    vAHI_DioInterruptEdge(E_AHI_DIO11_INT, 0);



    /* enable interrupt for DIO9 */
    vAHI_DioInterruptEnable(E_AHI_DIO9_INT, 0);
    vAHI_DioInterruptEnable(E_AHI_DIO10_INT, 0);
    vAHI_DioInterruptEnable(E_AHI_DIO11_INT, 0);

}

/****************************************************************************
 *
 * NAME: vLightBulbInit
 *
 * DESCRIPTION:
 * Initialises state of light bulb, configure DIO lines that drive the LEDs
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
PRIVATE void vLightBulbInit(void)
{
    /* Light bulb simulation uses LEDs D1-4 that are connected to GPIO14-17.
       Set these IO lines to outputs and then set them high to turn the LEDs
       off. */
    vLedInitFfd();
    vLightBulb(OFF);
    bLightBulbState = OFF;
    vLedControl(LED1, FALSE);
    vLedControl(LED2, FALSE);
    if(Node_Type==CONSUMER)
    {
        vLedControl(LED1, TRUE);
        vLedControl(LED2, FALSE);
    }
    else if(Node_Type==PRODUCER)
    {
        vLedControl(LED1, FALSE);
        vLedControl(LED2, TRUE);

    }
    else if(Node_Type==REPLICA)
    {
        vLedControl(LED1, TRUE);
        vLedControl(LED2, TRUE);

    }
    else if(Node_Type==NOTHING)
    {
        vLedControl(LED1, FALSE);
        vLedControl(LED2, FALSE);

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
    switch(psMlmeInd->u8Type)
    {
    case MAC_MLME_IND_ASSOCIATE:
        /* Only allow nodes to associate if network has been started */
        if (sCoordData.sSystem.eState == E_STATE_RUNNING_LIGHT_BULB_APP)
        {
            vHandleNodeAssociation(psMlmeInd);
        }
        break;
    }
}

/**************************************************************************
uint16 Calculate_NextHopAddress(uint16 SrcX,uint16 SrcY);

Calucuñates the the Next Hop from the node,which is nearest to the Destination
***************************************************************************/
uint16 Calculate_NextHopAddress(uint16 SrcX,uint16 SrcY,uint16 DstX,uint16 DstY){
     int dist=(DstX-SrcX)*(DstX-SrcX) + (DstY-SrcY)*(DstY-SrcY);
    uint16 NextHop_Addr=COORD_ADDR;
    int tempdist;
   int i=-1;
      do{
            i++;
            tempdist=(DstX-Node_Info[Next_Hop_Head[i].Neighbour_ID].G_Loc.X)*(DstX-Node_Info[Next_Hop_Head[i].Neighbour_ID].G_Loc.X)+(DstY-Node_Info[Next_Hop_Head[i].Neighbour_ID].G_Loc.Y)*(DstY-Node_Info[Next_Hop_Head[i].Neighbour_ID].G_Loc.Y);


            if(tempdist<dist)
            {
                dist=tempdist;
                NextHop_Addr=Node_Info[Next_Hop_Head[i].Neighbour_ID].Address.Node_Shrt_Addr;

            }

        }while(Next_Hop_Head[i].END!=TRUE);

        return NextHop_Addr;

    }
/**********************************************************************
Removes the older entries and cretes space for new entries of PUT and
REPLICATION REQ.

Removes oldest 10 elements from the array and creates space for new 10.

Clalled when space is full

***********************************************************************/
PRIVATE void Free_Space()
{
    //tsData_Storage tempData;
    TEMP_Counter=MAX_STORAGE-10;
    int i=0;
    for(i=0;i<MAX_STORAGE-10;i++)
    TEMP_Storage[i]=TEMP_Storage[i+10];

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
    MAC_RxFrameData_s *psFrame;
    uint16 u16NodeAddr;

    psFrame = &psMcpsInd->uParam.sIndData.sFrame;


    /* Check that this is a data frame */
    if (psMcpsInd->u8Type == MAC_MCPS_IND_DATA)
    {



        /* Check that data is from one of the light switches */
        u16NodeAddr = psFrame->sSrcAddr.uAddr.u16Short;
        vPrintString("Packet Recieved \r\n ");



        int i=-1;
        bool valid=FALSE;
        do{
            i++;
            if(u16NodeAddr==Node_Info[Next_Hop_Head[i].Neighbour_ID].Address.Node_Shrt_Addr)
            {
                valid=TRUE;
                break;
            }

        }while(Next_Hop_Head[i].END!=TRUE);

       // if ((u16NodeAddr == sCoordData.sNode.asAssocNodes[0].u16ShortAddr)
       //  || (u16NodeAddr == sCoordData.sNode.asAssocNodes[1].u16ShortAddr)||u16NodeAddr==0x0503U)
        if(valid)
        {
            Msg_Recv++;
            Msg_Con++;
            Bytes_Recv=Bytes_Recv+psFrame->u8SduLength;
            /* Toggle light bulb */

            //modification
            tsHeaders *Ptr=(tsHeaders *)psFrame->au8Sdu;


    uint16 Src_Add=COORD_ADDR;


    uint16 Dst_Add;
    uint16 DstX=Ptr->Routing_Header.Dst_Info.DstX;
    uint16 DstY=Ptr->Routing_Header.Dst_Info.DstY;
    if(Ptr->Routing_Header.TTL==0)
    {
        vPrintString("TTL ZERO DISCARDED \r\n ");
       // return;

    }
    Ptr->Routing_Header.TTL=Ptr->Routing_Header.TTL-1;


        uint16 NextHop_Addr=Calculate_NextHopAddress(Source_X,Source_Y,DstX,DstY);

        if(NextHop_Addr==Src_Add)
        {
            vPrintString("I am the Destination,Data won't transmit \r\n");


            uint8 *ptr8=psFrame->au8Sdu;
            vPrintString("\tRecieved ");
            vDisplayHex16(Ptr->Routing_Header.Dst_Info.DstX);
            vDisplayHex16(Ptr->Routing_Header.Dst_Info.DstY);


             if(Ptr->DCS_Header.OpCode==PUT&&Ptr->DCS_Header.AppID==TEMP)
             {
                 vPrintString("\tRECIEVED PUT REQUEST\r\n");
                 tsPUTData *PtrD=&ptr8[sizeof(Headers)];
                 vPrintString("\tNo Of Structures In PUT");
                 vDisplayHex16(Ptr->DCS_Header.Len);

           /*  vDisplayHex16(PtrD[0].Value);
             vDisplayHex16(PtrD[0].Time_Stamp);
             vDisplayHex16(PtrD[1].Value);
             vDisplayHex16(PtrD[1].Time_Stamp);*/
              // tsData_Storage Rep_Data[Ptr->DCS_Header.Len];


             if(Temp_Replica==FALSE)
                {
                     Temp_Replica=TRUE;
                     Node_Type=REPLICA;
                     vLightBulbInit();

                     int i=0;
                     for(i=0;i<NO_REPLICA;i++)
                     {
                        if(Array_HashX[i]==Ptr->Routing_Header.Dst_Info.DstX&&Array_HashY[i]==Ptr->Routing_Header.Dst_Info.DstY)
                         {
                             Replica_Num=i;
                             vPrintString("\tReplica_Num= ");
                             vDisplayHex16(i);

                             break;
                         }
                     }

                }

             if(TEMP_Counter==MAX_STORAGE)
                    {
                        vPrintString("\tReached Storage Limit Freeing Space  \r\n");
                    Free_Space();
                    }
             //else
             {

                 int i=0;
                 for(i=0;i<Ptr->DCS_Header.Len;i++)

                 {

                 TEMP_Storage[TEMP_Counter].CoordX=Ptr->Routing_Header.Src_Info.SrcX;
                 TEMP_Storage[TEMP_Counter].CoordY=Ptr->Routing_Header.Src_Info.SrcY;

                TEMP_Storage[TEMP_Counter].Value=PtrD[i].Value;
                TEMP_Storage[TEMP_Counter].Time_Stamp=PtrD[i].Time_Stamp;

                 vPrintString("\tTemp Value ");
                 vDisplayHex16(PtrD[i].Value);
                 vPrintString("\tTemp TimeStamp ");
                 vDisplayHex16(PtrD[i].Time_Stamp);
                Rep_Data[i]=TEMP_Storage[TEMP_Counter];
                    TEMP_Counter++;
                    }
             }

    Ptr->DCS_Header.Rep_Num=Replica_Num;
    Perform_Replication(Ptr->DCS_Header.Len,Ptr->DCS_Header.Rep_Num);


             }else if(Ptr->DCS_Header.OpCode==REPLICATION)
             {

                 vPrintString("\tRECIEVED REPLICATION REQUEST\r\n");

                 vPrintString("\tNo Of Structures In REPLICATION REQ");
                 vDisplayHex16(Ptr->DCS_Header.Len);
                 tsData_Storage *PtrD=&ptr8[sizeof(Headers)];
                // tsData_Storage Rep_Data[Ptr->DCS_Header.Len];




             if(Temp_Replica==FALSE)
                {
                     Temp_Replica=TRUE;
                     Node_Type=REPLICA;
                     vLightBulbInit();
                     int i=0;
                     for(i=0;i<NO_REPLICA;i++)
                     {
                        if(Array_HashX[i]==Ptr->Routing_Header.Dst_Info.DstX&&Array_HashY[i]==Ptr->Routing_Header.Dst_Info.DstY)
                         {
                             Replica_Num=i;
                             break;
                         }
                     }

                }

             if(TEMP_Counter==MAX_STORAGE)
                    {
                        vPrintString("\tReached Storage Limit Freeing Space  \r\n");
                        Free_Space();
                    }
             //else
             {

                 int i=0;
                 for(i=0;i<Ptr->DCS_Header.Len;i++)

                 {

                 TEMP_Storage[TEMP_Counter].CoordX=PtrD[i].CoordX;
                 TEMP_Storage[TEMP_Counter].CoordY=PtrD[i].CoordY;

                TEMP_Storage[TEMP_Counter].Value=PtrD[i].Value;
                TEMP_Storage[TEMP_Counter].Time_Stamp=PtrD[i].Time_Stamp;

                Rep_Data[i].CoordX=PtrD[i].CoordX;
                Rep_Data[i].CoordY=PtrD[i].CoordY;
                Rep_Data[i].Value=PtrD[i].Value;
                Rep_Data[i].Time_Stamp=PtrD[i].Time_Stamp;


                 vPrintString("\tTemp Value ");
                 vDisplayHex16(PtrD[i].Value);
                 vPrintString("\tTemp TimeStamp ");
                 vDisplayHex16(PtrD[i].Time_Stamp);

                    TEMP_Counter++;
                    }
             }
             Perform_Replication(Ptr->DCS_Header.Len,Ptr->DCS_Header.Rep_Num);



             }
             else if(Ptr->DCS_Header.OpCode==GET)
             {
                 vPrintString("RECIEVED GET REQUEST\r\n");
                 int Len=Ptr->DCS_Header.Len;

                 vPrintString("\tNo Of Structures Requested In GET");
                 vDisplayHex16(Ptr->DCS_Header.Len);

                 uint8* pu8Payload;
                 if(Ptr->DCS_Header.AppID==TEMP)
                 {
                     tsData_Storage Resp_Data[Len];


                 int i=0;
                 for(i=0;i<Len;i++)
                 {
                     Resp_Data[i].CoordX=TEMP_Storage[TEMP_Counter-1-i].CoordX;
                     Resp_Data[i].CoordY=TEMP_Storage[TEMP_Counter-1-i].CoordY;
                     Resp_Data[i].Value=TEMP_Storage[TEMP_Counter-1-i].Value;
                     Resp_Data[i].Time_Stamp=TEMP_Storage[TEMP_Counter-1-i].Time_Stamp;

                      vPrintString("\tCoordX ");
                        vDisplayHex16(Resp_Data[i].CoordX);
                 vPrintString("\tCoordY ");
                 vDisplayHex16(Resp_Data[i].CoordY);
                  vPrintString("\tTemp Value ");
                 vDisplayHex16(Resp_Data[i].Value);
                 vPrintString("\tTemp TimeStamp ");
                 vDisplayHex16(Resp_Data[i].Time_Stamp);

                 }
uint16 NextHop_Addr=Calculate_NextHopAddress(Source_X,Source_Y,Ptr->Routing_Header.Src_Info.SrcX,Ptr->Routing_Header.Src_Info.SrcY);

    Routing_Header.Version=1;   // 0
    Routing_Header.ToS=1;       // 1
    Routing_Header.TTL=5;       //2        //decrement it in each hop,when reduces to zero then discard it
    Routing_Header.Proto=1;     //3
    Routing_Header.Src_Info.SrcX=Source_X;       //4-5
    Routing_Header.Src_Info.SrcY=Source_Y;       // 6-7
    Routing_Header.Dst_Info.DstX=Ptr->Routing_Header.Src_Info.SrcX;       // 8-9
    Routing_Header.Dst_Info.DstY=Ptr->Routing_Header.Src_Info.SrcY;       // 10-11

    DCS_Header.OpCode=GET_RESP;     //12
    DCS_Header.Flags=0;      //13
    // Data_Transmit.Data_Type=TEMP;       //16
    DCS_Header.Data_Type=1;
    DCS_Header.Len=Len;

    vPrintString("\tBytes of Data");
    vDisplayHex16(sizeof(tsData_Storage));
    vDisplayHex16(DCS_Header.Len*sizeof(tsData_Storage));               //17

    uint8 * PtrD=(uint8 *)&Resp_Data;
    Headers.Routing_Header=Routing_Header;
    Headers.DCS_Header=DCS_Header;
    tsHeaders *Ptr=&Headers;
    pu8Payload=(uint8 *)Ptr;

    for(i=0;i<sizeof(Headers);i++)
    {
        PayLoad[i]=pu8Payload[i];
    }
    for(i=0;i<DCS_Header.Len*sizeof(tsData_Storage);i++)
    {
        PayLoad[sizeof(Headers)+i]=PtrD[i];

    }
    /*vPrintString("\tSize Of Header ");
    vDisplayHex16(sizeof(Routing_Header));
     vDisplayHex16(sizeof(DCS_Header));*/

     if(NextHop_Addr==Src_Add)
        vPrintString("\tI am the Destination,Data won't GET_RESPONSE \r\n");
        else{
            vPrintString("\tNext_Hop Address ");
            vDisplayHex16(NextHop_Addr);
            vPrintString("\tSENDING GET RESPONSE\r\n");
            Display();
            vTransmitDataPacket(PayLoad,sizeof(Headers)+DCS_Header.Len*sizeof(tsData_Storage), NextHop_Addr);
            }
            }
            }
             else if(Ptr->DCS_Header.OpCode==GET_RESP)
             {
                 tsData_Storage *PtrD=&ptr8[sizeof(Headers)];
            vPrintString("RECIEVED GET RESPONSE \r\n");

             for(i=0;i<Ptr->DCS_Header.Len;i++)
                 {

                      vPrintString("\tCoordX ");
                        vDisplayHex16(PtrD[i].CoordX);
                 vPrintString("\tCoordY ");
                 vDisplayHex16(PtrD[i].CoordY);
                  vPrintString("\tTemp Value ");
                 vDisplayHex16(PtrD[i].Value);
                 vPrintString("\tTemp TimeStamp ");
                 vDisplayHex16(PtrD[i].Time_Stamp);

                 }


            }
        }
        else{

            vPrintString("Packet Forwarding \r\n");
    vPrintString("\tNext_Hop Address ");
    vDisplayHex16(NextHop_Addr);
    vTransmitDataPacket((uint8 *)psFrame->au8Sdu,psFrame->u8SduLength, NextHop_Addr);

          /*  if (bLightBulbState == OFF)
            {
                vLightBulb(ON);
            }
            else
            {
                vLightBulb(OFF);
            }*/
        }
        }
    }
}

/***************************************************************************

***************************************************************************/
PRIVATE void Replication_Algo(uint8 Rep_Num)
{
 //   vPrintString("\tReplica Algo \r\n");

   // vPrintString("\tReplica_Num \r\n");
   // vDisplayHex16(Replica_Num);

   uint8 Selected_Nodes[10];
    uint8 Non_Selected_Nodes[10][2];
    uint8 Num_Non_Sel_Nodes;
    Replication_Forwarding_Sets[Rep_Num][0]=-1;
    Count_Forwarding_Sets[Rep_Num]=0;
    int i=0,j=0,Num_Sel_Nodes=0,I,J;;
    for(i=0;i<NO_REPLICA;i++)
    {
        if(i!=Rep_Num)
        {
            Non_Selected_Nodes[j][0]=i;
            Non_Selected_Nodes[j][1]=1;
            j++;
        }
    }
    Non_Selected_Nodes[j][0]=-1;
    Num_Non_Sel_Nodes=j;
    J=j;

    Selected_Nodes[Num_Sel_Nodes]=Rep_Num;
    Num_Sel_Nodes++;
    Selected_Nodes[Num_Sel_Nodes]=-1;
    int dist=2*(DIMX*DIMX+DIMY*DIMY);
    int tempdist;
    uint8 Non_Sel_Node,Sel_Node;
    while(Num_Non_Sel_Nodes!=0)
    {
        dist=2*(DIMX*DIMX+DIMY*DIMY);
        // vPrintString("\tNum_Non_Sel_Nodes \r\n");
       // vDisplayHex16(Num_Non_Sel_Nodes);
    i=0;
  //  while(Non_Selected_Nodes[i][0]!=-1)
    while(i<J)
    {
     //   vPrintString("\tNon_Selected_Nodes[i][0] \r\n");
      //  vDisplayHex16(Non_Selected_Nodes[i][0]);
        if(Non_Selected_Nodes[i][1]==1)
        {
        j=0;
        while(j<Num_Sel_Nodes)
        {
            tempdist=(Array_HashX[Selected_Nodes[j]]-Array_HashX[Non_Selected_Nodes[i][0]])*(Array_HashX[Selected_Nodes[j]]-Array_HashX[Non_Selected_Nodes[i][0]])+(Array_HashY[Selected_Nodes[j]]-Array_HashY[Non_Selected_Nodes[i][0]])*(Array_HashY[Selected_Nodes[j]]-Array_HashY[Non_Selected_Nodes[i][0]]);
            if(tempdist<dist)
            {
               // tempdist=dist;
                dist=tempdist;
                Non_Sel_Node=Non_Selected_Nodes[i][0];
                Sel_Node=Selected_Nodes[j];
                I=i;

            }
            j++;
        }
        }
        i++;
    }
    Non_Selected_Nodes[I][1]=0;

    if(Sel_Node==Replica_Num)
    {
        Replication_Forwarding_Sets[Rep_Num][Count_Forwarding_Sets[Rep_Num]]=Non_Sel_Node;
        Count_Forwarding_Sets[Rep_Num]++;

        Selected_Nodes[Num_Sel_Nodes]=Non_Sel_Node;
        Num_Sel_Nodes++;
         vPrintString("\tAdded in Selected set \r\n");
         vDisplayHex16(Non_Sel_Node);
         vDisplayHex16(Replica_Num);
         vDisplayHex16(Sel_Node);

    }else{
        Selected_Nodes[Num_Sel_Nodes]=Non_Sel_Node;
        Num_Sel_Nodes++;
        vPrintString("\tAdded in Non Selected set \r\n");
         vDisplayHex16(Non_Sel_Node);
         vDisplayHex16(Replica_Num);
         vDisplayHex16(Sel_Node);


        }
        Num_Non_Sel_Nodes--;
        // vPrintString("\tReplication Algo exit \r\n");
    }
    vPrintString("\tForwarding Set \r\n");
    for(i=0;i<Count_Forwarding_Sets[Rep_Num];i++)
    {


        vDisplayHex16(Replication_Forwarding_Sets[Rep_Num][i]);
        vPrintString("\tForwarding Set \r\n");

    }

}
PRIVATE void Perform_Replication(uint8 Len,uint8 Get_Root_Node)
{


               vPrintString("\tStarting Replication \r\n");
               vPrintString("\tGet_Root_Node \r\n");
                vDisplayHex8(Get_Root_Node);
   int Num=0;


   Replication_Algo(Get_Root_Node);

    for(Num=0;Num<Count_Forwarding_Sets[Get_Root_Node];Num++)
    {




        if(Replication_Forwarding_Sets[Get_Root_Node][Num]!=Replica_Num)

        {
            uint16 DstX=Array_HashX[Replication_Forwarding_Sets[Get_Root_Node][Num]];
            uint16 DstY=Array_HashY[Replication_Forwarding_Sets[Get_Root_Node][Num]];
            uint16 NextHop_Addr=Calculate_NextHopAddress(Source_X,Source_Y,DstX,DstY);

    Routing_Header.Version=1;   // 0
    Routing_Header.ToS=1;       // 1
    Routing_Header.TTL=5;       //2        //decrement it in each hop,when reduces to zero then discard it
    Routing_Header.Proto=1;     //3
    Routing_Header.Src_Info.SrcX=Source_X;       //4-5
    Routing_Header.Src_Info.SrcY=Source_Y;       // 6-7
   // Routing_Header.Dst_Info.DstX=Array_HashX[Num];       // 8-9
   // Routing_Header.Dst_Info.DstY=Array_HashY[Num];       // 10-11

    Routing_Header.Dst_Info.DstX=DstX;      // 8-9
    Routing_Header.Dst_Info.DstY=DstY;      // 10-11


    DCS_Header.OpCode=REPLICATION;     //12
    DCS_Header.Flags=0;      //13
    // Data_Transmit.Data_Type=TEMP;       //16
    DCS_Header.Data_Type=1;
    DCS_Header.Len=Len;
    DCS_Header.Rep_Num=Get_Root_Node;

    uint8 * PtrD=(uint8 *)&Rep_Data;
    Headers.Routing_Header=Routing_Header;
    Headers.DCS_Header=DCS_Header;
    tsHeaders *Ptr=&Headers;
    uint8 *pu8Payload=(uint8 *)Ptr;
    int i;

    for(i=0;i<sizeof(Headers);i++)
    {
        PayLoad[i]=pu8Payload[i];
    }
    for(i=0;i<DCS_Header.Len*sizeof(tsData_Storage);i++)
    {
        PayLoad[sizeof(Headers)+i]=PtrD[i];
    }

     vPrintString("\tSENDING REPLICATION MESSAGE\r\n");
     vPrintString("\tNext_Hop Address ");
     vDisplayHex16(NextHop_Addr);
     Display();

    vTransmitDataPacket(PayLoad,sizeof(Headers)+DCS_Header.Len*sizeof(tsData_Storage), NextHop_Addr);


    }
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


     /* Check to see if hardware indication was from DIO9 */
   // if ((psAHI_Ind->u32DeviceId == E_AHI_DEVICE_SYSCTRL)
        //&& (psAHI_Ind->u32ItemBitmap & E_AHI_DIO9_INT))
    if ((psAHI_Ind->u32DeviceId == E_AHI_DEVICE_SYSCTRL)
        && ((psAHI_Ind->u32ItemBitmap & E_AHI_DIO9_INT)||(psAHI_Ind->u32ItemBitmap & E_AHI_DIO10_INT)||(psAHI_Ind->u32ItemBitmap & E_AHI_DIO11_INT)))
    {
        if(OPR_MODE==1)
        {
        char *Char_Data_Type;

        //Read_Sensors();
        if((psAHI_Ind->u32ItemBitmap & E_AHI_DIO9_INT))
        {
            Char_Data_Type="TEMP";
            Node_Type=PRODUCER;
            DCS_Header.AppID=TEMP;
            vLightBulbInit();
        }
        else if((psAHI_Ind->u32ItemBitmap & E_AHI_DIO10_INT))
        {
            //Char_Data_Type="HUMIDITY";
            //DCS_Header.AppID=HUMIDITY;
            Char_Data_Type="TEMP";
            Node_Type=CONSUMER;
            DCS_Header.AppID=TEMP;
            vLightBulbInit();
        }else if((psAHI_Ind->u32ItemBitmap & E_AHI_DIO11_INT))
        {
            //Char_Data_Type="HUMIDITY";
            //DCS_Header.AppID=HUMIDITY;
            Char_Data_Type="TEMP";
            Node_Type=CONSUMER;
            DCS_Header.AppID=TEMP;
            vLightBulbInit();
        }
Send_Msg(Char_Data_Type,Node_Type);
        }else if(OPR_MODE==0)
        {


            if((psAHI_Ind->u32ItemBitmap & E_AHI_DIO9_INT))
        {
            if(!START)
            {
                vPrintString("Timer Starting \r\n");
            vTimerConfig();
            vLightBulbInit();
            START=TRUE;
            }else

            {
                vPrintString("Timer Stops \r\n");
                START = FALSE;
                vLightBulbInit();
            }
        }
        else if((psAHI_Ind->u32ItemBitmap & E_AHI_DIO10_INT))
        {
            if(START==FALSE)
            {
                vPrintString("Printing Summary \r\n");
                vPrintString("\t Short Address ");
                vDisplayHex16(COORD_ADDR);

                vPrintString("\t Mesg Sent ");
                vDisplayHex16(Msg_Sent);
                vPrintString("\t Msg Recv ");
                vDisplayHex16(Msg_Recv);

                vPrintString("\t Mesg Gen ");
                vDisplayHex16(Msg_Gen);
                vPrintString("\t Mesg Cons ");
                vDisplayHex16(Msg_Con);

            }else
            {
                vPrintString("First stop Timer \r\n");

            }

        }else if((psAHI_Ind->u32ItemBitmap & E_AHI_DIO11_INT))
        {
             if(START==FALSE)
            {
                vPrintString("Printing Summary \r\n");
                vPrintString("\t Short Address ");
                vDisplayHex16(COORD_ADDR);

                vPrintString("\t Mesg Sent ");
                vDisplayHex16(Msg_Sent);
                vPrintString("\t Msg Recv ");
                vDisplayHex16(Msg_Recv);

                 vPrintString("\t Mesg Gen ");
                vDisplayHex16(Msg_Gen);
                vPrintString("\t Mesg Con ");
                vDisplayHex16(Msg_Con);

            }else
            {
                vPrintString("First stop Timer \r\n");

            }

        }


        }


   /*  if(bToggle)
        {
            // drive DIO14 LOW
            vAHI_DioSetOutput(0, E_AHI_DIO14_INT);
        }
        else
        {
            // drive DIO14 HIGH
            vAHI_DioSetOutput(E_AHI_DIO14_INT, 0);
        }

        bToggle = !bToggle;*/
    }
    /* No hardware events generated to nothing to be done here */
}


/***********************************************
NAME:Sned Msg either PUT REQUEST or GET REQUEST
************************************************/
void Send_Msg(char *Char_Data_Type,NODE_TYPE Node_Type1)
{

    int i=0;
   uint8 *pu8Payload;


 /*  uint16 HashX=p[0];
   uint16 HashY=p[1];
   HashX=HashX%DIMX;
   HashY=HashY%DIMY;

   vPrintString("DIGEST X ");
   vDisplayHex16(HashX);
   vPrintString("DIGEST Y ");
   vDisplayHex16(HashY);*/

   uint16 Src_Add=COORD_ADDR;
   uint16 Dst_Add;

   Closest_Replica(Source_X,Source_Y);
   vPrintString("DIGEST X ");
   vDisplayHex16(HashX);
   vPrintString("DIGEST Y ");
   vDisplayHex16(HashY);

   uint16 NextHop_Addr=Calculate_NextHopAddress(Source_X,Source_Y,HashX,HashY);

    Routing_Header.Version=1;   // 0
    Routing_Header.ToS=1;       // 1
    Routing_Header.TTL=5;       //2        //decrement it in each hop,when reduces to zero then discard it
    Routing_Header.Proto=1;     //3
    Routing_Header.Src_Info.SrcX=Source_X;       //4-5
    Routing_Header.Src_Info.SrcY=Source_Y;       // 6-7
    Routing_Header.Dst_Info.DstX=HashX;       // 8-9
    Routing_Header.Dst_Info.DstY=HashY;       // 10-11


   if(Node_Type1==CONSUMER)    //Query
   {
       vPrintString("CONSUMER GET DATA \r\n");

        DCS_Header.OpCode=GET;     //12
        DCS_Header.Flags=0;      //13
        DCS_Header.Data_Type=1;      //16
        DCS_Header.Len=No_Of_GET;
       // DCS_Header.Rep_Num=Replica_Num;

        Headers.Routing_Header=Routing_Header;
        Headers.DCS_Header=DCS_Header;

        tsHeaders *Ptr=&Headers;

    pu8Payload=(uint8 *)Ptr;
    int i=0;
    for(i=0;i<sizeof(Headers);i++)
    {
        PayLoad[i]=pu8Payload[i];
    }
    vPrintString("\tSize Of Header ");
    vDisplayHex16(sizeof(Routing_Header));
     vDisplayHex16(sizeof(DCS_Header));

        if(NextHop_Addr==Src_Add)
        {
            vPrintString("\tI am the Destination,won't GET Data \r\n");
            Msg_Con++;
        }
        else{
            Display();
            vPrintString("\tNext_Hop Address ");
            vDisplayHex16(NextHop_Addr);
            vTransmitDataPacket(PayLoad,sizeof(Headers), NextHop_Addr);
            }                //17
    }
   else if(Node_Type1==PRODUCER){

       vPrintString("PRODUCER PUT DATA \r\n");

       DCS_Header.OpCode=PUT;     //12
       DCS_Header.Flags=0;      //13
       DCS_Header.Data_Type=1;       //16
       // Data_Transmit.Data_Type=TEMP;       //16
       DCS_Header.Len=No_Of_PUT;                 //17
     //  DCS_Header.Rep_Num=Replica_Num;
       tsPUTData PUTData[DCS_Header.Len];
       int num=DCS_Header.Len,i;


    for(i=0;i<num;i++)
    {
      /*if(DCS_Header.AppID=TEMP)
        PUTData[i].Value=u16HTSreadTempResult();
      else if(DCS_Header.AppID=HUMIDITY)
        PUTData[i].Value=u16HTSreadTempResult();*/
        PUTData[i].Value=25;
        PUTData[i].Time_Stamp=Seq_Num;
    }

   /* uint16 u16LightSensor,u16TempSensor,u16HumidSensor;
    // u16LightSensor = u16ALSreadChannelResult();
     vHTSstartReadTemp();
      vPrintString("Temp Value");
     u16TempSensor=u16HTSreadTempResult();
     vDisplayHex16(u16TempSensor);
    Data_Transmit.Data[0]=(uint8)u16TempSensor;     //20-21-22

    vDisplayHex8(Data_Transmit.Data[0]);*/
    uint8 * PtrD=PUTData;
    Headers.Routing_Header=Routing_Header;
    Headers.DCS_Header=DCS_Header;
    tsHeaders *Ptr=&Headers;
    pu8Payload=(uint8 *)Ptr;

    for(i=0;i<sizeof(Headers);i++)
    {
        PayLoad[i]=pu8Payload[i];
    }

    for(i=0;i<DCS_Header.Len*sizeof(tsPUTData);i++)
    {
        PayLoad[sizeof(Headers)+i]=PtrD[i];
    }

    /*vPrintString("\tSize Of Header ");
    vDisplayHex16(sizeof(Routing_Header));
     vDisplayHex16(sizeof(DCS_Header));*/

     if(NextHop_Addr==Src_Add)
     {
        vPrintString("\tI am the Destination,Data won't transmit \r\n");
        Msg_Gen++;

        if(Temp_Replica==FALSE)
                {
                     Temp_Replica=TRUE;
                     Node_Type=REPLICA;
                     vLightBulbInit();

                     int i=0;
                     for(i=0;i<NO_REPLICA;i++)
                     {
                        if(Array_HashX[i]==Routing_Header.Dst_Info.DstX&&Array_HashY[i]==Routing_Header.Dst_Info.DstY)
                         {
                             Replica_Num=i;
                             vPrintString("\tReplica_Num= ");
                             vDisplayHex16(i);

                             break;
                         }
                     }

                }


                 if(TEMP_Counter==MAX_STORAGE)
                    {
                        vPrintString("\tReached Storage Limit Freeing Space  \r\n");
                    Free_Space();
                    }
             //else
             {

                 int i=0;
                 for(i=0;i<DCS_Header.Len;i++)

                 {

                 TEMP_Storage[TEMP_Counter].CoordX=Routing_Header.Src_Info.SrcX;
                 TEMP_Storage[TEMP_Counter].CoordY=Routing_Header.Src_Info.SrcY;

                TEMP_Storage[TEMP_Counter].Value=PUTData[i].Value;
                TEMP_Storage[TEMP_Counter].Time_Stamp=PUTData[i].Time_Stamp;

                 vPrintString("\tTemp Value ");
                 vDisplayHex16(PUTData[i].Value);
                 vPrintString("\tTemp TimeStamp ");
                 vDisplayHex16(PUTData[i].Time_Stamp);
                Rep_Data[i]=TEMP_Storage[TEMP_Counter];
                    TEMP_Counter++;
                    }
             }

    DCS_Header.Rep_Num=Replica_Num;
    Perform_Replication(DCS_Header.Len,DCS_Header.Rep_Num);


        }else{
            Display();
            vPrintString("\tNext_Hop Address ");
            vDisplayHex16(NextHop_Addr);
            vTransmitDataPacket(PayLoad,sizeof(Headers)+DCS_Header.Len*sizeof(tsPUTData), NextHop_Addr);
            }
   }
   /*  if(bToggle)
        {
            // drive DIO14 LOW
            vAHI_DioSetOutput(0, E_AHI_DIO14_INT);
        }
        else
        {
            // drive DIO14 HIGH
            vAHI_DioSetOutput(E_AHI_DIO14_INT, 0);
        }

        bToggle = !bToggle;*/

}



/***********************************************
NAME: Closest_Replica
This Function finds out which is the closest Replica(Hash Coordinates) to the given Source and return its Coordinates(Hash Coordinates)
************************************************/
void Closest_Replica(uint16 SrcX,uint16 SrcY)
{
    int i=0;
    int dist=2*(DIMX*DIMX+DIMY*DIMY);
    int tempdist;




   for( i=0;i<NO_REPLICA;i++)
   {
       tempdist=(Array_HashX[i]-SrcX)*(Array_HashX[i]-SrcX)+(Array_HashY[i]-SrcY)*(Array_HashY[i]-SrcY);

        if(tempdist<dist)
        {
        dist=tempdist;
        HashX=Array_HashX[i];
        HashY=Array_HashY[i];
        vPrintString("Hash X");
        vDisplayHex16(HashX);
        vPrintString("Hash Y");
        vDisplayHex16(HashY);

        }
        }
}



/****************************************************************************
 *
 * NAME: vHandleNodeAssociation
 *
 * DESCRIPTION:
 * Handle request by node to join the network. If the nodes address matches
 * the address of a light switch then it is assumed to be a light switch and
 * is allowed to join the network.
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
PRIVATE void vHandleNodeAssociation(MAC_MlmeDcfmInd_s *psMlmeInd)
{
    MAC_MlmeReqRsp_s   sMlmeReqRsp;
    MAC_MlmeSyncCfm_s  sMlmeSyncCfm;

    uint16 u16ShortAddress;
    uint32 u32AddrLo;
    uint32 u32AddrHi;

    /* Default to PAN access denied */
    uint8 u8AssocStatus = 2;

    /* Default short address */
    u16ShortAddress = 0xffff;

    /* Check that the node wanting to associate is one of the light switch
       devices by checking its extended address */
    u32AddrLo = psMlmeInd->uParam.sIndAssociate.sDeviceAddr.u32L;
    u32AddrHi = psMlmeInd->uParam.sIndAssociate.sDeviceAddr.u32H;

    /* Check that the device only wants to use a short address */
    if (psMlmeInd->uParam.sIndAssociate.u8Capability & 0x80)
    {
        if (sCoordData.sNode.u8AssociatedNodes < MAX_LIGHT_SWITCHES)
        {
            /* Allocate short address as next in list */
            u16ShortAddress = LIGHT_SWITCH_NODE_ADDR_BASE + sCoordData.sNode.u8AssociatedNodes;
            /* Store details for future use */
            sCoordData.sNode.asAssocNodes[sCoordData.sNode.u8AssociatedNodes].u32ExtAddrHi = u32AddrHi;
            sCoordData.sNode.asAssocNodes[sCoordData.sNode.u8AssociatedNodes].u32ExtAddrLo = u32AddrLo;
            sCoordData.sNode.asAssocNodes[sCoordData.sNode.u8AssociatedNodes].u16ShortAddr = u16ShortAddress;
            sCoordData.sNode.u8AssociatedNodes++;

            /* Assume association succeeded */
            u8AssocStatus = 0;

            if (sCoordData.sNode.u8AssociatedNodes == 1)
            {
                vLedControl(LED3, TRUE);
                vLedControl(LED4, TRUE);
            }
            else if (sCoordData.sNode.u8AssociatedNodes == 2)
            {
                vLedControl(LED3, TRUE);
                vLedControl(LED4, TRUE);
                vLedControl(LED4, TRUE);
            }
        }
    }

    /* Create association response */
    sMlmeReqRsp.u8Type = MAC_MLME_RSP_ASSOCIATE;
    sMlmeReqRsp.u8ParamLength = sizeof(MAC_MlmeRspAssociate_s);
    memcpy(&sMlmeReqRsp.uParam.sRspAssociate.sDeviceAddr,
           &psMlmeInd->uParam.sIndAssociate.sDeviceAddr,
           MAC_EXT_ADDR_LEN);
    sMlmeReqRsp.uParam.sRspAssociate.u16AssocShortAddr = u16ShortAddress;
    sMlmeReqRsp.uParam.sRspAssociate.u8Status = u8AssocStatus;
    sMlmeReqRsp.uParam.sRspAssociate.u8SecurityEnable = FALSE;

    /* Send association response. There is no confirmation for an association
       response, hence no need to check */
    vAppApiMlmeRequest(&sMlmeReqRsp, &sMlmeSyncCfm);

}

/****************************************************************************
 *
 * NAME: bStartCoordinator
 *
 * DESCRIPTION:
 * Starts the network by configuring the controller board to act as the PAN
 * coordinator.
 *
 * PARAMETERS:      Name            RW  Usage
 * None.
 *
 * RETURNS:
 * TRUE if network was started successfully otherwise FALSE
 *
 * NOTES:
 * None.
 ****************************************************************************/
PRIVATE bool_t bStartCoordinator(void)
{
    /* Structures used to hold data for MLME request and response */
    MAC_MlmeReqRsp_s   sMlmeReqRsp;
    MAC_MlmeSyncCfm_s  sMlmeSyncCfm;

    /* Start Pan */
    sMlmeReqRsp.u8Type = MAC_MLME_REQ_START;
    sMlmeReqRsp.u8ParamLength = sizeof(MAC_MlmeReqStart_s);
    sMlmeReqRsp.uParam.sReqStart.u16PanId = PAN_ID;
    sMlmeReqRsp.uParam.sReqStart.u8Channel = sCoordData.sSystem.u8Channel;
    sMlmeReqRsp.uParam.sReqStart.u8BeaconOrder = 0x0f; /* No beacons */
    sMlmeReqRsp.uParam.sReqStart.u8SuperframeOrder = 0x0f;
    sMlmeReqRsp.uParam.sReqStart.u8PanCoordinator = TRUE;
    sMlmeReqRsp.uParam.sReqStart.u8BatteryLifeExt = FALSE;
    sMlmeReqRsp.uParam.sReqStart.u8Realignment = FALSE;
    sMlmeReqRsp.uParam.sReqStart.u8SecurityEnable = FALSE;
    vAppApiMlmeRequest(&sMlmeReqRsp, &sMlmeSyncCfm);

    if (sMlmeSyncCfm.u8Status != MAC_MLME_CFM_OK)
    {
        /* Error during MLME-Start */
        return(FALSE);
    }

    return(TRUE);
}

/****************************************************************************
 *
 * NAME: vLightBulb
 *
 * DESCRIPTION:
 * Set the state of the light bulb to ON or OFF.
 *
 * PARAMETERS:      Name            RW  Usage
 *                  bState
 *
 * RETURNS:
 * None.
 *
 * NOTES:
 * None.
 ****************************************************************************/
PRIVATE void vLightBulb(bool_t bState)
{
    if (bState)
    {
        vLedControl(LED1, TRUE);
        vLedControl(LED2, TRUE);
        vLedControl(LED3, TRUE);
        vLedControl(LED4, TRUE);
        bLightBulbState = ON;
    }
    else
    {
        vLedControl(LED1, FALSE);
        vLedControl(LED2, FALSE);
        vLedControl(LED3, FALSE);
        vLedControl(LED4, FALSE);
        bLightBulbState = OFF;
    }
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
 PRIVATE void vPrintString(unsigned char *pcMessage)
 {
     while (*pcMessage)
     {
         while ((u8AHI_UartReadLineStatus(0) & 0x20) == 0);
         vAHI_UartWriteData(0, *pcMessage);
         pcMessage++;
     }
 }

PRIVATE void vTransmitDataPacket(uint8 pu8Data[], uint8 u8Len, uint16 u16DestAdr)
//PRIVATE void vTransmitDataPacket(uint8 *pu8Data, uint8 u8Len, uint16 u16DestAdr)
{
    Msg_Sent++;
    Msg_Gen++;
    Bytes_Sent=Bytes_Sent+u8Len;
    Seq_Num++;
    vPrintString("\tTRANSMIT ");
    MAC_McpsReqRsp_s  sMcpsReqRsp;
    MAC_McpsSyncCfm_s sMcpsSyncCfm;
    uint8 *pu8Payload;
    uint8 i = 0;


    /* Create frame transmission request */
    sMcpsReqRsp.u8Type = MAC_MCPS_REQ_DATA;
    sMcpsReqRsp.u8ParamLength = sizeof(MAC_McpsReqData_s);
    /* Set handle so we can match confirmation to request */
    sMcpsReqRsp.uParam.sReqData.u8Handle = 1;
    /* Use short address for source */
    //vPrintString("data ");
    //vDisplayHex(pu8Payload[3]);
    sMcpsReqRsp.uParam.sReqData.sFrame.sSrcAddr.u8AddrMode = 2;
    sMcpsReqRsp.uParam.sReqData.sFrame.sSrcAddr.u16PanId = PAN_ID;
    sMcpsReqRsp.uParam.sReqData.sFrame.sSrcAddr.uAddr.u16Short = COORD_ADDR;
    /* Use short address for destination */
    sMcpsReqRsp.uParam.sReqData.sFrame.sDstAddr.u8AddrMode = 2;
    sMcpsReqRsp.uParam.sReqData.sFrame.sDstAddr.u16PanId = PAN_ID;
    sMcpsReqRsp.uParam.sReqData.sFrame.sDstAddr.uAddr.u16Short = u16DestAdr;
    /* Frame requires ack but not security, indirect transmit or GTS */
    sMcpsReqRsp.uParam.sReqData.sFrame.u8TxOptions = MAC_TX_OPTION_ACK;
 /* Set frame length */
    sMcpsReqRsp.uParam.sReqData.sFrame.u8SduLength = u8Len;
    pu8Payload = sMcpsReqRsp.uParam.sReqData.sFrame.au8Sdu;
    //pu8Payload[0] = sCoordinatorData.u8TxPacketSeqNb++;

    /*pu8Payload[0] = 0;
    pu8Payload[1] = 1;
    pu8Payload[2] = 2;
    pu8Payload[3] = 3;
    pu8Payload[4] = 4;
    pu8Payload[5] = 5;*/
    vPrintString("\tdata Length");
    vDisplayHex16(u8Len);

   // vPrintString("\tdata Values");

    for (i = 0; i < u8Len ; i++)
    {
       // pu8Payload[i] = *pu8Data++;
       pu8Payload[i] = pu8Data[i];
      // vDisplayHex8(pu8Data[i]);
    }
/* Request transmit */
    vAppApiMcpsRequest(&sMcpsReqRsp, &sMcpsSyncCfm);
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
