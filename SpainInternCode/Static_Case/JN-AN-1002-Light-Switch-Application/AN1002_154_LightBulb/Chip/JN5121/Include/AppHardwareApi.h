/*****************************************************************************
 *
 * MODULE:              Application Hardware API
 *
 * COMPONENT:           AppHardwareApi.h,v
 *
 * VERSION:             Build_Release_030308_RC1
 *
 * REVISION:            1.5
 *
 * DATED:               2007/10/12 13:06:41
 *
 * STATUS:              Exp
 *
 * AUTHOR:              CJG
 *
 * DESCRIPTION:
 * Abstraction of the hardware peripherals available on the 802.15.4 chip
 * that are not used directly for 802.15.4, such as UARTs and timers.
 *
 * LAST MODIFIED BY:    dclar
 *                      $Modtime: $
 *
 ****************************************************************************
 *
 *  (c) Copyright JENNIC Ltd 2006
 *
 ****************************************************************************/

#ifndef  AHI_H_INCLUDED
#define  AHI_H_INCLUDED

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
/* Device enumerations */
#define E_AHI_WAKE_TIMER_0         0
#define E_AHI_WAKE_TIMER_1         1
#define E_AHI_AP_DAC_1             0
#define E_AHI_AP_DAC_2             1
#define E_AHI_AP_COMPARATOR_1      0
#define E_AHI_AP_COMPARATOR_2      1
#define E_AHI_UART_0               0
#define E_AHI_UART_1               1
#define E_AHI_TIMER_0              0
#define E_AHI_TIMER_1              1

/* Value enumerations: wake timer */
#define E_AHI_WAKE_TIMER_MASK_0    1
#define E_AHI_WAKE_TIMER_MASK_1    2

/* Value enumerations: Analogue Peripherals */
#define E_AHI_ADC_SRC_ADC_1        0
#define E_AHI_ADC_SRC_ADC_2        1
#define E_AHI_ADC_SRC_ADC_3        2
#define E_AHI_ADC_SRC_ADC_4        3
#define E_AHI_ADC_SRC_TEMP         4
#define E_AHI_ADC_SRC_VOLT         5
#define E_AHI_AP_REGULATOR_ENABLE  TRUE
#define E_AHI_AP_REGULATOR_DISABLE FALSE
#define E_AHI_AP_SAMPLE_2          0
#define E_AHI_AP_SAMPLE_4          1
#define E_AHI_AP_SAMPLE_6          2
#define E_AHI_AP_SAMPLE_8          3
#define E_AHI_AP_CLOCKDIV_2MHZ     0
#define E_AHI_AP_CLOCKDIV_1MHZ     1
#define E_AHI_AP_CLOCKDIV_500KHZ   2
#define E_AHI_AP_CLOCKDIV_250KHZ   3
#define E_AHI_AP_INPUT_RANGE_2     TRUE
#define E_AHI_AP_INPUT_RANGE_1     FALSE
#define E_AHI_AP_GAIN_2            TRUE
#define E_AHI_AP_GAIN_1            FALSE
#define E_AHI_AP_EXTREF            TRUE
#define E_AHI_AP_INTREF            FALSE
#define E_AHI_ADC_CONVERT_ENABLE   TRUE
#define E_AHI_ADC_CONVERT_DISABLE  FALSE
#define E_AHI_ADC_CONTINUOUS       TRUE
#define E_AHI_ADC_SINGLE_SHOT      FALSE
#define E_AHI_AP_INT_ENABLE        TRUE
#define E_AHI_AP_INT_DISABLE       FALSE
#define E_AHI_DAC_RETAIN_ENABLE    TRUE
#define E_AHI_DAC_RETAIN_DISABLE   FALSE

/* Value enumerations: Comparator */
#define E_AHI_COMP_HYSTERESIS_0MV  0
#define E_AHI_COMP_HYSTERESIS_5MV  1
#define E_AHI_COMP_HYSTERESIS_10MV 2
#define E_AHI_COMP_HYSTERESIS_20MV 3
#define E_AHI_AP_COMPARATOR_MASK_1 1
#define E_AHI_AP_COMPARATOR_MASK_2 2
#define E_AHI_COMP_SEL_EXT         0x00
#define E_AHI_COMP_SEL_DAC         0x01
#define E_AHI_COMP_SEL_BANDGAP     0x03

