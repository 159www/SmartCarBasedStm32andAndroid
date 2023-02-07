#include"stm32f10x.h"
#include"usart1.h"
#include"usart3.h"
#include"delay.h"
int ok_flag=0;
int respond_flag=0;
int send_success_flag=0;
extern  char Usart3CommandBuffer[200];
char OK[2]="OK";
char QMTOPEN[13]="+QMTOPEN: 0,0";
char QMTCONN[15]="+QMTCONN: 0,0,0";
char CGPADDR[8]="+CGPADDR";
char QMTPUB[14]="+QMTPUB: 0,0,0";
char CGATT[9]="+CGATT: 1";
void BC26_Init(void)
{
	 //AT+CGATT?  +CGATT: 1 
	 while(1)
	 {
		 Usart3_SendString(USART3,"AT+CGATT?\r\n");
		 delay_ms(200);
		 if(research_str(Usart3CommandBuffer,200,CGATT,9)==1)
		 {
			  clear(Usart3CommandBuffer,200);
				break;
		 }
		 else
		 {
				printf("未就绪\r\n");
		 }	
	 }
		//查询是否分配IP
		Usart3_SendString(USART3,"AT+CGPADDR\r\n");
	  while(1)
		{
		delay_ms(500);//等待100ms等待BC26响应指令
		//验证是否已经分配ip
		 if(research_str(Usart3CommandBuffer,200,CGPADDR,8)==1)
	    { 
				 
			   clear(Usart3CommandBuffer,200);
		     printf("已经分配ip\r\n");
				 break;
	    }
	  else
	    {
		     printf("未分配ip\r\n");
	    }
  	}
	//设置MQTT版本(onenet旧版)
	 Usart3_SendString(USART3,"AT+QMTCFG=\"version\",0,4\r\n");
	while(1)
	{
	 delay_ms(500);//等待100ms等待BC26响应指令
	//验证是否返回ok\r\n
	 if(research_str(Usart3CommandBuffer,200,OK,2)==1)
	 {
				
			   clear(Usart3CommandBuffer,200);
		     printf("MQTT版本设置成功\r\n");
		     break;
	 }
	 else
	 {
		 printf("MQTT版本设置失败\r\n");
	 }
  }
	//设置onenet指定的MQTT的端口
	Usart3_SendString(USART3,"AT+QMTOPEN=0,\"183.230.40.39\",6002\r\n");
	while(1)
	{
	delay_ms(500);//等待100ms等待BC26响应指令
	//验证是否返回+QMTOPEN: 0,0\r\n
	 if(research_str(Usart3CommandBuffer,200,QMTOPEN,13)==1)
	 {
		 clear(Usart3CommandBuffer,200);
		 printf("连接地址和端口设置成功\r\n");
		 break;
	 }
	 else
	 {
			printf("连接地址和端口设置失败\r\n");
	 }		
  }
		//连接平台
	  // 1、设备号   2、产品号    3、apikey
		Usart3_SendString(USART3,"AT+QMTCONN=0,\"1042075544\",\"570463\",\"q9A0ViaCcsLlAlyZxy=ztZ=E3LQ=\"\r\n");
	  while(1)
		{
		delay_ms(500);//等待100ms等待BC26响应指令
		//验证是否返回+QMTCONN: 0,0,0\r\n
		 if(research_str(Usart3CommandBuffer,200,QMTCONN,15)==1)
		 {
			   clear(Usart3CommandBuffer,200);
				 printf("连接平台成功\r\n");
			   break;
		 }
		 else
		 {
			printf("连接平台失败\r\n");
		 }
	  }		
}
void BC26_Send(char *id,char *value)
{
	//固定长度53
  //id和value的长度为可变长度
  //{"datastreams":[{"id":"T","datapoints":[{"value":21}]}]}
	int base_lenth=53;//固定长度
	int variable_length=Get_CharPointer_Length(id)+Get_CharPointer_Length(value);//可变长度
	int terminal_length=base_lenth+variable_length;//最终长度
	delay_ms(100);
	Usart3_SendString(USART3,"AT+QMTPUB=0,0,0,0,\"$dp\"\r\n");
	delay_ms(100);
	//以十六进制发送01
	Usart3_SendByte(USART3,0x01);
	//以十六进制发送00
	delay_ms(100);
  Usart3_SendByte(USART3,0x00);
	delay_ms(100);
	//以十六进制发送数据帧长度（json）
	Usart3_SendByte(USART3,terminal_length);
	delay_ms(100);
	//发送json且不换行
	Usart3_SendString(USART3,"{\"datastreams\":[{\"id\":\""); //23
	delay_ms(100);
	//以字符串形式发送id
	Usart3_SendString(USART3,id);
	delay_ms(100);
	Usart3_SendString(USART3,"\",\"datapoints\":[{\"value\":");//25
	//以字符串形式发送value
	delay_ms(100);
	Usart3_SendString(USART3,value);
	delay_ms(100);
	Usart3_SendString(USART3,"}]}]}");//5
	//以十六进制发送1A以结束数据包
	delay_ms(100);
	Usart3_SendByte(USART3,0x1A);
	int i=0;
	for( ;i<10;i++)
	{
		delay_ms(200);
		if(research_str(Usart3CommandBuffer,200,QMTPUB,14)==1)
	  {
		 send_success_flag=1;
		 printf("数据已经成功上传\r\n");
		 clear(Usart3CommandBuffer,200);
		 break;
	  }
	  else
	  {
			//printf("数据上传中\r\n");
			//clear(Usart2CommandBuffer,200);
	  }		
	}
	
	send_success_flag=0;	
}