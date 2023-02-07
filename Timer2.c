/*
���ڣ�20230126
���ߣ�tom hanks
���ܣ��������
�汾��1.0
����޸����ݣ�����
�޸��ߣ� ����
*/
#include "Timer2.h"
#include "stm32f10x.h"
/*
 0.02(s)/100=0.0002(T) //����Ԥװ��ֵΪ100
 1/T=14400hz
 72000000/14400=5000(prescaler)//Ԥ��Ƶֵ5000 

*/
void Harder_PWM_ByTimer2Init(int prescaler,int peroid)
{
	 //1������Ӳ��PWM����A�˿�ʱ�Ӻ�AFIO���ù���ģ��ʱ��
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	 //2������Ӳ��PWM����A0��ģʽ
	 GPIO_InitTypeDef GPIO_PA0;
	 GPIO_PA0.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
	 GPIO_PA0.GPIO_Pin=GPIO_Pin_0;
	 GPIO_PA0.GPIO_Speed=GPIO_Speed_50MHz;
	 GPIO_Init(GPIOA,&GPIO_PA0);
	 //3��������ʱ��2ʱ��
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	 //4����ʼ����ʱ��2
	 TIM_TimeBaseInitTypeDef TIM_Timer2;
	 TIM_Timer2.TIM_RepetitionCounter=0;//���ظ�����
	 TIM_Timer2.TIM_Prescaler=prescaler-1;//Ԥ��Ƶϵ��
	 TIM_Timer2.TIM_ClockDivision=0;//��Ƶ����
	 TIM_Timer2.TIM_Period=peroid-1;//��װֵ
	 TIM_Timer2.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���
	 TIM_TimeBaseInit(TIM2,&TIM_Timer2);
	 //5��PWM����
	 TIM_OCInitTypeDef TIM_OC_PWM;
	 TIM_OC_PWM.TIM_OCMode=TIM_OCMode_PWM2;//��������С�ڱȽ�ֵʱ����ߵ�ƽ
	 TIM_OC_PWM.TIM_OCPolarity=TIM_OCPolarity_High;
	 TIM_OC_PWM.TIM_OutputState=ENABLE;
	 TIM_OC1Init(TIM2,&TIM_OC_PWM);//��ʼ��ͨ��һ
	 TIM_OC1PreloadConfig(TIM2,ENABLE);
	 TIM_Cmd(TIM2,ENABLE);	
}
void SetPluseWidth(int pluse_length)
{
	/*
	 87.5<pluse_length<97.5
	 һ����������0.0002s
	 pluse_length*0.0002=t��t��ʾ�ߵ�ƽ������
	 ��Ӧ��ϵ���£�
	 t = 0.5ms ������������ �����ת�� -90 ��
   t = 1.0ms ������������ �����ת�� -45��
   t = 1.5ms ������������ �����ת�� 0��
   t = 2.0ms ������������ �����ת�� 45��
   t = 2.5ms ������������ �����ת�� 90��
	*/
	 TIM_SetCompare1(TIM2,pluse_length);
	
}
