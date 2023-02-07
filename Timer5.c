#include "Timer5.h"
#include "stm32f10x.h"
#include "hc_sr04.h"
#include  "usart1.h"
void Timer5Init(int ReloadeValue,int Prescaler,int PreemptionPriority,int SubPriority)
{
	 //定时器3中断通道优先级设置
	NVIC_InitTypeDef NVIC_Timer5;
	NVIC_Timer5.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;//抢占优先级
	NVIC_Timer5.NVIC_IRQChannelSubPriority=SubPriority;//次级优先级
	NVIC_Timer5.NVIC_IRQChannel=TIM5_IRQn;//中断通道
	NVIC_Timer5.NVIC_IRQChannelCmd=ENABLE;//中断优先级使能
	NVIC_Init(&NVIC_Timer5);
	/*
		本stm32f10x系列的微处理器时钟频率为72Mhz
	  定时器的时钟频率即Timer2_ClockFrequency如下：
	  定时器的时钟频率=微处理器时钟频率/(预分频系数+1)/分频因子
    定时器计数周期=（1/定时器时钟频率）*重装值	
	*/
	//定时器3设置
	TIM_TimeBaseInitTypeDef TIM_Timer5;
	//开启定时器2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	//重装值寄存器值
	TIM_Timer5.TIM_Period=ReloadeValue-1;
	//预分频值
	TIM_Timer5.TIM_Prescaler=Prescaler-1;
	//分频因子为1
	TIM_Timer5.TIM_ClockDivision=TIM_CKD_DIV1;
	//计数模式向上计数
	TIM_Timer5.TIM_CounterMode=TIM_CounterMode_Up;
	//无需重复计数以增加计数周期
	TIM_Timer5.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM5,&TIM_Timer5);
	//清除定时器5更新标志
	TIM_ClearFlag(TIM5,TIM_FLAG_Update);
	//定时器5中断使能
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	//定时器5计数器使能
	TIM_Cmd(TIM5,ENABLE);		
}
//定时器5计数中断
/*
 每完成一次计数就执行一次计数中断
*/
void TIM5_IRQHandler()
{  //进入计数中断
	 if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET)
	 {
			//printf("开始测距\r\n");
		  Triger();
		 //清除中断标志位
		 TIM_ClearITPendingBit(TIM5,TIM_FLAG_Update);
	 }	
}
