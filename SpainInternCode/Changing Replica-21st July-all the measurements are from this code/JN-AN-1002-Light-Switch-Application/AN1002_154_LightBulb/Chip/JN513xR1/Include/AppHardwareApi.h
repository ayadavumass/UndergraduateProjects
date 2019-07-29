/*****************************************************************************
 *
 * MODULE:              Application Hardware API
 *
 * COMPONENT:           AppHardwareApi.h,v
 *
 * VERSION:             Build_Release_030308_RC1
 *
 * REVISION:            1.8
 *
 * DATED:               2008/01/04 13:15:47
 *
 * STATUS:              Exp
 *
 * AUTHOR:              CJG
 *
 * DESCRIPTION:
 * Abstraction of the hardware peripherals available on the 802.15.4 chip
 * that are not used directly for 802.15.4, such as UARTs and timers.
 *
 * LAST MODIFIED BY:    snich
 *                      $Modtime: $
 *
 ****************************************************************************
 *
 *  (c) Copyright JENNIC Ltd 2005
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
/*********** obsolete *************/
#define E_AHI_DAC_0                0
#define E_AHI_DAC_1                1
#define E_AHI_COMPARATOR_0         0
#define E_AHI_COMPARATOR_1         1
/**********************************/

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
/******* obsolete *****************/
#define E_AHI_COMPARATOR_MASK_0    1
#define E_AHI_COMPARATOR_MASK_1    2
/**********************************/
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
#define E_AHI_UART_2_STOP_BITS     TRUE
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

#endif

typedef void (*PR_HWINT_APPCALLBACK)(uint32 u32Device, uint32 u32ItemBitmap);

typedef uint32 tSpiConfiguration;

/* Sleep Modes */
typedef enum
{
    E_AHI_SLEEP_OSCON_RAMON,     /*32Khz Osc on and Ram On*/
    E_AHI_SLEEP_OSCON_RAMOFF,    /*32Khz Osc on and Ram off*/
    E_AHI_SLEEP_OSCOFF_RAMON,    /*32Khz Osc off and Ram on*/
    E_AHI_SLEEP_OSCOFF_RAMOFF,   /*32Khz Osc off and Ram off*/
    E_AHI_SLEEP_DEEP,            /*Deep Sleep*/
} teAHI_SleepMode;


/*Flash Chips*/
typedef enum {
    E_FL_CHIP_ST_M25P10_A,
    E_FL_CHIP_SST_25VF010,
    E_FL_CHIP_ATMEL_AT25F512,
    E_FL_CHIP_CUSTOM,
    E_FL_CHIP_AUTO
} teFlashChipType;



/// Type definitions for SPI Flash access functions
typedef void    (*tpfvZSPIflashInit)(int iDivisor, uint8 u8SlaveSel);
typedef void    (*tpfvZSPIflashSetSlaveSel)(uint8 u8SlaveSel);
typedef void    (*tpfvZSPIflashWREN)(void);
typedef void    (*tpfvZSPIflashEWRSR)(void);
typedef uint8   (*tpfu8ZSPIflashRDSR)(void);
typedef uint16  (*tpfu16ZSPIflashRDID)(void);
typedef void    (*tpfvZSPIflashWRSR)(uint8 u8Data);
typedef void    (*tpfvZSPIflashPP)(uint32 u32Addr, uint16 u16Len, uint8* pu8Data);
typedef bool_t    (*tpfvZSPIflashRead)(uint32 u32Addr,uint16 u16Len,uint8* pu8Data);
typedef void    (*tpfvZSPIflashBE)(void);
typedef void    (*tpfvZSPIflashSE)(uint8 u8Sector);

/// Table of SPI Flash access functions
typedef struct tagSPIflashFncTable {
    uint32                      u32Signature;
    uint16                      u16FlashId;
    uint16                      u16Reserved;

    tpfvZSPIflashInit           vZSPIflashInit;
    tpfvZSPIflashSetSlaveSel    vZSPIflashSetSlaveSel;
    tpfvZSPIflashWREN           vZSPIflashWREN;
    tpfvZSPIflashEWRSR          vZSPIflashEWRSR;
    tpfu8ZSPIflashRDSR          u8ZSPIflashRDSR;
    tpfu16ZSPIflashRDID         u16ZSPIflashRDID;
    tpfvZSPIflashWRSR           vZSPIflashWRSR;
    tpfvZSPIflashPP             vZSPIflashPP;
    tpfvZSPIflashRead           vZSPIflashRead;
    tpfvZSPIflashBE             vZSPIflashBE;
    tpfvZSPIflashSE             vZSPIflashSE;
} tSPIflashFncTable;





