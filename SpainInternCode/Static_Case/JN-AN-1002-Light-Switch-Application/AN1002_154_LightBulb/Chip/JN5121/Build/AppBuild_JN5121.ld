MEMORY
        {
        flashh : ORIGIN = 0xefffffdc, LENGTH = 0x00000024
        mac    : ORIGIN = 0xf0000000, LENGTH = 0x00000020
        ram    : ORIGIN = 0xf0000020, LENGTH = 0x000167e0
        sector3 : ORIGIN = 0xf0017fdc, LENGTH = 0x00008000
        }

SECTIONS
{
        .flashheader :
        {
        LONG(0xe1e1e1e1)
        LONG(ABSOLUTE(_data_beg))
        LONG(_data_len)
        LONG(0x00030021)
        LONG(0)
        LONG(ABSOLUTE(_bss_beg))
        LONG(_bss_len)
        LONG(ABSOLUTE(_AppWarmStart))
        LONG(ABSOLUTE(_AppColdStart))
        } > flashh

        .mac :
        {
        _data_beg = ABSOLUTE(.);
        LONG(0xffffffff)
        LONG(0xffffffff)
        LONG(0xffffffff)
        LONG(0xffffffff)
        LONG(0xffffffff)
        LONG(0xffffffff)
        LONG(0xffffffff)
        LONG(0xffffffff)
        } > mac

        .rodata :
        {
        *(.rodata*)
        } > ram

        .data :
        {
        *(.data*)
        } > ram

        .text :
        {
        *(.text*)
        _data_end = ABSOLUTE(.);
        } > ram

        .bss :
        {
        _bss_beg = ABSOLUTE(.);
        *(.bss*)
        } > ram
        _bss_end = ABSOLUTE(.);

        _data_len = _data_end - _data_beg;
        _bss_len = (_bss_end - _bss_beg + 3) & 0xfffffffc;
        
	.sector3 :
	{
	*(.sector3*)
	} > sector3

}

_apvReg       = 0x10000000;
_apvSysReg    = 0x10000000;
_apvAesReg    = 0x10000c00;
_apvAnaReg    = 0x10000f00;
_apvGpioReg   = 0x20000000;
_apvUart0Reg  = 0x30000000;
_apvUart1Reg  = 0x40000000;
_apvTimer0Reg = 0x50000000;
_apvTimer1Reg = 0x60000000;
_apvSiReg     = 0x70000000;
_apvSpiReg    = 0x80000000;
_apvSpiIpReg  = 0x90000000;
_apvSpiIpMem  = 0x98000000;