/* Value enumerations: UART */
#define E_AHI_UART_RATE_4800       0
#define E_AHI_UART_RATE_9600       1
#define E_AHI_UART_RATE_19200      2
#define E_AHI_UART_RATE_38400      3
#define E_AHI_UART_RATE_76800      4
#define E_AHI_UART_RATE_115200     5
#define E_AHI_UART_WORD_LEN_5      0
#define E_AHI_UART_WORD_LEN_6      1
#define E_AHI_UART_WORD_LEN_7      2
#define E_AHI_UART_WORD_LEN_8      3
#define E_AHI_UART_FIFO_LEVEL_1    0
#define E_AHI_UART_FIFO_LEVEL_4    1
#define E_AHI_UART_FIFO_LEVEL_8    2
#define E_AHI_UART_FIFO_LEVEL_14   3
#define E_AHI_UART_LS_ERROR        0x80
#define E_AHI_UART_LS_TEMT         0x40
#define E_AHI_UART_LS_THRE         0x20
#define E_AHI_UART_LS_BI           0x10
#define E_AHI_UART_LS_FE           0x08
#define E_AHI_UART_LS_PE           0x04
#define E_AHI_UART_LS_OE           0x02
#define E_AHI_UART_LS_DR           0x01
#define E_AHI_UART_MS_DCTS         0x01
#define E_AHI_UART_INT_MODEM       0
#define E_AHI_UART_INT_TX          1
#define E_AHI_UART_INT_RXDATA      2
#define E_AHI_UART_INT_RXLINE      3
#define E_AHI_UART_INT_TIMEOUT     6
#define E_AHI_UART_TX_RESET        TRUE
#define E_AHI_UART_RX_RESET        TRUE
#define E_AHI_UART_TX_ENABLE       FALSE
#define E_AHI_UART_RX_ENABLE       FALSE
#define E_AHI_UART_EVEN_PARITY     TRUE
#define E_AHI_UART_ODD_PARITY      FALSE
#define E_AHI_UART_PARITY_ENABLE   TRUE
#define E_AHI_UART_PARITY_DISABLE  FALSE
#define E_AHI_UART_1_STOP_BIT      TRUE
#define E_AHI_UART_2_STOP_BITS     FALSE
#define E_AHI_UART_RTS_HIGH        TRUE
#define E_AHI_UART_RTS_LOW         FALSE

/* Value enumerations: SPI */
#define E_AHI_SPIM_MSB_FIRST       FALSE
#define E_AHI_SPIM_LSB_FIRST       TRUE
#define E_AHI_SPIM_TXPOS_EDGE      FALSE
#define E_AHI_SPIM_TXNEG_EDGE      TRUE
#define E_AHI_SPIM_RXPOS_EDGE      FALSE
#define E_AHI_SPIM_RXNEG_EDGE      TRUE
#define E_AHI_SPIM_INT_ENABLE      TRUE
#define E_AHI_SPIM_INT_DISABLE     FALSE
#define E_AHI_SPIM_AUTOSLAVE_ENBL  TRUE
#define E_AHI_SPIM_AUTOSLAVE_DSABL FALSE
#define E_AHI_SPIM_SLAVE_ENBLE_0   0x1
#define E_AHI_SPIM_SLAVE_ENBLE_1   0x2
#define E_AHI_SPIM_SLAVE_ENBLE_2   0x4
#define E_AHI_SPIM_SLAVE_ENBLE_3   0x8

/* Value enumerations: Serial Interface */
#define E_AHI_SI_INT_AL            0x20
#define E_AHI_SI_SLAVE_RW_SET      FALSE
#define E_AHI_SI_START_BIT         TRUE
#define E_AHI_SI_NO_START_BIT      FALSE
#define E_AHI_SI_STOP_BIT          TRUE
#define E_AHI_SI_NO_STOP_BIT       FALSE
#define E_AHI_SI_SLAVE_READ        TRUE
#define E_AHI_SI_NO_SLAVE_READ     FALSE
#define E_AHI_SI_SLAVE_WRITE       TRUE
#define E_AHI_SI_NO_SLAVE_WRITE    FALSE
#define E_AHI_SI_SEND_ACK          FALSE
#define E_AHI_SI_SEND_NACK         TRUE
#define E_AHI_SI_IRQ_ACK           TRUE
#define E_AHI_SI_NO_IRQ_ACK        FALSE

/* Value enumerations: Intelligent Peripheral */
#define E_AHI_IP_MAX_MSG_SIZE      0x3F
#define E_AHI_IP_TXPOS_EDGE        FALSE
#define E_AHI_IP_TXNEG_EDGE        TRUE
#define E_AHI_IP_RXPOS_EDGE        FALSE
#define E_AHI_IP_RXNEG_EDGE        TRUE
#define E_AHI_IP_BIG_ENDIAN        TRUE
#define E_AHI_IP_LITTLE_ENDIAN     FALSE

/* Value enumerations: Timer */
#define E_AHI_TIMER_INT_PERIOD     1
#define E_AHI_TIMER_INT_RISE       2