/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/* Initialisation */
PUBLIC uint32 u32AHI_Init(void);

/* System controller */
PUBLIC uint8  u8AHI_PowerStatus(void);
PUBLIC void   vAHI_ProtocolPower(bool_t bOnNotOff);
PUBLIC void   vAHI_MemoryHold(bool_t bHoldDuringSleep);
PUBLIC void   vAHI_CpuDoze(void);
PUBLIC void   vAHI_PowerDown(bool_t bDeepNotNormalSleep);
PUBLIC void   vAHI_Sleep(teAHI_SleepMode eSleepMode);
PUBLIC void   vAHI_CPUClockDoublerEnable(bool_t);
PUBLIC void   vAHI_ExternalClockEnable(bool_t);
PUBLIC void   vAHI_AntennaDiversityOutputEnable(bool_t,  bool_t);
PUBLIC void   vAHI_HighPowerModuleEnable(bool_t,  bool_t);
PUBLIC void   vAHI_SysCtrlRegisterCallback(PR_HWINT_APPCALLBACK prSysCtrlCallback);

/* ADC/DAC */
PUBLIC void   vAHI_ApConfigure( bool_t bAPRegulator, bool_t bIntEnable,
                              uint8 u8SampleSelect, uint8 u8ClockDivRatio,
                              bool_t bRefSelect);
PUBLIC bool_t bAHI_APRegulatorEnabled(void);
PUBLIC void   vAHI_APRegisterCallback(PR_HWINT_APPCALLBACK prApCallback);
PUBLIC void   vAHI_AdcEnable(bool_t bContinuous, bool_t bInputRange, uint8 u8Source);
PUBLIC void   vAHI_AdcDisable(void);
PUBLIC void   vAHI_AdcStartSample(void);
PUBLIC bool_t bAHI_AdcPoll(void);
PUBLIC uint16 u16AHI_AdcRead(void);
PUBLIC void   vAHI_DacEnable(uint8 u8Dac, bool_t bInputRange, bool_t bRetainOutput,
                           uint16 u16InitialVal);
PUBLIC bool_t bAHI_DacPoll(void);
PUBLIC void   vAHI_DacOutput(uint8 u8Dac, uint16 u16Value);
PUBLIC void   vAHI_DacDisable(uint8 u8Dac);

/* Comparators */
PUBLIC void   vAHI_ComparatorEnable(uint8 u8Comparator, uint8 u8Hysteresis,
                                  uint8 u8SignalSelect);
PUBLIC void   vAHI_ComparatorDisable(uint8 u8Comparator);
PUBLIC void   vAHI_ComparatorIntEnable(uint8 u8Comparator, bool_t bIntEnable,
                                        bool_t bRisingNotFalling);
PUBLIC void   vAHI_ComparatorWakeEnable(uint8 u8Comparator, bool_t bIntEnable,
                                        bool_t bRisingNotFalling);
PUBLIC uint8  u8AHI_ComparatorStatus(void);
PUBLIC uint8  u8AHI_ComparatorWakeStatus(void);

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

/* Wake timers */
PUBLIC void   vAHI_WakeTimerEnable(uint8 u8Timer, bool_t bIntEnable);
PUBLIC void   vAHI_WakeTimerStart(uint8 u8Timer, uint32 u32Count);
PUBLIC void   vAHI_WakeTimerStop(uint8 u8Timer);
PUBLIC uint8  u8AHI_WakeTimerStatus(void);
PUBLIC uint32 u32AHI_WakeTimerCalibrate(void);
PUBLIC uint8  u8AHI_WakeTimerFiredStatus(void);
PUBLIC uint32 u32AHI_WakeTimerRead(uint8 u8Timer);

/* GPIO */
PUBLIC void   vAHI_DioSetDirection(uint32 u32Inputs, uint32 u32Outputs);
PUBLIC void   vAHI_DioSetOutput(uint32 u32On, uint32 u32Off);
PUBLIC void   vAHI_DioSetPullup(uint32 u32On, uint32 u32Off);
PUBLIC uint32 u32AHI_DioReadInput(void);
PUBLIC void   vAHI_DioWakeEdge(uint32 u32Rising, uint32 u32Falling);
PUBLIC void   vAHI_DioWakeEnable(uint32 u32Enable, uint32 u32Disable);
PUBLIC uint32 u32AHI_DioWakeStatus(void);

