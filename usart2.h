#ifndef __USART2_H
#define	__USART2_H


#include "stm32f10x.h"
#include <stdio.h>

/** 
  * ���ں궨�壬��ͬ�Ĵ��ڹ��ص����ߺ�IO��һ������ֲʱ��Ҫ�޸��⼸����
  */
	
// ����2-USART2
#define  USART2x                   USART2
#define  USART2_CLK                RCC_APB1Periph_USART2
#define  USART2_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  USART2_BAUDRATE           115200

// USART GPIO ���ź궨��
#define  USART2_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  USART2_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  USART2_TX_GPIO_PORT         GPIOA   
#define  USART2_TX_GPIO_PIN          GPIO_Pin_2
#define  USART2_RX_GPIO_PORT       GPIOA
#define  USART2_RX_GPIO_PIN        GPIO_Pin_3

#define  USART2_IRQ                USART2_IRQn
//#define  USART2_IRQHandler         USART2_IRQHandler
void USART2_Config(void);
void Usart2_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart2_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart2_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

#endif /* __USART_H */
