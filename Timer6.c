#include "Timer6.h"
#include "usart1.h"
#include "hc_sr04.h"
#include "stm32f10x.h"
#include "data.h"
#include "BC26.h"
char distance[50];
extern int ReadEchoTimes;//1个计数代表1us
extern int finalstate;
void Timer6Init(int ReloadeValue,int Prescaler,int PreemptionPriority,int SubPriority)
{
	distance[48]='\r';
	distance[49]='\n';
	 //定时器6中断通道优先级设置
	NVIC_InitTypeDef NVIC_Timer6;
	NVIC_Timer6.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;//抢占优先级
	NVIC_Timer6.NVIC_IRQChannelSubPriority=SubPriority;//次级优先级
	NVIC_Timer6.NVIC_IRQChannel=TIM6_IRQn;//中断通道
	NVIC_Timer6.NVIC_IRQChannelCmd=ENABLE;//中断优先级使能
	NVIC_Init(&NVIC_Timer6);
	
	//定时器6设置
	TIM_TimeBaseInitTypeDef TIM_Timer6;
	//开启定时器6时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	//重装值寄存器值
	TIM_Timer6.TIM_Period=ReloadeValue-1;
	//预分频值
	TIM_Timer6.TIM_Prescaler=Prescaler-1;
	//分频因子为1
	TIM_Timer6.TIM_ClockDivision=TIM_CKD_DIV1;
	//计数模式向上计数
	TIM_Timer6.TIM_CounterMode=TIM_CounterMode_Up;
	//无需重复计数以增加计数周期
	TIM_Timer6.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM6,&TIM_Timer6);
	//清除定时器6更新标志
	TIM_ClearFlag(TIM6,TIM_FLAG_Update);
	//定时器6中断使能
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	//定时器6计数器使能
	TIM_Cmd(TIM6,ENABLE);		
}
//定时器6计数中断
/*
 每完成一次计数就执行一次计数中断
*/
void TIM6_IRQHandler()
{  //进入计数中断
	 if(TIM_GetITStatus(TIM6,TIM_IT_Update)!=RESET)
	 {
			printf("上报测量距离\r\n");
		   if(finalstate==1)
		 {
			 if(ReadEchoTimes<38000)
			 {
			// printf("测距成功:%d\r\n",ReadEchoTimes);
			 double ok=(ReadEchoTimes*0.00017);
			// printf("距离:%lfm\r\n",ok);
			 sprintf(distance,"%lf",ok);
			 BC26_Send("T",distance);
			 ClearNull(distance,48);
			 }
			 finalstate=0;			 
		 }
		 //清除中断标志位
		 TIM_ClearITPendingBit(TIM6,TIM_FLAG_Update);
	 }	
}