#define vAHI_DioInterruptEdge(u32Rising, u32Falling)    (vAHI_DioWakeEdge(u32Rising, u32Falling))
#define vAHI_DioInterruptEnable(u32Enable, u32Disable)  (vAHI_DioWakeEnable(u32Enable, u32Disable))
#define u32AHI_DioInterruptStatus()                     (u32AHI_DioWakeStatus())

/* UARTs */
PUBLIC void   vAHI_UartEnable(uint8 u8Uart);
PUBLIC void   vAHI_UartDisable(uint8 u8Uart);
PUBLIC void   vAHI_UartSetBaudDivisor(uint8 u8Uart, uint16 u16Divisor);
PUBLIC void   vAHI_UartSetClockDivisor(uint8 u8Uart, uint8 u8BaudRate);
PUBLIC void   vAHI_UartSetControl(uint8 u8Uart, bool_t bEvenParity, bool_t bEnableParity,
                                  uint8 u8WordLength, bool_t bOneStopBit, bool_t bRtsValue);
PUBLIC void   vAHI_UartSetInterrupt(uint8 u8Uart, bool_t bEnableModemStatus,
                                    bool_t bEnableRxLineStatus, bool_t bEnableTxFifoEmpty,
                                    bool_t bEnableRxData, uint8 u8FifoLevel);
PUBLIC void   vAHI_UartReset(uint8 u8Uart, bool_t bTxReset, bool_t bRxReset);
PUBLIC uint8  u8AHI_UartReadLineStatus(uint8 u8Uart);
PUBLIC uint8  u8AHI_UartReadModemStatus(uint8 u8Uart);
PUBLIC uint8  u8AHI_UartReadInterruptStatus(uint8 u8Uart);
PUBLIC void   vAHI_UartWriteData(uint8 u8Uart, uint8 u8Data);
PUBLIC uint8  u8AHI_UartReadData(uint8 u8Uart);
PUBLIC void   vAHI_UartSetRTSCTS(uint8 u8Uart, bool_t bRTSCTSEn);
PUBLIC void   vAHI_Uart0RegisterCallback(PR_HWINT_APPCALLBACK prUart0Callback);
PUBLIC void   vAHI_Uart1RegisterCallback(PR_HWINT_APPCALLBACK prUart1Callback);

/* Timers */
PUBLIC void  vAHI_TimerEnable(uint8 u8Timer, uint8 u8Prescale,
                              bool_t bIntRiseEnable, bool_t bIntPeriodEnable,
                              bool_t bOutputEnable);
PUBLIC void  vAHI_TimerDisable(uint8 u8Timer);
PUBLIC void  vAHI_TimerClockSelect(uint8 u8Timer, bool_t bExternalClock, bool_t bInvertClock);
PUBLIC void  vAHI_TimerStartSingleShot(uint8 u8Timer, uint16 u16Hi, uint16 u16Lo);
PUBLIC void  vAHI_TimerStartRepeat(uint8 u8Timer, uint16 u16Hi, uint16 u16Lo);
PUBLIC void  vAHI_TimerStartDeltaSigma(uint8 u8Timer, uint16 u16Hi,
                                       uint16 u16Lo, bool_t bRtzEnable);
PUBLIC void  vAHI_TimerStartCapture(uint8 u8Timer);
PUBLIC void  vAHI_TimerReadCapture(uint8 u8Timer, uint16 *pu16Hi, uint16 *pu16Lo);
PUBLIC void  vAHI_TimerStop(uint8 u8Timer);
PUBLIC uint8 u8AHI_TimerFired(uint8 u8Timer);
PUBLIC void  vAHI_TimerDIOControl(uint8, bool_t);
PUBLIC void  vAHI_Timer0RegisterCallback(PR_HWINT_APPCALLBACK prTimer0Callback);
PUBLIC void  vAHI_Timer1RegisterCallback(PR_HWINT_APPCALLBACK prTimer1Callback);

/* Tick Timers */
PUBLIC uint32 u32AHI_TickTimerRead(void);
PUBLIC void   vAHI_TickTimerIntPendClr(void);
PUBLIC bool_t bAHI_TickTimerIntStatus(void);
PUBLIC void   vAHI_TickTimerWrite(uint32 u32Count);
PUBLIC void   vAHI_TickTimerConfigure(uint8 u8Mode);
PUBLIC void   vAHI_TickTimerInterval(uint32 u32Interval);
PUBLIC void   vAHI_TickTimerIntEnable(bool_t bIntEnable);
PUBLIC void   vAHI_TickTimerInit(PR_HWINT_APPCALLBACK prTickTimerCallback);