/* Value enumerations: Tick Timer */
#define E_AHI_TICK_TIMER_DISABLE   0x00 /* Disable tick timer */
#define E_AHI_TICK_TIMER_RESTART   0x01 /* Restart timer when match occurs */
#define E_AHI_TICK_TIMER_STOP      0x02 /* Stop timer when match occurs */
#define E_AHI_TICK_TIMER_CONT      0x03 /* Timer does not stop when match occurs */

/* Value enumerations: DIO */
#define E_AHI_DIO0_INT             0x00000001
#define E_AHI_DIO1_INT             0x00000002
#define E_AHI_DIO2_INT             0x00000004
#define E_AHI_DIO3_INT             0x00000008
#define E_AHI_DIO4_INT             0x00000010
#define E_AHI_DIO5_INT             0x00000020
#define E_AHI_DIO6_INT             0x00000040
#define E_AHI_DIO7_INT             0x00000080
#define E_AHI_DIO8_INT             0x00000100
#define E_AHI_DIO9_INT             0x00000200
#define E_AHI_DIO10_INT            0x00000400
#define E_AHI_DIO11_INT            0x00000800
#define E_AHI_DIO12_INT            0x00001000
#define E_AHI_DIO13_INT            0x00002000
#define E_AHI_DIO14_INT            0x00004000
#define E_AHI_DIO15_INT            0x00008000
#define E_AHI_DIO16_INT            0x00010000
#define E_AHI_DIO17_INT            0x00020000
#define E_AHI_DIO18_INT            0x00040000
#define E_AHI_DIO19_INT            0x00080000
#define E_AHI_DIO20_INT            0x00100000

/* Interrupt Item Bitmap Masks */
#define E_AHI_SYSCTRL_WK0_MASK     (1 << E_AHI_SYSCTRL_WK0)
#define E_AHI_SYSCTRL_WK1_MASK     (1 << E_AHI_SYSCTRL_WK1)
#define E_AHI_SYSCTRL_COMP0_MASK   (1 << E_AHI_SYSCTRL_COMP0)
#define E_AHI_SYSCTRL_COMP1_MASK   (1 << E_AHI_SYSCTRL_COMP1)

#define E_AHI_UART_TIMEOUT_MASK    (1 << E_AHI_UART_INT_TIMEOUT)
#define E_AHI_UART_RXLINE_MASK     (1 << E_AHI_UART_INT_RXLINE)
#define E_AHI_UART_RXDATA_MASK     (1 << E_AHI_UART_INT_RXDATA)
#define E_AHI_UART_TX_MASK         (1 << E_AHI_UART_INT_TX)
#define E_AHI_UART_MODEM_MASK      (1 << E_AHI_UART_INT_MODEM)

#define E_AHI_TIMER_RISE_MASK      E_AHI_TIMER_INT_RISE
#define E_AHI_TIMER_PERIOD_MASK    E_AHI_TIMER_INT_PERIOD

#define E_AHI_SI_RXACK_MASK        (1 << 7)
#define E_AHI_SI_BUSY_MASK         (1 << 6)
#define E_AHI_SI_AL_MASK           (1 << 5)
#define E_AHI_SI_ACK_CTRL_MASK     (1 << 2)
#define E_AHI_SI_TIP_MASK          (1 << 1)
#define E_AHI_SI_INT_STATUS_MASK   (1 << 0)

#define E_AHI_SPIM_TX_MASK         (1 << 0)

#define E_AHI_IP_INT_STATUS_MASK   (1 << 6)
#define E_AHI_IP_TXGO_MASK         (1 << 1)
#define E_AHI_IP_RXGO_MASK         (1 << 0)

#define E_AHI_AP_INT_STATUS_MASK   (1 << 0)

/* Version number of module */
#define AHI_VERSION                0x00010008

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

#ifndef AHI_DEVICE_ENUM
#define AHI_DEVICE_ENUM

/* Device types, used to identify interrupt source */
typedef enum
{
    E_AHI_DEVICE_TICK_TIMER = 0, /* Tick timer */
    E_AHI_DEVICE_SYSCTRL = 2,    /* System controller */
    E_AHI_DEVICE_BBC,            /* Baseband controller */
    E_AHI_DEVICE_AES,            /* Encryption engine */
    E_AHI_DEVICE_PHYCTRL,        /* Phy controller */
    E_AHI_DEVICE_UART0,          /* UART 0 */
    E_AHI_DEVICE_UART1,          /* UART 1 */
    E_AHI_DEVICE_TIMER0,         /* Timer 0 */
    E_AHI_DEVICE_TIMER1,         /* Timer 1 */
    E_AHI_DEVICE_SI,             /* Serial Interface (2 wire) */
    E_AHI_DEVICE_SPIM,           /* SPI master */
    E_AHI_DEVICE_INTPER,         /* Intelligent peripheral */
    E_AHI_DEVICE_ANALOGUE        /* Analogue peripherals */
} teAHI_Device;

