/*
作者：tom hanks
日期：20230124
功能：测距
最后修改日期：20230127
修改内容：添加外部中断来检测模块返回的高电平并在外部中断中读取高电平时间
修改者：tom hanks
*/
#include "hc_sr04.h"
#include "delay.h"
#include "stm32f10x.h"
#include "usart1.h"
#include "Timer4.h"
int ReadEchoTimes=-1;//1个计数代表1us
int finalstate=0;
void Hc_Sr04Init()
{
	//Echo 响应输出引脚
	
	 //开启响应输出引脚时钟
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	 GPIO_InitTypeDef GPIO_PB14;
	 GPIO_PB14.GPIO_Mode=GPIO_Mode_IPD;//下拉输入默认低电平
	 GPIO_PB14.GPIO_Pin=GPIO_Pin_14;
	 GPIO_PB14.GPIO_Speed=GPIO_Speed_50MHz;
	 GPIO_Init(GPIOB,&GPIO_PB14);
	//配置Echo 响应输出引脚的外部中断
	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
	 EXTI_InitTypeDef EXTI_PB14;
	 EXTI_PB14.EXTI_Line=EXTI_Line14;
	 EXTI_PB14.EXTI_LineCmd=ENABLE;
	 EXTI_PB14.EXTI_Mode=EXTI_Mode_Interrupt;
	 EXTI_PB14.EXTI_Trigger=EXTI_Trigger_Rising;//上升沿触发
	 EXTI_Init(&EXTI_PB14);
	//配置Echo 响应输出引脚的外部中断优先级
	 NVIC_InitTypeDef NVIC_PB14;
	 NVIC_PB14.NVIC_IRQChannel=EXTI15_10_IRQn;
	 NVIC_PB14.NVIC_IRQChannelCmd=ENABLE;
	 NVIC_PB14.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级
	 NVIC_PB14.NVIC_IRQChannelSubPriority=3;//次级优先级
	 NVIC_Init(&NVIC_PB14);
	
	
	//Triger 测距触发引脚
	
	//开启测距触发引脚时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitTypeDef GPIO_PD8;
	GPIO_PD8.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_PD8.GPIO_Pin=GPIO_Pin_8;
	GPIO_PD8.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_PD8); 
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_8);//将触发引脚设置为低电平
  GPIO_ResetBits(GPIOB,GPIO_Pin_14);//将测距触发引脚设置为低电平	
}
//发射探测脉冲
void Triger()
{
	//产生触发发射探测脉冲的信号
	GPIO_SetBits(GPIOD,GPIO_Pin_8);//将触发引脚设置为高电平
	delay_us(10);//维持高电平10us
	GPIO_ResetBits(GPIOD,GPIO_Pin_8);//将触发引脚设置为低电平	
}
//读取发射探测脉冲的反射时间
int ReadEchoTime()
{
	//等待高电平出现
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==RESET)
	{
		
	}
	//计算高电平时间即探测脉冲的传播时间
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==SET)
	{
		//运行一次循环的时间大约1us
		delay_us(1);
		ReadEchoTimes++;
	}
	//探测脉冲的反射时间超过以及等于38ms无效
	if(ReadEchoTimes>=38000)
	{
		 return -1;
	}
	else if(ReadEchoTimes<38000)
	{
		int temp=ReadEchoTimes;
		ReadEchoTimes=0;//清空计数以免影响下一次读取
		return temp;
	}
}
//计算障碍距离
float CalculateDistance(int times,int temperture,int humidity)
{
	int MicroSeconds=times/1000;//将计数转换为毫秒
	float Seconds=MicroSeconds/1000.0;//将毫秒转换为秒存在精度损失
	/*
	//实际情况
	//声速 m/s = 331.4 +（0.606 * 温度）+（0.0124 * 湿度）
	*/
	float WaveSpeedReal=331.4+(0.606*temperture)+(0.0124*humidity);//实际情况
	float WaveSpeedIdeal=340;//理想情况
	return Seconds*WaveSpeedIdeal/2.0;	
}
//外部中断函数
void EXTI15_10_IRQHandler(void)
{
	
	//int temp=0;//时间计数
	if(EXTI_GetITStatus(EXTI_Line14)!=RESET)
	{
//		 //计算高电平时间即探测脉冲的传播时间
//	   while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==SET)
//	   {
//		    //运行一次循环的时间大约1us
//		    delay_us(1);
//		    temp++;
//	   }
//		 printf("读取时间成功\r\n");		
//	  //探测脉冲的反射时间超过以及等于38ms无效
//	   if(temp>=38000)
//	   {
//		 
//	   }
//	   else if(temp<38000)
//	   {		     
//		     ReadEchoTimes=temp;         			 
//	   }
//		   TIM_SetCounter(TIM4,0);
//       Timer4Init(2,72,0,0);
		//开启检测高电平时间定时器
			TIM_Cmd(TIM4,ENABLE);
		//等待响应高电平结束变低
			while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==SET)
	   {
		    
	   }
		 //关闭检测高电平时间定时器
			TIM_Cmd(TIM4,DISABLE);
		  ReadEchoTimes=TIM_GetCounter(TIM4);
		 // printf("counter:%d\r\n",ReadEchoTimes);
		  TIM_SetCounter(TIM4,0);
		  finalstate=1;
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	
		
}