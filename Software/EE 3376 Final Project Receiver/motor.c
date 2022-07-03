/*
 * motor.c
 *
 *  Created on: May 4, 2022
 *      Author: jorge
 */
#include "motor.h"
#include "msp.h"
void MotorInit(const Motor *motor){
    P2->DIR |= motor->enable;                   //enable pins for H-bridge
    P4->DIR |= motor->pinA | motor->pinB;       //Set Motor Output Pins
    P4->OUT &= ~(motor->pinA | motor->pinB);    //Clear Motor Output Pins
    P4->SEL0 &= ~(motor->pinA | motor->pinB);   //Clear Special functions for Motor pins
    P4->SEL1 &= ~(motor->pinA | motor->pinB);   //Clear Special Functions for Motor pins
}
void MotorOff(const Motor *motor){
    P4->OUT &= ~(motor->pinA | motor->pinB);    //Clear Motor Output / Turn Off motors
}
void MotorForward(const Motor *motor){          //Change polarity for Motors input
    P4->OUT |= motor->pinA;
    P4->OUT &= ~(motor->pinB);
}
void MotorBackward(const Motor *motor){         //Change polarity for Motors input
    P4->OUT &= ~(motor->pinA);
    P4->OUT |= motor->pinB;
}

