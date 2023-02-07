#ifndef __USART1_H
#define	__USART1_H


#include <stm32f10x.h>
#include <stdio.h>
#include "data.h"

/*                                   
 * 串口宏定义，不同的串口挂载的总线和IO不一样，移植时需要修改这几个宏
 * 1-修改总线时钟的宏，uart1挂载到apb2总线，其他uart挂载到apb1总线
 * 2-修改GPIO的宏
 */

	
// 串口1-USART1
#define  USART1x                   USART1
#define  USART1_CLK                RCC_APB2Periph_USART1
#define  USART1_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  USART1_BAUDRATE           115200

// USART GPIO 引脚宏定义
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
