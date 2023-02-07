#include "Timer5.h"
#include "stm32f10x.h"
#include "hc_sr04.h"
#include  "usart1.h"
void Timer5Init(int ReloadeValue,int Prescaler,int PreemptionPriority,int SubPriority)
{
	 //��ʱ��3�ж�ͨ�����ȼ�����
	NVIC_InitTypeDef NVIC_Timer5;
	NVIC_Timer5.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;//��ռ���ȼ�
	NVIC_Timer5.NVIC_IRQChannelSubPriority=SubPriority;//�μ����ȼ�
	NVIC_Timer5.NVIC_IRQChannel=TIM5_IRQn;//�ж�ͨ��
	NVIC_Timer5.NVIC_IRQChannelCmd=ENABLE;//�ж����ȼ�ʹ��
	NVIC_Init(&NVIC_Timer5);
	/*
		��stm32f10xϵ�е�΢������ʱ��Ƶ��Ϊ72Mhz
	  ��ʱ����ʱ��Ƶ�ʼ�Timer2_ClockFrequency���£�
	  ��ʱ����ʱ��Ƶ��=΢������ʱ��Ƶ��/(Ԥ��Ƶϵ��+1)/��Ƶ����
    ��ʱ����������=��1/��ʱ��ʱ��Ƶ�ʣ�*��װֵ	
	*/
	//��ʱ��3����
	TIM_TimeBaseInitTypeDef TIM_Timer5;
	//������ʱ��2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	//��װֵ�Ĵ���ֵ
	TIM_Timer5.TIM_Period=ReloadeValue-1;
	//Ԥ��Ƶֵ
	TIM_Timer5.TIM_Prescaler=Prescaler-1;
	//��Ƶ����Ϊ1
	TIM_Timer5.TIM_ClockDivision=TIM_CKD_DIV1;
	//����ģʽ���ϼ���
	TIM_Timer5.TIM_CounterMode=TIM_CounterMode_Up;
	//�����ظ����������Ӽ�������
	TIM_Timer5.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM5,&TIM_Timer5);
	//�����ʱ��5���±�־
	TIM_ClearFlag(TIM5,TIM_FLAG_Update);
	//��ʱ��5�ж�ʹ��
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	//��ʱ��5������ʹ��
	TIM_Cmd(TIM5,ENABLE);		
}
//��ʱ��5�����ж�
/*
 ÿ���һ�μ�����ִ��һ�μ����ж�
*/
void TIM5_IRQHandler()
{  //��������ж�
	 if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET)
	 {
			//printf("��ʼ���\r\n");
		  Triger();
		 //����жϱ�־λ
		 TIM_ClearITPendingBit(TIM5,TIM_FLAG_Update);
	 }	
}