/* Individual interrupts */
typedef enum
{
    E_AHI_SYSCTRL_WK0   = 26,    /* Wake timer 0 */
    E_AHI_SYSCTRL_WK1   = 27,    /* Wake timer 1 */
    E_AHI_SYSCTRL_COMP0 = 28,    /* Comparator 0 */
    E_AHI_SYSCTRL_COMP1 = 29,    /* Comparator 1 */
} teAHI_Item;

#define E_AHI_SYSCTRL_COMP   E_AHI_SYSCTRL_COMP1

#endif

typedef void (*PR_HWINT_APPCALLBACK)(uint32 u32Device, uint32 u32ItemBitmap);

typedef uint32 tSpiConfiguration;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/*** ROM Function Pointer typedefs                                        ***/
/***                                                                      ***/
/*** These typedefs are used as prototype definitions for the functions   ***/
/*** that live in ROM.                                                    ***/
/****************************************************************************/

/* Initialisation */
typedef uint32 (*tpru32AHI_InitRom)(void);

/* System controller */
typedef uint8  (*tpru8AHI_PowerStatus)(void);
typedef void   (*tprvAHI_ProtocolPower)(bool_t);
typedef void   (*tprvAHI_CpuDoze)(void);
typedef void   (*tprvAHI_SysCtrlRegisterCallback)(PR_HWINT_APPCALLBACK);

/* ADC/DAC */
typedef void   (*tprvAHI_ApConfigure)( bool_t, bool_t, uint8, uint8, bool_t);
typedef bool_t (*tprbAHI_APRegulatorEnabled)(void);
typedef void   (*tprvAHI_AdcEnable)(bool_t, bool_t, uint8);
typedef void   (*tprvAHI_AdcDisable)(void);
typedef void   (*tprvAHI_AdcStartSample)(void);
typedef bool_t (*tprbAHI_AdcPoll)(void);
typedef uint16 (*tpru16AHI_AdcRead)(void);
typedef void   (*tprvAHI_DacEnable)(uint8, bool_t, bool_t, uint16);
typedef bool_t (*tprbAHI_DacPoll)(void);
typedef void   (*tprvAHI_DacOutput)(uint8, uint16);
typedef void   (*tprvAHI_DacDisable)(uint8);

/* Comparators */
typedef void   (*tprvAHI_ComparatorEnable)(uint8, uint8, uint8);
typedef void   (*tprvAHI_ComparatorDisable)(uint8);
typedef void   (*tprvAHI_ComparatorIntEnable)(uint8, bool_t, bool_t);
typedef void   (*tprvAHI_ComparatorWakeEnable)(uint8, bool_t, bool_t);
typedef uint8  (*tpru8AHI_ComparatorStatus)(void);

/* Wake timers */
typedef void   (*tprvAHI_WakeTimerEnable)(uint8, bool_t);
typedef void   (*tprvAHI_WakeTimerStart)(uint8, uint32);
typedef void   (*tprvAHI_WakeTimerStop)(uint8);
typedef uint8  (*tpru8AHI_WakeTimerStatus)(void);
typedef uint32 (*tpru32AHI_WakeTimerCalibrate)(void);

/* GPIO */
typedef void   (*tprvAHI_DioSetDirection)(uint32, uint32);
typedef void   (*tprvAHI_DioSetOutput)(uint32, uint32);
typedef void   (*tprvAHI_DioSetPullup)(uint32 u32On, uint32 u32Off);
typedef uint32 (*tpru32AHI_DioReadInput)(void);
typedef void   (*tprvAHI_DioWakeEdge)(uint32 u32Rising, uint32 u32Falling);
typedef void   (*tprvAHI_DioWakeEnable)(uint32 u32Enable, uint32 u32Disable);

/* UARTs */
typedef void   (*tprvAHI_UartEnable)(uint8);
typedef void   (*tprvAHI_UartSetClockDivisor)(uint8, uint8);
typedef void   (*tprvAHI_UartSetControl)(uint8, bool_t, bool_t, uint8, bool_t, bool_t);
typedef uint8  (*tpru8AHI_UartReadLineStatus)(uint8);
typedef uint8  (*tpru8AHI_UartReadModemStatus)(uint8);
typedef uint8  (*tpru8AHI_UartReadInterruptStatus)(uint8);
typedef void   (*tprvAHI_UartWriteData)(uint8, uint8);
typedef uint8  (*tpru8AHI_UartReadData)(uint8);
typedef void   (*tprvAHI_Uart0RegisterCallback)(PR_HWINT_APPCALLBACK);
typedef void   (*tprvAHI_Uart1RegisterCallback)(PR_HWINT_APPCALLBACK);


