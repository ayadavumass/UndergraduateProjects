/****************************************************************************
 *
 * MODULE:             MAC
 *
 * COMPONENT:          PeripheralRegs.h,v
 *
 * VERSION:            Build_Release_030308_RC1
 *
 * REVISION:           1.1
 *
 * DATED:              2007/05/09 15:21:44
 *
 * STATUS:             Exp
 *
 * AUTHOR:             rcc
 *
 * DESCRIPTION:
 * Peripheral device register addresses
 *
 * LAST MODIFIED BY:   cjg
 *                     $Modtime$
 *
 *
 ****************************************************************************
 *
 *  (c) Copyright 2006, Jennic Limited
 *
 ****************************************************************************/

#ifndef _periph_reg_h_
#define _periph_reg_h_

#ifdef __cplusplus
extern "C" {
#endif

/***********************/
/**** INCLUDE FILES ****/
/***********************/

/************************/
/**** MACROS/DEFINES ****/
/************************/

#ifndef BIT_W_1
#define BIT_W_1                         0x1
#define BIT_W_2                         0x3
#define BIT_W_3                         0x7
#define BIT_W_4                         0xF
#define BIT_W_5                         0x1F
#define BIT_W_6                         0x3F
#define BIT_W_7                         0x7F
#define BIT_W_8                         0xFF
#define BIT_W_12                        0xFFF
#define BIT_W_16                        0xFFFF
#define BIT_W_21                        0x1FFFFF
#endif

/**** System controller: PWR_CTRL ****/
#define REG_SYSCTRL_PWRCTRL_PDR_BIT       0
#define REG_SYSCTRL_PWRCTRL_PDR_MASK      ((uint32)(BIT_W_1 << REG_SYSCTRL_PWRCTRL_PDR_BIT))
#define REG_SYSCTRL_PWRCTRL_DSS_BIT       1
#define REG_SYSCTRL_PWRCTRL_DSS_MASK      ((uint32)(BIT_W_1 << REG_SYSCTRL_PWRCTRL_DSS_BIT))
#define REG_SYSCTRL_PWRCTRL_PPDC_BIT      3
#define REG_SYSCTRL_PWRCTRL_PPDC_MASK     ((uint32)(BIT_W_1 << REG_SYSCTRL_PWRCTRL_PPDC_BIT))
#define REG_SYSCTRL_PWRCTRL_MHUP_BIT      5
#define REG_SYSCTRL_PWRCTRL_MHUP_MASK     ((uint32)(BIT_W_1 << REG_SYSCTRL_PWRCTRL_MHUP_BIT))
#define REG_SYSCTRL_PWRCTRL_MS_BIT        10
#define REG_SYSCTRL_PWRCTRL_MS_MASK       ((uint32)(BIT_W_2 << REG_SYSCTRL_PWRCTRL_MS_BIT))
#define REG_SYSCTRL_PWRCTRL_UEN0_BIT      12
#define REG_SYSCTRL_PWRCTRL_T0EN_BIT      14
#define REG_SYSCTRL_PWRCTRL_SIFEN_BIT     16
#define REG_SYSCTRL_PWRCTRL_SIFEN_MASK    ((uint32)(BIT_W_1 << REG_SYSCTRL_PWRCTRL_SIFEN_BIT))
#define REG_SYSCTRL_PWRCTRL_NASSEN_BIT    17
#define REG_SYSCTRL_PWRCTRL_NASSEN_MASK   ((uint32)(BIT_W_3 << REG_SYSCTRL_PWRCTRL_NASSEN_BIT))
#define REG_SYSCTRL_PWRCTRL_APDLY_BIT     20
#define REG_SYSCTRL_PWRCTRL_APDLY_MASK    ((uint32)(BIT_W_2 << REG_SYSCTRL_PWRCTRL_APDLY_BIT))
#define REG_SYSCTRL_PWRCTRL_APEN_BIT      22
#define REG_SYSCTRL_PWRCTRL_APEN_MASK     ((uint32)(BIT_W_1 << REG_SYSCTRL_PWRCTRL_APEN_BIT))
#define REG_SYSCTRL_PWRCTRL_RFRXEN_BIT    24
#define REG_SYSCTRL_PWRCTRL_RFRXEN_MASK   ((uint32)(BIT_W_1 << REG_SYSCTRL_PWRCTRL_RFRXEN_BIT))
#define REG_SYSCTRL_PWRCTRL_RFTXEN_BIT    25
#define REG_SYSCTRL_PWRCTRL_RFTXEN_MASK   ((uint32)(BIT_W_1 << REG_SYSCTRL_PWRCTRL_RFTXEN_BIT))
#define REG_SYSCTRL_PWRCTRL_UARTFLOWEN_BIT 27
#define REG_SYSCTRL_PWRCTRL_IODISABLE_BIT  29

/**** System controller: Status register ****/
#define REG_SYSCTRL_STAT_WUS_BIT        0
#define REG_SYSCTRL_STAT_WUS_MASK       ((uint32)(BIT_W_1 << REG_SYSCTRL_STAT_WUS_BIT))
#define REG_SYSCTRL_STAT_MS_BIT         1
#define REG_SYSCTRL_STAT_MS_MASK        ((uint32)(BIT_W_1 << REG_SYSCTRL_STAT_MS_BIT))
#define REG_SYSCTRL_STAT_APPS_BIT       2
#define REG_SYSCTRL_STAT_APPS_MASK      ((uint32)(BIT_W_1 << REG_SYSCTRL_STAT_APPS_BIT))
#define REG_SYSCTRL_STAT_PROPS_BIT      3
#define REG_SYSCTRL_STAT_PROPS_MASK     ((uint32)(BIT_W_1 << REG_SYSCTRL_STAT_PROPS_BIT))

/**** System controller: Wake event registers ****/
#define REG_SYSCTRL_WKE_GE_BIT          0
#define REG_SYSCTRL_WKE_GE_MASK         ((uint32)(BIT_W_21 << REG_SYSCTRL_WKE_GE_BIT))
#define REG_SYSCTRL_WKE_WTE_BIT         26
#define REG_SYSCTRL_WKE_WTE_MASK        ((uint32)(BIT_W_2 << REG_SYSCTRL_WKE_WTE_BIT))
#define REG_SYSCTRL_WKE_CTRT_BIT        28
#define REG_SYSCTRL_WKE_CE_BIT          28
#define REG_SYSCTRL_WKE_CE_MASK         ((uint32)(BIT_W_2 << REG_SYSCTRL_WKE_CE_BIT))

/**** System controller: Reset registers ****/
#define REG_SYSCTRL_RSTCTRL_CG_BIT      0
#define REG_SYSCTRL_RSTCTRL_CG_MASK     ((uint32)(BIT_W_1 << REG_SYSCTRL_RSTCTRL_CG_BIT))
#define REG_SYSCTRL_RSTCTRL_CR_BIT      1
#define REG_SYSCTRL_RSTCTRL_CR_MASK     ((uint32)(BIT_W_1 << REG_SYSCTRL_RSTCTRL_CR_BIT))

/**** UART: IER_DLM ****/
#define REG_UART_IERDLM_ERBFI_BIT       0
#define REG_UART_IERDLM_ERBFI_MASK      ((uint8)(BIT_W_1 << REG_UART_IERDLM_ERBFI_BIT))
#define REG_UART_IERDLM_ETBEI_BIT       1
#define REG_UART_IERDLM_ETBEI_MASK      ((uint8)(BIT_W_1 << REG_UART_IERDLM_ETBEI_BIT))
#define REG_UART_IERDLM_ELSI_BIT        2
#define REG_UART_IERDLM_ELSI_MASK       ((uint8)(BIT_W_1 << REG_UART_IERDLM_ELSI_BIT))
#define REG_UART_IERDLM_EDSSI_BIT       3
#define REG_UART_IERDLM_EDSSI_MASK      ((uint8)(BIT_W_1 << REG_UART_IERDLM_EDSSI_BIT))

/**** UART: FCR ****/
#define REG_UART_FCR_RFRST_BIT          1
#define REG_UART_FCR_RFRST_MASK         ((uint8)(BIT_W_1 << REG_UART_FCR_RFRST_BIT))
#define REG_UART_FCR_TFRST_BIT          2
#define REG_UART_FCR_TFRST_MASK         ((uint8)(BIT_W_1 << REG_UART_FCR_TFRST_BIT))
#define REG_UART_FCR_RINTRIG_BIT        6
#define REG_UART_FCR_RINTRIG_MASK       ((uint8)(BIT_W_2 << REG_UART_FCR_RINTRIG_BIT))

/**** UART: LCR ****/
#define REG_UART_LCR_WLS_BIT            0
#define REG_UART_LCR_WLS_MASK           ((uint8)(BIT_W_2 << REG_UART_LCR_WLS_BIT))
#define REG_UART_LCR_STB_BIT            2
#define REG_UART_LCR_STB_MASK           ((uint8)(BIT_W_1 << REG_UART_LCR_STB_BIT))
#define REG_UART_LCR_PEN_BIT            3
#define REG_UART_LCR_PEN_MASK           ((uint8)(BIT_W_1 << REG_UART_LCR_PEN_BIT))
#define REG_UART_LCR_EPS_BIT            4
#define REG_UART_LCR_EPS_MASK           ((uint8)(BIT_W_1 << REG_UART_LCR_EPS_BIT))
#define REG_UART_LCR_DLAB_BIT           7
#define REG_UART_LCR_DLAB_MASK          ((uint8)(BIT_W_1 << REG_UART_LCR_DLAB_BIT))

/**** UART: MCR ****/
#define REG_UART_MCR_RTS_BIT            1
#define REG_UART_MCR_RTS_MASK           ((uint8)(BIT_W_1 << REG_UART_MCR_RTS_BIT))

/**** Timer: T_CTRL ****/
#define REG_TMR_CTRL_EN_BIT             0
#define REG_TMR_CTRL_EN_MASK            ((uint16)(BIT_W_1 << REG_TMR_CTRL_EN_BIT))
#define REG_TMR_CTRL_EEVENT_BIT         1
#define REG_TMR_CTRL_EEVENT_MASK        ((uint16)(BIT_W_1 << REG_TMR_CTRL_EEVENT_BIT))
#define REG_TMR_CTRL_INVCKGT_BIT        2
#define REG_TMR_CTRL_INVCKGT_MASK       ((uint16)(BIT_W_1 << REG_TMR_CTRL_INVCKGT_BIT))
#define REG_TMR_CTRL_OE_BIT             3
#define REG_TMR_CTRL_OE_MASK            ((uint16)(BIT_W_1 << REG_TMR_CTRL_OE_BIT))
#define REG_TMR_CTRL_SINGLE_BIT         4
#define REG_TMR_CTRL_SINGLE_MASK        ((uint16)(BIT_W_1 << REG_TMR_CTRL_SINGLE_BIT))
#define REG_TMR_CTRL_CTRRST_BIT         5
#define REG_TMR_CTRL_CTRRST_MASK        ((uint16)(BIT_W_1 << REG_TMR_CTRL_CTRRST_BIT))
#define REG_TMR_CTRL_CAPTE_BIT          6
#define REG_TMR_CTRL_CAPTE_MASK         ((uint16)(BIT_W_1 << REG_TMR_CTRL_CAPTE_BIT))
#define REG_TMR_CTRL_DSEN_BIT           7
#define REG_TMR_CTRL_DSEN_MASK          ((uint16)(BIT_W_1 << REG_TMR_CTRL_DSEN_BIT))
#define REG_TMR_CTRL_RZEN_BIT           8
#define REG_TMR_CTRL_RZEN_MASK          ((uint16)(BIT_W_1 << REG_TMR_CTRL_RZEN_BIT))
#define REG_TMR_CTRL_GDIS_BIT           9
#define REG_TMR_CTRL_GDIS_MASK          ((uint16)(BIT_W_1 << REG_TMR_CTRL_GDIS_BIT))
#define REG_TMR_CTRL_INVOUT_BIT         10
#define REG_TMR_CTRL_INVOUT_MASK        ((uint16)(BIT_W_1 << REG_TMR_CTRL_INVOUT_BIT))
#define REG_TMR_CTRL_INVCAPT_BIT        11
#define REG_TMR_CTRL_INVCAPT_MASK       ((uint16)(BIT_W_1 << REG_TMR_CTRL_INVCAPT_BIT))
#define REG_TMR_CTRL_EEDGE_BIT          12
#define REG_TMR_CTRL_EEDGE_MASK         ((uint16)(BIT_W_1 << REG_TMR_CTRL_EEDGE_BIT))

/**** Timer: T_INT and T_INTE ****/
#define REG_TMR_INT_P_EN_BIT            0
#define REG_TMR_INT_P_EN_MASK           ((uint8)(BIT_W_1 << REG_TMR_INT_P_EN_BIT))
#define REG_TMR_INT_L_EN_BIT            1
#define REG_TMR_INT_L_EN_MASK           ((uint8)(BIT_W_1 << REG_TMR_INT_L_EN_BIT))

/**** SPI master: SPIM_CTRL ****/
#define REG_SPIM_CTRL_GOBSY_BIT         0
#define REG_SPIM_CTRL_GOBSY_MASK        ((uint16)(BIT_W_1 << REG_SPIM_CTRL_GOBSY_BIT))
#define REG_SPIM_CTRL_RXNEG_BIT         1
#define REG_SPIM_CTRL_RXNEG_MASK        ((uint16)(BIT_W_1 << REG_SPIM_CTRL_RXNEG_BIT))
#define REG_SPIM_CTRL_TXNEG_BIT         2
#define REG_SPIM_CTRL_TXNEG_MASK        ((uint16)(BIT_W_1 << REG_SPIM_CTRL_TXNEG_BIT))
#define REG_SPIM_CTRL_CHARLEN_BIT       3
#define REG_SPIM_CTRL_CHARLEN_MASK      ((uint16)(BIT_W_7 << REG_SPIM_CTRL_CHARLEN_BIT))
#define REG_SPIM_CTRL_LSB_BIT           10
#define REG_SPIM_CTRL_LSB_MASK          ((uint16)(BIT_W_1 << REG_SPIM_CTRL_LSB_BIT))
#define REG_SPIM_CTRL_ASS_BIT           12
#define REG_SPIM_CTRL_ASS_MASK          ((uint16)(BIT_W_1 << REG_SPIM_CTRL_ASS_BIT))

/**** Analogue: AP_CTRL ****/
#define REG_ANPER_CTRL_BATT_EN_BIT      25
#define REG_ANPER_CTRL_BATT_EN_MASK     ((uint32)(BIT_W_1 << REG_ANPER_CTRL_BATT_EN_BIT))
#define REG_ANPER_CTRL_TEMP_EN_BIT      24
#define REG_ANPER_CTRL_TEMP_EN_MASK     ((uint32)(BIT_W_1 << REG_ANPER_CTRL_TEMP_EN_BIT))
#define REG_ANPER_CTRL_RET_DAC_OUT2_BIT 23
#define REG_ANPER_CTRL_RET_DAC_OUT2_MASK ((uint32)(BIT_W_1 << REG_ANPER_CTRL_RET_DAC_OUT2_BIT))
#define REG_ANPER_CTRL_RET_DAC_OUT1_BIT 22
#define REG_ANPER_CTRL_RET_DAC_OUT1_MASK ((uint32)(BIT_W_1 << REG_ANPER_CTRL_RET_DAC_OUT1_BIT))
#define REG_ANPER_CTRL_SAMPLE_PERIOD_BIT 20
#define REG_ANPER_CTRL_SAMPLE_PERIOD_MASK ((uint32)(BIT_W_2 << REG_ANPER_CTRL_SAMPLE_PERIOD_BIT))
#define REG_ANPER_CTRL_CLOCK_DIV_BIT    18
#define REG_ANPER_CTRL_CLOCK_DIV_MASK   ((uint32)(BIT_W_2 << REG_ANPER_CTRL_CLOCK_DIV_BIT))
#define REG_ANPER_CTRL_CONVERT_BIT      16
#define REG_ANPER_CTRL_CONVERT_MASK     ((uint32)(BIT_W_1 << REG_ANPER_CTRL_CONVERT_BIT))
#define REG_ANPER_CTRL_D2EN_BIT         13
#define REG_ANPER_CTRL_D2EN_MASK        ((uint32)(BIT_W_1 << REG_ANPER_CTRL_D2EN_BIT))
#define REG_ANPER_CTRL_D1EN_BIT         12
#define REG_ANPER_CTRL_D1EN_MASK        ((uint32)(BIT_W_1 << REG_ANPER_CTRL_D1EN_BIT))
#define REG_ANPER_CTRL_AEN_BIT          11
#define REG_ANPER_CTRL_AEN_MASK         ((uint32)(BIT_W_1 << REG_ANPER_CTRL_AEN_BIT))
#define REG_ANPER_CTRL_ADCSEL_BIT       4
#define REG_ANPER_CTRL_ADCSEL_MASK      ((uint32)(BIT_W_3 << REG_ANPER_CTRL_ADCSEL_BIT))
#define REG_ANPER_CTRL_ADC_GAIN_BIT     3
#define REG_ANPER_CTRL_ADC_GAIN_MASK    ((uint32)(BIT_W_1 << REG_ANPER_CTRL_ADC_GAIN_BIT))
#define REG_ANPER_CTRL_DAC2_GAIN_BIT    2
#define REG_ANPER_CTRL_DAC2_GAIN_MASK   ((uint32)(BIT_W_1 << REG_ANPER_CTRL_DAC2_GAIN_BIT))
#define REG_ANPER_CTRL_DAC1_GAIN_BIT    1
#define REG_ANPER_CTRL_DAC1_GAIN_MASK   ((uint32)(BIT_W_1 << REG_ANPER_CTRL_DAC1_GAIN_BIT))
#define REG_ANPER_CTRL_REF_SEL_BIT      0
#define REG_ANPER_CTRL_REF_SEL_MASK     ((uint32)(BIT_W_1 << REG_ANPER_CTRL_REF_SEL_BIT))

/**** Analogue: AP_VAL ****/
#define REG_ANPER_VAL_C2_BIT            13
#define REG_ANPER_VAL_C2_MASK           ((uint32)(BIT_W_1 << REG_ANPER_VAL_C2_BIT))
#define REG_ANPER_VAL_C1_BIT            12
#define REG_ANPER_VAL_C1_MASK           ((uint32)(BIT_W_1 << REG_ANPER_VAL_C1_BIT))
#define REG_ANPER_VAL_ADC_BIT           0
#define REG_ANPER_VAL_ADC_MASK          ((uint32)(BIT_W_12 << REG_ANPER_VAL_ADC_BIT))

/**** Analogue: AP_COMPCTRL ****/
#define REG_ANPER_COMPCTRL_BCTRL_BIT    8
#define REG_ANPER_COMPCTRL_BCTRL_MASK   ((uint32)(BIT_W_1 << REG_ANPER_COMPCTRL_BCTRL_BIT))
#define REG_ANPER_COMPCTRL_HYS_BIT      6
#define REG_ANPER_COMPCTRL_HYS_MASK     ((uint32)(BIT_W_2 << REG_ANPER_COMPCTRL_HYS_BIT))
#define REG_ANPER_COMPCTRL_C2SEL_BIT    4
#define REG_ANPER_COMPCTRL_C2SEL_MASK   ((uint32)(BIT_W_2 << REG_ANPER_COMPCTRL_C2SEL_BIT))
#define REG_ANPER_COMPCTRL_C1SEL_BIT    2
#define REG_ANPER_COMPCTRL_C1SEL_MASK   ((uint32)(BIT_W_2 << REG_ANPER_COMPCTRL_C1SEL_BIT))
#define REG_ANPER_COMPCTRL_C2EN_BIT     1
#define REG_ANPER_COMPCTRL_C2EN_MASK    ((uint32)(BIT_W_1 << REG_ANPER_COMPCTRL_C2EN_BIT))
#define REG_ANPER_COMPCTRL_C1EN_BIT     0
#define REG_ANPER_COMPCTRL_C1EN_MASK    ((uint32)(BIT_W_1 << REG_ANPER_COMPCTRL_C1EN_BIT))

/**** Serial Interface: SI_CTRL ****/
#define REG_SI_CTRL_EN_BIT              7
#define REG_SI_CTRL_EN_MASK             ((uint32)(BIT_W_1 << REG_SI_CTRL_EN_BIT))
#define REG_SI_CTRL_IE_BIT              6
#define REG_SI_CTRL_IE_MASK             ((uint32)(BIT_W_1 << REG_SI_CTRL_IE_BIT))

/**** Serial Interface: SI_CMD ****/
#define REG_SI_CMD_STA_BIT              7
#define REG_SI_CMD_STA_MASK             ((uint32)(BIT_W_1 << REG_SI_CMD_STA_BIT))
#define REG_SI_CMD_STO_BIT              6
#define REG_SI_CMD_STO_MASK             ((uint32)(BIT_W_1 << REG_SI_CMD_STO_BIT))
#define REG_SI_CMD_RD_BIT               5
#define REG_SI_CMD_RD_MASK              ((uint32)(BIT_W_1 << REG_SI_CMD_RD_BIT))
#define REG_SI_CMD_WR_BIT               4
#define REG_SI_CMD_WR_MASK              ((uint32)(BIT_W_1 << REG_SI_CMD_WR_BIT))
#define REG_SI_CMD_ACK_BIT              3
#define REG_SI_CMD_ACK_MASK             ((uint32)(BIT_W_1 << REG_SI_CMD_ACK_BIT))
#define REG_SI_CMD_IACK_BIT             0
#define REG_SI_CMD_IACK_MASK            ((uint32)(BIT_W_1 << REG_SI_CMD_IACK_BIT))

/**** Serial Interface: SI_STAT ****/
#define REG_SI_STAT_RXACK_BIT           7
#define REG_SI_STAT_RXACK_MASK          ((uint32)(BIT_W_1 << REG_SI_STAT_RXACK_BIT))
#define REG_SI_STAT_BUSY_BIT            6
#define REG_SI_STAT_BUSY_MASK           ((uint32)(BIT_W_1 << REG_SI_STAT_BUSY_BIT))
#define REG_SI_STAT_AL_BIT              5
#define REG_SI_STAT_AL_MASK             ((uint32)(BIT_W_1 << REG_SI_STAT_AL_BIT))
#define REG_SI_STAT_TIP_BIT             1
#define REG_SI_STAT_TIP_MASK            ((uint32)(BIT_W_1 << REG_SI_STAT_TIP_BIT))
#define REG_SI_STAT_INT_BIT             0
#define REG_SI_STAT_INT_MASK            ((uint32)(BIT_W_1 << REG_SI_STAT_INT_BIT))

/**** Intelligent Peripheral: INTPER_CTRL ****/
#define REG_INTPER_CTRL_IE_BIT          7
#define REG_INTPER_CTRL_IE_MASK         ((uint32)(BIT_W_1 << REG_INTPER_CTRL_IE_BIT))
#define REG_INTPER_CTRL_IS_BIT          6
#define REG_INTPER_CTRL_IS_MASK         ((uint32)(BIT_W_1 << REG_INTPER_CTRL_IS_BIT))
#define REG_INTPER_CTRL_TX_NEG_BIT      5
#define REG_INTPER_CTRL_TX_NEG_MASK     ((uint32)(BIT_W_1 << REG_INTPER_CTRL_TX_NEG_BIT))
#define REG_INTPER_CTRL_RX_NEG_BIT      4
#define REG_INTPER_CTRL_RX_NEG_MASK     ((uint32)(BIT_W_1 << REG_INTPER_CTRL_RX_NEG_BIT))
#define REG_INTPER_CTRL_TXQ_BIT         3
#define REG_INTPER_CTRL_TXQ_MASK        ((uint32)(BIT_W_1 << REG_INTPER_CTRL_TXQ_BIT))
#define REG_INTPER_CTRL_RXQ_BIT         2
#define REG_INTPER_CTRL_RXQ_MASK        ((uint32)(BIT_W_1 << REG_INTPER_CTRL_RXQ_BIT))
#define REG_INTPER_CTRL_TXGO_BIT        1
#define REG_INTPER_CTRL_TXGO_MASK       ((uint32)(BIT_W_1 << REG_INTPER_CTRL_TXGO_BIT))
#define REG_INTPER_CTRL_RXGO_BIT        0
#define REG_INTPER_CTRL_RXGO_MASK       ((uint32)(BIT_W_1 << REG_INTPER_CTRL_RXGO_BIT))

/**** Intelligent Peripheral: RXADDR ****/
#define REG_INTPER_RXADDR_BIT           0
#define REG_INTPER_RXADDR_MASK          ((uint32)(BIT_W_1 << REG_INTPER_RXADDR_BIT))

/**** Intelligent Peripheral: TXADDR ****/
#define REG_INTPER_TXADDR_BIT           0
#define REG_INTPER_TXADDR_MASK          ((uint32)(BIT_W_1 << REG_INTPER_TXADDR_BIT))

/**** Intelligent Peripheral: RXLEN ****/
#define REG_INTPER_RXLEN_BIT            0
#define REG_INTPER_RXLEN_MASK           ((uint32)(BIT_W_6 << REG_INTPER_RXLEN_BIT))

/**** Intelligent Peripheral: TXLEN ****/
#define REG_INTPER_TXLEN_BIT            0
#define REG_INTPER_TXLEN_MASK           ((uint32)(BIT_W_6 << REG_INTPER_TXLEN_BIT))

/**** AES ****/
#define AES_COMMAND_SHIFT               4
#define REG_AES_SET_MODE_CMD            ((0 << AES_COMMAND_SHIFT) | 1)
#define REG_AES_SET_KEY_CMD             ((1 << AES_COMMAND_SHIFT) | 1)
#define REG_AES_LOAD_CTR_CMD            ((3 << AES_COMMAND_SHIFT) | 1)
#define REG_AES_LOAD_IV_CMD             ((4 << AES_COMMAND_SHIFT) | 1)
#define REG_AES_GO_CMD                  ((5 << AES_COMMAND_SHIFT) | 1)
#define REG_AES_LOAD_ALENGTH_CMD        ((6 << AES_COMMAND_SHIFT) | 1)
#define REG_AES_LOAD_MLENGTH_CMD        ((7 << AES_COMMAND_SHIFT) | 1)
#define REG_AES_LOAD_ML_CMD             ((8 << AES_COMMAND_SHIFT) | 1)
#define REG_AES_LOAD_NONCE_CMD          ((9 << AES_COMMAND_SHIFT) | 1)
#define REG_AES_INIT_CCM_AUTH_CMD       ((10 << AES_COMMAND_SHIFT) | 1)
#define REG_AES_GEN_CCM_AUTH_CMD        ((11 << AES_COMMAND_SHIFT) | 1)
#define REG_AES_ACL_CMD_DONE_MASK       (1)


/**** CPU CTRL******/
#define REG_CPUCTRL_DOZE_BIT            0
#define REG_CPUCTRL_CLKDOUBLER_BIT      1

/**** GEN CTRL******/
#define REG_GENCTRL_EXT32KSRC_BIT       1

/**************************/
/**** TYPE DEFINITIONS ****/
/**************************/
typedef enum
{
    /* System controller: not a full list */
    REG_SYS_PWR_CTRL      = 0,
    REG_SYS_STAT          = 1,
    REG_SYS_WK_ET         = 2,
    REG_SYS_WK_EM         = 3,
    REG_SYS_WK_ES         = 4,
    REG_SYS_WK_T0         = 5,
    REG_SYS_WK_T1         = 6,
    REG_SYS_WK_TC         = 7,
    REG_SYS_WK_TS         = 8,
    REG_SYS_WK_CC         = 9,
    REG_SYS_WK_CRC        = 10,
    REG_SYS_CPU_CTRL      = 11,
    REG_SYS_GEN_CTRL      = 14,
    REG_SYS_PULLUP        = 15,
    REG_SYS_VBOCTRL       = 17,
    REG_SYS_RSTCTRL       = 19,
    REG_SYS_CHIP_ID       = 63,

    /* AES */
    REG_AES_ACLKEY        = 0, /* 4 words in size, 128-bit key */
    REG_AES_ACL_CMD       = 5,
    REG_AES_DATA_IN       = 0,
    REG_AES_DATA_OUT      = 7,
    REG_AES_IV            = 0,
    REG_AES_CTR           = 0,
    REG_AES_RI_CCM_ML     = 3,
    REG_AES_RI_CCM_ALEN_0 = 2,
    REG_AES_RI_CCM_ALEN_1 = 3,
    REG_AES_RI_CCM_MLEN_0 = 2,
    REG_AES_RI_CCM_MLEN_1 = 3,
    REG_AES_BCMODE        = 3,
    REG_AES_RI_CCM_NONCE  = 0,
    REG_AES_CCM_AUTH      = 0,
    REG_AES_STATUS        = 15,
    REG_AES_CCM_CHK_0     = 11,
    REG_AES_CCM_CHK_1     = 12,
    REG_AES_CCM_CHK_2     = 13,
    REG_AES_CCM_CHK_3     = 14,
    REG_AES_INT           = 4,
    REG_AES_INT_EN        = 6,

    /* GPIO */
    REG_GPIO_DIR          = 0,
    REG_GPIO_DOUT         = 1,
    REG_GPIO_DIN          = 2,

    /* UART: Repeated for each one */
    REG_UART_DATA         = 0,
    REG_UART_IER          = 1,
    REG_UART_IIR          = 2, /* Read only */
    REG_UART_FCR          = 2, /* Write only */
    REG_UART_LCR          = 3,
    REG_UART_MCR          = 4,
    REG_UART_LSR          = 5,
    REG_UART_MSR          = 6,

    /* Timer: Repeated for each one */
    REG_TMR_CTR           = 0,
    REG_TMR_HI            = 1,
    REG_TMR_LO            = 2,
    REG_TMR_CTRL          = 3,
    REG_TMR_PRESC         = 4,
    REG_TMR_INT           = 5,
    REG_TMR_INTE          = 6,

    /* Serial Interface (2 wire) */
    REG_SI_PS_L           = 0,
    REG_SI_PS_H           = 1,
    REG_SI_CTRL           = 2,
    REG_SI_TX             = 3,
    REG_SI_RX             = 3,
    REG_SI_CMD            = 4,
    REG_SI_STAT           = 4,

    /* SPI master */
    REG_SPIM_BUF          = 0,
    REG_SPIM_CTRL         = 1,
    REG_SPIM_DIV          = 2,
    REG_SPIM_SS           = 3,
    REG_SPIM_IE           = 4,
    REG_SPIM_IS           = 5,
    REG_SPIM_SCRAM        = 6,

    /* Intelligent peripheral (SPI-IP) */
    REG_INTPER_RXADDR     = 0,
    REG_INTPER_TXADDR     = 1,
    REG_INTPER_RXLEN      = 2,
    REG_INTPER_TXLEN      = 3,
    REG_INTPER_CTRL       = 4,

    REG_INTPER_LOBLK      = 0,
    REG_INTPER_HIBLK      = 64,

    /* Analogue peripherals */
    REG_ANPER_CTRL        = 0,
    REG_ANPER_VAL         = 1,
    REG_ANPER_DAC1        = 2,
    REG_ANPER_DAC2        = 3,
    REG_ANPER_CAPT        = 4,
    REG_ANPER_IS          = 5,
    REG_ANPER_IE          = 6,
    REG_ANPER_TEST        = 7,
    REG_ANPER_COMPCTRL    = 8
} tePeriphReg;

#define u32REG_SysRead(eOffset)                 apvSysReg[eOffset]
#define vREG_SysWrite(eOffset, u32Data)         apvSysReg[eOffset] = (u32Data)
#define u32REG_AesRead(eOffset)                 apvAesReg[eOffset]
#define vREG_AesWrite(eOffset, u32Data)         apvAesReg[eOffset] = (u32Data)
#define u32REG_GpioRead(eOffset)                apvGpioReg[eOffset]
#define vREG_GpioWrite(eOffset, u32Data)        apvGpioReg[eOffset] = (u32Data)
#define u32REG_Uart0Read(eOffset)               apvUart0Reg[eOffset]
#define vREG_Uart0Write(eOffset, u32Data)       apvUart0Reg[eOffset] = (u32Data)
#define u32REG_Uart1Read(eOffset)               apvUart1Reg[eOffset]
#define vREG_Uart1Write(eOffset, u32Data)       apvUart1Reg[eOffset] = (u32Data)
#define u32REG_Timer0Read(eOffset)              apvTimer0Reg[eOffset]
#define vREG_Timer0Write(eOffset, u32Data)      apvTimer0Reg[eOffset] = (u32Data)
#define u32REG_Timer1Read(eOffset)              apvTimer1Reg[eOffset]
#define vREG_Timer1Write(eOffset, u32Data)      apvTimer1Reg[eOffset] = (u32Data)
#define u32REG_SiRead(eOffset)                  apvSiReg[eOffset]
#define vREG_SiWrite(eOffset, u32Data)          apvSiReg[eOffset] = (u32Data)
#define u32REG_SpiRead(eOffset)                 apvSpiReg[eOffset]
#define vREG_SpiWrite(eOffset, u32Data)         apvSpiReg[eOffset] = (u32Data)
#define u32REG_SpiIpRead(eOffset)               apvSpiIpReg[eOffset]
#define vREG_SpiIpWrite(eOffset, u32Data)       apvSpiIpReg[eOffset] = (u32Data)
#define u32REG_SpiIpMemRead(eOffset)            apvSpiIpMem[eOffset]
#define vREG_SpiIpMemWrite(eOffset, u32Data)    apvSpiIpMem[eOffset] = (u32Data)
#define u32REG_AnaRead(eOffset)                 apvAnaReg[eOffset]
#define vREG_AnaWrite(eOffset, u32Data)         apvAnaReg[eOffset] = (u32Data)

/****************************/
/**** EXPORTED VARIABLES ****/
/****************************/
extern volatile uint32 apvSysReg[];
extern volatile uint32 apvAesReg[];
extern volatile uint32 apvGpioReg[];
extern volatile uint32 apvUart0Reg[];
extern volatile uint32 apvUart1Reg[];
extern volatile uint32 apvTimer0Reg[];
extern volatile uint32 apvTimer1Reg[];
extern volatile uint32 apvSiReg[];
extern volatile uint32 apvSpiReg[];
extern volatile uint32 apvSpiIpReg[];
extern volatile uint32 apvSpiIpMem[];
extern volatile uint32 apvAnaReg[];

/****************************/
/**** EXPORTED FUNCTIONS ****/
/****************************/

#ifdef __cplusplus
};
#endif

#endif /* _periph_reg_h_ */
