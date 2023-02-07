/*
���ڣ�20230112
���ߣ�tom hanks
�汾��1.0
���ܣ����Ʒ���������
����޸����ڣ�����
�޸��ˣ�����
*/
#include "buzzer.h"
#include "delay.h"
#include "stm32f10x.h"
//��������ʼ��
void BuzzerInit()
{
	/*
	 ����PB2��PE14�����Կ��Ʒ�����
	 ͨ��PNP�����������Ʒ�������ͨ�ϵ�->�͵�ƽ�����ܵ�ͨ->������ͨ��
	 ������Ϊ��Դ������ͨ��ͻᷢ������
	*/
	/*
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	 GPIO_InitTypeDef GPIO_INIT_STRUCTURE;
	 GPIO_INIT_STRUCTURE.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	 GPIO_INIT_STRUCTURE.GPIO_Pin=GPIO_Pin_2;//����2
	 GPIO_INIT_STRUCTURE.GPIO_Speed=GPIO_Speed_50MHz;//50MHz
	 GPIO_Init(GPIOB,&GPIO_INIT_STRUCTURE);
	 */
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	 GPIO_InitTypeDef GPIO_INIT_STRUCTURE;
	 GPIO_INIT_STRUCTURE.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	 GPIO_INIT_STRUCTURE.GPIO_Pin=GPIO_Pin_14;//����14
	 GPIO_INIT_STRUCTURE.GPIO_Speed=GPIO_Speed_50MHz;//50MHz
	 GPIO_Init(GPIOE,&GPIO_INIT_STRUCTURE);	
		
}
//��������
void BuzzerOn()
{
	/*
	//PB2�͵�ƽ->PNP�����ܵ�ͨ->������ͨ�粢����
	GPIO_ResetBits(GPIOB,GPIO_Pin_2);
	*/
	//PE14�͵�ƽ->PNP�����ܵ�ͨ->������ͨ�粢����
	GPIO_ResetBits(GPIOE,GPIO_Pin_14);
		
}
//����������
void BuzzerOff()
{
	/*
	 //PB2�ߵ�ƽ->PNP�����ܽ�ֹ->��������ͨ�粢������
	GPIO_SetBits(GPIOB,GPIO_Pin_2);
	*/
	//PE14�ߵ�ƽ->PNP�����ܽ�ֹ->��������ͨ�粢������
	GPIO_SetBits(GPIOE,GPIO_Pin_14);
			
}
//��������Ъ������
void BuzzerOn0ffInternal()
{
	//��100ms
	BuzzerOn();
	delay_ms(500);//�ӳ�100ms
	//ͣ100ms
	BuzzerOff();
	delay_ms(100);//�ӳ�100ms	
}