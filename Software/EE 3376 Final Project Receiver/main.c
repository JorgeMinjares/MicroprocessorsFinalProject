#include "msp.h"
#include "bluetooth.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "motor.h"
#include "pwm.h"
// Initialize motor pins
Motor motor1 = {.pinA = BIT0, .pinB = BIT1, .enable = BIT4};
Motor motor2 = {.pinA = BIT2, .pinB = BIT3, .enable = BIT5};
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer
    set3Mhz();                                  // Set DCO to 3Mhz

    MotorInit(&motor1);    // initialize Left Track Motor
    MotorInit(&motor2);    // initialize Right Track Motor
    PWM_Init(60000, 4500); // initialize Servo Motor (Period, 7.5% for Servo)
    UART2_Init();          // Initialize UART 2

    __enable_irq();
    //  SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;
    while (1)
    {
    }
}
void EUSCIA2_IRQHandler(void)
{
    if (EUSCI_A2->IFG & EUSCI_A_IFG_RXIFG)
    { // Check if UART received information
        char c = EUSCI_A2->RXBUF;
        if (c == 'F')
        {
            PWM_FIRE();
        }
        else if (c == 'H')
        { // Honk
            Honk(4);
        }
        else if (c == 'W')
        { // Move Tank Forward
            MotorBackward(&motor1);
            MotorForward(&motor2);
        }
        else if (c == 'S')
        { // Move Tank Backward
            MotorForward(&motor1);
            MotorBackward(&motor2);
        }
        else if (c == 'D')
        { // Move Tank Right
            MotorOff(&motor1);
            MotorForward(&motor2);
        }
        else if (c == 'A')
        { // Move Tank Left
            MotorBackward(&motor1);
            MotorOff(&motor2);
        }
        else
        { // Turn Off both Motors
            MotorOff(&motor1);
            MotorOff(&motor2);
        }
    }
}
