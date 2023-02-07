/*
���ߣ�tom hanks
���ڣ�20230124
���ܣ����
����޸����ڣ�20230127
�޸����ݣ�����ⲿ�ж������ģ�鷵�صĸߵ�ƽ�����ⲿ�ж��ж�ȡ�ߵ�ƽʱ��
�޸��ߣ�tom hanks
*/
#include "hc_sr04.h"
#include "delay.h"
#include "stm32f10x.h"
#include "usart1.h"
#include "Timer4.h"
int ReadEchoTimes=-1;//1����������1us
int finalstate=0;
void Hc_Sr04Init()
{
	//Echo ��Ӧ�������
	
	 //������Ӧ�������ʱ��
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	 GPIO_InitTypeDef GPIO_PB14;
	 GPIO_PB14.GPIO_Mode=GPIO_Mode_IPD;//��������Ĭ�ϵ͵�ƽ
	 GPIO_PB14.GPIO_Pin=GPIO_Pin_14;
	 GPIO_PB14.GPIO_Speed=GPIO_Speed_50MHz;
	 GPIO_Init(GPIOB,&GPIO_PB14);
	//����Echo ��Ӧ������ŵ��ⲿ�ж�
	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
	 EXTI_InitTypeDef EXTI_PB14;
	 EXTI_PB14.EXTI_Line=EXTI_Line14;
	 EXTI_PB14.EXTI_LineCmd=ENABLE;
	 EXTI_PB14.EXTI_Mode=EXTI_Mode_Interrupt;
	 EXTI_PB14.EXTI_Trigger=EXTI_Trigger_Rising;//�����ش���
	 EXTI_Init(&EXTI_PB14);
	//����Echo ��Ӧ������ŵ��ⲿ�ж����ȼ�
	 NVIC_InitTypeDef NVIC_PB14;
	 NVIC_PB14.NVIC_IRQChannel=EXTI15_10_IRQn;
	 NVIC_PB14.NVIC_IRQChannelCmd=ENABLE;
	 NVIC_PB14.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�
	 NVIC_PB14.NVIC_IRQChannelSubPriority=3;//�μ����ȼ�
	 NVIC_Init(&NVIC_PB14);
	
	
	//Triger ��ഥ������
	
	//������ഥ������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitTypeDef GPIO_PD8;
	GPIO_PD8.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_PD8.GPIO_Pin=GPIO_Pin_8;
	GPIO_PD8.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_PD8); 
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_8);//��������������Ϊ�͵�ƽ
  GPIO_ResetBits(GPIOB,GPIO_Pin_14);//����ഥ����������Ϊ�͵�ƽ	
}
//����̽������
void Triger()
{
	//������������̽��������ź�
	GPIO_SetBits(GPIOD,GPIO_Pin_8);//��������������Ϊ�ߵ�ƽ
	delay_us(10);//ά�ָߵ�ƽ10us
	GPIO_ResetBits(GPIOD,GPIO_Pin_8);//��������������Ϊ�͵�ƽ	
}
//��ȡ����̽������ķ���ʱ��
int ReadEchoTime()
{
	//�ȴ��ߵ�ƽ����
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==RESET)
	{
		
	}
	//����ߵ�ƽʱ�伴̽������Ĵ���ʱ��
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==SET)
	{
		//����һ��ѭ����ʱ���Լ1us
		delay_us(1);
		ReadEchoTimes++;
	}
	//̽������ķ���ʱ�䳬���Լ�����38ms��Ч
	if(ReadEchoTimes>=38000)
	{
		 return -1;
	}
	else if(ReadEchoTimes<38000)
	{
		int temp=ReadEchoTimes;
		ReadEchoTimes=0;//��ռ�������Ӱ����һ�ζ�ȡ
		return temp;
	}
}
//�����ϰ�����
float CalculateDistance(int times,int temperture,int humidity)
{
	int MicroSeconds=times/1000;//������ת��Ϊ����
	float Seconds=MicroSeconds/1000.0;//������ת��Ϊ����ھ�����ʧ
	/*
	//ʵ�����
	//���� m/s = 331.4 +��0.606 * �¶ȣ�+��0.0124 * ʪ�ȣ�
	*/
	float WaveSpeedReal=331.4+(0.606*temperture)+(0.0124*humidity);//ʵ�����
	float WaveSpeedIdeal=340;//�������
	return Seconds*WaveSpeedIdeal/2.0;	
}
//�ⲿ�жϺ���
void EXTI15_10_IRQHandler(void)
{
	
	//int temp=0;//ʱ�����
	if(EXTI_GetITStatus(EXTI_Line14)!=RESET)
	{
//		 //����ߵ�ƽʱ�伴̽������Ĵ���ʱ��
//	   while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==SET)
//	   {
//		    //����һ��ѭ����ʱ���Լ1us
//		    delay_us(1);
//		    temp++;
//	   }
//		 printf("��ȡʱ��ɹ�\r\n");		
//	  //̽������ķ���ʱ�䳬���Լ�����38ms��Ч
//	   if(temp>=38000)
//	   {
//		 
//	   }
//	   else if(temp<38000)
//	   {		     
//		     ReadEchoTimes=temp;         			 
//	   }
//		   TIM_SetCounter(TIM4,0);
//       Timer4Init(2,72,0,0);
		//�������ߵ�ƽʱ�䶨ʱ��
			TIM_Cmd(TIM4,ENABLE);
		//�ȴ���Ӧ�ߵ�ƽ�������
			while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==SET)
	   {
		    
	   }
		 //�رռ��ߵ�ƽʱ�䶨ʱ��
			TIM_Cmd(TIM4,DISABLE);
		  ReadEchoTimes=TIM_GetCounter(TIM4);
		 // printf("counter:%d\r\n",ReadEchoTimes);
		  TIM_SetCounter(TIM4,0);
		  finalstate=1;
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	
		
}