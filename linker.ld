OUTPUT_FORMAT(binary)
ENTRY(START)

SECTIONS
{
    . = 0x8000;

    .text : {
        *(.text)
        *(.text*)
    }

    .rodata : {
        *(.rodata)
        *(.rodata.*)
    }

    .data : {
        *(.data)
        *(.data*)
    }

    .bss : {
        *(.bss)
        *(.bss*)
        *(COMMON)
    }

    /DISCARD/ : {
        *(.eh_frame)
        *(.note.gnu.property)
        *(.comment)
    }
}