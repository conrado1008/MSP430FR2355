#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>

void config_timer_B3();

void motor_off();
void motor_forward(uint16_t x);
void motor_backward(uint16_t x);
void motor_left(uint16_t x);
void motor_right(uint16_t x);

void teste(uint16_t velocidade);


#endif /* MOTOR_H_ */
