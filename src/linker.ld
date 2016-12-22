/* Linker script to link the object code into the final kernel */
ENTRY(_start)

SECTIONS
{
	/* Starts at LOADER_ADDR */
	. = 0x8000; /* this will vary based on emulation needs */
	__start = .;
	__text_start = .;
	.text :
	{
		KEEP(*(.text.boot))
		*(.text)
	}
	. = ALIGN(4096); /* align to it's page size */
	__text_end = .;

	/* rodata section */
	__rodata_start = .;
	.rodata :
	{
		*(.rodata)
	}
	. = ALIGN(4096); /* align again */
	__rodata_end = .;

	/* data section */
	__data_start = .;
	.data :
	{
		*(.data)
	}
	. = ALIGN(4096); /* can't we macro this */
	__data_end = .;

	/* bss section */
	__bss_start = .;
	.bss :
	{
		bss = .;
		*(.bss)
	}
	. = ALIGN(4096); /* hi mom */
	__bss_end = .;
	__end = .;
}
