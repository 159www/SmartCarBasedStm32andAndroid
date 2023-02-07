#include "motor.h"
#include "stm32f10x.h"
/*
���ڣ�20230114
���ߣ�tom hanks
�汾��1.1
�޸����ڣ�20230115
�޸����ݣ�ȥ�����˶������еĵ��ʹ��ֱ�ӵ����˶�������ʧЧ�����˶�������Ҫ�ȳ�ʼ��timer2������PWM����
�޸��ߣ�tom hanks
*/
/*
�������ģ��Ϊ����L293DоƬ��оƬ����·���������·��������Ͽ��Կ����������
����·����ʵ��ֻ����·���뼴ʹ����·����ͬ����������ͬ����
input1<--->input3
input2<--->input4
*/
//��ʼ�������������
void MotorInit()
{
	 //������������ų�ʼ��
	
	//������������ų�ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitTypeDef GPIO_PD4;
	GPIO_PD4.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_PD4.GPIO_Pin=GPIO_Pin_4;
	GPIO_PD4.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOD,&GPIO_PD4);
	//����������1 IN1��ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_PC11;
	GPIO_PC11.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_PC11.GPIO_Pin=GPIO_Pin_11;
	GPIO_PC11.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_PC11);
	//����������2 IN2��ʼ��
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitTypeDef GPIO_PD0;
	GPIO_PD0.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_PD0.GPIO_Pin=GPIO_Pin_0;
	GPIO_PD0.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOD,&GPIO_PD0);
	
	//�Ҳ����������ų�ʼ��
	
	//�Ҳ����������ų�ʼ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitTypeDef GPIO_PD2;
	GPIO_PD2.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_PD2.GPIO_Pin=GPIO_Pin_2;
	GPIO_PD2.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOD,&GPIO_PD2);
	//�Ҳ��������3 IN3��ʼ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitTypeDef GPIO_PD6;
	GPIO_PD6.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_PD6.GPIO_Pin=GPIO_Pin_6;
	GPIO_PD6.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOD,&GPIO_PD6);
	//�Ҳ��������4 IN4��ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
	GPIO_InitTypeDef GPIO_PG9;
	GPIO_PG9.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_PG9.GPIO_Pin=GPIO_Pin_9;
	GPIO_PG9.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOG,&GPIO_PG9);
	GPIO_ResetBits(GPIOD,GPIO_Pin_4);
	GPIO_ResetBits(GPIOD,GPIO_Pin_2);	
}
//���Ҳ���ʹ��
void MotorEnable()
{
	GPIO_SetBits(GPIOD,GPIO_Pin_4);
	GPIO_SetBits(GPIOD,GPIO_Pin_2);		
}
//���Ҳ���ʧ��
void MotoeDisable()
{
	GPIO_ResetBits(GPIOD,GPIO_Pin_4);
	GPIO_ResetBits(GPIOD,GPIO_Pin_2);		
}
//���������ͬ��ת��
void LeftFrontMotor(int input1,int input2)
{
	if(input1==1)
	{
		 GPIO_SetBits(GPIOC,GPIO_Pin_11);
	}
	else if(input1==0)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_11);		
	}
	if(input2==1)
	{
		GPIO_SetBits(GPIOD,GPIO_Pin_0);
	}
	else if(input2==0)
	{	
		GPIO_ResetBits(GPIOD,GPIO_Pin_0);
	}	
}
//���������ͬ��ת��
void LeftBackMotor(int input1,int input2)
{	
	if(input1==1)
	{
		 GPIO_SetBits(GPIOC,GPIO_Pin_11);
	}
	else if(input1==0)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_11);		
	}
	if(input2==1)
	{
		GPIO_SetBits(GPIOD,GPIO_Pin_0);
	}
	else if(input2==0)
	{	
		GPIO_ResetBits(GPIOD,GPIO_Pin_0);
	}		
}
//�����Ҳ���ͬ��ת��
void RightFrontMotor(int input3,int input4)
{
	if(input3==1)
	{
		 GPIO_SetBits(GPIOD,GPIO_Pin_6);
	}
	else if(input3==0)
	{
		GPIO_ResetBits(GPIOD,GPIO_Pin_6);		
	}
	if(input4==1)
	{
		GPIO_SetBits(GPIOG,GPIO_Pin_9);
	}
	else if(input4==0)
	{	
		GPIO_ResetBits(GPIOG,GPIO_Pin_9);
	}			
}
//�����Ҳ���ͬ��ת��
void RightBackMotor(int input3,int input4)
{
	if(input3==1)
	{
		 GPIO_SetBits(GPIOD,GPIO_Pin_6);
	}
	else if(input3==0)
	{
		GPIO_ResetBits(GPIOD,GPIO_Pin_6);		
	}
	if(input4==1)
	{
		GPIO_SetBits(GPIOG,GPIO_Pin_9);
	}
	else if(input4==0)
	{	
		GPIO_ResetBits(GPIOG,GPIO_Pin_9);
	}				
}
//ǰ��
void go()
{
//	 //�����ʹ��
//	 GPIO_SetBits(GPIOD,GPIO_Pin_4);	
	 LeftFrontMotor(1,0); //�����ͬ����ת
//	 //�Ҳ���ʹ��
//	 GPIO_SetBits(GPIOD,GPIO_Pin_2);
	 RightFrontMotor(1,0); //�Ҳ���ͬ����ת	
}
//����
void back()
{
//	//�����ʹ��
//	 GPIO_SetBits(GPIOD,GPIO_Pin_4);	
	 LeftFrontMotor(0,1); //�����ͬ��ǰת
//	//�Ҳ���ʹ��
//	 GPIO_SetBits(GPIOD,GPIO_Pin_2);
	 RightFrontMotor(0,1); //�Ҳ���ͬ��ǰת
	
}
//��ת
void TurnLeft()
{
//	 //�Ҳ���ʹ��
//	 GPIO_SetBits(GPIOD,GPIO_Pin_2);
	 RightFrontMotor(1,0); //�Ҳ���ͬ����ת
//	 //�����ʧ��
//	 GPIO_ResetBits(GPIOD,GPIO_Pin_4);
   //�����ֹͣ
   LeftFrontMotor(0,0);	
}
//��ת
void TurnRight()
{
//	 //�����ʹ��
//	 GPIO_SetBits(GPIOD,GPIO_Pin_4);
	 LeftFrontMotor(1,0); //�����ͬ����ת
//	 //�Ҳ���ʧ��
//   GPIO_ResetBits(GPIOD,GPIO_Pin_2);
   //�Ҳ���ֹͣ
   RightFrontMotor(0,0);	
}
//ֹͣ
void stop(void)
{
	//�����ֹͣ
	LeftFrontMotor(0,0);
	//�Ҳ���ֹͣ
	RightFrontMotor(0,0);	
}