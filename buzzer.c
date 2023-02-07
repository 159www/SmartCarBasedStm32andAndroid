/*
日期：20230112
作者：tom hanks
版本：1.0
功能：控制蜂鸣器鸣叫
最后修改日期：暂无
修改人：暂无
*/
#include "buzzer.h"
#include "delay.h"
#include "stm32f10x.h"
//蜂鸣器初始化
void BuzzerInit()
{
	/*
	 引脚PB2、PE14均可以控制蜂鸣器
	 通过PNP三极管来控制蜂鸣器的通断电->低电平三极管导通->蜂鸣器通电
	 蜂鸣器为有源蜂鸣器通电就会发生鸣叫
	*/
	/*
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	 GPIO_InitTypeDef GPIO_INIT_STRUCTURE;
	 GPIO_INIT_STRUCTURE.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	 GPIO_INIT_STRUCTURE.GPIO_Pin=GPIO_Pin_2;//引脚2
	 GPIO_INIT_STRUCTURE.GPIO_Speed=GPIO_Speed_50MHz;//50MHz
	 GPIO_Init(GPIOB,&GPIO_INIT_STRUCTURE);
	 */
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	 GPIO_InitTypeDef GPIO_INIT_STRUCTURE;
	 GPIO_INIT_STRUCTURE.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	 GPIO_INIT_STRUCTURE.GPIO_Pin=GPIO_Pin_14;//引脚14
	 GPIO_INIT_STRUCTURE.GPIO_Speed=GPIO_Speed_50MHz;//50MHz
	 GPIO_Init(GPIOE,&GPIO_INIT_STRUCTURE);	
		
}
//蜂鸣器响
void BuzzerOn()
{
	/*
	//PB2低电平->PNP三极管导通->蜂鸣器通电并鸣叫
	GPIO_ResetBits(GPIOB,GPIO_Pin_2);
	*/
	//PE14低电平->PNP三极管导通->蜂鸣器通电并鸣叫
	GPIO_ResetBits(GPIOE,GPIO_Pin_14);
		
}
//蜂鸣器不响
void BuzzerOff()
{
	/*
	 //PB2高电平->PNP三极管截止->蜂鸣器不通电并不鸣叫
	GPIO_SetBits(GPIOB,GPIO_Pin_2);
	*/
	//PE14高电平->PNP三极管截止->蜂鸣器不通电并不鸣叫
	GPIO_SetBits(GPIOE,GPIO_Pin_14);
			
}
//蜂鸣器间歇性鸣叫
void BuzzerOn0ffInternal()
{
	//响100ms
	BuzzerOn();
	delay_ms(500);//延迟100ms
	//停100ms
	BuzzerOff();
	delay_ms(100);//延迟100ms	
}