/*
 * motor.h
 *
 *  Created on: May 4, 2022
 *      Author: jorge
 */

#ifndef MOTOR_H_
#define MOTOR_H_
#include <stdint.h>
#include "msp.h"
typedef struct Motor{
        uint8_t pinA;
        uint8_t pinB;
        uint8_t enable;
}Motor;
void MotorInit(const Motor *motor);
void MotorOff(const Motor *motor);
void MotorForward(const Motor *motor);
void MotorBackward(const Motor *motor);


#endif /* MOTOR_H_ */
