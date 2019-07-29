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


#define Run_Time                       180    //mins


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




#define MAX_REPLICA                     10
#define No_App                          2
#define TEM                             0
#define HUM                             1


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
/*typedef enum
{
    TEM,
    HUM,
    LIG
} uint8;*/

typedef enum
{
    PUT,
    GET,
    GET_RESP,
    REPLICATION,
    REPLICA_STATISTICS,
    REPLICA_UPDATE,
    ACK
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
    uint8 AppID;
    uint8 Data_Type;
    uint8 Len;
    uint8 Rep_Num;
    uint16 Epoch_No;
    }tsDCS_Header;



/**************************
Payload = Routing + DCS
***************************/
typedef struct{
    tsRouting_Header Routing_Header;
    tsDCS_Header DCS_Header;
}tsHeaders;


/*********************************
Structure used to send production and consumption messages from one replica to another
*********************************/
typedef struct{
    uint16 Tr_Mesg_Cons;
    uint16 Tr_Mesg_Prod;
}tsReplication_Stat;

/*************************************************
Structure used to send next epoch information to consumers and producers(nodes except replica)
**************************************************/
typedef struct{
    int EpochExpiryTime;
    int NextNoReplica;
    uint8 Num_NextEpoch;
}tsNextEpochInfo;




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
PRIVATE tsData_Storage TEMP_Storage[No_App][MAX_STORAGE];

PRIVATE uint8 PayLoad[MAX_PAYLOAD];
PRIVATE tsData_Storage Rep_Data[10];

PRIVATE int TEMP_Counter[No_App];

PRIVATE bool Temp_Replica[No_App];
PRIVATE NODE_TYPE Node_Type[No_App];
PRIVATE int No_Of_GET=1;
PRIVATE int No_Of_PUT=1;
PRIVATE uint16 Current_Time=0;
PRIVATE char *Temp[10]={"TEM00","TEM10","TEM20","TEM30","TEM40","TEM50","TEM60","TEM70","TEM80","TEM90"};
PRIVATE char *App_Name[No_App]={"TEM","HUM","LIG"};
PRIVATE uint16 HashX[No_App],HashY[No_App];
PRIVATE uint16 Array_HashX[No_App][MAX_REPLICA];
PRIVATE uint16 Array_HashY[No_App][MAX_REPLICA];
PRIVATE uint16 Replica_Num[No_App];
PRIVATE uint16 Seq_Num=0;
PRIVATE uint16 Replication_Forwarding_Sets[MAX_REPLICA][MAX_REPLICA];    //  -1 means the end of the list
PRIVATE uint16 Count_Forwarding_Sets[MAX_REPLICA];
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



#define Traffic_Measurement_Time       1    //mins
#define Max_Packets                    240            //Earlier 120

#define ACK_Timeout                    5   //in seconds
#define MAX_ACK_RETRY                  30

PRIVATE char Str1[50];
PRIVATE char Str2[50];
PRIVATE char Str[50];
PRIVATE int EPOCH_No[No_App];
PRIVATE int Traffic_Measure[No_App][MAX_REPLICA][2];
PRIVATE tsReplication_Stat Replication_Stat[No_App];
PRIVATE tsReplication_Stat SendingReplication_Stat[No_App];
PRIVATE tsNextEpochInfo NextEpochInfo;
PRIVATE int Traffic_Counter[No_App];

PRIVATE int No_Stat_Recv[No_App];
PRIVATE int Curr_Replicas[No_App];
PRIVATE int Epoch_Expiry_Time[No_App];
PRIVATE int No_Replica_NextEpoch[No_App];

PRIVATE int Wakeup_Curr_Time=0;
PRIVATE bool NextEpoch_Info[No_App];
PRIVATE int PreCurr_Replicas[No_App];
PRIVATE uint16 PreReplica_Num[No_App];
PRIVATE uint16 PreArray_HashX[No_App][MAX_REPLICA];
PRIVATE uint16 PreArray_HashY[No_App][MAX_REPLICA];
PRIVATE uint8 Num_Replicas_Node[No_App];
PRIVATE uint8 Replica_Num_Array[No_App][MAX_REPLICA];
PRIVATE int ACK_Timer[No_App];
PRIVATE bool ACK_Wait[No_App];
PRIVATE int No_ACK_Recv[No_App];
PRIVATE int ACK_Recv[No_App][MAX_REPLICA];
PRIVATE int ACK_Retry[No_App];
PRIVATE int NextEpochNo[No_App];
PRIVATE bool Measure_Flag[No_App];

PRIVATE int OverHeadSent=0;
PRIVATE int OverHeadRecv=0;

PRIVATE int Time_Per_Thousand_Mesg[11];






















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
PRIVATE void vLightBulbInit();
PRIVATE void vTransmitDataPacket(uint8 pu8Data[], uint8 u8Len, uint16 u16DestAdr);

PRIVATE void vPrintString(unsigned char *pcMessage);
PRIVATE void vInitUart(void);
PRIVATE void vTimerConfig(void);
PRIVATE void vTimer0ISR(uint32 u32DeviceId, uint32 u32ItemBitmap);
PRIVATE void vDebug(char *pcMessage);

PRIVATE void vDisplayHex16(uint16 u32Data);
PRIVATE void Topology(uint16 ShorAddress);
PRIVATE void Read_Sensors(void);
PRIVATE void Display(void);
PRIVATE void Closest_Replica(uint16 SrcX,uint16 SrcY,uint8 App_ID);
PRIVATE void Send_Replication_Message();
PRIVATE void Calculate_Hashes(uint16 No_Replicas,uint8 App_ID);
PRIVATE uint16 Calculate_NextHopAddress(uint16 SrcX,uint16 SrcY,uint16 DstX,uint16 DstY);
PRIVATE void Replication_Algo(uint8 Rep_Num,uint8 Replica_Num_Used,uint8 App_ID);
PRIVATE void Perform_Replication(uint8 Len,uint8 Get_Root_Node,uint8 App_ID);
PRIVATE void Send_Msg(uint8 App_ID,NODE_TYPE Node_Type1);
PRIVATE void Free_Space(uint8 App_ID);






PRIVATE void vTimer1ISR(uint32 u32DeviceId, uint32 u32ItemBitmap);
PRIVATE void EpochExpiry(uint8 App_ID);
PRIVATE void Epoch_Initialization(uint8 App_ID);
PRIVATE void Send_Rep_Stat(uint8 App_ID);
PRIVATE void Replication_Tree_RepStat(uint8 Get_Root_Node,uint8 No_Of_Replica,uint8 App_ID);
PRIVATE int StringLength(char *str1);
PRIVATE void StringConcat(char *str1,char *str2);
PRIVATE void vNum2String(uint32 u32Number, uint8 u8Base);
PRIVATE void StringCopy(char *str1,char *str2);
PRIVATE bool Replica_Num_Check(uint16 DstX,uint16 DstY,uint8 App_ID);

PRIVATE void vNum2StringPrint(uint32 u32Number);

PRIVATE bool Replica_Num_Check(uint16 DstX,uint16 DstY,uint8 App_ID)
{
    int i=0,j=0;
    bool Flag=FALSE;
    bool Me_Flag=FALSE;
                     for(i=0;i<Curr_Replicas[App_ID];i++)
                     {
                        Flag=FALSE;
                        if(Array_HashX[App_ID][i]==DstX&&Array_HashY[App_ID][i]==DstY)
                         {
                             Me_Flag=TRUE;

                             for(j=0;j<Num_Replicas_Node[App_ID];j++)
                             {
                                 if(Replica_Num_Array[App_ID][j]==i)
                                 {
                                     Flag=TRUE;
                                     break;

                                 }
                            }
                            if(Flag==FALSE)
                            {
                                vPrintString("\n\n\n New Replica Found  \n\n\n");

                                Replica_Num_Array[App_ID][Num_Replicas_Node[App_ID]]=i;
                                Num_Replicas_Node[App_ID]++;
                                break;

                            }
                         }
                     }
                     return Me_Flag;
}

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
   /* vPrintString("\tRouting Header \r\n");

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
    vDisplayHex16(DCS_Header.Len);*/
}


PRIVATE void vNum2String(uint32 u32Number, uint8 u8Base)
{
    char buf[33];
    char *p = buf + 33;
    uint32 c, n;

    *--p = '\0';
    do {
        n = u32Number / u8Base;
        c = u32Number - (n * u8Base);
        if (c < 10) {
            *--p = '0' + c;
        } else {
            *--p = 'a' + (c - 10);
        }
        u32Number /= u8Base;
    } while (u32Number != 0);

  /*  while (*p){
        vPutChar(*p);
        p++;
    }*/
    StringCopy(&Str[0],p);
    return;
}

PRIVATE void vNum2StringPrint(uint32 u32Number)
{
    uint8 u8Base=10;
    char buf[33];
    char *p = buf + 33;
    uint32 c, n;

    *--p = '\0';
    do {
        n = u32Number / u8Base;
        c = u32Number - (n * u8Base);
        if (c < 10) {
            *--p = '0' + c;
        } else {
            *--p = 'a' + (c - 10);
        }
        u32Number /= u8Base;
    } while (u32Number != 0);

  /*  while (*p){
        vPutChar(*p);
        p++;
    }*/
    vPrintString(p);
    vPrintString("\r\n");
    //StringCopy(&Str[0],p);
    return;
}

PRIVATE int StringLength(char *str1)
{
    int i=0;
    while(str1[i]!='\0')
    {
        i++;
    }
    return i;

}
PRIVATE void StringConcat(char *str1,char *str2)
{


   int i=0;
   int len=StringLength(str1);
   for(i=0;i<StringLength(str2);i++)
   {
       str1[len+i]=str2[i];
   }
   str1[len+i]='\0';

}

