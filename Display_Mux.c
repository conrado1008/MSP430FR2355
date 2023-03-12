/*
 * 02_simple_display_mux.c
 *
 *  Created on: September, 2022
 *      Author: Conrado Becker Gressler
 *      Instituto Federal de Santa Catarina
 *
 *      This program makes one counter count up 
 *      to a maximum value of x, and other that 
 *      count from y to zero.
 */

#include <msp430.h>

/* Tipos uint16_t, uint8_t, ... */
#include <stdint.h>

#include "simple_display_mux.h"

#define LED BIT6

void main(void)
{
    /* Stop watchdog timer
     * Necessary for codes on depuration */
    WDTCTL = WDTPW | WDTHOLD;

#if defined (__MSP430FR2355__)
    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    volatile uint16_t i;
    uint8_t x = 0;
    uint8_t y;

    P6DIR = LED;

    /* Initialize displays */
    display_mux_init();

    while(1)
    {
        x=0x80;
        y=0x85;
        while(x<=0x84){
            x++;
            display_mux_write(x);
            for(i=500;i>0;i--){
                P6OUT &= ~LED;
                display_mux_write(x);
            }
        }
        while(y!=0){
            y--;
            display_mux_write(y);
            for(i=500;i>0;i--){
                P6OUT &= ~LED;
                display_mux_write(y);
            }
        }
    }
}
