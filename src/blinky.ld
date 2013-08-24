/*
 * generic gcc linker script for LPC810
 * Kamal Mostafa <kamal@whence.com>.
 *
 * License: Public Domain
 */

MEMORY
{
    /* NXP LPC810 */
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x00010000 /* 4 Kbytes */
    RAM (rwx) : ORIGIN = 0x10000000, LENGTH = 0x00000400 /* 1 Kbyte */
}

SECTIONS
{
    .text :
    {
        _text = .;
        KEEP(*(.isr_vector))
        KEEP(*(.after_vectors))
        *(.text*)
        *(.rodata*)
        _etext = .;
    } > FLASH

    .data : AT (ADDR(.text) + SIZEOF(.text))
    {
        _data = .;
        *(vtable)
        *(.data*)
        _edata = .;
    } > RAM

    .bss :
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > RAM
}