PRIVATE void StringCopy(char *str1,char *str2)
{
   int i=0;
   int len=StringLength(str2);
   for(i=0;i<StringLength(str2);i++)
   {
       str1[i]=str2[i];
   }
   str1[i]='\0';

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
    Node_Info[2].G_Loc.X=101;
    Node_Info[2].G_Loc.Y=1;

    Node_Info[3].Address.Node_Shrt_Addr=0x0505U;
    Node_Info[3].G_Loc.X=150;
    Node_Info[3].G_Loc.Y=1;

    Node_Info[4].Address.Node_Shrt_Addr=0x0506U;
    Node_Info[4].G_Loc.X=191;
    Node_Info[4].G_Loc.Y=1;

    Node_Info[5].Address.Node_Shrt_Addr=0x0507U;
    Node_Info[5].G_Loc.X=1;
    Node_Info[5].G_Loc.Y=60;

    Node_Info[6].Address.Node_Shrt_Addr=0x0508U;
    Node_Info[6].G_Loc.X=50;
    Node_Info[6].G_Loc.Y=60;

    Node_Info[7].Address.Node_Shrt_Addr=0x0509U;
    Node_Info[7].G_Loc.X=101;
    Node_Info[7].G_Loc.Y=60;

    Node_Info[8].Address.Node_Shrt_Addr=0x0510U;
    Node_Info[8].G_Loc.X=150;
    Node_Info[8].G_Loc.Y=60;

    Node_Info[9].Address.Node_Shrt_Addr=0x0511U;
    Node_Info[9].G_Loc.X=191;
    Node_Info[9].G_Loc.Y=60;

    Node_Info[10].Address.Node_Shrt_Addr=0x0512U;
    Node_Info[10].G_Loc.X=1;
    Node_Info[10].G_Loc.Y=121;

    Node_Info[11].Address.Node_Shrt_Addr=0x0513U;
    Node_Info[11].G_Loc.X=50;
    Node_Info[11].G_Loc.Y=121;

    Node_Info[12].Address.Node_Shrt_Addr=0x0514U;
    Node_Info[12].G_Loc.X=101;
    Node_Info[12].G_Loc.Y=121;

    Node_Info[13].Address.Node_Shrt_Addr=0x0515U;
    Node_Info[13].G_Loc.X=150;
    Node_Info[13].G_Loc.Y=121;

    Node_Info[14].Address.Node_Shrt_Addr=0x0516U;
    Node_Info[14].G_Loc.X=191;
    Node_Info[14].G_Loc.Y=121;

    Node_Info[15].Address.Node_Shrt_Addr=0x0517U;
    Node_Info[15].G_Loc.X=1;
    Node_Info[15].G_Loc.Y=190;

    Node_Info[16].Address.Node_Shrt_Addr=0x0518U;
    Node_Info[16].G_Loc.X=50;
    Node_Info[16].G_Loc.Y=190;

    Node_Info[17].Address.Node_Shrt_Addr=0x0519U;
    Node_Info[17].G_Loc.X=101;
    Node_Info[17].G_Loc.Y=190;

    Node_Info[18].Address.Node_Shrt_Addr=0x0520U;
    Node_Info[18].G_Loc.X=150;
    Node_Info[18].G_Loc.Y=190;

    Node_Info[19].Address.Node_Shrt_Addr=0x0521U;
    Node_Info[19].G_Loc.X=191;
    Node_Info[19].G_Loc.Y=190;

    //Node_Type=NOTHING;



    if(ShorAddress==0x0502U)
     {
        Next_Hop_Head[0].Neighbour_ID=5;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=1;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=6;
         Next_Hop_Head[2].END=TRUE;

         Own_ID=0;

       //  Node_Type=CONSUMER;



     }else if(ShorAddress==0x0503U)
     {
         Next_Hop_Head[0].Neighbour_ID=0;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=2;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=6;
         Next_Hop_Head[2].END=TRUE;

         Own_ID=1;

        // Node_Type=CONSUMER;




     }else if(ShorAddress==0x0504U)
     {
         Next_Hop_Head[0].Neighbour_ID=1;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=3;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=7;
         Next_Hop_Head[2].END=TRUE;

         Own_ID=2;

       //  Node_Type=CONSUMER;



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
         // Node_Type=CONSUMER;


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

         // Node_Type=CONSUMER;


     }else if(ShorAddress==0x0516U)
     {
         Next_Hop_Head[0].Neighbour_ID=13;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=9;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=19;
         Next_Hop_Head[2].END=TRUE;

         Own_ID=14;

       //  Node_Type=CONSUMER;


     }else if(ShorAddress==0x0517U)
     {


         Next_Hop_Head[0].Neighbour_ID=10;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=16;
         Next_Hop_Head[1].END=TRUE;

         Own_ID=15;

        //  Node_Type=NOTHING;


     }else if(ShorAddress==0x0518U)
     {
         Next_Hop_Head[0].Neighbour_ID=15;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=11;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=17;
         Next_Hop_Head[2].END=TRUE;

         Own_ID=16;

        // Node_Type=CONSUMER;


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
       //  Node_Type=CONSUMER;




     }else if(ShorAddress==0x0521U)
     {
         Next_Hop_Head[0].Neighbour_ID=14;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=18;
         Next_Hop_Head[1].END=TRUE;

         Own_ID=19;

      //   Node_Type=NOTHING;
    }

    Source_X=Node_Info[Own_ID].G_Loc.X;
    Source_Y=Node_Info[Own_ID].G_Loc.Y;


      int i=0;
      for (i=0;i<MAX_REPLICA;i++)
      {
          Replication_Forwarding_Sets[i][0]=-1;
          Count_Forwarding_Sets[i]=0;
      }


      for(i=0;i<11;i++)
      {
          Time_Per_Thousand_Mesg[i]=1;
      }
       vPrintString("Topology End \r\n ");

       for(i=0;i<No_App;i++)
       {
           TEMP_Counter[i]=0;
           Temp_Replica[i]=FALSE;
           EPOCH_No[i]=-1;
           Traffic_Counter[i]=0;
           No_Stat_Recv[i]=0;
           Curr_Replicas[i]=NO_REPLICA;
           Epoch_Expiry_Time[i]=Run_Time*60;
           No_Replica_NextEpoch[i]=NO_REPLICA;
           NextEpoch_Info[i]=FALSE;
           Num_Replicas_Node[i]=0;
           ACK_Timer[i]=0;
           ACK_Wait[i]=FALSE;
           No_ACK_Recv[i]=0;
           NextEpochNo[i]=0;
           Measure_Flag[i]=TRUE;
           ACK_Retry[i]=0;
           Node_Type[i]=NOTHING;
    }
}


/*PRIVATE void Topology(uint16 ShorAddress)
{
    vPrintString("ShortAddress ");
    vDisplayHex16( ShorAddress);

    Node_Info[0].Address.Node_Shrt_Addr=0x0502U;
    Node_Info[0].G_Loc.X=1;
    Node_Info[0].G_Loc.Y=1;

    Node_Info[1].Address.Node_Shrt_Addr=0x0503U;
    Node_Info[1].G_Loc.X=94;
    Node_Info[1].G_Loc.Y=1;

    Node_Info[2].Address.Node_Shrt_Addr=0x0504U;
    Node_Info[2].G_Loc.X=191;
    Node_Info[2].G_Loc.Y=1;

    Node_Info[3].Address.Node_Shrt_Addr=0x0505U;
    Node_Info[3].G_Loc.X=1;
    Node_Info[3].G_Loc.Y=94;

    Node_Info[4].Address.Node_Shrt_Addr=0x0506U;
    Node_Info[4].G_Loc.X=94;
    Node_Info[4].G_Loc.Y=94;

    Node_Info[5].Address.Node_Shrt_Addr=0x0507U;
    Node_Info[5].G_Loc.X=191;
    Node_Info[5].G_Loc.Y=94;

    Node_Info[6].Address.Node_Shrt_Addr=0x0508U;
    Node_Info[6].G_Loc.X=1;
    Node_Info[6].G_Loc.Y=191;

    Node_Info[7].Address.Node_Shrt_Addr=0x0509U;
    Node_Info[7].G_Loc.X=94;
    Node_Info[7].G_Loc.Y=191;

    Node_Info[8].Address.Node_Shrt_Addr=0x0510U;
    Node_Info[8].G_Loc.X=191;
    Node_Info[8].G_Loc.Y=191;

   // Node_Type=NOTHING;



    if(ShorAddress==0x0502U)
     {
        Next_Hop_Head[0].Neighbour_ID=1;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=4;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=3;
         Next_Hop_Head[2].END=TRUE;

         Own_ID=0;

        // Node_Type=CONSUMER;



     }else if(ShorAddress==0x0503U)
     {
         Next_Hop_Head[0].Neighbour_ID=0;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=2;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=4;
         Next_Hop_Head[2].END=TRUE;

         Own_ID=1;

        // Node_Type=CONSUMER;




     }else if(ShorAddress==0x0504U)
     {
         Next_Hop_Head[0].Neighbour_ID=1;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=4;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=5;
         Next_Hop_Head[2].END=TRUE;

         Own_ID=2;

        // Node_Type=CONSUMER;



     }else if(ShorAddress==0x0505U)
     {
         Next_Hop_Head[0].Neighbour_ID=0;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=4;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=7;
         Next_Hop_Head[2].END=FALSE;

         Next_Hop_Head[3].Neighbour_ID=6;
         Next_Hop_Head[3].END=TRUE;

         Own_ID=3;


     }else if(ShorAddress==0x0506U)
     {
         Next_Hop_Head[0].Neighbour_ID=3;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=0;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=1;
         Next_Hop_Head[2].END=FALSE;

         Next_Hop_Head[3].Neighbour_ID=2;
         Next_Hop_Head[3].END=FALSE;

         Next_Hop_Head[4].Neighbour_ID=5;
         Next_Hop_Head[4].END=FALSE;

         Next_Hop_Head[5].Neighbour_ID=7;
         Next_Hop_Head[5].END=TRUE;

         Own_ID=4;
        //  Node_Type=CONSUMER;


     }else if(ShorAddress==0x0507U)
     {
        Next_Hop_Head[0].Neighbour_ID=2;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=8;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=7;
         Next_Hop_Head[2].END=FALSE;

         Next_Hop_Head[3].Neighbour_ID=4;
         Next_Hop_Head[3].END=TRUE;

         Own_ID=5;


     }else if(ShorAddress==0x0508U)
     {
         Next_Hop_Head[0].Neighbour_ID=3;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=7;
         Next_Hop_Head[1].END=TRUE;

         Own_ID=6;


     }else if(ShorAddress==0x0509U)
     {
        Next_Hop_Head[0].Neighbour_ID=6;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=3;
         Next_Hop_Head[1].END=FALSE;

         Next_Hop_Head[2].Neighbour_ID=4;
         Next_Hop_Head[2].END=FALSE;

         Next_Hop_Head[3].Neighbour_ID=5;
         Next_Hop_Head[3].END=FALSE;

         Next_Hop_Head[4].Neighbour_ID=8;
         Next_Hop_Head[4].END=TRUE;

         Own_ID=7;



     }else if(ShorAddress==0x0510U)
     {
         Next_Hop_Head[0].Neighbour_ID=7;
         Next_Hop_Head[0].END=FALSE;

         Next_Hop_Head[1].Neighbour_ID=5;
         Next_Hop_Head[1].END=TRUE;

         Own_ID=8;
     }

      Source_X=Node_Info[Own_ID].G_Loc.X;
    Source_Y=Node_Info[Own_ID].G_Loc.Y;


      int i=0;
      for (i=0;i<MAX_REPLICA;i++)
      {
          Replication_Forwarding_Sets[i][0]=-1;
          Count_Forwarding_Sets[i]=0;
      }
       vPrintString("Topology End \r\n ");

       for(i=0;i<11;i++)
      {
          Time_Per_Thousand_Mesg[i]=1;
      }

       for(i=0;i<No_App;i++)
       {
           TEMP_Counter[i]=0;
           Temp_Replica[i]=FALSE;
           EPOCH_No[i]=-1;
           Traffic_Counter[i]=0;
           No_Stat_Recv[i]=0;
           Curr_Replicas[i]=NO_REPLICA;
           Epoch_Expiry_Time[i]=Run_Time*60;
           No_Replica_NextEpoch[i]=NO_REPLICA;
           NextEpoch_Info[i]=FALSE;
           Num_Replicas_Node[i]=0;
           ACK_Timer[i]=0;
           ACK_Wait[i]=FALSE;
           No_ACK_Recv[i]=0;
           NextEpochNo[i]=0;
           Measure_Flag[i]=TRUE;
           ACK_Retry[i]=0;
           Node_Type[i]=NOTHING;
    }

}*/






