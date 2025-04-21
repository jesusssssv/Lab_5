// PWM.c
#include "PWM_manual.h"

static uint8_t pwmPin; // Pin de salida para el PWM
static volatile uint8_t dutyCycle; // Ciclo de trabajo
static volatile uint8_t counter = 0; // Contador para la interrupci�n

void PWM_init(uint8_t pin) {
	pwmPin = pin; // Guardar el pin seleccionado

	// Configurar el pin como salida
	if (pwmPin >= 0 && pwmPin <= 13) { // Verificar que el pin est� en el rango v�lido
		DDRD |= (1 << pwmPin); // Configurar el pin como salida
	}

	// Configurar el Timer 0
	TCCR0A |= (1 << WGM01); // Modo CTC
	TCCR0B |= (1 << CS01) | (1 << CS00); // Prescaler 64
	OCR0A = 5; // Valor de comparaci�n para el Timer 0
	TIMSK0 |= (1 << OCIE0A); // Habilitar la interrupci�n de comparaci�n
}

void PWM_setDutyCycle(uint8_t duty) {
	// Asegurarse de que el duty cycle est� en el rango de 0 a 255
	if (duty > 255) {
		duty = 255;
	}
	dutyCycle = duty; // Establecer el ciclo de trabajo
}

void PWM_start() {
	counter = 0; // Reiniciar el contador
	sei(); // Habilitar interrupciones globales
}

void PWM_stop() {
	cli(); // Deshabilitar interrupciones globales
	PORTD &= ~(1 << pwmPin); // Asegurarse de que el pin est� bajo
}

// PWM.c (continuaci�n)
ISR(TIMER0_COMPA_vect) {
	counter++; // Incrementar el contador

	if (counter < dutyCycle) {
		PORTD |= (1 << pwmPin); // Encender el pin
		} else {
		PORTD &= ~(1 << pwmPin); // Apagar el pin
	}

	if (counter >= 255) {
		counter = 0; // Reiniciar el contador
	}
}