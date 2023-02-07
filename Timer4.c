#include "Timer4.h"
#include "usart1.h"
#include "stm32f10x.h"
int HighPluseWidthTimes=-1;//�ߵ�ƽʱ�������־
int temp_HighPluseWidthTimes=0;

void Timer4Init(int ReloadeValue,int Prescaler,int PreemptionPriority,int SubPriority)
{
	//��ʱ��4�ж�ͨ�����ȼ�����
	NVIC_InitTypeDef NVIC_Timer4;
	NVIC_Timer4.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;//��ռ���ȼ�
	NVIC_Timer4.NVIC_IRQChannelSubPriority=SubPriority;//�μ����ȼ�
	NVIC_Timer4.NVIC_IRQChannel=TIM4_IRQn;//�ж�ͨ��
	NVIC_Timer4.NVIC_IRQChannelCmd=ENABLE;//�ж����ȼ�ʹ��
	NVIC_Init(&NVIC_Timer4);
	/*
		��stm32f10xϵ�е�΢������ʱ��Ƶ��Ϊ72Mhz
	  ��ʱ����ʱ��Ƶ�ʼ�Timer2_ClockFrequency���£�
	  ��ʱ����ʱ��Ƶ��=΢������ʱ��Ƶ��/(Ԥ��Ƶϵ��+1)/��Ƶ����
    ��ʱ����������=��1/��ʱ��ʱ��Ƶ�ʣ�*��װֵ	
	*/
	//��ʱ��4����
	TIM_TimeBaseInitTypeDef TIM_Timer4;
	//������ʱ��2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	//��װֵ�Ĵ���ֵ
	TIM_Timer4.TIM_Period=ReloadeValue-1;
	//Ԥ��Ƶֵ
	TIM_Timer4.TIM_Prescaler=Prescaler-1;
	//��Ƶ����Ϊ1
	TIM_Timer4.TIM_ClockDivision=TIM_CKD_DIV1;
	//����ģʽ���ϼ���
	TIM_Timer4.TIM_CounterMode=TIM_CounterMode_Up;
	//�����ظ����������Ӽ�������
	TIM_Timer4.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM4,&TIM_Timer4);
	//�����ʱ��4���±�־
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	//��ʱ��4�ж�ʹ��
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	//��ʱ��4������ʹ��
	//TIM_Cmd(TIM4,ENABLE);
  TIM_Cmd(TIM4,DISABLE);	
}

//��ʱ��2�����ж�
/*
 ÿ���һ�μ�����ִ��һ�μ����ж�
*/
void TIM4_IRQHandler()
{  //��������ж�
	 if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
	 {
//		 if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==RESET)
//		 {
//			 //��ʱ��4������ʧ��
//	     TIM_Cmd(TIM4,DISABLE);
//			 TIM_SetCounter(TIM4,0);
//       HighPluseWidthTimes=temp_HighPluseWidthTimes;
//			 if(HighPluseWidthTimes<38000)
//			 {
//			 printf("���ɹ�:%d\r\n",HighPluseWidthTimes);
//			 double ok=(HighPluseWidthTimes*170/1000000.0);
//			 printf("����:%lfm",ok);
//			 }
//			 temp_HighPluseWidthTimes=0;
//       finalstate=1;			 
//		 }
//		 else
//		 {
//			 temp_HighPluseWidthTimes++;
//		 }
			
		 //����жϱ�־λ
		 TIM_ClearITPendingBit(TIM4,TIM_FLAG_Update);
	 }	
}