/***************************************
Calculate All The Hashes X And Y Takes paramenter as No of Replica for Hashes are to be Generated
****************************************/
PRIVATE void Calculate_Hashes(uint16 No_Replicas,uint8 App_ID)
{
    int i=0;
    uint16 *p;


{
   for( i=0;i<No_Replicas;i++)
   {
       a:

 /* p=(uint16 *)MDString(Temp[i],5);

 // uint16 *p=(uint16 *)MDString("TEMP0");
  vPrintString(Temp[i]);
  vPrintString("\r\n");*/

  vNum2String(i,10);
       StringCopy(&Str1[0],&Str[0]);
       vNum2String(EPOCH_No[App_ID],10);
       StringCopy(&Str2[0],&Str[0]);
       StringConcat(&Str1[0],&Str2[0]);
      // StringCopy(&Str[0],"TEM");
      StringCopy(&Str[0],App_Name[App_ID]);

       StringConcat(&Str[0],&Str1[0]);



       vPrintString("DIGEST STRING  ");
       vPrintString(Str);
       vPrintString("\r\n");

       p=(uint16 *)MDString(Str,StringLength(Str));
       //p=(uint16 *)MDString(Str,6);



    Array_HashX[App_ID][i]=p[0];
    Array_HashY[App_ID][i]=p[1];
    Array_HashX[App_ID][i]=Array_HashX[App_ID][i]%DIMX;
    Array_HashY[App_ID][i]=Array_HashY[App_ID][i]%DIMY;
    if(Array_HashX[App_ID][i]==0&&Array_HashY[App_ID][i]==0)
    goto a;

    vPrintString("Array_Hash X ");
    vDisplayHex16(Array_HashX[App_ID][i]);
    vPrintString("Array_Hash Y ");
    vDisplayHex16(Array_HashY[App_ID][i]);
    }
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




                        vAHI_TimerEnable(E_AHI_TIMER_1,
                     16,      // prescalar value 2^16
                     FALSE,
                     TRUE,
                     FALSE);
    vAHI_TimerClockSelect(E_AHI_TIMER_1,
                          FALSE,
                          TRUE);    // gate input pin active high
    vAHI_TimerStartRepeat(E_AHI_TIMER_1,
                          0x0000,       // null value
                        //  122);         // number of pulses to count
                        244);  // 244 is 1 sec         // number of pulses to count






    /* register Timer0 interrupt */
    vAHI_Timer0RegisterCallback(vTimer0ISR);

    vAHI_Timer1RegisterCallback(vTimer1ISR);
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

            DCS_Header.AppID=TEM;
           // if(Node_Type==CONSUMER&&CONSUMER_Counter==2)
         if(CONSUMER_Counter==15)
            {
                CONSUMER_Counter=0;
                int i=0;
                for(i=0;i<No_App;i++)
                {
                    Send_Msg(i,CONSUMER);
                }

             //   Send_Msg(LIG,CONSUMER);

            }

            if(PRODUCER_Counter==45)
            {
                PRODUCER_Counter=0;
                int i=0;
                for(i=0;i<No_App;i++)
                {
                    Send_Msg(i,PRODUCER);
                }
             //   Send_Msg(LIG,PRODUCER);
            }


            }
}



PRIVATE void vTimer1ISR(uint32 u32DeviceId, uint32 u32ItemBitmap)
{
    if(OPR_MODE==0&&START)
            {



                Wakeup_Curr_Time=Wakeup_Curr_Time+1;


              /*   vPrintString("Wakeup_Curr_Time ");
                 vDisplayHex16(Wakeup_Curr_Time);

                  vPrintString(" No_Stat_Recv");
                  vDisplayHex16(No_Stat_Recv);

                  vPrintString(" Epoch_Expiry_Time");
                  vDisplayHex16(Epoch_Expiry_Time);

                  vPrintString(" Curr Replica");
                  vDisplayHex16(Curr_Replicas);*/
                  int j=0;
                  for(j=0;j<No_App;j++)
                  {
                      Traffic_Counter[j]=Traffic_Counter[j]+1;





                  if(Traffic_Counter[j]==(Traffic_Measurement_Time*60))
            {
                Measure_Flag[j]=FALSE;



                if(Node_Type[j]==REPLICA)
                {
                    vPrintString("In Replica Statistics Timer \r\n\n\n\n");

                    Send_Rep_Stat(j);
                }
            }

            if((No_Stat_Recv[j]==Curr_Replicas[j])&&NextEpoch_Info[j]==FALSE&&ACK_Wait[j]==FALSE&&(No_ACK_Recv[j]==Curr_Replicas[j]-Num_Replicas_Node[j]))
                {


                    int i=0,max;
                     int var,c,p,var1,c1,num1,num;
                    for(i=0;i<Curr_Replicas[j];i++)
                    {
                        if(i==0)
                        {
                            max=Traffic_Measure[j][i][0]+Traffic_Measure[j][i][1];
                            c=Traffic_Measure[j][i][1];
                            p=Traffic_Measure[j][i][0];

                        }
                        else if(max<(Traffic_Measure[j][i][0]+Traffic_Measure[j][i][1]))
                        {
                            max=Traffic_Measure[j][i][0]+Traffic_Measure[j][i][1];
                            c=Traffic_Measure[j][i][1];
                            p=Traffic_Measure[j][i][0];
                        }
                    }
                    Epoch_Expiry_Time[j]=Wakeup_Curr_Time+((((60*Traffic_Measurement_Time)*Max_Packets)/max)-(60*Traffic_Measurement_Time));

                     c1=c*10;
                    num1=c1/p;
                    num=c/p;
                    if((num1%10)>5)
                    {
                        num++;
                    }




                    // if(p!=0)

                    var1=14*num;
                    var=var1/10;
                    if((var1%10)>5)
                    {
                        var++;
                    }


                    No_Replica_NextEpoch[j]=var;
                    NextEpoch_Info[j]=TRUE;
                    NextEpochNo[j]=EPOCH_No[j]+1;

                    vPrintString("\tEpoch Expiry Time \r\n");
                  //  vNum2StringPrint(Epoch_Expiry_Time,10);
                  vDisplayHex16(Epoch_Expiry_Time[j]);

                    vPrintString("\tNo of Replica in Next Epoch \r\n");
                   // vNum2StringPrint(var,10);
                   vDisplayHex16(var);

                   //  No_Stat_Recv=0;
                }


                if(Wakeup_Curr_Time==Epoch_Expiry_Time[j]&&ACK_Wait[j]==FALSE&&NextEpoch_Info[j]==TRUE)
                {
                    //NextEpoch_Info=FALSE;
                    vPrintString("EPOCH Expires \r\n");

                    EpochExpiry(j);

                }
                if(ACK_Wait[j]==TRUE)
                {
                    ACK_Timer[j]=ACK_Timer[j]+1;

                    if(ACK_Timer[j]==ACK_Timeout)
                    {
                        ACK_Retry[j]=ACK_Retry[j]+1;






      /* int i=0;
    for(i=0;i<MAX_REPLICA;i++)
    {

        ACK_Recv[i]=-1;
    }*/
                        ACK_Timer[j]=0;
                        ACK_Wait[j]=TRUE;
                      //  No_ACK_Recv=0;
                        Send_Rep_Stat(j);


                    }

                }
            }
        }
}



PRIVATE void EpochExpiry(uint8 App_ID)
{

    vPrintString("In Func Current Epoch Expires \r\n");
                 PreCurr_Replicas[App_ID]=Curr_Replicas[App_ID];
                 PreReplica_Num[App_ID]=Replica_Num[App_ID];
                 int i=0;
                 for(i=0;i<Curr_Replicas[App_ID];i++)
                 {
                     PreArray_HashX[App_ID][i]=Array_HashX[App_ID][i];
                     PreArray_HashY[App_ID][i]=Array_HashY[App_ID][i];
                 }
                 Epoch_Initialization(App_ID);
                 if(Node_Type[App_ID]==REPLICA)
                 {
                    Temp_Replica[App_ID]=FALSE;
                    Perform_Replica_Transtion(PreReplica_Num[App_ID],App_ID);

                    Node_Type[App_ID]=NOTHING;

                    vLightBulbInit();
                 }
}


PRIVATE void Epoch_Initialization(uint8 App_ID)
{
    ACK_Retry[App_ID]=0;
    ACK_Wait[App_ID]=FALSE;
    Num_Replicas_Node[App_ID]=0;
    No_ACK_Recv[App_ID]=0;

    ACK_Wait[App_ID]=FALSE;
    ACK_Timer[App_ID]=0;
    No_ACK_Recv[App_ID]=0;

    Measure_Flag[App_ID]=TRUE;

    Curr_Replicas[App_ID]=No_Replica_NextEpoch[App_ID];
    EPOCH_No[App_ID]=NextEpochNo[App_ID];
    vPrintString("EPoch Initialisation \r\n");
    int i=0;
    for(i=0;i<MAX_REPLICA;i++)
    {
        Traffic_Measure[App_ID][i][0]=-1;
        Traffic_Measure[App_ID][i][1]=-1;
        ACK_Recv[App_ID][i]=-1;
    }

    Replication_Stat[App_ID].Tr_Mesg_Cons=0;
    Replication_Stat[App_ID].Tr_Mesg_Prod=0;
    No_Stat_Recv[App_ID]=0;
    NextEpoch_Info[App_ID]=FALSE;
    Epoch_Expiry_Time[App_ID]=Run_Time*60;
    Traffic_Counter[App_ID]=0;

    Calculate_Hashes(Curr_Replicas[App_ID],App_ID);





      for (i=0;i<MAX_REPLICA;i++)
      {
          Replication_Forwarding_Sets[i][0]=-1;
          Count_Forwarding_Sets[i]=0;
      }
       Closest_Replica(Source_X,Source_Y,App_ID);


       for(i=0;i<Curr_Replicas[App_ID];i++)
    {
        uint16 NextHop_Address=Calculate_NextHopAddress(Source_X,Source_Y,Array_HashX[App_ID][i],Array_HashY[App_ID][i]);
        if(NextHop_Address==COORD_ADDR)
        {
            Replica_Num_Check(Array_HashX[App_ID][i],Array_HashY[App_ID][i],App_ID);
        }
    }
    vPrintString("Num_Replica_Node ");
    vDisplayHex16(Num_Replicas_Node[App_ID]);

      /*for(i=0;i<Curr_Replicas;i++)
      {
          Replication_Algo(i);
      }*/

}

