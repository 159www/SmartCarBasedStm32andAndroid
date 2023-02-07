/*
日期：20220112
作者：tom hanks
功能：实现小车前方的左右避障
版本：1.0
最后修改日期：暂无
修改人：  暂无
*/
#include "AvoidBarrier.h"
#include "LED.h"
#include "motor.h"
#include "stm32f10x.h"
void AvoidBarrierInit()
{
	/*
	本文件区域假设抢占优先级和次级优先级各占两位且并未设置抢占优先级位数即NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	请在调用本初始化函数前将抢占优先级位数设置为2
	避障模块监测到障碍后输出电平会由高变低产生电平下降沿
	*/
	//左避障外部中断初始化配置
	
	//1、开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
  //2、引脚PG2模式设置
	GPIO_InitTypeDef GPIO_PG2;
	GPIO_PG2.GPIO_Pin=GPIO_Pin_2;
  GPIO_PG2.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_PG2.GPIO_Mode=GPIO_Mode_IPU;//上拉输入
	GPIO_Init(GPIOG,&GPIO_PG2);
	//3、中断线路配置
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource2);
	//4、中断线路配置
	EXTI_InitTypeDef  EXTI_PG2;
	EXTI_PG2.EXTI_Mode=EXTI_Mode_Interrupt;//中断模式
	EXTI_PG2.EXTI_Line=EXTI_Line2;//中断线路2
	EXTI_PG2.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿触发
	EXTI_PG2.EXTI_LineCmd=ENABLE;//线路使能
	EXTI_Init(&EXTI_PG2);
	//5、优先级设置
	NVIC_InitTypeDef NVIC_PG2;
	NVIC_PG2.NVIC_IRQChannel=EXTI2_IRQn;//中断通道2
	NVIC_PG2.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级0
	NVIC_PG2.NVIC_IRQChannelSubPriority=1;//次级优先级1
	NVIC_PG2.NVIC_IRQChannelCmd=ENABLE;//中断优先级使能
	NVIC_Init(&NVIC_PG2);
	
	//右避障外部中断初始化配置
	
	//1、开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
  //2、引脚PG2模式设置
	GPIO_InitTypeDef GPIO_PC7;
	GPIO_PC7.GPIO_Pin=GPIO_Pin_7;
  GPIO_PC7.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_PC7.GPIO_Mode=GPIO_Mode_IPU;//上拉输入
	GPIO_Init(GPIOG,&GPIO_PC7);
	//3、中断线路配置
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource7);
	//4、中断线路配置
	EXTI_InitTypeDef  EXTI_PC7;
	EXTI_PC7.EXTI_Mode=EXTI_Mode_Interrupt;//中断模式
	EXTI_PC7.EXTI_Line=EXTI_Line7;//中断线路7
	EXTI_PC7.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿触发
	EXTI_PC7.EXTI_LineCmd=ENABLE;//线路使能
	EXTI_Init(&EXTI_PC7);
	//5、优先级设置
	NVIC_InitTypeDef NVIC_PC7;
	NVIC_PC7.NVIC_IRQChannel=EXTI9_5_IRQn;//中断线路5至9共用通道
	NVIC_PC7.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级0
	NVIC_PC7.NVIC_IRQChannelSubPriority=1;//次级优先级1
	NVIC_PC7.NVIC_IRQChannelCmd=ENABLE;//中断优先级使能
	NVIC_Init(&NVIC_PC7);
	
}
//左避障中断函数
void EXTI2_IRQHandler()
{
		LED2_On();
	  LED3_Off();
	  TurnRight();
	
		
		EXTI_ClearITPendingBit(EXTI_Line2);//清除中断标志位
}
//右避障中断函数
void EXTI9_5_IRQHandler()
{
	   //中断线7产生中断
		 if(EXTI_GetITStatus(EXTI_Line7)!=RESET)
		 {
			 LED2_Off();
			 LED3_On();
			 TurnLeft();
			 //清空中断标志
			 EXTI_ClearITPendingBit(EXTI_Line7);
		 }
	
}
/*
该避障函数应位于main函数中的死循环中
功能：解决左右避障模块均未检测到障碍物和
*/
void AvoidBarrierInMain()
{
	//左右避障模块均未检测到障碍物
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)==1&&GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_2)==1)
	{
		go();//小车前进
	}
	//左右避障模块均检测到障碍物
	else if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)==0&&GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_2)==0)
	{
		stop();//小车停止等待遥控指令
	}
	
}