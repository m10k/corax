ENTRY (loader)

SECTIONS {
    .text 0x100000 : {
        *(.text)
    }
    .rodata ALIGN (0x1000) : {
        *(.rodata)
    }
    .data ALIGN (0x1000) : {
        *(.data)
    }
    .bss :{
        sbss = .;
        *(COMMON)
        *(.bss)
        ebss = .;
    }
    _mem_start = .;
}
