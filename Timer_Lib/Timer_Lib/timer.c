/*
 * timer.c
 *
 * Created: 9/20/2024 3:52:08 PM
 *  Author: muntasir
 */ 

#include "timer.h"

void (*timer2_comp_callback)() = 0;
void (*timer2_ovf_callback)() = 0;

void set_tcnt2(uint8_t value) {
	TCNT2 = value;
} 

void set_ocr2(uint8_t value) {
	OCR2 = value;
}

void enable_timer2_comp_int() {
	TIMSK |= (1 << OCIE2);
	sei();
}

void set_timer2_mode(timer2_mode_t mode) {
	TCCR2 &= ~((1 << WGM21) | (1 << WGM20));
	TCCR2 |= mode;
}

void start_timer2(timer2_prescaler_t prescaler) {
	TCCR2 &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
	TCCR2 |= prescaler;
}

ISR(TIMER2_OVF_vect) {
	if(timer2_ovf_callback) timer2_ovf_callback();
}

ISR(TIMER2_COMP_vect) {
	if(timer2_comp_callback) timer2_comp_callback();
}