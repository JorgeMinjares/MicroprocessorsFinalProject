/*
 * pwm.h
 *
 *  Created on: May 4, 2022
 *      Author: jorge
 */

#ifndef PWM_H_
#define PWM_H_

#define MAX_SERVO 6500
#define MIN_SERVO 2500
void PWM_Init(int period, int duty);
void PWM_FIRE(void);
void Honk(int times);

#endif /* PWM_H_ */
