/*
���ڣ�20230115
���ߣ�tom hanks
���ܣ��������
�汾��1.0
����޸����ݣ�����
�޸��ߣ� ����
*/
#include "Timer3.h"
#include "motor.h"
#include "stm32f10x.h"
/*
���ܣ�ͨ�ö�ʱ��2��ʼ��
������
1����װֵ
2��Ԥ��Ƶϵ��
3���ж���ռ���ȼ�
4���жϴμ����ȼ�
*/
int counter=0;//���ٻ�׼  ȡֵ��Χ��0<=counter<=ReloadValue  0����ֹ ReloadValue��ȫ��   
int times=0; //��ʱ���ж�ִ�д���
int times_max=10; //�������ֵ����װֵ

void Timer3Init(int ReloadeValue,int Prescaler,int PreemptionPriority,int SubPriority)
{
	//times_max=ReloadeValue;
	//��ʱ��3�ж�ͨ�����ȼ�����
	NVIC_InitTypeDef NVIC_Timer3;
	NVIC_Timer3.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;//��ռ���ȼ�
	NVIC_Timer3.NVIC_IRQChannelSubPriority=SubPriority;//�μ����ȼ�
	NVIC_Timer3.NVIC_IRQChannel=TIM3_IRQn;//�ж�ͨ��
	NVIC_Timer3.NVIC_IRQChannelCmd=ENABLE;//�ж����ȼ�ʹ��
	NVIC_Init(&NVIC_Timer3);
	/*
		��stm32f10xϵ�е�΢������ʱ��Ƶ��Ϊ72Mhz
	  ��ʱ����ʱ��Ƶ�ʼ�Timer2_ClockFrequency���£�
	  ��ʱ����ʱ��Ƶ��=΢������ʱ��Ƶ��/(Ԥ��Ƶϵ��+1)/��Ƶ����
    ��ʱ����������=��1/��ʱ��ʱ��Ƶ�ʣ�*��װֵ	
	*/
	//��ʱ��3����
	TIM_TimeBaseInitTypeDef TIM_Timer3;
	//������ʱ��2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	//��װֵ�Ĵ���ֵ
	TIM_Timer3.TIM_Period=ReloadeValue-1;
	//Ԥ��Ƶֵ
	TIM_Timer3.TIM_Prescaler=Prescaler-1;
	//��Ƶ����Ϊ1
	TIM_Timer3.TIM_ClockDivision=TIM_CKD_DIV1;
	//����ģʽ���ϼ���
	TIM_Timer3.TIM_CounterMode=TIM_CounterMode_Up;
	//�����ظ����������Ӽ�������
	TIM_Timer3.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3,&TIM_Timer3);
	//�����ʱ��3���±�־
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	//��ʱ��3�ж�ʹ��
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	//��ʱ��3������ʹ��
	TIM_Cmd(TIM3,ENABLE);	
}
//��ʱ��3�����ж�
/*
 ÿ���һ�μ�����ִ��һ�μ����ж�
*/
void TIM3_IRQHandler()
{  //��������ж�
	 if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	 {
			//ִ�С�������
		 times++;
		 if(counter==0)
		 {
				//����͵�ƽ���ʧ��
			 MotoeDisable();
		 }
		 else if(counter>0&&counter<=times_max)
		 {
			 if(times<=counter)
		   {
				 //����ߵ�ƽ���ʹ��
				 MotorEnable();			 
		   }
		   else
		   {
				 //����͵�ƽ���ʧ��
				 MotoeDisable();
		   }
		 }
     else
     {
			 //�쳣�׳������벻����Ҫ��
		 }			 
		 if(times==times_max)
		 {
				times=0;
		 }		 
		 //����жϱ�־λ
		 TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);
	 }	
}
void ChangeSpeed(int speed_value)
{
	counter=speed_value;
}