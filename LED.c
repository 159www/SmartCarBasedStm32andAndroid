/*
���ڣ�20230112
���ߣ�tom hanks
���ܣ�LED�������
�汾��1.0
����޸����ڣ�����
�޸��ˣ�����
*/
#include "LED.h"
#include "stm32f10x.h"
void LED_Init()
{
	/*
	LED2<---->PB0
	LED3<---->PB8
	LED���������ķ�ʽ����
	*/
	//����B�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//�����������ýṹ��
	GPIO_InitTypeDef gpio_b_structure_initer;
	//��������PB0
	gpio_b_structure_initer.GPIO_Mode=GPIO_Mode_Out_PP;//�������ģʽ
	gpio_b_structure_initer.GPIO_Pin=GPIO_Pin_0;//����0
	gpio_b_structure_initer.GPIO_Speed=GPIO_Speed_50MHz;//Ƶ��50MHz
	GPIO_Init(GPIOB,&gpio_b_structure_initer);
	//��������PB8
	gpio_b_structure_initer.GPIO_Mode=GPIO_Mode_Out_PP;//�������ģʽ
	gpio_b_structure_initer.GPIO_Pin=GPIO_Pin_8;//����8
	gpio_b_structure_initer.GPIO_Speed=GPIO_Speed_50MHz;//Ƶ��50MHz
	GPIO_Init(GPIOB,&gpio_b_structure_initer);	
}
//LED2��
void LED2_On()
{
	//PB0����͵�ƽ-->LED2(�Ź������)�ϵĵ�ѹΪ3.3V-->����������ѹ-->�����ܵ�ͨ-->����
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);		
}
//LED2��
void LED2_Off()
{
	//PB0����ߵ�ƽ-->LED2(�Ź������)�ϵĵ�ѹΪ0V-->С��������ѹ-->�����ܽ�ֹ-->������
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
}
//LED3��
void LED3_On()
{
	//PB8����͵�ƽ-->LED2(�Ź������)�ϵĵ�ѹΪ3.3V-->����������ѹ-->�����ܵ�ͨ-->����
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);		
}
//LED3��
void LED3_Off()
{
	//PB0����ߵ�ƽ-->LED2(�Ź������)�ϵĵ�ѹΪ0V-->С��������ѹ-->�����ܽ�ֹ-->������
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
}