/* SPI master */
PUBLIC void   vAHI_SpiConfigure(uint8 u8SlaveEnable, bool_t bLsbFirst, bool_t bTxNegEdge,
                                bool_t bRxNegEdge, uint8 u8ClockDivider, bool_t bInterruptEnable,
                                bool_t bAutoSlaveSelect);
PUBLIC void   vAHI_SpiReadConfiguration(tSpiConfiguration *ptConfiguration);
PUBLIC void   vAHI_SpiRestoreConfiguration(tSpiConfiguration *ptConfiguration);
PUBLIC void   vAHI_SpiSelect(uint8 u8SlaveMask);
PUBLIC void   vAHI_SpiStop(void);
PUBLIC void   vAHI_SpiStartTransfer32(uint32 u32Out);
PUBLIC uint32 u32AHI_SpiReadTransfer32(void);
PUBLIC void   vAHI_SpiStartTransfer16(uint16 u16Out);
PUBLIC uint16 u16AHI_SpiReadTransfer16(void);
PUBLIC void   vAHI_SpiStartTransfer8(uint8 u8Out);
PUBLIC uint8  u8AHI_SpiReadTransfer8(void);
PUBLIC bool_t bAHI_SpiPollBusy(void);
PUBLIC void   vAHI_SpiWaitBusy(void);
PUBLIC void   vAHI_SpiRegisterCallback(PR_HWINT_APPCALLBACK prSpiCallback);

/* Serial 2-wire interface */
PUBLIC void   vAHI_SiConfigure(bool_t bSiEnable, bool_t bInterruptEnable, uint16 u16PreScaler);
PUBLIC void   vAHI_SiSetCmdReg(bool_t bSetSTA, bool_t bSetSTO, bool_t bSetRD,
                               bool_t bSetWR, bool_t bSetAckCtrl, bool_t bSetIACK);
PUBLIC void   vAHI_SiWriteData8(uint8 u8Out);
PUBLIC void   vAHI_SiWriteSlaveAddr(uint8 u8SlaveAddress, bool_t bReadStatus);
PUBLIC uint8  u8AHI_SiReadData8(void);
PUBLIC bool_t bAHI_SiPollBusy(void);
PUBLIC bool_t bAHI_SiPollTransferInProgress(void);
PUBLIC bool_t bAHI_SiPollRxNack(void);
PUBLIC bool_t bAHI_SiPollArbitrationLost(void);
PUBLIC void   vAHI_SiRegisterCallback(PR_HWINT_APPCALLBACK prSiCallback);

/* Intelligent Peripheral */
PUBLIC void   vAHI_IpEnable(bool_t bTxEdge, bool_t bRxEdge, bool_t bEndian);
PUBLIC bool_t bAHI_IpSendData(uint8 u8Length, uint8 *pau8Data);
PUBLIC bool_t bAHI_IpReadData(uint8 *pu8Length, uint8 *pau8Data);
PUBLIC bool_t bAHI_IpTxDone(void);
PUBLIC bool_t bAHI_IpRxDataAvailable(void);
PUBLIC void   vAHI_IpRegisterCallback(PR_HWINT_APPCALLBACK prIpCallback);

/* Flash access */
PUBLIC bool_t bAHI_FlashInit(teFlashChipType flashType, tSPIflashFncTable *pCustomFncTable);
PUBLIC bool_t bAHI_FlashEraseSector(uint8 u8Sector);
PUBLIC bool_t bAHI_FullFlashProgram(uint32 u32Addr, uint16 u16Len, uint8 *pu8Data);
PUBLIC bool_t bAHI_FullFlashRead(uint32 u32Addr, uint16 u16Len, uint8 *pu8Data);
/* Backwards compatible with JN5121: these only access top sector of flash */
PUBLIC bool_t bAHI_FlashErase(void);
PUBLIC bool_t bAHI_FlashProgram(uint16 u16Addr, uint16 u16Len, uint8 *pu8Data);
PUBLIC bool_t bAHI_FlashRead(uint16 u16Addr, uint16 u16Len, uint8 *pu8Data);

/* Reset functions */
PUBLIC void vAHI_SwReset(void);
PUBLIC void vAHI_DriveResetOut(uint8 u8Period);

/* eFuse functions */
PUBLIC bool_t bAHI_eFuseBlow(uint8 u8FuseNum, uint8 u8Bank);
PUBLIC bool_t bAHI_eFuseReadBank(uint8 u8Bank, uint32 *pu32Buffer);

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
