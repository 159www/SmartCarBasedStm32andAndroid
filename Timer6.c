#include "Timer6.h"
#include "usart1.h"
#include "hc_sr04.h"
#include "stm32f10x.h"
#include "data.h"
#include "BC26.h"
char distance[50];
extern int ReadEchoTimes;//1����������1us
extern int finalstate;
void Timer6Init(int ReloadeValue,int Prescaler,int PreemptionPriority,int SubPriority)
{
	distance[48]='\r';
	distance[49]='\n';
	 //��ʱ��6�ж�ͨ�����ȼ�����
	NVIC_InitTypeDef NVIC_Timer6;
	NVIC_Timer6.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;//��ռ���ȼ�
	NVIC_Timer6.NVIC_IRQChannelSubPriority=SubPriority;//�μ����ȼ�
	NVIC_Timer6.NVIC_IRQChannel=TIM6_IRQn;//�ж�ͨ��
	NVIC_Timer6.NVIC_IRQChannelCmd=ENABLE;//�ж����ȼ�ʹ��
	NVIC_Init(&NVIC_Timer6);
	
	//��ʱ��6����
	TIM_TimeBaseInitTypeDef TIM_Timer6;
	//������ʱ��6ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	//��װֵ�Ĵ���ֵ
	TIM_Timer6.TIM_Period=ReloadeValue-1;
	//Ԥ��Ƶֵ
	TIM_Timer6.TIM_Prescaler=Prescaler-1;
	//��Ƶ����Ϊ1
	TIM_Timer6.TIM_ClockDivision=TIM_CKD_DIV1;
	//����ģʽ���ϼ���
	TIM_Timer6.TIM_CounterMode=TIM_CounterMode_Up;
	//�����ظ����������Ӽ�������
	TIM_Timer6.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM6,&TIM_Timer6);
	//�����ʱ��6���±�־
	TIM_ClearFlag(TIM6,TIM_FLAG_Update);
	//��ʱ��6�ж�ʹ��
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	//��ʱ��6������ʹ��
	TIM_Cmd(TIM6,ENABLE);		
}
//��ʱ��6�����ж�
/*
 ÿ���һ�μ�����ִ��һ�μ����ж�
*/
void TIM6_IRQHandler()
{  //��������ж�
	 if(TIM_GetITStatus(TIM6,TIM_IT_Update)!=RESET)
	 {
			printf("�ϱ���������\r\n");
		   if(finalstate==1)
		 {
			 if(ReadEchoTimes<38000)
			 {
			// printf("���ɹ�:%d\r\n",ReadEchoTimes);
			 double ok=(ReadEchoTimes*0.00017);
			// printf("����:%lfm\r\n",ok);
			 sprintf(distance,"%lf",ok);
			 BC26_Send("T",distance);
			 ClearNull(distance,48);
			 }
			 finalstate=0;			 
		 }
		 //����жϱ�־λ
		 TIM_ClearITPendingBit(TIM6,TIM_FLAG_Update);
	 }	
}