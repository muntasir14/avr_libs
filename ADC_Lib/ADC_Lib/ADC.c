/*
 * ADC.c
 *
 * Created: 9/21/2024 12:55:12 AM
 *  Author: Ikramuzzaman Muntasir
 */ 

#include "ADC.h"

void (*adc_callback)(uint16_t adc_result) = 0;

ISR(ADC_vect) {
	uint16_t result;
	if(ADMUX & (1 << ADLAR)) {
		result = (ADCL >> 6);
		result |= (ADCH << 2);
	} else {
		result = ADCL;
		result |= (ADCH << 8);
	}
	if(adc_callback) adc_callback(result);
}

void set_channel(adc_channel_t channel) {
	ADMUX &= ~((1 << MUX2) | (1 << MUX1) | (1 << MUX0));
	ADMUX |= channel;
	ADC_DDR &= ~(1 << channel);
}

void set_ref_volt(adc_ref_volt_t ref) {
	ADMUX &= ~((1 << REFS1) | (1 << REFS0));
	ADMUX |= ref;
}

void set_adc_prescaler(adc_prescaler_t prescaler) {
	ADCSRA &= ~((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));
	ADCSRA |= prescaler;
}

void make_result_left_adjusted() {
	ADMUX |= (1 << ADLAR);
}

void make_result_right_adjusted() {
	ADMUX &= ~(1 << ADLAR);
}

void enable_adc_int() {
	ADCSRA |= (1 << ADIE);
	sei();
}

void enable_adc() {
	ADCSRA |= (1 << ADEN);
}

void start_adc() {
	ADCSRA |= (1 << ADSC);
}

void adc_free_running() {
	ADCSRA |= (1 << ADFR);
}