/* Timers */
typedef void   (*tprvAHI_TimerEnable)(uint8, uint8, bool_t, bool_t);
typedef void   (*tprvAHI_TimerDisable)(uint8 );
typedef void   (*tprvAHI_TimerClockSelect)(uint8, bool_t, bool_t);
typedef void   (*tprvAHI_TimerStartSingleShot)(uint8, uint16, uint16);
typedef void   (*tprvAHI_TimerStartRepeat)(uint8, uint16, uint16);
typedef void   (*tprvAHI_TimerStartDeltaSigma)(uint8, uint16, uint16, bool_t);
typedef void   (*tprvAHI_TimerStartCapture)(uint8);
typedef void   (*tprvAHI_TimerReadCapture)(uint8, uint16 *, uint16 *);
typedef void   (*tprvAHI_TimerStop)(uint8);
typedef bool_t (*tprbAHI_TimerFired)(uint8);
typedef void   (*tprvAHI_Timer0RegisterCallback)(PR_HWINT_APPCALLBACK);
typedef void   (*tprvAHI_Timer1RegisterCallback)(PR_HWINT_APPCALLBACK);

/* Tick Timers */
typedef uint32 (*tpru32AHI_TickTimerRead)(void);
typedef void   (*tprvAHI_TickTimerIntPendClr)(void);
typedef bool_t (*tprbAHI_TickTimerIntStatus)(void);
typedef void   (*tprvAHI_TickTimerWrite)(uint32);
typedef void   (*tprvAHI_TickTimerConfigure)(uint8);
typedef void   (*tprvAHI_TickTimerInterval)(uint32);
typedef void   (*tprvAHI_TickTimerIntEnable)(bool_t);


/* SPI master */
typedef void   (*tprvAHI_SpiConfigure)(uint8, bool_t, bool_t, bool_t, uint8, bool_t, bool_t);
typedef void   (*tprvAHI_SpiReadConfiguration)(tSpiConfiguration *);
typedef void   (*tprvAHI_SpiRestoreConfiguration)(tSpiConfiguration *);
typedef void   (*tprvAHI_SpiSelect)(uint8);
typedef void   (*tprvAHI_SpiStop)(void);
typedef void   (*tprvAHI_SpiStartTransfer32)(uint32);
typedef uint32 (*tpru32AHI_SpiReadTransfer32)(void);
typedef void   (*tprvAHI_SpiStartTransfer16)(uint16 u16Out);
typedef uint16 (*tpru16AHI_SpiReadTransfer16)(void);
typedef void   (*tprvAHI_SpiStartTransfer8)(uint8);
typedef uint8  (*tpru8AHI_SpiReadTransfer8)(void);
typedef bool_t (*tprbAHI_SpiPollBusy)(void);
typedef void   (*tprvAHI_SpiWaitBusy)(void);
typedef void   (*tprvAHI_SpiRegisterCallback)(PR_HWINT_APPCALLBACK);

/* Serial 2-wire interface */
typedef void   (*tprvAHI_SiConfigure)(bool_t, bool_t, uint16);
typedef void   (*tprvAHI_SiSetCmdReg)(bool_t, bool_t, bool_t, bool_t, bool_t, bool_t);
typedef void   (*tprvAHI_SiWriteData8)(uint8);
typedef void   (*tprvAHI_SiWriteSlaveAddr)(uint8, bool_t);
typedef uint8  (*tpru8AHI_SiReadData8)(void);
typedef bool_t (*tprbAHI_SiPollBusy)(void);
typedef bool_t (*tprbAHI_SiPollTransferInProgress)(void);
typedef bool_t (*tprbAHI_SiPollRxNack)(void);
typedef bool_t (*tprbAHI_SiPollArbitrationLost)(void);
typedef void   (*tprvAHI_SiRegisterCallback)(PR_HWINT_APPCALLBACK);

/* Flash access */
typedef bool_t (*tprbAHI_FlashErase)(void);
typedef bool_t (*tprbAHI_FlashProgram)(uint16, uint16, uint8 *);
typedef bool_t (*tprbAHI_FlashRead)(uint16, uint16, uint8 *);

/****************************************************************************/
/*** ROM Function Pointer Casts                                           ***/
/***                                                                      ***/
/*** These map function calls into accesses into memory locations in the  ***/
/*** ROM area.                                                            ***/
/****************************************************************************/

/* Initialisation */
#define u32AHI_Init                  ((tpru32AHI_InitRom)                 0x2f8c)

/* System controller */
#define u8AHI_PowerStatus            ((tpru8AHI_PowerStatus)              0x37dc)
#define vAHI_ProtocolPower           ((tprvAHI_ProtocolPower)             0x3844)
#define vAHI_CpuDoze                 ((tprvAHI_CpuDoze)                   0x3884)
#define vAHI_SysCtrlRegisterCallback ((tprvAHI_SysCtrlRegisterCallback)   0x3984)

