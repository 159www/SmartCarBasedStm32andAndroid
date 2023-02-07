/**
  ******************************************************************************
  * @file    usart2.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��STM32 F103-ָ���� ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  //���ڶ���������豸�ĳ�ʼ�������ơ��������ݺ�����ȹ���
	//��Ӧ���жϺ�������stm32f10x_it.c��,Ϊ��߸����Ծͽ�USART2�����жϺ������ڱ��ļ�
#include "usart2.h"
#include "usart1.h"
#include "stdlib.h"
#include "data.h"
#include "string.h"

//������Ӧ
//�涨���������\r\n����
char Usart2CommandBuffer[200];//������ջ�����
int Usart2Index=0;//������ջ����������±�
int const Usart2CommandLength=10; //�����
//+QMTRECV: 0,0,"$creq/9ccab73d-fae4-5858-b749-1dc975560751","ok"
//����Ԥ������
char Usart2Command_Echo[Usart2CommandLength]={"00000000\r\n"}; //����
char Usart2Command_Stop[Usart2CommandLength]={"11111111\r\n"};
char *MQTT_OPEN="+QMTOPEN: 0,0";
char *MQTT_CONNECT="+QMTCONN: 0,0,0";

 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void USART2_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
//  /* Ƕ�������жϿ�������ѡ�� */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQ;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  USART2 GPIO ����,������������
  * @param  ��
  * @retval ��
  */
void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	USART2_GPIO_APBxClkCmd(USART2_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	USART2_APBxClkCmd(USART2_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = USART2_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = USART2_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART2_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = USART2_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(USART2x, &USART_InitStructure);
	
	// �����ж����ȼ�����
	USART2_NVIC_Configuration();
	
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(USART2x, USART_IT_RXNE, ENABLE);
  //�رմ��ڷ����ж�
	USART_ITConfig(USART2x,USART_IT_TXE,DISABLE);
	// ���������ɱ�־
	USART_ClearFlag(USART2x, USART_FLAG_TC);  	
	// ʹ�ܴ���
	USART_Cmd(USART2x, ENABLE);		

     
}



/*****************  ����һ���ַ� **********************/
void Usart2_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  �����ַ��� **********************/
void Usart2_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart2_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  ����һ��16λ�� **********************/
void Usart2_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* ȡ���߰�λ */
	temp_h = (ch&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = ch&0XFF;
	
	/* ���͸߰�λ */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}
//����2�����жϺ���
void USART2_IRQHandler(void)
{
	
	unsigned char ucTemp;
	if(USART_GetITStatus(USART2x,USART_IT_RXNE)!=RESET)//����һ���ֽ�
	{		
		 USART_ClearITPendingBit(USART2x, USART_IT_RXNE);//����նϱ�־
		  //���������ݷ��뻺����
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


