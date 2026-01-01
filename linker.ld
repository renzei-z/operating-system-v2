ENTRY(_kernel_start)
OUTPUT_FORMAT("binary")

SECTIONS
{
	. = 0x1000;

	.text : {
	      *(.text.entry)
	      *(.text)
	}

	.data : { *(.data) }
	.bss : { *(.bss) }

	_kernel_end = .;
}
