/*
日期：20230126
作者：tom hanks
功能：舵机驱动
版本：1.0
最后修改内容：暂无
修改者： 暂无
*/
#include "Timer2.h"
#include "stm32f10x.h"
/*
 0.02(s)/100=0.0002(T) //假设预装载值为100
 1/T=14400hz
 72000000/14400=5000(prescaler)//预分频值5000 

*/
void Harder_PWM_ByTimer2Init(int prescaler,int peroid)
{
	 //1、开启硬件PWM引脚A端口时钟和AFIO复用功能模块时钟
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	 //2、设置硬件PWM引脚A0的模式
	 GPIO_InitTypeDef GPIO_PA0;
	 GPIO_PA0.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	 GPIO_PA0.GPIO_Pin=GPIO_Pin_0;
	 GPIO_PA0.GPIO_Speed=GPIO_Speed_50MHz;
	 GPIO_Init(GPIOA,&GPIO_PA0);
	 //3、开启定时器2时钟
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	 //4、初始化定时器2
	 TIM_TimeBaseInitTypeDef TIM_Timer2;
	 TIM_Timer2.TIM_RepetitionCounter=0;//不重复计数
	 TIM_Timer2.TIM_Prescaler=prescaler-1;//预分频系数
	 TIM_Timer2.TIM_ClockDivision=0;//分频因子
	 TIM_Timer2.TIM_Period=peroid-1;//重装值
	 TIM_Timer2.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	 TIM_TimeBaseInit(TIM2,&TIM_Timer2);
	 //5、PWM设置
	 TIM_OCInitTypeDef TIM_OC_PWM;
	 TIM_OC_PWM.TIM_OCMode=TIM_OCMode_PWM2;//计数次数小于比较值时输出高电平
	 TIM_OC_PWM.TIM_OCPolarity=TIM_OCPolarity_High;
	 TIM_OC_PWM.TIM_OutputState=ENABLE;
	 TIM_OC1Init(TIM2,&TIM_OC_PWM);//初始化通道一
	 TIM_OC1PreloadConfig(TIM2,ENABLE);
	 TIM_Cmd(TIM2,ENABLE);	
}
void SetPluseWidth(int pluse_length)
{
	/*
	 87.5<pluse_length<97.5
	 一个计数代表0.0002s
	 pluse_length*0.0002=t，t表示高电平脉冲宽度
	 对应关系如下：
	 t = 0.5ms —————— 舵机会转到 -90 °
   t = 1.0ms —————— 舵机会转到 -45°
   t = 1.5ms —————— 舵机会转到 0°
   t = 2.0ms —————— 舵机会转到 45°
   t = 2.5ms —————— 舵机会转到 90°
	*/
	 TIM_SetCompare1(TIM2,pluse_length);
	
}
