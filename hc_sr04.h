#ifndef __HC_SR04_H__
#define __HC_SR04_H__
void Hc_Sr04Init(void);
void Triger(void);
int ReadEchoTime(void);
float CalculateDistance(int times,int temperture,int humidity);


#endif