PRIVATE void Send_Rep_Stat(uint8 App_ID)
{
    vPrintString("SENDING REPLICA REPLICATION STATISTICS \r\n");

                vPrintString("\tProduction Mesg");
                vDisplayHex16(Replication_Stat[App_ID].Tr_Mesg_Prod);
             //   vNum2StringPrint(Replication_Stat.Tr_Mesg_Prod,10);
                vPrintString("\tConsumption Mesg");
                vDisplayHex16(Replication_Stat[App_ID].Tr_Mesg_Cons);
               // vNum2StringPrint(Replication_Stat.Tr_Mesg_Cons,10);

          if(Traffic_Measure[App_ID][Replica_Num[App_ID]][0]==-1)
                {
                    No_Stat_Recv[App_ID]=Num_Replicas_Node[App_ID]+No_Stat_Recv[App_ID];
                    Traffic_Measure[App_ID][Replica_Num[App_ID]][0]=Replication_Stat[App_ID].Tr_Mesg_Prod;
                    Traffic_Measure[App_ID][Replica_Num[App_ID]][1]=Replication_Stat[App_ID].Tr_Mesg_Cons;
                }

                vPrintString("\tNo_Stat_Recv");
                vDisplayHex16(No_Stat_Recv[App_ID]);
              //  vNum2StringPrint(No_Stat_Recv,10);
                vPrintString("\tCurr_Replicas");
                vDisplayHex16(Curr_Replicas[App_ID]);
                //vNum2StringPrint(Curr_Replicas,10);


               /* if((No_Stat_Recv==Curr_Replicas)&&NextEpoch_Info==FALSE)
                {
                    vPrintString("\tCurr_Replicas");

                    int i=0,max=1;
                    int var=0,c=0,p=0;
                    for(i=0;i<Curr_Replicas;i++)
                    {
                        vPrintString("Loop\r\n");
                        if(i==0)
                        {
                            max=Traffic_Measure[i][0]+Traffic_Measure[i][1];
                            c=Traffic_Measure[i][1];
                            p=Traffic_Measure[i][0];

                        }
                        else if(max<(Traffic_Measure[i][0]+Traffic_Measure[i][1]))
                        {
                            max=Traffic_Measure[i][0]+Traffic_Measure[i][1];
                            c=Traffic_Measure[i][1];
                            p=Traffic_Measure[i][0];
                        }
                    }

                    Epoch_Expiry_Time=Wakeup_Curr_Time+((((60*Traffic_Measurement_Time)*Max_Packets)/max)-(60*Traffic_Measurement_Time));
                    if(p!=0)
                    var=14*(c/p);
                    var=var/10;

                    No_Replica_NextEpoch=var;
                   // No_Replica_NextEpoch=NO_REPLICA;

                  //  vNum2StringPrint(No_Replica_NextEpoch,10);
                    NextEpoch_Info=TRUE;
                    vPrintString("\tEpoch Expiry Time \r\n");
                    vDisplayHex16(Epoch_Expiry_Time);
//                    int a=Epoch_Expiry_Time-Wakeup_Curr_Time;
                   // vNum2StringPrint(Epoch_Expiry_Time,10);

                    vPrintString("\tNo of Replica in Next Epoch \r\n");
                    vDisplayHex16(No_Replica_NextEpoch);
                   // vNum2StringPrint(No_Replica_NextEpoch,10);


                  //  No_Stat_Recv=0;
                }*/
                SendingReplication_Stat[App_ID].Tr_Mesg_Prod=Replication_Stat[App_ID].Tr_Mesg_Prod;
                SendingReplication_Stat[App_ID].Tr_Mesg_Cons=Replication_Stat[App_ID].Tr_Mesg_Cons;


        Replication_Tree_RepStat(Replica_Num[App_ID],Num_Replicas_Node[App_ID],App_ID);
               /*     int i=0;
    for(i=0;i<MAX_REPLICA;i++)
    {
        ACK_Recv[i]=-1;
    }*/
    if(Num_Replicas_Node[App_ID]==Curr_Replicas[App_ID])
    {
        ACK_Wait[App_ID]=FALSE;
    }else{
        ACK_Wait[App_ID]=TRUE;
        ACK_Timer[App_ID]=0;
    }
        // No_ACK_Recv=0;
}

PRIVATE void Replication_Tree_RepStat(uint8 Get_Root_Node,uint8 No_Of_Replica,uint8 App_ID)
{
    int Count=0;
    for(Count=0;Count<Num_Replicas_Node[App_ID];Count++)
    {

                   vPrintString("\tStarting Replication \r\n");
               vPrintString("\tGet_Root_Node \r\n");
                vDisplayHex16(Get_Root_Node);
   int Num=0;


   Replication_Algo(Get_Root_Node,Replica_Num_Array[App_ID][Count],App_ID);



    for(Num=0;Num<Count_Forwarding_Sets[Get_Root_Node];Num++)
    {




        if(Replication_Forwarding_Sets[Get_Root_Node][Num]!=Replica_Num[App_ID])

        {
            uint16 DstX=Array_HashX[App_ID][Replication_Forwarding_Sets[Get_Root_Node][Num]];
            uint16 DstY=Array_HashY[App_ID][Replication_Forwarding_Sets[Get_Root_Node][Num]];
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


    DCS_Header.OpCode=REPLICA_STATISTICS;     //12
    DCS_Header.Flags=No_Of_Replica;      //13
    // Data_Transmit.Data_Type=TEMP;       //16
    DCS_Header.Data_Type=1;
    DCS_Header.Len=1;
    DCS_Header.Epoch_No=EPOCH_No[App_ID];
    DCS_Header.AppID=App_ID;
    DCS_Header.Rep_Num=Get_Root_Node;

    uint8 * PtrD=(uint8 *)&SendingReplication_Stat[App_ID];
    Headers.Routing_Header=Routing_Header;
    Headers.DCS_Header=DCS_Header;
    tsHeaders *Ptr=&Headers;
    uint8 *pu8Payload=(uint8 *)Ptr;
    int i;

    for(i=0;i<sizeof(Headers);i++)
    {
        PayLoad[i]=pu8Payload[i];
    }
    for(i=0;i<DCS_Header.Len*sizeof(tsReplication_Stat);i++)
    {
        PayLoad[sizeof(Headers)+i]=PtrD[i];
    }

     vPrintString("\tSENDING REPLICATION MESSAGE\r\n");
     vPrintString("\tNext_Hop Address ");
     vDisplayHex16(NextHop_Addr);
     Display();
    if(NextHop_Addr!=COORD_ADDR)
    vTransmitDataPacket(PayLoad,sizeof(Headers)+DCS_Header.Len*sizeof(tsReplication_Stat), NextHop_Addr);


    }
    }
    }

}


