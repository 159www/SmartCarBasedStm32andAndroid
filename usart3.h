#ifndef __USART3_H
#define __USART3_H
#include "stm32f10x.h"
#include <stdio.h>
// ´®¿Ú3-USART3
#define  USART3x                   USART3
#define  USART3_CLK                RCC_APB1Periph_USART3
#define  USART3_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  USART3_BAUDRATE           9600


#define  USART3_GPIO_CLK           (RCC_APB2Periph_GPIOB)
#define  USART3_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  USART3_TX_GPIO_PORT         GPIOB   
#define  USART3_TX_GPIO_PIN          GPIO_Pin_10
#define  USART3_RX_GPIO_PORT       GPIOB
#define  USART3_RX_GPIO_PIN        GPIO_Pin_11

#define  USART3_IRQ                USART3_IRQn
//#define  USART3_IRQHandler         USART3_IRQHandler

void USART3_Config(void);
void Usart3_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart3_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart3_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

#endif