#ifndef __MOTOR_H__
#define __MOTOR_H__
void MotorInit(void);
void LeftFrontMotor(int input1,int input2);
void LeftBackMotor(int input1,int input2);
void RightFrontMotor(int input3,int input4);
void RightBackMotor(int input3,int input4);
void go(void);
void back(void);
void stop(void);
void TurnLeft(void);
void TurnRight(void);
void MotorEnable(void);
void MotoeDisable(void);
#endif