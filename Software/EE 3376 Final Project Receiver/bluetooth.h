/*
 * bluetooth.h
 *
 *  Created on: Apr 9, 2022
 *      Author: jorge
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#define BUFFER_SIZE 32
#define FIRE BIT2
#define BUZZ_ON_OFF BIT4
#define START_STOP BIT5
void set3Mhz(void);
void UART0_Init(void);
void UART2_Init(void);
void sendUART_0(char *str);
void sendUART_2(char *str);
void sendCharUART_0(char c);
void sendCharUART_2(char c);

#endif /* BLUETOOTH_H_ */
