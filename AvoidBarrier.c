/*
���ڣ�20220112
���ߣ�tom hanks
���ܣ�ʵ��С��ǰ�������ұ���
�汾��1.0
����޸����ڣ�����
�޸��ˣ�  ����
*/
#include "AvoidBarrier.h"
#include "LED.h"
#include "motor.h"
#include "stm32f10x.h"
void AvoidBarrierInit()
{
	/*
	���ļ����������ռ���ȼ��ʹμ����ȼ���ռ��λ�Ҳ�δ������ռ���ȼ�λ����NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	���ڵ��ñ���ʼ������ǰ����ռ���ȼ�λ������Ϊ2
	����ģ���⵽�ϰ��������ƽ���ɸ߱�Ͳ�����ƽ�½���
	*/
	//������ⲿ�жϳ�ʼ������
	
	//1������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
  //2������PG2ģʽ����
	GPIO_InitTypeDef GPIO_PG2;
	GPIO_PG2.GPIO_Pin=GPIO_Pin_2;
  GPIO_PG2.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_PG2.GPIO_Mode=GPIO_Mode_IPU;//��������
	GPIO_Init(GPIOG,&GPIO_PG2);
	//3���ж���·����
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource2);
	//4���ж���·����
	EXTI_InitTypeDef  EXTI_PG2;
	EXTI_PG2.EXTI_Mode=EXTI_Mode_Interrupt;//�ж�ģʽ
	EXTI_PG2.EXTI_Line=EXTI_Line2;//�ж���·2
	EXTI_PG2.EXTI_Trigger=EXTI_Trigger_Falling;//�½��ش���
	EXTI_PG2.EXTI_LineCmd=ENABLE;//��·ʹ��
	EXTI_Init(&EXTI_PG2);
	//5�����ȼ�����
	NVIC_InitTypeDef NVIC_PG2;
	NVIC_PG2.NVIC_IRQChannel=EXTI2_IRQn;//�ж�ͨ��2
	NVIC_PG2.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�0
	NVIC_PG2.NVIC_IRQChannelSubPriority=1;//�μ����ȼ�1
	NVIC_PG2.NVIC_IRQChannelCmd=ENABLE;//�ж����ȼ�ʹ��
	NVIC_Init(&NVIC_PG2);
	
	//�ұ����ⲿ�жϳ�ʼ������
	
	//1������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
  //2������PG2ģʽ����
	GPIO_InitTypeDef GPIO_PC7;
	GPIO_PC7.GPIO_Pin=GPIO_Pin_7;
  GPIO_PC7.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_PC7.GPIO_Mode=GPIO_Mode_IPU;//��������
	GPIO_Init(GPIOG,&GPIO_PC7);
	//3���ж���·����
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource7);
	//4���ж���·����
	EXTI_InitTypeDef  EXTI_PC7;
	EXTI_PC7.EXTI_Mode=EXTI_Mode_Interrupt;//�ж�ģʽ
	EXTI_PC7.EXTI_Line=EXTI_Line7;//�ж���·7
	EXTI_PC7.EXTI_Trigger=EXTI_Trigger_Falling;//�½��ش���
	EXTI_PC7.EXTI_LineCmd=ENABLE;//��·ʹ��
	EXTI_Init(&EXTI_PC7);
	//5�����ȼ�����
	NVIC_InitTypeDef NVIC_PC7;
	NVIC_PC7.NVIC_IRQChannel=EXTI9_5_IRQn;//�ж���·5��9����ͨ��
	NVIC_PC7.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�0
	NVIC_PC7.NVIC_IRQChannelSubPriority=1;//�μ����ȼ�1
	NVIC_PC7.NVIC_IRQChannelCmd=ENABLE;//�ж����ȼ�ʹ��
	NVIC_Init(&NVIC_PC7);
	
}
//������жϺ���
void EXTI2_IRQHandler()
{
		LED2_On();
	  LED3_Off();
	  TurnRight();
	
		
		EXTI_ClearITPendingBit(EXTI_Line2);//����жϱ�־λ
}
//�ұ����жϺ���
void EXTI9_5_IRQHandler()
{
	   //�ж���7�����ж�
		 if(EXTI_GetITStatus(EXTI_Line7)!=RESET)
		 {
			 LED2_Off();
			 LED3_On();
			 TurnLeft();
			 //����жϱ�־
			 EXTI_ClearITPendingBit(EXTI_Line7);
		 }
	
}
/*
�ñ��Ϻ���Ӧλ��main�����е���ѭ����
���ܣ�������ұ���ģ���δ��⵽�ϰ����
*/
void AvoidBarrierInMain()
{
	//���ұ���ģ���δ��⵽�ϰ���
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)==1&&GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_2)==1)
	{
		go();//С��ǰ��
	}
	//���ұ���ģ�����⵽�ϰ���
	else if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)==0&&GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_2)==0)
	{
		stop();//С��ֹͣ�ȴ�ң��ָ��
	}
	
}