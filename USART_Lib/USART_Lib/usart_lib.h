/*
 * usart_lib.h
 *
 * Created: 9/19/2024 6:05:29 PM
 *  Author: Ikramuzzaman Muntasir
 */ 

#ifndef UART_H
#define UART_H

#include <stdint.h>

// Initializes USART for the given baud rate (e.g. 9600, 19200, 115200) and
// enables the RX-complete interrupt. Must be called before any other uart_*
// function. Enables global interrupts (sei()).
void uart_init(uint32_t baud);

// Queues a single byte for transmission. Returns once the byte is in the
// TX buffer; actual sending happens in the background via UDRE interrupt.
// Blocks only if the TX buffer is full.
void uart_putc(char data);

// Queues a null-terminated string for transmission.
void uart_puts(const char *str);

// Returns the number of bytes currently waiting in the RX buffer.
uint8_t uart_available(void);

// Returns the next received byte. Blocks until one is available; check
// uart_available() first to avoid blocking.
char uart_getc(void);

// Blocks until a full line is received (terminated by '\r' or '\n', which
// is consumed but not stored) or maxlen-1 bytes have been read, then
// null-terminates buf. maxlen is the size of buf, including the terminator.
void uart_gets(char *buf, uint8_t maxlen);

#endif
