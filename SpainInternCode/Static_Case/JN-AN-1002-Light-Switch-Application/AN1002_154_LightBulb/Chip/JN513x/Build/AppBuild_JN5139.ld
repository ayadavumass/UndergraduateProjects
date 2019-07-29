INCLUDE Rom_Functions.ld

MEMORY
        {
        flashh : ORIGIN = 0x04000fd0, LENGTH = 0x00000024
        oad    : ORIGIN = 0x04000ff4, LENGTH = 0x0000000c
        mac    : ORIGIN = 0x04001000, LENGTH = 0x00000020
        ram    : ORIGIN = 0x04001020, LENGTH = 0x000167e0
        }

SECTIONS
{
        .flashheader :
        {
        LONG(0xe0e0e0e0)
        LONG(ABSOLUTE(_data_beg))
        LONG(_data_len)
        LONG(0x01040031)
        LONG(0)
        LONG(ABSOLUTE(_bss_beg))
        LONG(_bss_len)
        LONG(ABSOLUTE(_AppWarmStart))
        LONG(ABSOLUTE(_AppColdStart))
        } > flashh

        .oad :
        {
        LONG(0xffffffff)
        LONG(0xffffffff)
        LONG(0xf0f0f0f0)
        } > oad

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
