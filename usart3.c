/**
  ******************************************************************************
  * @file    bsp_usart.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   重定向c库printf函数到usart端口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火STM32 F103-指南者 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
	
#include "usart3.h"
#include "usart1.h"
#include "data.h"
#include "string.h"
#include "motor.h"
#include "Timer3.h"
#include "Timer2.h"
char Usart3CommandBuffer[200];//命令接收缓冲区
char OnlyHaveCommandBuffer[22];//纯命令帧储存区
int temp_index;
//unsigned char usart3_buffer[100];
int usart3_state=0;
unsigned char position[100];
int gprs_init_flag=0;
int locate_flag=0;

 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
//  /* 嵌套向量中断控制器组选择 */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQ;
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
  * @brief  USART GPIO 配置,工作参数配置
  * @param  无
  * @retval 无
  */
void USART3_Config(void)
{
	OnlyHaveCommandBuffer[20]='\r';
	OnlyHaveCommandBuffer[21]='\n';
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	USART3_GPIO_APBxClkCmd(USART3_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	USART3_APBxClkCmd(USART3_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = USART3_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART3_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = USART3_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART3_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = USART3_BAUDRATE;
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
	USART_Init(USART3x, &USART_InitStructure);
	
	// 串口中断优先级配置
	NVIC_Configuration();
	
	// 使能串口接收中断
	USART_ITConfig(USART3x, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(USART3x, ENABLE);		

  // 清除发送完成标志
	//USART_ClearFlag(USART1, USART_FLAG_TC);     
}

/*****************  发送一个字符 **********************/
void Usart3_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  发送字符串 **********************/
void Usart3_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart3_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  发送一个16位数 **********************/
void Usart3_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
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
// 串口中断服务函数
void USART3_IRQHandler(void) 
{
  uint8_t ucTemp;
	if(USART_GetITStatus(USART3x,USART_IT_RXNE)!=RESET)
	{		
//		ucTemp = USART_ReceiveData(USART3x);
//    USART_SendData(USART3x,ucTemp);   
      USART_ClearITPendingBit(USART3, USART_IT_RXNE);//清空终断标志
		  //将接收数据放入缓冲区 
		  ucTemp=USART3->DR;
		  if(ucTemp=='<') //命令帧开始
			{
				 temp_index=0; //接收到指令的长度
				 while(1)
				 {
						if(USART_GetITStatus(USART3x,USART_IT_RXNE)!=RESET)
						{
							
							//USART_ClearITPendingBit(USART3, USART_IT_RXNE);//清空终断标 
		          ucTemp=USART3->DR;
							if(ucTemp=='>')//命令帧结束
							{
								 printf("%s\r\n",OnlyHaveCommandBuffer);
								 
								 //判断指令含义
								 if(temp_index==2)//前进
								 {
										if(isEqual(OnlyHaveCommandBuffer,"go",2)==1)
										{
												go();
										}
								 }
								 else if(temp_index==3)//调速、调测距角度
								 {
									   //调试
										 if(OnlyHaveCommandBuffer[temp_index-3]=='C'&&OnlyHaveCommandBuffer[temp_index-1]=='E')
										 {
												ChangeSpeed(OnlyHaveCommandBuffer[temp_index-2]-'0');
										 }//调测距角度
										 else if(OnlyHaveCommandBuffer[temp_index-3]=='S')
										 {
											 SetPluseWidth((OnlyHaveCommandBuffer[temp_index-2]-'0')*10+(OnlyHaveCommandBuffer[temp_index-1]-'0'));
										 }
								 }
								 else if(temp_index==4)//后退、左转、右转、停止
								 {
										 if(isEqual(OnlyHaveCommandBuffer,"back",4)==1)
										 {
												 back();
											 
										 }
										 else if(isEqual(OnlyHaveCommandBuffer,"left",4)==1)
										 {
												 TurnLeft();
										 }
										 else if(isEqual(OnlyHaveCommandBuffer,"stop",4)==1)
										 {
												stop();
										 }		 
								 }
								 else if(temp_index==5)
								 {
										if(isEqual(OnlyHaveCommandBuffer,"right",5)==1)
										{
												 TurnRight();
										}	 
								 }
								 //扩展其它长度的命令
								 
								 //....................................
								 ClearNull(OnlyHaveCommandBuffer,temp_index);//清空命令缓冲区
								 break;
							}
							else
							{
								OnlyHaveCommandBuffer[temp_index++]=ucTemp;
							}
							USART_ClearITPendingBit(USART3, USART_IT_RXNE);//清空终断标 
						}
				 }
			}
			else
			{
		  Usart3CommandBuffer[usart3_state++]=ucTemp;
			}
			if( usart3_state>199)
			{
				 usart3_state=0;
				 clear(Usart3CommandBuffer,200);
			}
		 		   
  }
		      		
		 
}
