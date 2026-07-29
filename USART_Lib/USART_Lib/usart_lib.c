
/*
 * usart_lib.c
 *
 * Created: 9/19/2024 6:05:29 PM
 *  Author: Ikramuzzaman Muntasir
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

#define UART_RX_BUFFER_SIZE 32
#define UART_TX_BUFFER_SIZE 32

static volatile char rx_buffer[UART_RX_BUFFER_SIZE];
static volatile uint8_t rx_head = 0;
static volatile uint8_t rx_tail = 0;

static volatile char tx_buffer[UART_TX_BUFFER_SIZE];
static volatile uint8_t tx_head = 0;
static volatile uint8_t tx_tail = 0;

void uart_init(uint32_t baud) {
    uint16_t ubrr = (uint16_t)(F_CPU / (16UL * baud)) - 1;

    UBRRH = (uint8_t)(ubrr >> 8);
    UBRRL = (uint8_t)ubrr;

    // Enable RX, TX and RX-complete interrupt
    UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);

    // Async, no parity, 1 stop bit, 8 data bits (URSEL selects UCSRC on this address)
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);

    sei();
}

void uart_putc(char data) {
    uint8_t next = (uint8_t)(tx_head + 1) % UART_TX_BUFFER_SIZE;

    while (next == tx_tail) {
        // TX buffer full, wait for ISR to free a slot
    }

    tx_buffer[tx_head] = data;
    tx_head = next;

    // Make sure the "data register empty" interrupt is enabled to drain the buffer
    UCSRB |= (1 << UDRIE);
}

void uart_puts(const char *str) {
    while (*str) {
        uart_putc(*str++);
    }
}

uint8_t uart_available(void) {
    return (uint8_t)(rx_head - rx_tail + UART_RX_BUFFER_SIZE) % UART_RX_BUFFER_SIZE;
}

char uart_getc(void) {
    while (rx_head == rx_tail) {
        // RX buffer empty, wait for ISR to receive data
    }

    char data = rx_buffer[rx_tail];
    rx_tail = (uint8_t)(rx_tail + 1) % UART_RX_BUFFER_SIZE;

    return data;
}

void uart_gets(char *buf, uint8_t maxlen) {
    uint8_t i = 0;

    while (i < maxlen - 1) {
        char c = uart_getc();

        if (c == '\r' || c == '\n') {
            break;
        }

        buf[i++] = c;
    }

    buf[i] = '\0';
}

ISR(USART_RXC_vect) {
    uint8_t data = UDR;
    uint8_t next = (uint8_t)(rx_head + 1) % UART_RX_BUFFER_SIZE;

    if (next != rx_tail) {
        rx_buffer[rx_head] = data;
        rx_head = next;
    }
    // else: buffer full, byte dropped
}

ISR(USART_UDRE_vect) {
    if (tx_head == tx_tail) {
        // Nothing left to send, disable this interrupt until uart_putc() re-enables it
        UCSRB &= ~(1 << UDRIE);
    } else {
        UDR = tx_buffer[tx_tail];
        tx_tail = (uint8_t)(tx_tail + 1) % UART_TX_BUFFER_SIZE;
    }
}
