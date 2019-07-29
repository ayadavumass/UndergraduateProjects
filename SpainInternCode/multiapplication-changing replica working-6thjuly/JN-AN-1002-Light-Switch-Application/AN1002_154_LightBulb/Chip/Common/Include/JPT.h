/*****************************************************************************
 *
 * MODULE:              Production Test API - Header
 *
 * COMPONENT:           JPT.h,v
 *
 * VERSION:             Build_Release_030308_RC1
 *
 * REVISION:            1.1
 *
 * DATED:               2007/10/04 13:46:16
 *
 * STATUS:              Exp
 *
 * AUTHOR:              Lee Mitchell
 *
 * DESCRIPTION:
 * Provides an API for access to functions to aid in the test of products
 * based upon Jennic wireless microcontroller IC's.
 *
 * LAST MODIFIED BY:    dcook
 *                      $Modtime: $
 *
 ****************************************************************************
 *
 * This software is owned by Jennic and/or its supplier and is protected
 * under applicable copyright laws. All rights are reserved. We grant You,
 * and any third parties, a license to use this software solely and
 * exclusively on Jennic products. You, and any third parties must reproduce
 * the copyright and warranty notice and any other legend of ownership on
 * each copy or partial copy of the software.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS". JENNIC MAKES NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
 * ACCURACY OR LACK OF NEGLIGENCE. JENNIC SHALL NOT, IN ANY CIRCUMSTANCES,
 * BE LIABLE FOR ANY DAMAGES, INCLUDING, BUT NOT LIMITED TO, SPECIAL,
 * INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER.
 *
 * Copyright Jennic Ltd 2005, 2006. All rights reserved
 *
 ***************************************************************************/

#ifndef  JPT_H_INCLUDED
#define  JPT_H_INCLUDED

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/

#include <jendefs.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/* Value enumerations: Radio Initialisation */
#define E_JPT_MODE_LOPOWER				0
#define E_JPT_MODE_BOOST				1
#define E_JPT_MODE_HIPOWER				2

/* Value enumerations: Xtal Oscillator Test */
#define E_JPT_XOT_DIO10					0
#define E_JPT_XOT_STOP					1

/* Value enumerations: Tx Power Test */
#define E_JPT_TXPT_RUN_CW				0
#define E_JPT_TXPT_RUN_PRBS				1
#define E_JPT_TXPT_STOP					2

/* Value enumerations: Site Survey Per Test */
#define E_JPT_SSPT_MODE_LOCATE			1
#define E_JPT_SSPT_MODE_STOPPED			2
#define E_JPT_SSPT_MODE_RUNNING_ACKS	3
#define E_JPT_SSPT_MODE_RUNNING_NO_ACKS	4
#define E_JPT_SSPT_MODE_RESTART			5

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef struct {
	uint8 u8Mode;
	uint8 u8Channel;
	uint8 u8Retries;
	uint32 u32Total;
	uint32 u32Seen;
	uint32 u32Errors;
} tsJPT_SSPT_MasterState;

typedef struct {
	uint8 u8Mode;
	uint8 u8Channel;
} tsJPT_SSPT_SlaveState;

typedef struct {
	bool_t bPacketGood;
	uint16 u16FrameControl;
	uint16 u16SourceShortAddress;
	uint16 u16DestinationShortAddress;
	uint64 u64SourceExtendedAddress;
	uint64 u64DestinationExtendedAddress;
	uint16 u16SourcePanID;
	uint16 u16DestinationPanID;
	uint8 u8PayloadLength;
	uint8 u8Payload[127] __attribute__ ((aligned (4)));
	uint8 u8Energy;
} tsJPT_PT_Packet ;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/* Misc functions */
PUBLIC uint32 u32JPT_Init(void);

/* Radio initialisation */
PUBLIC bool_t bJPT_RadioInit(uint32 u32RadioMode);
PUBLIC void vJPT_RadioDeInit(void);

/* Radio manipulation functions */
PUBLIC bool_t bJPT_RadioSetChannel(uint8 u8Channel);
PUBLIC void vJPT_RadioSetPower(uint8 u8PowerLevel);
PUBLIC void vJPT_RadioSetPowerFine(uint8 u8PowerLevel);

/* Energy detect functions */
PUBLIC uint8 u8JPT_EnergyDetect(uint8 u8Channel, uint32 u32Samples);
PUBLIC uint8 u8JPT_FineTuneEnergyDetect(uint32 u32Frequency, uint32 u32Samples);
PUBLIC int16 i16JPT_ConvertEnergyTodBm(uint8 u8Energy);

/* TX power test functions */
PUBLIC void vJPT_TxPowerTest(uint32 u32Mode);

/* Crystal oscillator test functions */
PUBLIC void vJPT_XtalOscillatorTest(uint32 u32Mode);

/* Module Test PER test functions */
PUBLIC void vJPT_MTPT_MasterStart(uint8 u8PayloadLength);
PUBLIC void vJPT_MTPT_MasterTxPacket(uint32 *pu32Acks);
PUBLIC void vJPT_MTPT_MasterStop(void);

PUBLIC void vJPT_MTPT_SlaveStart(void);
PUBLIC void vJPT_MTPT_SlavePoll(uint32 *pu32HeadersSeen, uint32 *pu32FramesSeen, uint32 *pu32ErrorsSeen);
PUBLIC void vJPT_MTPT_SlaveStop(void);

/* Trigger packet test functions */
PUBLIC void vJPT_TPT_Start(void);
PUBLIC uint32 u32JPT_TPT_WaitPacket(uint32 u32Timeout, uint32 *pu32Total,
									uint32 *pu32Seen, uint32 *pu32ChipErrors);
PUBLIC void vJPT_TPT_Stop(void);

/* Site Survey PER Test functions */
PUBLIC void vJPT_SSPT_MasterInit(void);
PUBLIC bool_t bJPT_SSPT_MasterSetState(tsJPT_SSPT_MasterState *psState);
PUBLIC void vJPT_SSPT_MasterGetState(tsJPT_SSPT_MasterState *psState);
PUBLIC void vJPT_SSPT_SlaveInit(void);
PUBLIC void vJPT_SSPT_SlaveGetState(tsJPT_SSPT_SlaveState *psState);

/* Packet RX / TX Test Functions */
PUBLIC bool_t bJPT_PacketRx(uint8 u8Channel, tsJPT_PT_Packet *psPacket);
PUBLIC void vJPT_PacketTx(uint8 u8Channel, tsJPT_PT_Packet *psPacket);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

extern uint64 u64JPT_MacAddress;


#if defined __cplusplus
}
#endif

#endif  /* JPT_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