/* ADC/DAC */
#define vAHI_ApConfigure              ((tprvAHI_ApConfigure)              0x20a8)
#define bAHI_APRegulatorEnabled       ((tprbAHI_APRegulatorEnabled)       0x2198)
/* vAHI_APRegisterCallback is in the library: see below */
#define vAHI_AdcEnable                ((tprvAHI_AdcEnable)                0x21c4)
#define vAHI_AdcDisable               ((tprvAHI_AdcDisable)               0x228c)
#define vAHI_AdcStartSample           ((tprvAHI_AdcStartSample)           0x22bc)
#define bAHI_AdcPoll                  ((tprbAHI_AdcPoll)                  0x22e4)
#define u16AHI_AdcRead                ((tpru16AHI_AdcRead)                0x230c)
#define vAHI_DacEnable                ((tprvAHI_DacEnable)                0x2334)
#define bAHI_DacPoll                  ((tprbAHI_DacPoll)                  0x2420)
#define vAHI_DacOutput                ((tprvAHI_DacOutput)                0x246c)
#define vAHI_DacDisable               ((tprvAHI_DacDisable)               0x24c4)

/* Comparators */
#define vAHI_ComparatorEnable         ((tprvAHI_ComparatorEnable)         0x25a8)
#define vAHI_ComparatorDisable        ((tprvAHI_ComparatorDisable)        0x2634)
#define vAHI_ComparatorIntEnable      ((tprvAHI_ComparatorIntEnable)      0x2678)
#define vAHI_ComparatorWakeEnable     ((tprvAHI_ComparatorWakeEnable)     0x2714)
#define u8AHI_ComparatorStatus        ((tpru8AHI_ComparatorStatus)        0x2744)

/* Wake timers */
#define vAHI_WakeTimerEnable          ((tprvAHI_WakeTimerEnable)          0x40f8)
#define vAHI_WakeTimerStart           ((tprvAHI_WakeTimerStart)           0x41c4)
#define vAHI_WakeTimerStop            ((tprvAHI_WakeTimerStop)            0x4274)
#define u8AHI_WakeTimerStatus         ((tpru8AHI_WakeTimerStatus)         0x42d8)
#define u32AHI_WakeTimerCalibrate     ((tpru32AHI_WakeTimerCalibrate)     0x4300)

/* GPIO */
#define vAHI_DioSetDirection          ((tprvAHI_DioSetDirection)          0x27bc)
#define u32AHI_DioReadInput           ((tpru32AHI_DioReadInput)           0x2858)
#define vAHI_DioSetOutput             ((tprvAHI_DioSetOutput)             0x27f0)
#define vAHI_DioSetPullup             ((tprvAHI_DioSetPullup)             0x2824)
#define vAHI_DioWakeEdge              ((tprvAHI_DioWakeEdge)              0x287c)
#define vAHI_DioWakeEnable            ((tprvAHI_DioWakeEnable)            0x28b0)

/* DIO aliases: DIO interrupt and DIO wake are the same thing */
#define vAHI_DioInterruptEdge     (vAHI_DioWakeEdge)
#define vAHI_DioInterruptEnable   (vAHI_DioWakeEnable)
#define u32AHI_DioInterruptStatus (u32AHI_DioWakeStatus)

/* UARTs */
#define vAHI_UartEnable               ((tprvAHI_UartEnable)               0x39bc)
#define vAHI_UartSetClockDivisor      ((tprvAHI_UartSetClockDivisor)      0x3ae0)
#define vAHI_UartSetControl           ((tprvAHI_UartSetControl)           0x3bb0)
#define u8AHI_UartReadLineStatus      ((tpru8AHI_UartReadLineStatus)      0x3db8)
#define u8AHI_UartReadModemStatus     ((tpru8AHI_UartReadModemStatus)     0x3e0c)
#define u8AHI_UartReadInterruptStatus ((tpru8AHI_UartReadInterruptStatus) 0x3e60)
#define vAHI_UartWriteData            ((tprvAHI_UartWriteData)            0x3eb4)
#define u8AHI_UartReadData            ((tpru8AHI_UartReadData)            0x3f04)
#ifndef GDB
#define vAHI_Uart0RegisterCallback    ((tprvAHI_Uart0RegisterCallback)    0x4088)
#endif
#define vAHI_Uart1RegisterCallback    ((tprvAHI_Uart0RegisterCallback)    0x40c0)



