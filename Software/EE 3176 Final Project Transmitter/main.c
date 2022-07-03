#include "msp.h"
#include "bluetooth.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#define DATA_SIZE 5
uint8_t index = 0;

typedef struct
{
    uint32_t x_axis[DATA_SIZE]; // array for data gather from ADC corresponding for x-axis
    uint32_t y_axis[DATA_SIZE]; // array for data gather from ADC corresponding for x-axis
    uint32_t z_axis[DATA_SIZE]; // array for data gather from ADC corresponding for x-axis
} ADXL335_t;

ADXL335_t sensor;

void ADC_Setup(void);
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer

    set3Mhz(); // Set DCO to 3Mhz

    P4->DIR &= ~(0xB0); // Enable Port inputs as Pull-ups
    P4->OUT |= (0xB0);
    P4->REN |= (0xB0);

    SysTick->CTRL = 0x07; // Enable Systick Timer
    SysTick->VAL = 1 - 1;
    SysTick->LOAD = 100000; // 33ms - 30 ADC Readings per second
    UART2_Init();           // Initialize UART 2
    ADC_Setup();            // Set up adc channels
                 // Using A5, A4, A3 : P5.0 - P5.2
    __enable_irq();

    uint8_t ticStart = 0; // Variable to reutilize button 3
    sendCharUART_2('H');  // Turn on buzzer to confirm connection
    while (1)
    {
        uint8_t result = P4->IN; // Check button input using polling method
        __delay_cycles(500000);
        if ((result & 0x10) == 0)
        { // Button 1 pressed simulate tank firing
            sendCharUART_2('F');
        }
        if ((result & 0x20) == 0)
        { // Button 2 pressed turn on Buzzer aka Honk Tank
            sendCharUART_2('H');
        }
        if ((result & 0x80) == 0)
        { // Button 3 Turn on and Turn off Systick Interrupt
            if (ticStart == 0)
            {
                SysTick->CTRL &= ~(0x02);
                ticStart++;
            }
            else
            {
                SysTick->CTRL |= 0x02;
                ticStart = 0;
            }
        }
    }
}
void SysTick_Handler(void)
{ // Sample ADC channel using Systick
    ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
}
void ADC_Setup(void)
{
    P5->SEL0 |= (BIT0 | BIT1); // Select P5.0 and P5.1 as ADC Channels
    P5->SEL1 &= ~(BIT0 | BIT1);

    ADC14->CTL0 |= ADC14_CTL0_SHP |                         // Sampling using Timers
                   ADC14_CTL0_CONSEQ_1 |                    // Sample mulitple Channels
                   ADC14_CTL0_MSC |                         // Multiple samples and conversions
                   ADC14_CTL0_ON;                           // Turn on ADC14
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_5;                    // Enable P5.0 as ADC14 Channel
    ADC14->MCTL[1] |= ADC14_MCTLN_INCH_4 | ADC14_MCTLN_EOS; // Enable P5.1 as ADC14 Channel and Final ADC Channel
    ADC14->CTL1 |= ADC14_CTL1_RES__14BIT;                   // Uses 16 clock cycle conversion time
    ADC14->IER0 |= ADC14_IER0_IE1;                          // Enable ADC14 Interrupt as interrupt 1
    NVIC->ISER[0] = 1 << ((ADC14_IRQn)&31);                 // Enable ADC14 NVIC table
}
void ADC14_IRQHandler(void)
{
    if (ADC14->IFGR0 & ADC14_IFGR0_IFG1)
    {
        // check if ADC14 channel P5.1 has read
        // capture data and store into data structure
        sensor.x_axis[index] = ADC14->MEM[0];
        sensor.y_axis[index] = ADC14->MEM[1];

        if (index++ > DATA_SIZE)
        { // check if index is reached threshold

            uint32_t axis_data[2] = {0, 0}; // create temp variable

            int i;
            for (i = 0; i < DATA_SIZE; i++)
            { // iterate over the data structure axis readings
                // store in temp variables
                axis_data[0] += sensor.x_axis[i];
                axis_data[1] += sensor.y_axis[i];
            }
            // get the average reading of the axis data
            axis_data[0] /= DATA_SIZE;
            axis_data[1] /= DATA_SIZE;
            int x = axis_data[0];
            int y = axis_data[1];
            // Check if the ADC14 average is within the range for movements
            if (x >= 8000 && x < 9000 && y >= 8000 && y < 8500)
            { // put tank in sleep mode
                sendCharUART_2('P');
            }
            else if (x >= 8000 && x < 9000 && y >= 8501 && y < 9999)
            { // moving foward
                sendCharUART_2('W');
            }
            else if (x >= 8000 && x < 9000 && y >= 6000 && y < 7999)
            { // moving backwards
                sendCharUART_2('S');
            }
            else if (x >= 9000 && x < 9999 && y >= 8000 && y < 9000)
            { // Turn right
                sendCharUART_2('D');
            }
            else if (x >= 6000 && x < 6999 && y >= 7000 && y < 8000)
            { // turn left
                sendCharUART_2('A');
            }
            index = 0; // restart index for future average measurement
        }
    }
}
