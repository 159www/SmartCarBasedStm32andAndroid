/*
日期：20230115
作者：tom hanks
功能：电机调速
版本：1.0
最后修改内容：暂无
修改者： 暂无
*/
#include "Timer3.h"
#include "motor.h"
#include "stm32f10x.h"
/*
功能：通用定时器2初始化
参数：
1、重装值
2、预分频系数
3、中断抢占优先级
4、中断次级优先级
*/
int counter=0;//调速基准  取值范围：0<=counter<=ReloadValue  0：静止 ReloadValue：全速   
int times=0; //计时器中断执行次数
int times_max=10; //计数最大值即重装值

void Timer3Init(int ReloadeValue,int Prescaler,int PreemptionPriority,int SubPriority)
{
	//times_max=ReloadeValue;
	//定时器3中断通道优先级设置
	NVIC_InitTypeDef NVIC_Timer3;
	NVIC_Timer3.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;//抢占优先级
	NVIC_Timer3.NVIC_IRQChannelSubPriority=SubPriority;//次级优先级
	NVIC_Timer3.NVIC_IRQChannel=TIM3_IRQn;//中断通道
	NVIC_Timer3.NVIC_IRQChannelCmd=ENABLE;//中断优先级使能
	NVIC_Init(&NVIC_Timer3);
	/*
		本stm32f10x系列的微处理器时钟频率为72Mhz
	  定时器的时钟频率即Timer2_ClockFrequency如下：
	  定时器的时钟频率=微处理器时钟频率/(预分频系数+1)/分频因子
    定时器计数周期=（1/定时器时钟频率）*重装值	
	*/
	//定时器3设置
	TIM_TimeBaseInitTypeDef TIM_Timer3;
	//开启定时器2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	//重装值寄存器值
	TIM_Timer3.TIM_Period=ReloadeValue-1;
	//预分频值
	TIM_Timer3.TIM_Prescaler=Prescaler-1;
	//分频因子为1
	TIM_Timer3.TIM_ClockDivision=TIM_CKD_DIV1;
	//计数模式向上计数
	TIM_Timer3.TIM_CounterMode=TIM_CounterMode_Up;
	//无需重复计数以增加计数周期
	TIM_Timer3.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3,&TIM_Timer3);
	//清除定时器3更新标志
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	//定时器3中断使能
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	//定时器3计数器使能
	TIM_Cmd(TIM3,ENABLE);	
}
//定时器3计数中断
/*
 每完成一次计数就执行一次计数中断
*/
void TIM3_IRQHandler()
{  //进入计数中断
	 if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	 {
			//执行。。。。
		 times++;
		 if(counter==0)
		 {
				//输出低电平电机失能
			 MotoeDisable();
		 }
		 else if(counter>0&&counter<=times_max)
		 {
			 if(times<=counter)
		   {
				 //输出高电平电机使能
				 MotorEnable();			 
		   }
		   else
		   {
				 //输出低电平电机失能
				 MotoeDisable();
		   }
		 }
     else
     {
			 //异常抛出：输入不符合要求
		 }			 
		 if(times==times_max)
		 {
				times=0;
		 }		 
		 //清除中断标志位
		 TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);
	 }	
}
void ChangeSpeed(int speed_value)
{
	counter=speed_value;
}