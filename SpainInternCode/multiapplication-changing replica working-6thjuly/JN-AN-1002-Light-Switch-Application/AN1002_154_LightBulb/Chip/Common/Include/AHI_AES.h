/*****************************************************************************
 *
 * MODULE:              AHI AES engine
 *
 * COMPONENT:           AHI_AES.h,v
 *
 * VERSION:             Build_Release_030308_RC1
 *
 * REVISION:            1.1
 *
 * DATED:               2007/02/22 15:45:56
 *
 * STATUS:              Exp
 *
 * AUTHOR:              Wayne Ellis
 *
 * DESCRIPTION:
 * AES coprocessor API bit masks, defines and function prototypes.
 *
 * LAST MODIFIED BY:    cjg
 *                      $Modtime: $
 *
 ****************************************************************************
 *
 *  (c) Copyright JENNIC Ltd 2006
 *
 ****************************************************************************/

#ifndef  AHI_AES_H_INCLUDED
#define  AHI_AES_H_INCLUDED

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

/* CCM parameter limits */
#define M_MAX       (16)
#define ALENGTH_MAX (31)
#define MLENGTH_MAX (127)

/* AES Encryption Modes */
#define XCV_REG_AES_SET_MODE_ECB          (0)
#define XCV_REG_AES_SET_MODE_CTR          (2)
#define XCV_REG_AES_SET_MODE_CCM          (3)
#define XCV_REG_AES_SET_MODE_CCM_D        (4)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/* 128-bit data and config data structure */
typedef struct
{
    uint32 u32register0;
    uint32 u32register1;
    uint32 u32register2;
    uint32 u32register3;
} tsReg128;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/* ECB Encoding API Calls */
PUBLIC bool_t bACI_ECBencodeBuffer(tsReg128 *psKeyData,
                                   bool_t    bLoadKey,
                                   uint32    u32stripes,
                                   uint8    *pau8inputData,
                                   uint8    *pau8outputData,
                                   uint32   *pu32stripesComplete);
PUBLIC bool_t bACI_ECBencodeStripe(tsReg128 *psKeyData,
                                   bool_t    bLoadKey,
                                   tsReg128 *psInputData,
                                   tsReg128 *psOutputData);

/* CCM* Encode/Decode API Calls */
/* full CCM operation with authentication and Encryption/Decryption */
PUBLIC bool_t bACI_CCMstar(tsReg128 *psKeyData,
                           bool_t    bLoadKey,
                           uint8     u8AESmode,
                           uint8     u8M,
                           uint8     u8alength,
                           uint8     u8mlength,
                           tsReg128 *psNonce,
                           uint8    *pau8authenticationData,
                           uint8    *pau8inputData,
                           uint8    *pau8outputData,
                           uint8    *pau8checksumData,
                           bool_t   *pbChecksumVerify);

/* CCM* Data Authentication only */
PUBLIC bool_t bACI_CCMstarAuthenticateData(tsReg128 *psKeyData,
                                           bool_t    bLoadKey,
                                           uint8     u8AESmode,
                                           uint8     u8M,
                                           uint8     u8alength,
                                           tsReg128 *psNonce,
                                           uint8    *pau8authenticationData,
                                           uint8    *pau8checksumData,
                                           bool_t   *pbChecksumVerify);

/* CCM* Data Encode Decode Only */
PUBLIC bool_t bACI_CCMstarEncodeData(tsReg128 *psKeyData,
                                     bool_t    bLoadKey,
                                     uint8     u8mlength,
                                     tsReg128 *psNonce,
                                     uint8    *pau8inputData,
                                     uint8    *pau8outputData);

/* IEEE CTR Data Encode Decode Only */
PUBLIC bool_t bACI_IEEECTRencodeData(tsReg128 *psKeyData,
                                     bool_t    bLoadKey,
                                     uint8     u8mlength,
                                     tsReg128 *psNonce,
                                     uint8    *pau8inputData,
                                     uint8    *pau8outputData);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* AHI_AES_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
