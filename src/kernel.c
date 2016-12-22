#if !defined(__cplusplus)
#include <stdbool.h>
#endif

// The Kernel can target RPI v1, v2 and v3
// v2 and v3 use different base register addresses
// these instructions are used in compiling to hot switch the register
#if DRPI1
#define BASE_ADDR 0x20200000
#define UART_BASE 0x20201000
#else
#define BASE_ADDR 0x3F200000
#define UART_BASE 0x3F201000
#endif

#include <stddef.h>
#include <stdint.h>

// copy data to a register
static inline void mmio_write(uint32_t reg, uint32_t data)
{
    *(volatile uint32_t *)reg = data;
}

// read data back from a register
static inline uint32_t mmio_read(uint32_t reg)
{
    return *(volatile uint32_t *)reg;
}

// Loop <delay> times in a way that the compiler won't optimize away
// Delay blocks execution on the whole chip at this level
static inline void delay(int32_t count)
{
    // what on earth does this even do
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
		 : "=r"(count): [count]"0"(count) : "cc");
}

size_t strlen(const char* str)
{
    size_t ret = 0;
    while(str[ret] != 0) ret++;
    return ret;
}

// Register enumerations
enum
{
    // The GPIO registers base address (see macro at top)
    GPIO_BASE  = BASE_ADDR,

    // Controls actuation of pull up/down to all GPIO pins
    GPPUD      = (GPIO_BASE + 0x94),

    // Controls actuation of pull up/down for specific GPIO pin
    GPPUDCLK0  = (GPIO_BASE + 0x98),

    // the base address for the UART (macro at top (do we need macro?))
    UART0_BASE = UART_BASE, 

    // The offsets for each register in the UART
    UART0_DR     = (UART0_BASE + 0x00),
    UART0_RSPECR = (UART0_BASE + 0x04),
    UART0_FR     = (UART0_BASE + 0x18),
    UART0_ILPR   = (UART0_BASE + 0x20),
    UART0_IBRD   = (UART0_BASE + 0x24),
    UART0_FBRD   = (UART0_BASE + 0x28),
    UART0_LCRH   = (UART0_BASE + 0x2C),
    UART0_CR     = (UART0_BASE + 0x30),
    UART0_IFLS   = (UART0_BASE + 0x34),
    UART0_IMSC   = (UART0_BASE + 0x38),
    UART0_RIS    = (UART0_BASE + 0x3C),
    UART0_MIS    = (UART0_BASE + 0x40),
    UART0_ICR    = (UART0_BASE + 0x44),
    UART0_DMACR  = (UART0_BASE + 0x48),
    UART0_ITCR   = (UART0_BASE + 0x80),
    UART0_ITIP   = (UART0_BASE + 0x84),
    UART0_ITOP   = (UART0_BASE + 0x88),
    UART0_TDR    = (UART0_BASE + 0x8C),
};

void uart_init()
{
    // disable UART0
    mmio_write(UART0_CR, 0x00000000);
    // Setup the GPIO pin 14 && 15

    // disable pull up/down for all GPIO pins & delay for 150c
    mmio_write(GPPUD, 0x00000000);
    delay(150);

    // disable pull up/down for pin 14/15 & delay for 150c
    // yes, this basically sends 0x1100000000000000 to GPPUDCLK0
    mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
    delay(150);

    // write 0 to GPPUDCLK0 to make it take effect
    mmio_write(GPPUDCLK0, 0x00000000);

    // clear pending interrupts
    mmio_write(UART0_ICR, 0x7FF);

    // Set integer & fractional part of baud rate
    // Divider                = UART_CLOCK/(16 * Baud)
    // Fraction part register = (Fractional part * 64) + 0.5
    // UART_CLOCK             = 300000; Baud = 115200

    // Divider         = 3000000 / (16 * 115200) = 1.627 = ~1
    // Fractional part = (.627 * 64) + 0.5 = 40.6        = ~40

    mmio_write(UART0_IBRD, 1);
    mmio_write(UART0_FBRD, 40);

    // enable FIFO & 8 bit data transmission (1 stop bit, no parity)
    mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

    // Mask all interrupts
    mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
	       (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

    // enable UART0, receive and transfer part of UART
    mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

void uart_putc(unsigned char byte)
{
    // wait for UART to become ready to transmit
    while ( mmio_read(UART0_FR) & (1 << 5) ) {}
    mmio_write(UART0_DR, byte);
}

unsigned char uart_getc()
{
    // wait for UART to have received something
    while ( mmio_read(UART0_FR) & (1 << 4) ) {}
    return mmio_read(UART0_DR);
}

void uart_write(const unsigned char* buffer, size_t size)
{
    for ( size_t i = 0; i < size; i++)
	uart_putc(buffer[i]);
}

void uart_puts(const char* str)
{
    uart_write((const unsigned char*) str, strlen(str));
}

#if defined(__cplusplus)
extern "C"
#endif
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    // All operating system code goes here yo
    (void) r0;
    (void) r1;
    (void) atags;

    // start the OS through uart_init()
    uart_init();

    // complimentary hello world
    uart_puts("Hello, Kernel World!\r\n");

    // basically infinite loop forever to keep the system running
    while ( true )
	uart_putc(uart_getc());
}


    

