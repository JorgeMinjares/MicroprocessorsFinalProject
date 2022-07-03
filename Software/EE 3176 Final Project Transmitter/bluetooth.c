#include <msp.h>
#include <string.h>
void set3Mhz(void){
    CS->KEY = CS_KEY_VAL;
    CS->CTL0 = 0;
    CS->CTL0 = CS_CTL0_DCORSEL_1;
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
    CS->KEY = 0;
}
void UART0_Init(void){
    //Enable UART_0
    P1->SEL0 |= (BIT2|BIT3);    //Enable special functions 0 for P1.2 and P1.3
    P1->SEL1 &= ~(BIT2|BIT3);   //Disable special functions 1 for P1.2 and P1.3

    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST;              // Enable Software Reset
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_UCSSEL_2;          // Choose UART clock source
    EUSCI_A0->BRW = 4;                                  // Calculate Baurdrate - 38400
    EUSCI_A0->MCTLW |= (14 << EUSCI_A_MCTLW_BRF_OFS)|(EUSCI_A_MCTLW_OS16);       // Over sample UART for package loss
    EUSCI_A0->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);          // Clear Software Reset
    EUSCI_A0->IFG &= ~(EUSCI_A_IFG_RXIFG);              // Clear UART Interrupt Flag
    EUSCI_A0->IE = EUSCI_A_IFG_RXIFG;                   // Enable UART Interrupt

    NVIC->ISER[0] = 1 << ((EUSCIA0_IRQn) & 31);         //Enable UART NVIC configuration
}
void UART2_Init(void){
    P3->SEL0 |= (BIT2|BIT3);    //Enable special functions 0 for P3.2 and P3.3
    P3->SEL1 &= ~(BIT2|BIT3);   //Disable special functions 1 for P3.2 and P3.3

    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST;             // Enable Software Reset
    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_UCSSEL_2;          // Choose UART clock source
    EUSCI_A2->BRW = 19;                                 // Calculate Baurdrate - 9600
    EUSCI_A2->MCTLW = (9 << EUSCI_A_MCTLW_BRF_OFS)|(EUSCI_A_MCTLW_OS16);  //Over sample UART for package loss
    EUSCI_A2->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);          // Clear Software Reset
    EUSCI_A2->IFG &= ~(EUSCI_A_IFG_RXIFG);              // Clear UART Interrupt
    EUSCI_A2->IE |= EUSCI_A_IE_RXIE;                    // Enable UART Interrupt

    NVIC->ISER[0] = 1 << ((EUSCIA2_IRQn) & 31);         //Enable UART NVIC configuration
}
void sendUART_0(char *str){
    int i;
    for(i = 0; i < strlen(str); i++){                   //  Iterate over the char array
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));    //  Check if Transmitter is ready to Transmit
            EUSCI_A0->TXBUF = str[i];                   // Transmit current character to UART 0
    }
}
void sendUART_2(char *str){
    int i;
    for(i = 0; i < strlen(str); i++){                   // Iterate over the char array
        while(!(EUSCI_A2->IFG & EUSCI_A_IFG_TXIFG));    // Check if Transmitter is ready to Transmit
            EUSCI_A2->TXBUF = str[i];                   // Transmit current character to UART 2
    }
}
void sendCharUART_0(char c){
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));        //  Check if Transmitter is ready to Transmit
    EUSCI_A0->TXBUF = c;                                // Transmit character to UART 0
    return;
}
void sendCharUART_2(char c){
    while(!(EUSCI_A2->IFG & EUSCI_A_IFG_TXIFG));        // Check if Transmitter is ready to Transmit
    EUSCI_A2->TXBUF = c;                                // Transmit character to UART 2
    return;
}
