/*
 * pwm.c
 *
 *  Created on: May 4, 2022
 *      Author: jorge
 */
#include "pwm.h"
#include "msp.h"
//search array used to move servo motor
int search[] = {4500, 6000, 4500, 3000, 4500, 6000, 4500, 3000,4500};
void PWM_Init(int period, int duty){
    P2->DIR |= (BIT6 | BIT7); // Initialize both PWM Channel and Port output
    P2->OUT &= ~(BIT7);
    P2->SEL0 |= BIT6;
    P2->SEL0 &= ~(BIT7);
    P2->SEL1 &= ~(BIT6 | BIT7);
                    //SMCLK                 Up-mode             Clear
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_1 | TIMER_A_CTL_CLR;
    TIMER_A0->CCR[0] = period - 1;          //Set period for PWM
    TIMER_A0->CCR[3] = duty - 1;            //Set channel frequency
    TIMER_A0->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7; //Reset and set mode

}
void PWM_FIRE(void){
    int i = 0;
    //Use search array to move servo
    for(; i < sizeof(search)/sizeof(int); i++){
        TIMER_A0->CCR[3] = search[i];
        __delay_cycles(20000);
    }
}
void Honk(int times){
    int i = 0;
    //Toggle buzzer output
    for( ; i < times; i++){
        P2->OUT ^= BIT7;
        __delay_cycles(500000);
    }
}


