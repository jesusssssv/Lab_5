//***************************************************************************
// Universidad del Valle de Guatemala
// IE2023: Programaci�n de Microcontroladores 
// Hardware: ATMEGA328PB
// Created: 10/04/2025

//***************************************************************************
// Post Lab 5
//***************************************************************************

// Define la frecuencia del CPU a 16MHz - importante para las funciones de delay
#define F_CPU 16000000

// Incluye la librer�a para funciones de delay como _delay_ms()
#include <util/delay.h>

// Incluye la librer�a para acceso a los registros de entrada/salida del AVR
#include <avr/io.h>

// Incluye la librer�a personalizada para controlar servomotores
#include "ServoControl.h"

// Incluye la librer�a personalizada para manejar el convertidor anal�gico-digital
#include "ADC.h"

// Incluye la librer�a personalizada para controlar el segundo m�dulo PWM
#include "PWM2.h"

// Incluye la librer�a personalizada para control manual de PWM
#include "PWM_manual.h"

// Declara variables globales para almacenar los valores le�dos del ADC
float adcValue1 = 0;  // Para almacenar el valor del ADC del canal 7
float adcValue2 = 0;  // Para almacenar el valor del ADC del canal 6
float adcValue3 = 0;  // Para almacenar el valor del ADC del canal 5

// Variable para almacenar el ciclo de trabajo calculado para PWM
float dutyCycle = 0;

// Funci�n principal del programa
int main(void)
{
    // Configura todos los pines del puerto B como salidas (0xFF = 11111111 en binario)
    DDRB = 0xFF;
    
    // Configura todos los pines del puerto D como salidas (0xFF = 11111111 en binario)
    DDRD = 0xFF;
    
    // Inicializa el m�dulo ADC (Convertidor Anal�gico-Digital)
    ADC_init();
    
    // Inicializa el primer m�dulo PWM (probablemente para el servo)
    PWM1_init();
    
    // Inicializa el segundo m�dulo PWM
    PWM2_init();
    
    // Inicializa un PWM manual en el pin PD4 (pin 4 del puerto D)
    PWM_init(4);
    
    // Inicia la generaci�n del PWM manual
    PWM_start();
    
    // Bucle infinito del programa principal
    while (1) 
    {
        // Lee el valor anal�gico del canal 7 del ADC
        adcValue1 = ADC_read(7);
        
        // Controla la posici�n del servomotor A basado en el valor le�do
        servo_writeA(adcValue1);
        
        // Espera 5 ms - NOTA: Hay un error aqu�, deber�a ser _delay_ms(5)
        _delay_ms(5);
        
        // Lee el valor anal�gico del canal 6 del ADC
        adcValue2 = ADC_read(6);
        
        // Calcula el ciclo de trabajo en porcentaje (0-100%) basado en el valor le�do
        // ADC da valores de 0-1023, por lo que se multiplica por 100 y divide por 1023
        dutyCycle = (adcValue2 * 100)/1023;
        
        // Establece el ciclo de trabajo calculado en el canal A del PWM2 en modo no invertido
        PWM2_dca(dutyCycle, NO_INVERTING);
        
        // Espera 5 ms - NOTA: Hay un error aqu�, deber�a ser _delay_ms(5)
        _delay_ms(5);
        
        // Lee el valor anal�gico del canal 5 del ADC
        adcValue3 = ADC_read(5);
        
        // Calcula el ciclo de trabajo en un rango de 0-255 para el PWM manual
        // ADC da valores de 0-1023, por lo que se multiplica por 255 y divide por 1023
        dutyCycle = (adcValue3 * 255) / 1023;
        
        // Establece el ciclo de trabajo calculado en el PWM manual
        PWM_setDutyCycle(dutyCycle);
        
        // Espera 5 ms - NOTA: Hay un error aqu�, deber�a ser _delay_ms(5)
        _delay_ms(5);
    }
}