/* Tick Timers */
#define u32AHI_TickTimerRead          ((tpru32AHI_TickTimerRead)          0x44f4)
#define vAHI_TickTimerIntPendClr      ((tprvAHI_TickTimerIntPendClr)      0x4418)
#define bAHI_TickTimerIntStatus       ((tprbAHI_TickTimerIntStatus)       0x4444)
#define vAHI_TickTimerWrite           ((tprvAHI_TickTimerWrite)           0x43fc)
#define vAHI_TickTimerConfigure       ((tprvAHI_TickTimerConfigure)       0x4478)
#define vAHI_TickTimerInterval        ((tprvAHI_TickTimerInterval)        0x4510)
#define vAHI_TickTimerIntEnable       ((tprvAHI_TickTimerIntEnable)       0x44b0)


/* SPI master */
#define vAHI_SpiReadConfiguration     ((tprvAHI_SpiReadConfiguration)     0x3484)
#define vAHI_SpiRestoreConfiguration  ((tprvAHI_SpiRestoreConfiguration)  0x34dc)
#define vAHI_SpiConfigure             ((tprvAHI_SpiConfigure)             0x3360)
#define vAHI_SpiSelect                ((tprvAHI_SpiSelect)                0x353c)
#define vAHI_SpiStop                  ((tprvAHI_SpiStop)                  0x3564)
#define vAHI_SpiStartTransfer32       ((tprvAHI_SpiStartTransfer32)       0x358c)
#define u32AHI_SpiReadTransfer32      ((tpru32AHI_SpiReadTransfer32)      0x35cc)
#define vAHI_SpiStartTransfer16       ((tprvAHI_SpiStartTransfer16)       0x35f0)
#define u16AHI_SpiReadTransfer16      ((tpru16AHI_SpiReadTransfer16)      0x3634)
#define vAHI_SpiStartTransfer8        ((tprvAHI_SpiStartTransfer8)        0x365c)
#define u8AHI_SpiReadTransfer8        ((tpru8AHI_SpiReadTransfer8)        0x36a0)
#define bAHI_SpiPollBusy              ((tprbAHI_SpiPollBusy)              0x36c8)
#define vAHI_SpiWaitBusy              ((tprvAHI_SpiWaitBusy)              0x3700)
#define vAHI_SpiRegisterCallback      ((tprvAHI_SpiRegisterCallback)      0x37a4)

/* Serial 2-wire interface */
/* vAHI_SiConfigure is in the library: see below */
#define vAHI_SiSetCmdReg              ((tprvAHI_SiSetCmdReg)              0x30b0)
#define vAHI_SiWriteData8             ((tprvAHI_SiWriteData8)             0x3144)
#define vAHI_SiWriteSlaveAddr         ((tprvAHI_SiWriteSlaveAddr)         0x316c)
#define u8AHI_SiReadData8             ((tpru8AHI_SiReadData8)             0x31b4)
#define bAHI_SiPollBusy               ((tprbAHI_SiPollBusy)               0x31dc)
#define bAHI_SiPollTransferInProgress ((tprbAHI_SiPollTransferInProgress) 0x3218)
#define bAHI_SiPollRxNack             ((tprbAHI_SiPollRxNack)             0x3254)
#define bAHI_SiPollArbitrationLost    ((tprbAHI_SiPollArbitrationLost)    0x3290)
/* vAHI_SiRegisterCallback is in the library: see below */

/* Flash access */
#define bAHI_FlashErase               ((tprbAHI_FlashErase)               0x292c)
#define bAHI_FlashProgram             ((tprbAHI_FlashProgram)             0x29b0)
#define bAHI_FlashRead                ((tprbAHI_FlashRead)                0x2b3c)

/* Comparator aliases */
#define vAHI_CompEnable(u8Hysteresis, u8SignalSelect) \
            vAHI_ComparatorEnable(E_AHI_AP_COMPARATOR_2, u8Hysteresis, u8SignalSelect)
#define vAHI_CompDisable() vAHI_ComparatorDisable(E_AHI_AP_COMPARATOR_2)
#define vAHI_CompIntEnable(bIntEnable, bRisingNotFalling) \
            vAHI_ComparatorIntEnable(E_AHI_AP_COMPARATOR_2, bIntEnable, bRisingNotFalling);
#define vAHI_CompWakeEnable(bIntEnable, bRisingNotFalling) \
            vAHI_ComparatorWakeEnable(E_AHI_AP_COMPARATOR_2, bIntEnable, bRisingNotFalling);
#define u8AHI_CompStatus() (u8AHI_ComparatorStatus() & E_AHI_AP_COMPARATOR_MASK_2)
#define u8AHI_CompWakeStatus() (u8AHI_ComparatorWakeStatus() & E_AHI_AP_COMPARATOR_MASK_2)

/* LPRAM aliases */
#define u8AHI_LpRamBase  u8AHI_NvRamBase
#define vAHI_LpRamWrite  vAHI_NvRamWrite
#define u32AHI_LpRamRead u32AHI_NvRamRead

