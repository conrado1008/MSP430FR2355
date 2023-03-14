/*   
*     Author: Conrado Becker Gressler
*     Florianópolis, December, 2022
*
*     Program responsible for adjusting
*     the pwm of the microcontroller
*     and changing states of the car
*/


#include <msp430.h>
#include <stdint.h>
#include <bits.h>
#include <gpio.h>


enum {OFF, FORWARD, BACKWARD, LEFT, RIGHT};

volatile uint8_t state = {OFF};

void config_timer_B3(){

    /* Timer B
     * TBSSEL_2 -> Clock de SMCLK.
     * MC_3 -> Up/down mode.
     * TBIE -> Habilitação de IRQ.
     * ID_0 -> Prescaler = /1
     *
     * TB3CCR1 PWM duty cycle: TB3CCR1 / TB3CCR0 *
     * TB3CCR2 PWM duty cycle: TB3CCR2 / TB3CCR0 */
    TB3CTL = TBSSEL_2 | MC_3 | ID_0;// | TBIE;


    P6DIR = BIT0 | BIT1 | BIT2 | BIT3;

    P6SEL0 = BIT0 | BIT1 | BIT2 | BIT3;


    TB3CCR0 = 8000;



    /*      .
     *      /|\                  +                < -Comparador 0: (maximo da contagem) -> periodo do PWM
     *       |                 +   +
     *       |               +       +
     *       |-------------+---------- +          <--  Comparadores 1 e 2: razao ciclica
     *       |           +  |         | +
     *       |         +    |         |   +
     *       |       +      |         |     +
     *       |     +        |         |       +
     *       |   +          |         |         +
     *       | +            |         |           +
     * Timer +--------------|---- ----|-------------->
     *       |
     *       |
     *
     *       |--------------|         |--------------
     * Saida |              |         |
     *       +---------------++++++++++------------->
     */

}

void motor_off(){

    TB3CCTL1 = OUTMOD_0;
    TB3CCTL2 = OUTMOD_0;
    TB3CCTL3 = OUTMOD_0;
    TB3CCTL4 = OUTMOD_0;

    state = OFF;

}

void motor_forward(uint16_t x){

    TB3CCTL1 = OUTMOD_2;
    TB3CCTL2 = OUTMOD_0;
    TB3CCTL3 = OUTMOD_2;
    TB3CCTL4 = OUTMOD_0;

    TB3CCR1 = x;
    TB3CCR3 = x;

    state = FORWARD;

}

void motor_backward(uint16_t x){

    TB3CCTL1 = OUTMOD_0;
    TB3CCTL2 = OUTMOD_2;
    TB3CCTL3 = OUTMOD_0;
    TB3CCTL4 = OUTMOD_2;

    TB3CCR2 = x;
    TB3CCR4 = x;

    state = BACKWARD;
}

void motor_left(uint16_t x){

    TB3CCTL1 = OUTMOD_2;
    TB3CCTL2 = OUTMOD_0;
    TB3CCTL3 = OUTMOD_0;
    TB3CCTL4 = OUTMOD_2;

    TB3CCR1 = x;
    TB3CCR4 = x;

    state = LEFT;

}

void motor_right(uint16_t x){

    TB3CCTL1 = OUTMOD_0;
    TB3CCTL2 = OUTMOD_2;
    TB3CCTL3 = OUTMOD_2;
    TB3CCTL4 = OUTMOD_0;

    TB3CCR2 = x;
    TB3CCR3 = x;

    state = RIGHT;
}
