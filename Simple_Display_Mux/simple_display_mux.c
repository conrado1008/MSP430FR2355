/*
 * simple_display_mux.c
 *
 *  Created on: Feb 27, 2020
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 */

#include <msp430.h>
#include <stdint.h>

#include "simple_display_mux.h"

/* Flash conversion table: Common Anode */
#ifdef COM_ANODO
const uint8_t convTable[] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02,
        0x78, 0x00, 0x18, 0x08, 0x03, 0x46, 0x21, 0x06, 0x0E};
#endif

#ifdef COM_CATODO
const uint8_t convTable[] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD,
    0x87, 0xFF, 0xE7, 0xF7, 0xFC, 0xB9, 0xDE, 0xF9, 0xF1};
#endif

void display_mux_init() {

    /* Port Configuration */
    DISPLAYS_DATA_PORT_DIR = 0xff;
    DISPLAYS_DATA_PORT_OUT = 0;

    DISPLAYS_MUX_PORT_DIR |= BIT0 | BIT1;
    DISPLAYS_MUX_PORT_OUT = 0;
}


/* Function to write using multiple displays using */
void display_mux_write(uint8_t data){

    /*the four least significant bits*/
    uint8_t i = 15;
    i = i & data;
    /*the four most significant bits*/
    data = data >> 4;

    /* Turn off all displays */
    DISPLAYS_DATA_PORT_OUT = 0;
    DISPLAYS_MUX_PORT_OUT = 0;

    /* Write the converted value of digit 1 on the GPIO */
    DISPLAYS_DATA_PORT_OUT = convTable[i];

    /* Turn on display 1*/
    DISPLAYS_MUX_PORT_OUT |= BIT0;

    /* Keep the display on for a time */
    _delay_cycles(1000);

    /* Turn off display 1 */
    DISPLAYS_MUX_PORT_OUT = DISPLAYS_MUX_PORT_OUT & (~BIT0);

    /* Write the converted value of digit 2 on the GPIO */
    DISPLAYS_DATA_PORT_OUT = convTable[data];

    /* Turn off display 2 */
    DISPLAYS_MUX_PORT_OUT |= BIT1;

    /* Keep the display on for some time */

    _delay_cycles(1000);
}