void Perform_Replica_Transtion(uint16 PreReplica_Num,uint8 App_ID)
{
    int i=0,j=0,index=-1;
    int dist,tempdist;
    dist=2*((DIMX*DIMX)+(DIMY*DIMY));
    for(i=0;i<Curr_Replicas[App_ID];i++)
    {
        dist=2*((DIMX*DIMX)+(DIMY*DIMY));
        index=-1;
        for(j=0;j<PreCurr_Replicas[App_ID];j++)
        {
            tempdist=(PreArray_HashX[App_ID][j]-Array_HashX[App_ID][i])*(PreArray_HashX[App_ID][j]-Array_HashX[App_ID][i])+(PreArray_HashY[App_ID][j]-Array_HashY[App_ID][i])*(PreArray_HashY[App_ID][j]-Array_HashY[App_ID][i]);
            if(tempdist<dist)
            {
                dist=tempdist;
                index=j;

            }


        }
        if(index==PreReplica_Num)
        {
            vPrintString("\tSending Replica Update \r\n");
            uint16 DstX=Array_HashX[App_ID][i];
            uint16 DstY=Array_HashY[App_ID][i];
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

    DCS_Header.OpCode=REPLICA_UPDATE;     //12
    DCS_Header.Flags=0;      //13
    // Data_Transmit.Data_Type=TEMP;       //16
    DCS_Header.Data_Type=1;
    DCS_Header.Len=1;
    DCS_Header.Epoch_No=EPOCH_No[App_ID];
    DCS_Header.AppID=App_ID;
    DCS_Header.Rep_Num=PreReplica_Num;
    int i;

    for(i=0;i<DCS_Header.Len;i++)
    {
        Rep_Data[i]=TEMP_Storage[App_ID][TEMP_Counter[App_ID]-i];
    }



    uint8 * PtrD=(uint8 *)&Rep_Data;
    Headers.Routing_Header=Routing_Header;
    Headers.DCS_Header=DCS_Header;
    tsHeaders *Ptr=&Headers;
    uint8 *pu8Payload=(uint8 *)Ptr;


    for(i=0;i<sizeof(Headers);i++)
    {
        PayLoad[i]=pu8Payload[i];
    }
    for(i=0;i<DCS_Header.Len*sizeof(tsData_Storage);i++)
    {
        PayLoad[sizeof(Headers)+i]=PtrD[i];
    }

     vPrintString("\tSENDING REPLICA UPDATE MESSAGE\r\n");
     vPrintString("\tNext_Hop Address ");
     vDisplayHex16(NextHop_Addr);
     Display();
     if(NextHop_Addr!=COORD_ADDR)
     vTransmitDataPacket(PayLoad,sizeof(Headers)+DCS_Header.Len*sizeof(tsData_Storage), NextHop_Addr);

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
   // Calculate_Hashes(NO_REPLICA);
   int i=0;
   for(i=0;i<No_App;i++)
   {
       Epoch_Initialization(i);
   }
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
    int i=0;
    uint8 Node_Type1=NOTHING;
    for(i=0;i<No_App;i++)
    {
        if(Node_Type[i]==REPLICA)
        {
            Node_Type1=REPLICA;
        }

    }
    if(Node_Type1==CONSUMER)
    {
        vLedControl(LED1, TRUE);
        vLedControl(LED2, FALSE);
    }
    else if(Node_Type1==PRODUCER)
    {
        vLedControl(LED1, FALSE);
        vLedControl(LED2, TRUE);

    }
    else if(Node_Type1==REPLICA)
    {
        vLedControl(LED1, TRUE);
        vLedControl(LED2, TRUE);

    }
    else if(Node_Type1==NOTHING)
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



/**************************************************************************
uint16 Calculate_NextHopAddress(uint16 SrcX,uint16 SrcY);

Calucuñates the the Next Hop from the node,which is nearest to the Destination
***************************************************************************/
uint16 Calculate_NextHopAddress(uint16 SrcX,uint16 SrcY,uint16 DstX,uint16 DstY){
    DstX=DstX;
    DstY=DstY;

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
PRIVATE void Free_Space(uint8 App_ID)
{
    //tsData_Storage tempData;
    TEMP_Counter[App_ID]=MAX_STORAGE-10;
    int i=0;
    for(i=0;i<MAX_STORAGE-10;i++)
    TEMP_Storage[App_ID][i]=TEMP_Storage[App_ID][i+10];

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

            int Total=Msg_Sent+Msg_Recv;
            if(((Total>=1000&&Total<2000)||(Total>=2000&&Total<3000)||(Total>=3000&&Total<4000)||(Total>=4000&&Total<5000)||(Total>=5000&&Total<6000)||(Total>=6000&&Total<7000)||(Total>=7000&&Total<8000)||(Total>=8000&&Total<9000)||(Total>=9000&&Total<10000)||(Total>=10000&&Total<11000))
                &&Time_Per_Thousand_Mesg[Total/1000]==1)
                {
                    Time_Per_Thousand_Mesg[Total/1000]=Time_Counter-60;;
                }

            Msg_Con++;
            Bytes_Recv=Bytes_Recv+psFrame->u8SduLength;
            /* Toggle light bulb */

            //modification
            tsHeaders *Ptr=(tsHeaders *)psFrame->au8Sdu;





    if(Ptr->DCS_Header.OpCode==REPLICA_STATISTICS||Ptr->DCS_Header.OpCode==REPLICA_UPDATE||Ptr->DCS_Header.OpCode==ACK)
    {
        OverHeadRecv++;
    }
    if(Ptr->DCS_Header.OpCode==GET_RESP&&Ptr->DCS_Header.Len==0)
    {
        OverHeadRecv++;
    }


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
    uint8 App_ID=Ptr->DCS_Header.AppID;


        uint16 NextHop_Addr=Calculate_NextHopAddress(Source_X,Source_Y,DstX,DstY);

        if(NextHop_Addr==Src_Add&&((Ptr->DCS_Header.Epoch_No==EPOCH_No[App_ID])||Ptr->DCS_Header.OpCode==REPLICA_UPDATE))
        {
            vPrintString("I am the Destination,Data won't transmit \r\n");


            uint8 *ptr8=psFrame->au8Sdu;
            vPrintString("\tRecieved ");
            vDisplayHex16(Ptr->Routing_Header.Dst_Info.DstX);
            vDisplayHex16(Ptr->Routing_Header.Dst_Info.DstY);


             if(Ptr->DCS_Header.OpCode==PUT)
             {
                bool Flag=Replica_Num_Check(Ptr->Routing_Header.Dst_Info.DstX,Ptr->Routing_Header.Dst_Info.DstY,App_ID);


                // Closest_Replica(Source_X,Source_Y);
                 if(Flag==TRUE)      //here avoiding the little synchronization problem by discarding the PUT REQ after epoch change
                 {
                     if(Measure_Flag[App_ID]==TRUE)
                     Replication_Stat[App_ID].Tr_Mesg_Prod++;

                 vPrintString("\tRECIEVED PUT REQUEST\r\n");
                 vPrintString("APP ID ");
                 vNum2StringPrint(App_ID);

                 tsPUTData *PtrD=&ptr8[sizeof(Headers)];
                 vPrintString("\tNo Of Structures In PUT");
                 vDisplayHex16(Ptr->DCS_Header.Len);

           /*  vDisplayHex16(PtrD[0].Value);
             vDisplayHex16(PtrD[0].Time_Stamp);
             vDisplayHex16(PtrD[1].Value);
             vDisplayHex16(PtrD[1].Time_Stamp);*/
              // tsData_Storage Rep_Data[Ptr->DCS_Header.Len];



             if(Temp_Replica[App_ID]==FALSE)
                {
                     Temp_Replica[App_ID]=TRUE;
                     Node_Type[App_ID]=REPLICA;
                     vLightBulbInit();

                     int i=0;
                     for(i=0;i<Curr_Replicas[App_ID];i++)
                     {
                        if(Array_HashX[App_ID][i]==Ptr->Routing_Header.Dst_Info.DstX&&Array_HashY[App_ID][i]==Ptr->Routing_Header.Dst_Info.DstY)
                         {
                             Replica_Num[App_ID]=i;
                             vPrintString("\tReplica_Num= ");
                             vDisplayHex16(i);

                             break;
                         }
                     }

                }

             if(TEMP_Counter[App_ID]==MAX_STORAGE)
                    {
                        vPrintString("\tReached Storage Limit Freeing Space  \r\n");
                    Free_Space(App_ID);
                    }
             //else
             {

                 int i=0;
                 for(i=0;i<Ptr->DCS_Header.Len;i++)

                 {

                 TEMP_Storage[App_ID][TEMP_Counter[App_ID]].CoordX=Ptr->Routing_Header.Src_Info.SrcX;
                 TEMP_Storage[App_ID][TEMP_Counter[App_ID]].CoordY=Ptr->Routing_Header.Src_Info.SrcY;

                TEMP_Storage[App_ID][TEMP_Counter[App_ID]].Value=PtrD[i].Value;
                TEMP_Storage[App_ID][TEMP_Counter[App_ID]].Time_Stamp=PtrD[i].Time_Stamp;

                 vPrintString("\tTemp Value ");
                 vDisplayHex16(PtrD[i].Value);
                 vPrintString("\tTemp TimeStamp ");
                 vDisplayHex16(PtrD[i].Time_Stamp);
                Rep_Data[i]=TEMP_Storage[App_ID][TEMP_Counter[App_ID]];
                    TEMP_Counter[App_ID]++;
                    }
             }

             uint16 SrcX=Ptr->Routing_Header.Src_Info.SrcX;
             uint16 SrcY=Ptr->Routing_Header.Src_Info.SrcY;
             uint16 Flags=Ptr->DCS_Header.Flags;

    Ptr->DCS_Header.Rep_Num=Replica_Num[App_ID];

    Perform_Replication(Ptr->DCS_Header.Len,Ptr->DCS_Header.Rep_Num,App_ID);







     int Len=0;



                 uint8* pu8Payload;
                // if(Ptr->DCS_Header.AppID==TEMP)
                 {
                    // tsData_Storage Resp_Data[Len];


                 int i=0;

uint16 NextHop_Addr=Calculate_NextHopAddress(Source_X,Source_Y,SrcX,SrcY);

    Routing_Header.Version=1;   // 0
    Routing_Header.ToS=1;       // 1
    Routing_Header.TTL=5;       //2        //decrement it in each hop,when reduces to zero then discard it
    Routing_Header.Proto=1;     //3
    Routing_Header.Src_Info.SrcX=Source_X;       //4-5
    Routing_Header.Src_Info.SrcY=Source_Y;       // 6-7
    Routing_Header.Dst_Info.DstX=SrcX;       // 8-9
    Routing_Header.Dst_Info.DstY=SrcY;       // 10-11

    DCS_Header.OpCode=GET_RESP;     //12
    if(Flags==1&&NextEpoch_Info[App_ID]==TRUE)
    {
        DCS_Header.Flags=1;      //13              Next Epoch Info Being Sent

        NextEpochInfo.EpochExpiryTime=Epoch_Expiry_Time[App_ID]-Wakeup_Curr_Time;
        NextEpochInfo.NextNoReplica=No_Replica_NextEpoch[App_ID];
        NextEpochInfo.Num_NextEpoch=EPOCH_No[App_ID]+1;
        //vPrintString("\t EpochExpiryTime ");
        //vNum2StringPrint(NextEpochInfo.EpochExpiryTime,10);
        //vPrintString("\t NextNoReplica ");
        //vNum2StringPrint(NextEpochInfo.NextNoReplica,10);


    }else{
        DCS_Header.Flags=0;      //13              Next Epoch Info Not Being Sent
    }
    // Data_Transmit.Data_Type=TEMP;       //16
    DCS_Header.Data_Type=1;
    DCS_Header.Len=Len;

    DCS_Header.Epoch_No=EPOCH_No[App_ID];
    DCS_Header.AppID=App_ID;



  //  vPrintString("\tBytes of Data");
   // vDisplayHex16(sizeof(tsData_Storage));
   // vDisplayHex16(DCS_Header.Len*sizeof(tsData_Storage));               //17


    uint8 *PtrNextEpoch=(uint8 *)&NextEpochInfo;

    Headers.Routing_Header=Routing_Header;
    Headers.DCS_Header=DCS_Header;
    tsHeaders *Ptr=&Headers;
    pu8Payload=(uint8 *)Ptr;

    for(i=0;i<sizeof(Headers);i++)
    {
        PayLoad[i]=pu8Payload[i];
    }

    if(DCS_Header.Flags==1)
    {
        for(i=0;i<sizeof(tsNextEpochInfo);i++)
    {
        PayLoad[sizeof(Headers)+i]=PtrNextEpoch[i];

    }

    }


   // vPrintString("\tSize Of Header ");
   // vDisplayHex16(sizeof(Routing_Header));
    // vDisplayHex16(sizeof(DCS_Header));

     if(NextHop_Addr==Src_Add)
        vPrintString("\tI am the Destination,Data won't GET_RESPONSE NEXT EPOCH INFO\r\n");
        else{

            if(DCS_Header.Flags==1)
            {
                vPrintString("\tNext_Hop Address ");
            vDisplayHex16(NextHop_Addr);
            vPrintString("\tSENDING GET RESPONSE NEXT EPOCH INFO\r\n");
            Display();
                vTransmitDataPacket(PayLoad,sizeof(Headers)+sizeof(tsNextEpochInfo), NextHop_Addr);
            }
            }
            }


             }
             }else if(Ptr->DCS_Header.OpCode==REPLICATION)
             {

                 vPrintString("\tRECIEVED REPLICATION REQUEST\r\n");
                 vPrintString("APP ID ");
                 vNum2StringPrint(App_ID);

                 vPrintString("\tNo Of Structures In REPLICATION REQ");
                 vDisplayHex16(Ptr->DCS_Header.Len);
                 tsData_Storage *PtrD=&ptr8[sizeof(Headers)];
                // tsData_Storage Rep_Data[Ptr->DCS_Header.Len];
                bool Flag=Replica_Num_Check(Ptr->Routing_Header.Dst_Info.DstX,Ptr->Routing_Header.Dst_Info.DstY,App_ID);




                 if(Flag==TRUE)      //here avoiding the little synchronization problem by discarding the PUT REQ after epoch change
                 {


             if(Temp_Replica[App_ID]==FALSE)
                {
                     Temp_Replica[App_ID]=TRUE;
                     Node_Type[App_ID]=REPLICA;
                     vLightBulbInit();
                     int i=0;
                     for(i=0;i<Curr_Replicas[App_ID];i++)
                     {
                        if(Array_HashX[App_ID][i]==Ptr->Routing_Header.Dst_Info.DstX&&Array_HashY[App_ID][i]==Ptr->Routing_Header.Dst_Info.DstY)
                         {
                             Replica_Num[App_ID]=i;
                             break;
                         }
                     }

                }

             if(TEMP_Counter[App_ID]==MAX_STORAGE)
                    {
                        vPrintString("\tReached Storage Limit Freeing Space  \r\n");
                        Free_Space(App_ID);
                    }
             //else
             {

                 int i=0;
                 for(i=0;i<Ptr->DCS_Header.Len;i++)

                 {

                 TEMP_Storage[App_ID][TEMP_Counter[App_ID]].CoordX=PtrD[i].CoordX;
                 TEMP_Storage[App_ID][TEMP_Counter[App_ID]].CoordY=PtrD[i].CoordY;

                TEMP_Storage[App_ID][TEMP_Counter[App_ID]].Value=PtrD[i].Value;
                TEMP_Storage[App_ID][TEMP_Counter[App_ID]].Time_Stamp=PtrD[i].Time_Stamp;

                Rep_Data[i].CoordX=PtrD[i].CoordX;
                Rep_Data[i].CoordY=PtrD[i].CoordY;
                Rep_Data[i].Value=PtrD[i].Value;
                Rep_Data[i].Time_Stamp=PtrD[i].Time_Stamp;


                 vPrintString("\tTemp Value ");
                 vDisplayHex16(PtrD[i].Value);
                 vPrintString("\tTemp TimeStamp ");
                 vDisplayHex16(PtrD[i].Time_Stamp);

                    TEMP_Counter[App_ID]++;
                    }
             }
             Perform_Replication(Ptr->DCS_Header.Len,Ptr->DCS_Header.Rep_Num,App_ID);


                 }
             }
             else if(Ptr->DCS_Header.OpCode==GET)
             {
                 if(Measure_Flag[App_ID]==TRUE)
                 Replication_Stat[App_ID].Tr_Mesg_Cons++;

                 vPrintString("RECIEVED GET REQUEST\r\n");
                 vPrintString("APP ID ");
                 vNum2StringPrint(App_ID);
                 int Len=Ptr->DCS_Header.Len;

                 vPrintString("\tNo Of Structures Requested In GET");
                 vDisplayHex16(Ptr->DCS_Header.Len);

                 uint8* pu8Payload;
                // if(Ptr->DCS_Header.AppID==TEM)
                 {
                     tsData_Storage Resp_Data[Len];


                 int i=0;
                 for(i=0;i<Len;i++)
                 {
                     Resp_Data[i].CoordX=TEMP_Storage[App_ID][TEMP_Counter[App_ID]-1-i].CoordX;
                     Resp_Data[i].CoordY=TEMP_Storage[App_ID][TEMP_Counter[App_ID]-1-i].CoordY;
                     Resp_Data[i].Value=TEMP_Storage[App_ID][TEMP_Counter[App_ID]-1-i].Value;
                     Resp_Data[i].Time_Stamp=TEMP_Storage[App_ID][TEMP_Counter[App_ID]-1-i].Time_Stamp;

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
    //DCS_Header.Flags=0;      //13
    // Data_Transmit.Data_Type=TEMP;       //16


    if(Ptr->DCS_Header.Flags==1&&NextEpoch_Info[App_ID]==TRUE)
    {
        DCS_Header.Flags=1;      //13              Next Epoch Info Being Sent

        NextEpochInfo.EpochExpiryTime=Epoch_Expiry_Time[App_ID]-Wakeup_Curr_Time;
        NextEpochInfo.NextNoReplica=No_Replica_NextEpoch[App_ID];
        NextEpochInfo.Num_NextEpoch=EPOCH_No[App_ID]+1;

    }else{
        DCS_Header.Flags=0;      //13              Next Epoch Info Not Being Sent
    }

DCS_Header.Epoch_No=EPOCH_No[App_ID];
DCS_Header.AppID=App_ID;


    DCS_Header.Data_Type=1;
    DCS_Header.Len=Len;

    vPrintString("\tBytes of Data");
    vDisplayHex16(sizeof(tsData_Storage));
    vDisplayHex16(DCS_Header.Len*sizeof(tsData_Storage));               //17

    uint8 * PtrD=(uint8 *)&Resp_Data;
     uint8 *PtrNextEpoch=(uint8 *)&NextEpochInfo;


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

    if(DCS_Header.Flags==1)
    {
        for(i=0;i<sizeof(tsNextEpochInfo);i++)
    {
        PayLoad[sizeof(Headers)+DCS_Header.Len*sizeof(tsData_Storage)+i]=PtrNextEpoch[i];

    }

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
            if(DCS_Header.Flags==1)
            vTransmitDataPacket(PayLoad,sizeof(Headers)+DCS_Header.Len*sizeof(tsData_Storage)+sizeof(tsNextEpochInfo), NextHop_Addr);
            else{
                vTransmitDataPacket(PayLoad,sizeof(Headers)+DCS_Header.Len*sizeof(tsData_Storage), NextHop_Addr);

            }
            }
            }
            }
             else if(Ptr->DCS_Header.OpCode==GET_RESP)
             {
                 tsData_Storage *PtrD=&ptr8[sizeof(Headers)];
            vPrintString("RECIEVED GET RESPONSE \r\n");
            vPrintString("APP ID ");
                 vNum2StringPrint(App_ID);

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

                 if(Ptr->DCS_Header.Flags==1&&NextEpoch_Info[App_ID]==FALSE)
                 {
                     tsNextEpochInfo *NextEpochPtr=&ptr8[sizeof(Headers)+Ptr->DCS_Header.Len*sizeof(tsData_Storage)];
                     Epoch_Expiry_Time[App_ID]=Wakeup_Curr_Time+NextEpochPtr->EpochExpiryTime;
                     No_Replica_NextEpoch[App_ID]=NextEpochPtr->NextNoReplica;
                     NextEpoch_Info[App_ID]=TRUE;
                     NextEpochNo[App_ID]=NextEpochPtr->Num_NextEpoch;
                     vPrintString("\t Epoch_Expiry_Time");
                     vDisplayHex16(Epoch_Expiry_Time[App_ID]);
                   //  vNum2StringPrint(Epoch_Expiry_Time,10);
                     vPrintString("\t No_Replica_NextEpoch");
                      vDisplayHex16(No_Replica_NextEpoch[App_ID]);
                     //vNum2StringPrint(No_Replica_NextEpoch,10);

                 }




            }else if(Ptr->DCS_Header.OpCode==REPLICA_STATISTICS)
            {
                vPrintString("Replica Statistics \r\n");
                vPrintString("ROOT NODE \r\n");
               vDisplayHex16(Ptr->DCS_Header.Rep_Num);
               vPrintString("APP ID ");
                 vNum2StringPrint(App_ID);


                tsReplication_Stat *PtrD=&ptr8[sizeof(Headers)];
                vPrintString("\tProduction Mesg");
               // vNum2StringPrint(PtrD->Tr_Mesg_Prod,10);
                vPrintString("\tConsumption Mesg");
              //  vNum2StringPrint(PtrD->Tr_Mesg_Cons,10);
              SendingReplication_Stat[App_ID].Tr_Mesg_Prod=PtrD->Tr_Mesg_Prod;
              SendingReplication_Stat[App_ID].Tr_Mesg_Cons=PtrD->Tr_Mesg_Cons;
              uint8 Root_Node=Ptr->DCS_Header.Rep_Num;
                if(Traffic_Measure[App_ID][Root_Node][0]==-1)
                {
                    No_Stat_Recv[App_ID]=Ptr->DCS_Header.Flags+No_Stat_Recv[App_ID];
                    Traffic_Measure[App_ID][Root_Node][0]=PtrD->Tr_Mesg_Prod;
                    Traffic_Measure[App_ID][Root_Node][1]=PtrD->Tr_Mesg_Cons;
                }
                /*if((No_Stat_Recv==Curr_Replicas)&&NextEpoch_Info==FALSE)
                {


                    int i=0,max;
                    int var,c,p;
                    for(i=0;i<Curr_Replicas;i++)
                    {
                        if(i==0)
                        {
                            max=Traffic_Measure[i][0]+Traffic_Measure[i][1];
                            c=Traffic_Measure[i][1];
                            p=Traffic_Measure[i][0];

                        }
                        else if(max<(Traffic_Measure[i][0]+Traffic_Measure[i][1]))
                        {
                            max=Traffic_Measure[i][0]+Traffic_Measure[i][1];
                            c=Traffic_Measure[i][1];
                            p=Traffic_Measure[i][0];
                        }
                    }
                    Epoch_Expiry_Time=Wakeup_Curr_Time+((((60*Traffic_Measurement_Time)*Max_Packets)/max)-(60*Traffic_Measurement_Time));
                    if(p!=0)
                    var=14*(c/p);
                    var=var/10;

                    No_Replica_NextEpoch=var;
                    NextEpoch_Info=TRUE;

                    vPrintString("\tEpoch Expiry Time \r\n");
                  //  vNum2StringPrint(Epoch_Expiry_Time,10);
                  vDisplayHex16(Epoch_Expiry_Time);

                    vPrintString("\tNo of Replica in Next Epoch \r\n");
                   // vNum2StringPrint(var,10);
                   vDisplayHex16(var);

                  //  No_Stat_Recv=0;
                }*/
  Replication_Tree_RepStat(Root_Node,Ptr->DCS_Header.Flags,App_ID);








   uint8 *pu8Payload;




   uint16 Src_Add=COORD_ADDR;
   uint16 Dst_Add;



   uint16 NextHop_Addr=Calculate_NextHopAddress(Source_X,Source_Y,Array_HashX[App_ID][Root_Node],Array_HashY[App_ID][Root_Node]);

    Routing_Header.Version=1;   // 0
    Routing_Header.ToS=1;       // 1
    Routing_Header.TTL=5;       //2        //decrement it in each hop,when reduces to zero then discard it
    Routing_Header.Proto=1;     //3
    Routing_Header.Src_Info.SrcX=Source_X;       //4-5
    Routing_Header.Src_Info.SrcY=Source_Y;       // 6-7
    Routing_Header.Dst_Info.DstX=Array_HashX[App_ID][Root_Node];       // 8-9
    Routing_Header.Dst_Info.DstY=Array_HashY[App_ID][Root_Node];       // 10-11

                vPrintString("SENDING ACK \r\n");

        DCS_Header.OpCode=ACK;     //12

        DCS_Header.Flags=Num_Replicas_Node[App_ID];      //13

        DCS_Header.Data_Type=1;      //16
        DCS_Header.Len=0;
DCS_Header.Epoch_No=EPOCH_No[App_ID];
DCS_Header.AppID=App_ID;
        DCS_Header.Rep_Num=Replica_Num[App_ID];
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
if(NextHop_Addr==Src_Add)
        {
            vPrintString("\tI am the Destination,won't Send ACK \r\n");
            Msg_Con++;
        }
        else{
            Display();
            vPrintString("\tNext_Hop Address ");
            vDisplayHex16(NextHop_Addr);
            vTransmitDataPacket(PayLoad,sizeof(Headers), NextHop_Addr);
            }





           }else if(Ptr->DCS_Header.OpCode==REPLICA_UPDATE)
           {
               vPrintString("\tRECIEVED REPLICA_UPDATE REQUEST\r\n");
                 tsPUTData *PtrD=&ptr8[sizeof(Headers)];
                 vPrintString("\tNo Of Structures In REPLICA_UPDATE");
                 vDisplayHex16(Ptr->DCS_Header.Len);

                 vPrintString("APP ID ");
                 vNum2StringPrint(App_ID);

           //  vDisplayHex16(PtrD[0].Value);
            // vDisplayHex16(PtrD[0].Time_Stamp);
             //vDisplayHex16(PtrD[1].Value);
            // vDisplayHex16(PtrD[1].Time_Stamp);
              // tsData_Storage Rep_Data[Ptr->DCS_Header.Len];

            bool Flag=Replica_Num_Check(Ptr->Routing_Header.Dst_Info.DstX,Ptr->Routing_Header.Dst_Info.DstY,App_ID);
                 if(Flag==TRUE)      //here avoiding the little synchronization problem by discarding the PUT REQ after epoch change
                {



             if(Temp_Replica[App_ID]==FALSE)
                {
                     Temp_Replica[App_ID]=TRUE;
                     Node_Type[App_ID]=REPLICA;
                     vLightBulbInit();

                     int i=0;
                     for(i=0;i<Curr_Replicas[App_ID];i++)
                     {
                        if(Array_HashX[App_ID][i]==Ptr->Routing_Header.Dst_Info.DstX&&Array_HashY[App_ID][i]==Ptr->Routing_Header.Dst_Info.DstY)
                         {
                             Replica_Num[App_ID]=i;
                             vPrintString("\tReplica_Num= ");
                             vDisplayHex16(i);

                             break;
                         }
                     }

                }

             if(TEMP_Counter[App_ID]==MAX_STORAGE)
                    {
                        vPrintString("\tReached Storage Limit Freeing Space  \r\n");
                    Free_Space(App_ID);
                    }
             //else
             {

                 int i=0;
                 for(i=0;i<Ptr->DCS_Header.Len;i++)

                 {

                 TEMP_Storage[App_ID][TEMP_Counter[App_ID]].CoordX=Ptr->Routing_Header.Src_Info.SrcX;
                 TEMP_Storage[App_ID][TEMP_Counter[App_ID]].CoordY=Ptr->Routing_Header.Src_Info.SrcY;

                TEMP_Storage[App_ID][TEMP_Counter[App_ID]].Value=PtrD[i].Value;
                TEMP_Storage[App_ID][TEMP_Counter[App_ID]].Time_Stamp=PtrD[i].Time_Stamp;

                 vPrintString("\tTemp Value ");
                 vDisplayHex16(PtrD[i].Value);
                 vPrintString("\tTemp TimeStamp ");
                 vDisplayHex16(PtrD[i].Time_Stamp);
                //Rep_Data[i]=TEMP_Storage[TEMP_Counter];
                    TEMP_Counter[App_ID]++;
                    }
             }
           }

           }else if(Ptr->DCS_Header.OpCode==ACK)
           {
               vPrintString("\tRECIEVED ACK \r\n");

               vPrintString("APP ID ");
                 vNum2StringPrint(App_ID);

               if(ACK_Wait[App_ID]==TRUE&&ACK_Recv[App_ID][Ptr->DCS_Header.Rep_Num]==-1)
               {
                   vPrintString("Ptr->DCS_Header.Rep_Num");
                   vDisplayHex16(Ptr->DCS_Header.Rep_Num);
                   ACK_Recv[App_ID][Ptr->DCS_Header.Rep_Num]=1;

                   No_ACK_Recv[App_ID]=No_ACK_Recv[App_ID]+Ptr->DCS_Header.Flags;
               }

               if(No_ACK_Recv[App_ID]==(Curr_Replicas[App_ID]-Num_Replicas_Node[App_ID])&&ACK_Wait[App_ID]==TRUE)
               {
                   vPrintString("ACK MADE FALSE \r\n");

                   ACK_Wait[App_ID]=FALSE;



               }

           }
        }
        else{

            vPrintString("Packet Forwarding \r\n");
    vPrintString("\tNext_Hop Address ");
    vDisplayHex16(NextHop_Addr);
    vTransmitDataPacket((uint8 *)psFrame->au8Sdu,psFrame->u8SduLength, NextHop_Addr);


        }
        }
    }
}

/***************************************************************************

***************************************************************************/
PRIVATE void Replication_Algo(uint8 Rep_Num,uint8 Replica_Num_Used,uint8 App_ID)
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
    for(i=0;i<Curr_Replicas[App_ID];i++)
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
            tempdist=(Array_HashX[App_ID][Selected_Nodes[j]]-Array_HashX[App_ID][Non_Selected_Nodes[i][0]])*(Array_HashX[App_ID][Selected_Nodes[j]]-Array_HashX[App_ID][Non_Selected_Nodes[i][0]])+(Array_HashY[App_ID][Selected_Nodes[j]]-Array_HashY[App_ID][Non_Selected_Nodes[i][0]])*(Array_HashY[App_ID][Selected_Nodes[j]]-Array_HashY[App_ID][Non_Selected_Nodes[i][0]]);
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

    if(Sel_Node==Replica_Num_Used)
    {
        Replication_Forwarding_Sets[Rep_Num][Count_Forwarding_Sets[Rep_Num]]=Non_Sel_Node;
        Count_Forwarding_Sets[Rep_Num]++;

        Selected_Nodes[Num_Sel_Nodes]=Non_Sel_Node;
        Num_Sel_Nodes++;
         vPrintString("\tAdded in Selected set \r\n");
         vDisplayHex16(Non_Sel_Node);
         vDisplayHex16(Replica_Num_Used);
         vDisplayHex16(Sel_Node);

    }else{
        Selected_Nodes[Num_Sel_Nodes]=Non_Sel_Node;
        Num_Sel_Nodes++;
        vPrintString("\tAdded in Non Selected set \r\n");
         vDisplayHex16(Non_Sel_Node);
         vDisplayHex16(Replica_Num_Used);
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
PRIVATE void Perform_Replication(uint8 Len,uint8 Get_Root_Node,uint8 App_ID)
{
    int Count=0;
    for(Count=0;Count<Num_Replicas_Node[App_ID];Count++)
    {


               vPrintString("\tStarting Replication \r\n");
               vPrintString("\tGet_Root_Node \r\n");
                vDisplayHex16(Get_Root_Node);
   int Num=0;


   Replication_Algo(Get_Root_Node,Replica_Num_Array[App_ID][Count],App_ID);

    for(Num=0;Num<Count_Forwarding_Sets[Get_Root_Node];Num++)
    {




        if(Replication_Forwarding_Sets[Get_Root_Node][Num]!=Replica_Num[App_ID])

        {
            uint16 DstX=Array_HashX[App_ID][Replication_Forwarding_Sets[Get_Root_Node][Num]];
            uint16 DstY=Array_HashY[App_ID][Replication_Forwarding_Sets[Get_Root_Node][Num]];
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
    DCS_Header.AppID=App_ID;

    DCS_Header.Epoch_No=EPOCH_No[App_ID];

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
            Char_Data_Type="TEM";
            //Node_Type=PRODUCER;
            DCS_Header.AppID=TEM;
            vLightBulbInit();
          //  Send_Msg(TEM,Node_Type);
        }
        else if((psAHI_Ind->u32ItemBitmap & E_AHI_DIO10_INT))
        {
            //Char_Data_Type="HUMIDITY";
            //DCS_Header.AppID=HUMIDITY;
            Char_Data_Type="TEMP";
           // Node_Type=CONSUMER;
            DCS_Header.AppID=TEM;
            vLightBulbInit();
            //Send_Rep_Stat();
            int i=0;
            for(i=0;i<No_App;i++)
            {

             vPrintString(" No_Stat_Recv");
                  vDisplayHex16(No_Stat_Recv[i]);

                  vPrintString(" Epoch_Expiry_Time");
                  vDisplayHex16(Epoch_Expiry_Time[i]);

                  vPrintString(" Curr Replica");
                  vDisplayHex16(Curr_Replicas[i]);

                  vPrintString(" EPOCH_No");
                  vDisplayHex16(EPOCH_No[i]);

                   vPrintString(" No ACK");
                  vDisplayHex16(No_ACK_Recv[i]);

                  vPrintString(" ACK WAIT  ");
                  if(ACK_Wait[i])
                  {
                       vPrintString(" TRUE \r\n");

                  }else{
                      vPrintString(" FALSE \r\n");

                  }
                  vPrintString(" Replica_Num_Check ");
                  vDisplayHex16(Num_Replicas_Node[i]);

                  vPrintString(" Replica in Next Epoch ");
                  vDisplayHex16(No_Replica_NextEpoch[i]);
            }
        }else if((psAHI_Ind->u32ItemBitmap & E_AHI_DIO11_INT))
        {
            //Char_Data_Type="HUMIDITY";
            //DCS_Header.AppID=HUMIDITY;
            Char_Data_Type="TEMP";
         //   Node_Type=CONSUMER;
            DCS_Header.AppID=TEM;
            vLightBulbInit();
            //Send_Msg(Char_Data_Type,Node_Type);
            //Send_Rep_Stat();
            int i=0;
            for(i=0;i<No_App;i++)
            {

             vPrintString(" No_Stat_Recv");
                  vDisplayHex16(No_Stat_Recv[i]);

                  vPrintString(" Epoch_Expiry_Time");
                  vDisplayHex16(Epoch_Expiry_Time[i]);

                  vPrintString(" Curr Replica");
                  vDisplayHex16(Curr_Replicas[i]);

                  vPrintString(" EPOCH_No");
                  vDisplayHex16(EPOCH_No[i]);

                   vPrintString(" No ACK");
                  vDisplayHex16(No_ACK_Recv[i]);

                  vPrintString(" ACK WAIT  ");
                  if(ACK_Wait[i])
                  {
                       vPrintString(" TRUE \r\n");

                  }else{
                      vPrintString(" FALSE \r\n");

                  }

                  vPrintString(" Replica_Num_Check ");
                  vDisplayHex16(Num_Replicas_Node[i]);
                  vPrintString(" Replica in Next Epoch ");
                  vDisplayHex16(No_Replica_NextEpoch[i]);

            }

        }


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
                vNum2StringPrint(Msg_Sent);
               // vDisplayHex16(Msg_Sent);
                vPrintString("\t Msg Recv ");
                vNum2StringPrint(Msg_Recv);
                //vDisplayHex16(Msg_Recv);

                int j=1;
            for(j=1;j<=10;j++)
            {
                vNum2StringPrint(j*1000);

                vNum2StringPrint(Time_Per_Thousand_Mesg[j]);

            }

              /*  vPrintString("\t Mesg Gen ");
                vDisplayHex16(Msg_Gen);
                vPrintString("\t Mesg Cons ");
                vDisplayHex16(Msg_Con);*/

               /* vPrintString("\t OverHead Sent ");
                vDisplayHex16(OverHeadSent);
                vPrintString("\t OverHead Recv ");
                vDisplayHex16(OverHeadRecv);*/

            }else
            {
                vPrintString("First stop Timer \r\n");
                vPrintString("Wakeup_Curr_Time ");
                 vDisplayHex16(Wakeup_Curr_Time);



                 int i=0;
                 for(i=0;i<No_App;i++)
                 {

                vPrintString("App ID");
                 vDisplayHex16(i);

                  vPrintString(" No_Stat_Recv");
                  vDisplayHex16(No_Stat_Recv[i]);

                  vPrintString(" Epoch_Expiry_Time");
                  vDisplayHex16(Epoch_Expiry_Time[i]);

                  vPrintString(" Curr Replica");
                  vDisplayHex16(Curr_Replicas[i]);

                  vPrintString(" EPOCH_No");
                  vDisplayHex16(EPOCH_No[i]);

                   vPrintString(" No ACK");
                  vDisplayHex16(No_ACK_Recv[i]);

                  vPrintString(" ACK WAIT  ");
                  if(ACK_Wait[i])
                  {
                       vPrintString(" TRUE \r\n");

                  }else{
                      vPrintString(" FALSE \r\n");

                  }

                  vPrintString(" Replica_Num_Check ");
                  vDisplayHex16(Num_Replicas_Node[i]);

                  vPrintString(" Replica in Next Epoch ");
                  vDisplayHex16(No_Replica_NextEpoch[i]);

                   //Send_Rep_Stat();
                 }

            }

        }else if((psAHI_Ind->u32ItemBitmap & E_AHI_DIO11_INT))
        {
             if(START==FALSE)
            {
                vPrintString("Printing Summary \r\n");
                vPrintString("\t Short Address ");
                vDisplayHex16(COORD_ADDR);

                vPrintString("\t Mesg Sent ");
                vNum2StringPrint(Msg_Sent);
               // vDisplayHex16(Msg_Sent);
                vPrintString("\t Msg Recv ");
                vNum2StringPrint(Msg_Recv);
                //vDisplayHex16(Msg_Recv);

                int j=1;
            for(j=1;j<=10;j++)
            {
                vNum2StringPrint(j*1000);

                vNum2StringPrint(Time_Per_Thousand_Mesg[j]);

            }

            }else
            {
                vPrintString("First stop Timer \r\n");

                vPrintString("Wakeup_Curr_Time ");
                 vDisplayHex16(Wakeup_Curr_Time);

                 int i=0;
                 for(i=0;i<No_App;i++)
                 {
                     vPrintString("App ID");
                 vDisplayHex16(i);

                  vPrintString(" No_Stat_Recv");
                  vDisplayHex16(No_Stat_Recv[i]);

                  vPrintString(" Epoch_Expiry_Time");
                  vDisplayHex16(Epoch_Expiry_Time[i]);

                  vPrintString(" Curr Replica");
                  vDisplayHex16(Curr_Replicas[i]);

                  vPrintString(" EPOCH_No");
                  vDisplayHex16(EPOCH_No[i]);

                   vPrintString(" No ACK");
                  vDisplayHex16(No_ACK_Recv[i]);

                  vPrintString(" ACK WAIT  ");
                  if(ACK_Wait[i])
                  {
                       vPrintString(" TRUE \r\n");

                  }else{
                      vPrintString(" FALSE \r\n");

                  }

                  vPrintString(" Replica_Num_Check ");
                  vDisplayHex16(Num_Replicas_Node[i]);

                  vPrintString(" Replica in Next Epoch ");
                  vDisplayHex16(No_Replica_NextEpoch[i]);

                 }


                  // Send_Rep_Stat();

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
void Send_Msg(uint8 App_ID,NODE_TYPE Node_Type1)
{

    int i=0;
   uint8 *pu8Payload;




   uint16 Src_Add=COORD_ADDR;
   uint16 Dst_Add;

   Closest_Replica(Source_X,Source_Y,App_ID);
   vPrintString("DIGEST X ");
   vDisplayHex16(HashX[App_ID]);
   vPrintString("DIGEST Y ");
   vDisplayHex16(HashY[App_ID]);

   uint16 NextHop_Addr=Calculate_NextHopAddress(Source_X,Source_Y,HashX[App_ID],HashY[App_ID]);

    Routing_Header.Version=1;   // 0
    Routing_Header.ToS=1;       // 1
    Routing_Header.TTL=5;       //2        //decrement it in each hop,when reduces to zero then discard it
    Routing_Header.Proto=1;     //3
    Routing_Header.Src_Info.SrcX=Source_X;       //4-5
    Routing_Header.Src_Info.SrcY=Source_Y;       // 6-7
    Routing_Header.Dst_Info.DstX=HashX[App_ID];       // 8-9
    Routing_Header.Dst_Info.DstY=HashY[App_ID];       // 10-11


   if(Node_Type1==CONSUMER)    //Query
   {
       if(Measure_Flag[App_ID]==TRUE)
       Replication_Stat[App_ID].Tr_Mesg_Cons++;

       vPrintString("CONSUMER GET DATA \r\n");
       vPrintString("APP ID ");
       vNum2StringPrint(App_ID);

        DCS_Header.OpCode=GET;     //12

      //  DCS_Header.Flags=0;      //13

       if(NextEpoch_Info[App_ID]==FALSE)
        {
             DCS_Header.Flags=1;      //13                  lowest bit 1 means info is needed
        }else{
            DCS_Header.Flags=0;      //13                  lowest bit 0 means info is not needed
        }

DCS_Header.Epoch_No=EPOCH_No[App_ID];

        DCS_Header.Data_Type=1;      //16
        DCS_Header.Len=No_Of_GET;

        DCS_Header.AppID=App_ID;
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
       if(Measure_Flag[App_ID]==TRUE)
        Replication_Stat[App_ID].Tr_Mesg_Prod++;

        vPrintString("PRODUCER PUT DATA \r\n");
        vPrintString("APP ID ");
       vNum2StringPrint(App_ID);

        DCS_Header.OpCode=PUT;     //12
        // DCS_Header.Flags=0;      //13
        if(NextEpoch_Info[App_ID]==FALSE)
        {
             DCS_Header.Flags=1;      //13                  lowest bit 1 means info is needed
        }else{
            DCS_Header.Flags=0;      //13                  lowest bit 0 means info is not needed
        }

DCS_Header.Epoch_No=EPOCH_No[App_ID];

       DCS_Header.Data_Type=1;       //16
       // Data_Transmit.Data_Type=TEMP;       //16
       DCS_Header.Len=No_Of_PUT;
       DCS_Header.AppID=App_ID;                //17
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
    Data_Transmit.Data[0]=(uint8)u16TempSensor;     //20-21-22*/


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

        Closest_Replica(Source_X,Source_Y,App_ID);
         bool Flag=Replica_Num_Check(Routing_Header.Dst_Info.DstX,Routing_Header.Dst_Info.DstY,App_ID);



        if(Temp_Replica[App_ID]==FALSE)
                {
                     Temp_Replica[App_ID]=TRUE;
                     Node_Type[App_ID]=REPLICA;
                     vLightBulbInit();

                     int i=0;
                     for(i=0;i<Curr_Replicas[App_ID];i++)
                     {
                        if(Array_HashX[App_ID][i]==Routing_Header.Dst_Info.DstX&&Array_HashY[App_ID][i]==Routing_Header.Dst_Info.DstY)
                         {
                             Replica_Num[App_ID]=i;
                             vPrintString("\tReplica_Num= ");
                             vDisplayHex16(i);

                             break;
                         }
                     }

                }


                 if(TEMP_Counter[App_ID]==MAX_STORAGE)
                    {
                        vPrintString("\tReached Storage Limit Freeing Space  \r\n");
                    Free_Space(App_ID);
                    }
             //else
             {

                 int i=0;
                 for(i=0;i<DCS_Header.Len;i++)

                 {

                 TEMP_Storage[App_ID][TEMP_Counter[App_ID]].CoordX=Routing_Header.Src_Info.SrcX;
                 TEMP_Storage[App_ID][TEMP_Counter[App_ID]].CoordY=Routing_Header.Src_Info.SrcY;

                TEMP_Storage[App_ID][TEMP_Counter[App_ID]].Value=PUTData[i].Value;
                TEMP_Storage[App_ID][TEMP_Counter[App_ID]].Time_Stamp=PUTData[i].Time_Stamp;

                 vPrintString("\tTemp Value ");
                 vDisplayHex16(PUTData[i].Value);
                 vPrintString("\tTemp TimeStamp ");
                 vDisplayHex16(PUTData[i].Time_Stamp);
                Rep_Data[i]=TEMP_Storage[App_ID][TEMP_Counter[App_ID]];
                    TEMP_Counter[App_ID]++;
                    }
             }

    DCS_Header.Rep_Num=Replica_Num[App_ID];
    Perform_Replication(DCS_Header.Len,DCS_Header.Rep_Num,App_ID);



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
void Closest_Replica(uint16 SrcX,uint16 SrcY,uint8 App_ID)
{
    int i=0;
    int dist=2*(DIMX*DIMX+DIMY*DIMY);
    int tempdist;




   for( i=0;i<Curr_Replicas[App_ID];i++)
   {
       tempdist=(Array_HashX[App_ID][i]-SrcX)*(Array_HashX[App_ID][i]-SrcX)+(Array_HashY[App_ID][i]-SrcY)*(Array_HashY[App_ID][i]-SrcY);

        if(tempdist<dist)
        {
        dist=tempdist;
        HashX[App_ID]=Array_HashX[App_ID][i];
        HashY[App_ID]=Array_HashY[App_ID][i];
        vPrintString("Hash X");
        vDisplayHex16(HashX[App_ID]);
        vPrintString("Hash Y");
        vDisplayHex16(HashY[App_ID]);

        }
        }
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
    uint8 * ptr8=pu8Data;
    tsHeaders *Ptr=(tsHeaders *)ptr8;
    if(Ptr->DCS_Header.OpCode==REPLICA_STATISTICS||Ptr->DCS_Header.OpCode==REPLICA_UPDATE||Ptr->DCS_Header.OpCode==ACK)
    {
        OverHeadSent++;
    }
    if(Ptr->DCS_Header.OpCode==GET_RESP&&Ptr->DCS_Header.Len==0)
    {
        OverHeadSent++;

    }

    Msg_Sent++;

      int Total=Msg_Sent+Msg_Recv;
                if(((Total>=1000&&Total<2000)||(Total>=2000&&Total<3000)||(Total>=3000&&Total<4000)||(Total>=4000&&Total<5000)||(Total>=5000&&Total<6000)||(Total>=6000&&Total<7000)||(Total>=7000&&Total<8000)||(Total>=8000&&Total<9000)||(Total>=9000&&Total<10000)||(Total>=10000&&Total<11000))
                &&Time_Per_Thousand_Mesg[Total/1000]==1)
                {
                    Time_Per_Thousand_Mesg[Total/1000]=Time_Counter-60;;
                }

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

    vPrintString("\tdata Length");
    vDisplayHex16(u8Len);

   // vPrintString("\tdata Values");

    for (i = 0; i < u8Len ; i++)
    {
       // pu8Payload[i] = *pu8Data++;
       pu8Payload[i] = pu8Data[i];

    }
/* Request transmit */
    vAppApiMcpsRequest(&sMcpsReqRsp, &sMcpsSyncCfm);
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
