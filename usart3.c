/**
  ******************************************************************************
  * @file    bsp_usart.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   �ض���c��printf������usart�˿�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��STM32 F103-ָ���� ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
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
char Usart3CommandBuffer[200];//������ջ�����
char OnlyHaveCommandBuffer[22];//������֡������
int temp_index;
//unsigned char usart3_buffer[100];
int usart3_state=0;
unsigned char position[100];
int gprs_init_flag=0;
int locate_flag=0;

 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
//  /* Ƕ�������жϿ�������ѡ�� */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQ;
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
  * @brief  USART GPIO ����,������������
  * @param  ��
  * @retval ��
  */
void USART3_Config(void)
{
	OnlyHaveCommandBuffer[20]='\r';
	OnlyHaveCommandBuffer[21]='\n';
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	USART3_GPIO_APBxClkCmd(USART3_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	USART3_APBxClkCmd(USART3_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = USART3_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART3_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = USART3_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART3_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = USART3_BAUDRATE;
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
	USART_Init(USART3x, &USART_InitStructure);
	
	// �����ж����ȼ�����
	NVIC_Configuration();
	
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(USART3x, USART_IT_RXNE, ENABLE);	
	
	// ʹ�ܴ���
	USART_Cmd(USART3x, ENABLE);		

  // ���������ɱ�־
	//USART_ClearFlag(USART1, USART_FLAG_TC);     
}

/*****************  ����һ���ַ� **********************/
void Usart3_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  �����ַ��� **********************/
void Usart3_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart3_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  ����һ��16λ�� **********************/
void Usart3_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
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
// �����жϷ�����
void USART3_IRQHandler(void) 
{
  uint8_t ucTemp;
	if(USART_GetITStatus(USART3x,USART_IT_RXNE)!=RESET)
	{		
//		ucTemp = USART_ReceiveData(USART3x);
//    USART_SendData(USART3x,ucTemp);   
      USART_ClearITPendingBit(USART3, USART_IT_RXNE);//����նϱ�־
		  //���������ݷ��뻺���� 
		  ucTemp=USART3->DR;
		  if(ucTemp=='<') //����֡��ʼ
			{
				 temp_index=0; //���յ�ָ��ĳ���
				 while(1)
				 {
						if(USART_GetITStatus(USART3x,USART_IT_RXNE)!=RESET)
						{
							
							//USART_ClearITPendingBit(USART3, USART_IT_RXNE);//����նϱ� 
		          ucTemp=USART3->DR;
							if(ucTemp=='>')//����֡����
							{
								 printf("%s\r\n",OnlyHaveCommandBuffer);
								 
								 //�ж�ָ���
								 if(temp_index==2)//ǰ��
								 {
										if(isEqual(OnlyHaveCommandBuffer,"go",2)==1)
										{
												go();
										}
								 }
								 else if(temp_index==3)//���١������Ƕ�
								 {
									   //����
										 if(OnlyHaveCommandBuffer[temp_index-3]=='C'&&OnlyHaveCommandBuffer[temp_index-1]=='E')
										 {
												ChangeSpeed(OnlyHaveCommandBuffer[temp_index-2]-'0');
										 }//�����Ƕ�
										 else if(OnlyHaveCommandBuffer[temp_index-3]=='S')
										 {
											 SetPluseWidth((OnlyHaveCommandBuffer[temp_index-2]-'0')*10+(OnlyHaveCommandBuffer[temp_index-1]-'0'));
										 }
								 }
								 else if(temp_index==4)//���ˡ���ת����ת��ֹͣ
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
								 //��չ�������ȵ�����
								 
								 //....................................
								 ClearNull(OnlyHaveCommandBuffer,temp_index);//����������
								 break;
							}
							else
							{
								OnlyHaveCommandBuffer[temp_index++]=ucTemp;
							}
							USART_ClearITPendingBit(USART3, USART_IT_RXNE);//����նϱ� 
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
