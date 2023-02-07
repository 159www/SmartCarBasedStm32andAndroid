/*
日期：20230112
作者：tom hanks
功能：LED亮灭控制
版本：1.0
最后修改日期：暂无
修改人：暂无
*/
#include "LED.h"
#include "stm32f10x.h"
void LED_Init()
{
	/*
	LED2<---->PB0
	LED3<---->PB8
	LED按共阳极的方式连接
	*/
	//开启B端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//创建引脚设置结构体
	GPIO_InitTypeDef gpio_b_structure_initer;
	//设置引脚PB0
	gpio_b_structure_initer.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出模式
	gpio_b_structure_initer.GPIO_Pin=GPIO_Pin_0;//引脚0
	gpio_b_structure_initer.GPIO_Speed=GPIO_Speed_50MHz;//频率50MHz
	GPIO_Init(GPIOB,&gpio_b_structure_initer);
	//设置引脚PB8
	gpio_b_structure_initer.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出模式
	gpio_b_structure_initer.GPIO_Pin=GPIO_Pin_8;//引脚8
	gpio_b_structure_initer.GPIO_Speed=GPIO_Speed_50MHz;//频率50MHz
	GPIO_Init(GPIOB,&gpio_b_structure_initer);	
}
//LED2亮
void LED2_On()
{
	//PB0输出低电平-->LED2(放光二级管)上的电压为3.3V-->大于死区电压-->二极管导通-->发光
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);		
}
//LED2灭
void LED2_Off()
{
	//PB0输出高电平-->LED2(放光二级管)上的电压为0V-->小于死区电压-->二极管截止-->不发光
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
}
//LED3亮
void LED3_On()
{
	//PB8输出低电平-->LED2(放光二级管)上的电压为3.3V-->大于死区电压-->二极管导通-->发光
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);		
}
//LED3灭
void LED3_Off()
{
	//PB0输出高电平-->LED2(放光二级管)上的电压为0V-->小于死区电压-->二极管截止-->不发光
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
}