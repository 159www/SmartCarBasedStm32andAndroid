#include "Timer4.h"
#include "usart1.h"
#include "stm32f10x.h"
int HighPluseWidthTimes=-1;//高电平时间计数标志
int temp_HighPluseWidthTimes=0;

void Timer4Init(int ReloadeValue,int Prescaler,int PreemptionPriority,int SubPriority)
{
	//定时器4中断通道优先级设置
	NVIC_InitTypeDef NVIC_Timer4;
	NVIC_Timer4.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;//抢占优先级
	NVIC_Timer4.NVIC_IRQChannelSubPriority=SubPriority;//次级优先级
	NVIC_Timer4.NVIC_IRQChannel=TIM4_IRQn;//中断通道
	NVIC_Timer4.NVIC_IRQChannelCmd=ENABLE;//中断优先级使能
	NVIC_Init(&NVIC_Timer4);
	/*
		本stm32f10x系列的微处理器时钟频率为72Mhz
	  定时器的时钟频率即Timer2_ClockFrequency如下：
	  定时器的时钟频率=微处理器时钟频率/(预分频系数+1)/分频因子
    定时器计数周期=（1/定时器时钟频率）*重装值	
	*/
	//定时器4设置
	TIM_TimeBaseInitTypeDef TIM_Timer4;
	//开启定时器2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	//重装值寄存器值
	TIM_Timer4.TIM_Period=ReloadeValue-1;
	//预分频值
	TIM_Timer4.TIM_Prescaler=Prescaler-1;
	//分频因子为1
	TIM_Timer4.TIM_ClockDivision=TIM_CKD_DIV1;
	//计数模式向上计数
	TIM_Timer4.TIM_CounterMode=TIM_CounterMode_Up;
	//无需重复计数以增加计数周期
	TIM_Timer4.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM4,&TIM_Timer4);
	//清除定时器4更新标志
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	//定时器4中断使能
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	//定时器4计数器使能
	//TIM_Cmd(TIM4,ENABLE);
  TIM_Cmd(TIM4,DISABLE);	
}

//定时器2计数中断
/*
 每完成一次计数就执行一次计数中断
*/
void TIM4_IRQHandler()
{  //进入计数中断
	 if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
	 {
//		 if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==RESET)
//		 {
//			 //定时器4计数器失能
//	     TIM_Cmd(TIM4,DISABLE);
//			 TIM_SetCounter(TIM4,0);
//       HighPluseWidthTimes=temp_HighPluseWidthTimes;
//			 if(HighPluseWidthTimes<38000)
//			 {
//			 printf("测距成功:%d\r\n",HighPluseWidthTimes);
//			 double ok=(HighPluseWidthTimes*170/1000000.0);
//			 printf("距离:%lfm",ok);
//			 }
//			 temp_HighPluseWidthTimes=0;
//       finalstate=1;			 
//		 }
//		 else
//		 {
//			 temp_HighPluseWidthTimes++;
//		 }
			
		 //清除中断标志位
		 TIM_ClearITPendingBit(TIM4,TIM_FLAG_Update);
	 }	
}