/*    
*     Author: Conrado Becker Gressler
*     Florianópolis, December 2022
*
*     This program is responsible for the control
*     of a remote controlled car via a bluetooth app
*     creating, a communication protocol using UART
*     and controlling speed using power-width-modulation
*/


#include <msp430.h> 

#include <uart.h>
#include <motor.h>
#include <linha.h>
#include <gpio.h>
#include <bits.h>


volatile uint16_t velocidade = 4000;

 //Clock configuration
void init_clock_system(){

    // Configure two FRAM wait state as required by the device data sheet for MCLK
    // operation at 24MHz(beyond 8MHz) _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_2 ;

    P2SEL1 |= BIT6 | BIT7;                       // P2.6~P2.7: crystal pins
    do
    {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);           // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);                   // Test oscillator fault flag

    __bis_SR_register(SCG0);                     // disable FLL
    CSCTL3 |= SELREF__XT1CLK;                    // Set XT1 as FLL reference source
    CSCTL0 = 0;                                  // clear DCO and MOD registers

    /* Selecionar DCORSEL_xyz de acordo com a máxima frequência da CPU */
    CSCTL1 = DCORSEL_7;                          // Set DCO = 24MHz

    /* Ajustar o multiplicador (ex. 731) para a frequência desejada *
     *
     * F = 32768*731 / 1  ---->   (32768 * n) / FLLD_x
     */
    CSCTL2 = FLLD_0 + 731;                       // DCOCLKDIV = 32768*731 / 1
    __delay_cycles(3);
    __bic_SR_register(SCG0);                     // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));   // FLL locked

    /* CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK;
     * set XT1 (~32768Hz) as ACLK source, ACLK = 32768Hz
     * default DCOCLKDIV as MCLK and SMCLK source
     - Selects the ACLK source.
     * 00b = XT1CLK with divider (must be no more than 40 kHz)
     * 01b = REFO (internal 32-kHz clock source)
     * 10b = VLO (internal 10-kHz clock source) (1)   */
    CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK;
}


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

#if defined (__MSP430FR2355__)
    /* Disable the GPIO power-on default high-impedance mode */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    init_clock_system();

//    config_timer_B0();

    config_timer_B3();

    init_uart();

    char my_data[1];

    while (1){
        /* Configura o recebimento de um pacote de 1 byte */
        uart_receive_package((uint8_t *)my_data, 1);

        /* Desliga a CPU enquanto pacote não chega */
        __bis_SR_register(CPUOFF | GIE);

        switch(my_data[0]){

        /*For each character sended, move or adjust speed*/
        case 'F':
            motor_frente(velocidade);
            break;

        case 'B':
            motor_re(velocidade);
            break;

        case 'L':
            motor_esquerda(velocidade);
            break;

        case 'R':
            motor_direita(velocidade);
            break;

        case 'S':
            motor_desligado(velocidade);
            break;

        case '0':
            velocidade = 0;
            break;

        case '1':
            velocidade = 800;
            break;

        case '2':
            velocidade = 1600;
            break;

        case '3':
            velocidade = 2400;
            break;

        case '4':
            velocidade = 3200;
            break;

        case '5':
            velocidade = 4000;
            break;

        case '6':
            velocidade = 4800;
            break;

        case '7':
            velocidade = 5600;
            break;

        case '8':
            velocidade = 6400;
            break;

        case '9':
            velocidade = 7200;
            break;

        case 'q':
            velocidade = 8000;
            break;

        case 'X':
            linha(1);
            break;

        case 'x':
            linha(0);
            break;
        }
    }
    __bis_SR_register(LPM0_bits + GIE);
}
