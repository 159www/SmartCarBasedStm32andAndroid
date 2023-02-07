/**
  ******************************************************************************
  * @file    usart2.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火STM32 F103-指南者 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  //串口二用于外接设备的初始化、控制、接收数据和命令等功能
	//本应将中断函数放入stm32f10x_it.c中,为提高复用性就将USART2接收中断函数置于本文件
#include "usart2.h"
#include "usart1.h"
#include "stdlib.h"
#include "data.h"
#include "string.h"

//命令响应
//规定命令必须以\r\n结束
char Usart2CommandBuffer[200];//命令接收缓冲区
int Usart2Index=0;//命令接收缓冲区遍历下标
int const Usart2CommandLength=10; //命令长度
//+QMTRECV: 0,0,"$creq/9ccab73d-fae4-5858-b749-1dc975560751","ok"
//具体预设命令
char Usart2Command_Echo[Usart2CommandLength]={"00000000\r\n"}; //回声
char Usart2Command_Stop[Usart2CommandLength]={"11111111\r\n"};
char *MQTT_OPEN="+QMTOPEN: 0,0";
char *MQTT_CONNECT="+QMTCONN: 0,0,0";

 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void USART2_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
//  /* 嵌套向量中断控制器组选择 */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQ;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  USART2 GPIO 配置,工作参数配置
  * @param  无
  * @retval 无
  */
void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	USART2_GPIO_APBxClkCmd(USART2_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	USART2_APBxClkCmd(USART2_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = USART2_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = USART2_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART2_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = USART2_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(USART2x, &USART_InitStructure);
	
	// 串口中断优先级配置
	USART2_NVIC_Configuration();
	
	// 使能串口接收中断
	USART_ITConfig(USART2x, USART_IT_RXNE, ENABLE);
  //关闭串口发送中断
	USART_ITConfig(USART2x,USART_IT_TXE,DISABLE);
	// 清除发送完成标志
	USART_ClearFlag(USART2x, USART_FLAG_TC);  	
	// 使能串口
	USART_Cmd(USART2x, ENABLE);		

     
}



/*****************  发送一个字符 **********************/
void Usart2_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  发送字符串 **********************/
void Usart2_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart2_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  发送一个16位数 **********************/
void Usart2_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* 取出高八位 */
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;
	
	/* 发送高八位 */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* 发送低八位 */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}
//串口2接收中断函数
void USART2_IRQHandler(void)
{
	
	unsigned char ucTemp;
	if(USART_GetITStatus(USART2x,USART_IT_RXNE)!=RESET)//接收一个字节
	{		
		 USART_ClearITPendingBit(USART2x, USART_IT_RXNE);//清空终断标志
		  //将接收数据放入缓冲区
	   if(USART2x->DR)
		 {
		 ucTemp=USART2x->DR;
		 Usart2CommandBuffer[Usart2Index++]=ucTemp;
		 USART_SendData(USART1,ucTemp);     	
		 }
		 if(Usart2Index>199)
		 {
				Usart2Index=0;
		 }		 		
	}	
	
}


