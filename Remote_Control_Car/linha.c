/*
 * linha.c
 *
 *  Created on: Dec, 2022
 *      Author: Conrado Becker Gressler
 *      Instituto Federal de Santa Catarina
 *
 *      Library created for the use of the line
 *      following mode, besides changing the car
 *      direction and lighting two leds.
 *      
 */


#include <msp430.h>

#include <bits.h>
#include <gpio.h>
#include <linha.h>
#include <motor.h>


#define SENSOR1_PORT P5
#define SENSOR1_PIN BIT4

#define SENSOR2_PORT P3
#define SENSOR2_PIN BIT0

#define BUTTON_SAMPLES (12)


volatile uint8_t bit_left = 0;
volatile uint8_t bit_right = 0;
volatile uint8_t t_linha = 0;


void config_timer_B0(){

    /* Timer B
     * TBSSEL_2 -> Clock de SMCLK.
     * MC_2 -> Contagem crescente.
     * TBIE -> Habilitação de IRQ.
     * ID_3 -> Prescaler = /2
     */
    TB0CTL = TBSSEL_2 | MC_2 | ID_1;
    TB0CCTL0 = CCIE;

    TB0CCR0 = 45000;
}

void linha(uint8_t l){
    t_linha = l;
}

/* ISR0 do Timer B: executado no evento de comparacao  comparador 0 (TBCCR0)
 * Debouncer necessario para filtrar os possiveis "ruidos do botao
 * quando eles sao pressionados.*/
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMER0_B0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B0_VECTOR))) Timer_B (void)
#else
#error Compiler not supported!
#endif
{
    static int counter = BUTTON_SAMPLES;
    static int counter2 = BUTTON_SAMPLES;


    if (TST_BIT(PORT_IN(SENSOR1_PORT), SENSOR1_PIN))  {
        /* Debouncer por software */
        if (!(--counter)) {
           bit_right = 1;
        }
    }
    else{
        counter = BUTTON_SAMPLES;
        bit_right = 0;
    }

    if (TST_BIT(PORT_IN(SENSOR2_PORT), SENSOR2_PIN))  {
        /* Debouncer por software */
        if (!(--counter2)) {
            bit_left = 1;
        }
    }
    else{
        counter2 = BUTTON_SAMPLES;
        bit_left = 0;
    }

    if(t_linha == 1){

        if ((bit_right == 0) && (bit_left == 0)){
            motor_forward(4000);
        }

        else if((bit_right == 0) && (bit_left == 1)){
            motor_right(4000);
        }

        else if((bit_right == 1) && (bit_left == 0)){
            motor_left(4000);
        }
        else if((bit_right == 1) && (bit_left == 1)){
            motor_off(4000);
        }

    }

}
