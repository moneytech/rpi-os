	// To keep this in the first portion of the binary
	.section ".text.boot"

	// make _start global
	.globl _start

_start:
	// Entry point for the kernel
	// r15 -> should begin exec at 0x8000
	// r0  -> 0x00000000
	// r1  -> 0x00000C42
	// r2  -> 0x00000100
	mov sp, #0x8000

	ldr r4, =__bss_start
	ldr r9, =__bss_end
	mov r5, #0
	mov r6, #0
	mov r7, #0
	mov r8, #0
	b       2f

1:
	// store multiple at r4
	stmia r4!, {r5-r8}
2:
	// if we are still below bss_end, loop
	cmp r4, r9
	blo 1b

	// call kernel_main
	ldr r3, =kernel_main
	blx r3

	//halt
halt:
	wfe
	b halt