/****************************************************************************/
/*** Library function definitions                                         ***/
/***                                                                      ***/
/*** These functions exist in the library as C code that can be linked    ***/
/*** dynamically at build time, so the function prototypes are 'normal'   ***/
/****************************************************************************/

/* Timers */
PUBLIC void  vAHI_TimerEnable(uint8 u8Timer, uint8 u8Prescale,
                              bool_t bIntRiseEnable, bool_t bIntPeriodEnable,
                              bool_t bOutputEnable);
PUBLIC void  vAHI_TimerDisable(uint8 u8Timer);
PUBLIC void  vAHI_TimerClockSelect(uint8 u8Timer, bool_t bExternalClock,
                                   bool_t bInvertClock);
PUBLIC void  vAHI_TimerStartSingleShot(uint8 u8Timer, uint16 u16Hi,
                                       uint16 u16Lo);
PUBLIC void  vAHI_TimerStartRepeat(uint8 u8Timer, uint16 u16Hi, uint16 u16Lo);
PUBLIC void  vAHI_TimerStartDeltaSigma(uint8 u8Timer, uint16 u16Hi,
                                       uint16 u16Lo, bool_t bRtzEnable);
PUBLIC void  vAHI_TimerStartCapture(uint8 u8Timer);
PUBLIC void  vAHI_TimerReadCapture(uint8 u8Timer, uint16 *pu16Hi,
                                   uint16 *pu16Lo);
PUBLIC void  vAHI_TimerStop(uint8 u8Timer);
PUBLIC uint8 u8AHI_TimerFired(uint8 u8Timer);
PUBLIC void  vAHI_Timer0RegisterCallback(PR_HWINT_APPCALLBACK prTimer0Callback);
PUBLIC void  vAHI_Timer1RegisterCallback(PR_HWINT_APPCALLBACK prTimer1Callback);

/* ADC/DAC */
PUBLIC void vAHI_APRegisterCallback(PR_HWINT_APPCALLBACK prApCallback);

/* Serial 2-wire interface */
PUBLIC void vAHI_SiConfigure(bool_t bSiEnable, bool_t bInterruptEnable,
                             uint16 u16PreScaler);
PUBLIC void vAHI_SiRegisterCallback(PR_HWINT_APPCALLBACK prSiCallback);

/* Intelligent Peripheral */
PUBLIC void   vAHI_IpEnable(bool_t bTxEdge, bool_t bRxEdge, bool_t bEndian);
PUBLIC bool_t bAHI_IpSendData(uint8 u8Length, uint8 *pau8Data);
PUBLIC bool_t bAHI_IpReadData(uint8 *pu8Length, uint8 *pau8Data);
PUBLIC bool_t bAHI_IpTxDone(void);
PUBLIC bool_t bAHI_IpRxDataAvailable(void);
PUBLIC void   vAHI_IpRegisterCallback(PR_HWINT_APPCALLBACK prIpCallback);

/* NVRAM */
PUBLIC uint8  u8AHI_NvRamBase(void);
PUBLIC void   vAHI_NvRamWrite(uint8 u8Address, uint32 u32Value);
PUBLIC uint32 u32AHI_NvRamRead(uint8 u8Address);

/* Reset functions */
PUBLIC void vAHI_SwReset(void);
PUBLIC void vAHI_DriveResetOut(uint8 u8Period);

/* Comparators */
PUBLIC uint8  u8AHI_ComparatorWakeStatus(void);

/* GPIO */
PUBLIC uint32 u32AHI_DioWakeStatus(void);
PUBLIC uint8  vAHI_DioReadByte(bool_t bDIOSelect);
PUBLIC void   vAHI_DioSetByte(bool_t bDIOSelect, uint8 u8DataByte);

/* Tick Timers */
PUBLIC void   vAHI_TickTimerInit(PR_HWINT_APPCALLBACK prTickTimerCallback);

/* Wake timers */
PUBLIC uint8  u8AHI_WakeTimerFiredStatus(void);
PUBLIC uint32 u32AHI_WakeTimerRead(uint8 u8Timer);

/* System controller */
PUBLIC void   vAHI_MemoryHold(bool_t);
PUBLIC void   vAHI_PowerDown(bool_t);

/* UARTs */
PUBLIC void vAHI_UartDisable(uint8 u8Uart);
PUBLIC void vAHI_UartSetBaudDivisor(uint8 u8Uart, uint16 u16Divisor);
PUBLIC void vAHI_UartSetInterrupt(uint8 u8Uart, bool_t bEnableModemStatus,
                                  bool_t bEnableRxLineStatus,
                                  bool_t bEnableTxFifoEmpty,
                                  bool_t bEnableRxData, uint8 u8FifoLevel);
PUBLIC void vAHI_UartReset(uint8 u8Uart, bool_t bTxReset, bool_t bRxReset);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* AHI_H_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
