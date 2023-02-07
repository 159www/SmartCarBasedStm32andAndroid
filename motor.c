#include "motor.h"
#include "stm32f10x.h"
/*
日期：20230114
作者：tom hanks
版本：1.1
修改日期：20230115
修改内容：去除了运动函数中的电机使能直接调用运动函数已失效调用运动函数需要先初始化timer2并进行PWM调速
修改者：tom hanks
*/
/*
电机驱动模块为两块L293D芯片该芯片有四路输入控制四路输出理论上可以控制两个电机
但四路输入实际只有两路输入即使用两路数据同步控制两个同侧电机
input1<--->input3
input2<--->input4
*/
//初始化电机控制引脚
void MotorInit()
{
	 //左侧电机控制引脚初始化
	
	//左侧电机控制引脚初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitTypeDef GPIO_PD4;
	GPIO_PD4.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_PD4.GPIO_Pin=GPIO_Pin_4;
	GPIO_PD4.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOD,&GPIO_PD4);
	//左侧控制输入1 IN1初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_PC11;
	GPIO_PC11.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_PC11.GPIO_Pin=GPIO_Pin_11;
	GPIO_PC11.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_PC11);
	//左侧控制输入2 IN2初始化
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitTypeDef GPIO_PD0;
	GPIO_PD0.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_PD0.GPIO_Pin=GPIO_Pin_0;
	GPIO_PD0.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOD,&GPIO_PD0);
	
	//右侧电机控制引脚初始化
	
	//右侧电机控制引脚初始化
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitTypeDef GPIO_PD2;
	GPIO_PD2.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_PD2.GPIO_Pin=GPIO_Pin_2;
	GPIO_PD2.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOD,&GPIO_PD2);
	//右侧控制输入3 IN3初始化
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitTypeDef GPIO_PD6;
	GPIO_PD6.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_PD6.GPIO_Pin=GPIO_Pin_6;
	GPIO_PD6.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOD,&GPIO_PD6);
	//右侧控制输入4 IN4初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
	GPIO_InitTypeDef GPIO_PG9;
	GPIO_PG9.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_PG9.GPIO_Pin=GPIO_Pin_9;
	GPIO_PG9.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOG,&GPIO_PG9);
	GPIO_ResetBits(GPIOD,GPIO_Pin_4);
	GPIO_ResetBits(GPIOD,GPIO_Pin_2);	
}
//左右侧电机使能
void MotorEnable()
{
	GPIO_SetBits(GPIOD,GPIO_Pin_4);
	GPIO_SetBits(GPIOD,GPIO_Pin_2);		
}
//左右侧电机失能
void MotoeDisable()
{
	GPIO_ResetBits(GPIOD,GPIO_Pin_4);
	GPIO_ResetBits(GPIOD,GPIO_Pin_2);		
}
//控制左侧电机同步转动
void LeftFrontMotor(int input1,int input2)
{
	if(input1==1)
	{
		 GPIO_SetBits(GPIOC,GPIO_Pin_11);
	}
	else if(input1==0)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_11);		
	}
	if(input2==1)
	{
		GPIO_SetBits(GPIOD,GPIO_Pin_0);
	}
	else if(input2==0)
	{	
		GPIO_ResetBits(GPIOD,GPIO_Pin_0);
	}	
}
//控制左侧电机同步转动
void LeftBackMotor(int input1,int input2)
{	
	if(input1==1)
	{
		 GPIO_SetBits(GPIOC,GPIO_Pin_11);
	}
	else if(input1==0)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_11);		
	}
	if(input2==1)
	{
		GPIO_SetBits(GPIOD,GPIO_Pin_0);
	}
	else if(input2==0)
	{	
		GPIO_ResetBits(GPIOD,GPIO_Pin_0);
	}		
}
//控制右侧电机同步转动
void RightFrontMotor(int input3,int input4)
{
	if(input3==1)
	{
		 GPIO_SetBits(GPIOD,GPIO_Pin_6);
	}
	else if(input3==0)
	{
		GPIO_ResetBits(GPIOD,GPIO_Pin_6);		
	}
	if(input4==1)
	{
		GPIO_SetBits(GPIOG,GPIO_Pin_9);
	}
	else if(input4==0)
	{	
		GPIO_ResetBits(GPIOG,GPIO_Pin_9);
	}			
}
//控制右侧电机同步转动
void RightBackMotor(int input3,int input4)
{
	if(input3==1)
	{
		 GPIO_SetBits(GPIOD,GPIO_Pin_6);
	}
	else if(input3==0)
	{
		GPIO_ResetBits(GPIOD,GPIO_Pin_6);		
	}
	if(input4==1)
	{
		GPIO_SetBits(GPIOG,GPIO_Pin_9);
	}
	else if(input4==0)
	{	
		GPIO_ResetBits(GPIOG,GPIO_Pin_9);
	}				
}
//前进
void go()
{
//	 //左侧电机使能
//	 GPIO_SetBits(GPIOD,GPIO_Pin_4);	
	 LeftFrontMotor(1,0); //左侧电机同步后转
//	 //右侧电机使能
//	 GPIO_SetBits(GPIOD,GPIO_Pin_2);
	 RightFrontMotor(1,0); //右侧电机同步后转	
}
//后退
void back()
{
//	//左侧电机使能
//	 GPIO_SetBits(GPIOD,GPIO_Pin_4);	
	 LeftFrontMotor(0,1); //左侧电机同步前转
//	//右侧电机使能
//	 GPIO_SetBits(GPIOD,GPIO_Pin_2);
	 RightFrontMotor(0,1); //右侧电机同步前转
	
}
//左转
void TurnLeft()
{
//	 //右侧电机使能
//	 GPIO_SetBits(GPIOD,GPIO_Pin_2);
	 RightFrontMotor(1,0); //右侧电机同步后转
//	 //左侧电机失能
//	 GPIO_ResetBits(GPIOD,GPIO_Pin_4);
   //左侧电机停止
   LeftFrontMotor(0,0);	
}
//右转
void TurnRight()
{
//	 //左侧电机使能
//	 GPIO_SetBits(GPIOD,GPIO_Pin_4);
	 LeftFrontMotor(1,0); //左侧电机同步后转
//	 //右侧电机失能
//   GPIO_ResetBits(GPIOD,GPIO_Pin_2);
   //右侧电机停止
   RightFrontMotor(0,0);	
}
//停止
void stop(void)
{
	//左侧电机停止
	LeftFrontMotor(0,0);
	//右侧电机停止
	RightFrontMotor(0,0);	
}