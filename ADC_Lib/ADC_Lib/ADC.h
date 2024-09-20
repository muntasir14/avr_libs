/*
 * ADC.h
 *
 * Created: 9/21/2024 12:55:41 AM
 *  Author: Ikramuzzaman Muntasir
 */ 

#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define ADC_DDR		DDRC
#define ADC_PORT	PORTC
#define ADC_PIN		PINC

// in ADMUX
typedef enum {
	CHANNEL_0 = 0,
	CHANNEL_1 = (1 << MUX0),
	CHANNEL_2 = (1 << MUX1),
	CHANNEL_3 = (1 << MUX1) | (1 << MUX0),
	CHANNEL_4 = (1 << MUX2),
	CHANNEL_5 = (1 << MUX2) | (1 << MUX0),
	CHANNEL_6 = (1 << MUX2) | (1 << MUX1),
	CHANNEL_7 = (1 << MUX2)	| (1 << MUX1) | (1 << MUX0)
} adc_channel_t;

typedef enum {
	AREF = 0,
	AVCC = (1 << REFS0),
	INTERNAL_REF_2_56 = (1 << REFS1) | (1 << REFS0)
} adc_ref_volt_t;


typedef enum {
	ADC_PRESCALER_2 = 0,
	ADC_PRESCALER_4 = (1 << ADPS1),
	ADC_PRESCALER_8 = (1 << ADPS1) | (1 << ADPS0),
	ADC_PRESCALER_16 = (1 << ADPS2),
	ADC_PRESCALER_32 = (1 << ADPS2) | (1 << ADPS0),
	ADC_PRESCALER_64 = (1 << ADPS2) | (1 << ADPS1),
	ADC_PRESCALER_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)
} adc_prescaler_t;

extern void (*adc_callback)();

void set_channel(adc_channel_t channel);
void set_ref_volt(adc_ref_volt_t ref);
void set_adc_prescaler(adc_prescaler_t prescaler);
void make_result_left_adjusted();
void make_result_right_adjusted();
void enable_adc_int();
void enable_adc();
void start_adc();
void adc_free_running();



#endif /* ADC_H_ */