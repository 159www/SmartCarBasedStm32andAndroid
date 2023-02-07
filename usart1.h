#ifndef __USART1_H
#define	__USART1_H


#include <stm32f10x.h>
#include <stdio.h>
#include "data.h"

/*                                   
 * ���ں궨�壬��ͬ�Ĵ��ڹ��ص����ߺ�IO��һ������ֲʱ��Ҫ�޸��⼸����
 * 1-�޸�����ʱ�ӵĺ꣬uart1���ص�apb2���ߣ�����uart���ص�apb1����
 * 2-�޸�GPIO�ĺ�
 */

	
// ����1-USART1
#define  USART1x                   USART1
#define  USART1_CLK                RCC_APB2Periph_USART1
#define  USART1_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  USART1_BAUDRATE           115200

// USART GPIO ���ź궨��
#define   USART1_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  USART1_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  USART1_TX_GPIO_PORT       GPIOA   
#define  USART1_TX_GPIO_PIN        GPIO_Pin_9
#define  USART1_RX_GPIO_PORT       GPIOA
#define  USART1_RX_GPIO_PIN        GPIO_Pin_10

#define  USART1_IRQ                USART1_IRQn



void USART1_Config(void);
void Usart1_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart1_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart1_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);
//void USART1_IRQHandler(void);
#endif /* __USART_H */
