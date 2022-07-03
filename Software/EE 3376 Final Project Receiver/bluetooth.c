#include <msp.h>
#include <string.h>
void set3Mhz(void){
    CS->KEY = CS_KEY_VAL;   //
    CS->CTL0 = 0;           //
    CS->CTL0 = CS_CTL0_DCORSEL_1;   //
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3; //
    CS->KEY = 0;            //
}
/*
 * @param Initialize UART 0
 * @return None
 * @note 38400 baud ratecccx
 *       1 stop bit
 *       0 parity
 */
void UART0_Init(void){
    //Enable UART_0
    P1->SEL0 |= (BIT2|BIT3);    //Enable special functions 0 for P1.2 and P1.3
    P1->SEL1 &= ~(BIT2|BIT3);   //Disable special functions 1 for P1.2 and P1.3

    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST;      //
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_UCSSEL_2;  //
    EUSCI_A0->BRW = 4;                         //
    EUSCI_A0->MCTLW |= (14 << EUSCI_A_MCTLW_BRF_OFS)|(EUSCI_A_MCTLW_OS16);       //
    EUSCI_A0->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);      //
    EUSCI_A0->IFG &= ~(EUSCI_A_IFG_RXIFG);          //
    EUSCI_A0->IE = EUSCI_A_IFG_RXIFG;               //

    NVIC->ISER[0] = 1 << ((EUSCIA0_IRQn) & 31);     //
}
/*
 * @param Initialize UART 2
 * @return None
 * @note 9600 baud ratecccx
 *       1 stop bit
 *       0 parity
 */
void UART2_Init(void){
    P3->SEL0 |= (BIT2|BIT3);    //
    P3->SEL1 &= ~(BIT2|BIT3);   //

    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST;     //
    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_UCSSEL_2;  //
    EUSCI_A2->BRW = 19;                          //
    EUSCI_A2->MCTLW = (9 << EUSCI_A_MCTLW_BRF_OFS)|(EUSCI_A_MCTLW_OS16);  //

    EUSCI_A2->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);      //
    EUSCI_A2->IFG &= ~(EUSCI_A_IFG_RXIFG);          //
    EUSCI_A2->IE |= EUSCI_A_IE_RXIE;                //

    NVIC->ISER[0] = 1 << ((EUSCIA2_IRQn) & 31);     //
}
/*
 *
 * @param str(string) to sent via bluetooth
 * @return None
 * @note 38400 baud ratecccx
 *       1 stop bit
 *       0 parity
 */
void sendUART_0(char *str){
    int i;      //
    for(i = 0; i < strlen(str); i++){       //
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));   //
            EUSCI_A0->TXBUF = str[i];           //
    }
}
/*
 *
 * @param str(string) to sent via bluetooth
 * @return None
 * @note 9600 baud ratecccx
 *       1 stop bit
 *       0 parity
 */
void sendUART_2(char *str){
    int i;          //
    for(i = 0; i < strlen(str); i++){           //
        while(!(EUSCI_A2->IFG & EUSCI_A_IFG_TXIFG));        //
            EUSCI_A2->TXBUF = str[i];           //
    }
}
/*
 * @param str(character) to sent via bluetooth
 * @return None
 * @note 3840000 baud ratecccx
 *       1 stop bit
 *       0 parity
 */
void sendCharUART_0(char c){
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = c;
    return;
}
/*
 * @param str(character) to sent via bluetooth
 * @return None
 * @note 9600 baud ratecccx
 *       1 stop bit
 *       0 parity
 */
void sendCharUART_2(char c){
    while(!(EUSCI_A2->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A2->TXBUF = c;
    return;
}
