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
				printf("δ����\r\n");
		 }	
	 }
		//��ѯ�Ƿ����IP
		Usart3_SendString(USART3,"AT+CGPADDR\r\n");
	  while(1)
		{
		delay_ms(500);//�ȴ�100ms�ȴ�BC26��Ӧָ��
		//��֤�Ƿ��Ѿ�����ip
		 if(research_str(Usart3CommandBuffer,200,CGPADDR,8)==1)
	    { 
				 
			   clear(Usart3CommandBuffer,200);
		     printf("�Ѿ�����ip\r\n");
				 break;
	    }
	  else
	    {
		     printf("δ����ip\r\n");
	    }
  	}
	//����MQTT�汾(onenet�ɰ�)
	 Usart3_SendString(USART3,"AT+QMTCFG=\"version\",0,4\r\n");
	while(1)
	{
	 delay_ms(500);//�ȴ�100ms�ȴ�BC26��Ӧָ��
	//��֤�Ƿ񷵻�ok\r\n
	 if(research_str(Usart3CommandBuffer,200,OK,2)==1)
	 {
				
			   clear(Usart3CommandBuffer,200);
		     printf("MQTT�汾���óɹ�\r\n");
		     break;
	 }
	 else
	 {
		 printf("MQTT�汾����ʧ��\r\n");
	 }
  }
	//����onenetָ����MQTT�Ķ˿�
	Usart3_SendString(USART3,"AT+QMTOPEN=0,\"183.230.40.39\",6002\r\n");
	while(1)
	{
	delay_ms(500);//�ȴ�100ms�ȴ�BC26��Ӧָ��
	//��֤�Ƿ񷵻�+QMTOPEN: 0,0\r\n
	 if(research_str(Usart3CommandBuffer,200,QMTOPEN,13)==1)
	 {
		 clear(Usart3CommandBuffer,200);
		 printf("���ӵ�ַ�Ͷ˿����óɹ�\r\n");
		 break;
	 }
	 else
	 {
			printf("���ӵ�ַ�Ͷ˿�����ʧ��\r\n");
	 }		
  }
		//����ƽ̨
	  // 1���豸��   2����Ʒ��    3��apikey
		Usart3_SendString(USART3,"AT+QMTCONN=0,\"1042075544\",\"570463\",\"q9A0ViaCcsLlAlyZxy=ztZ=E3LQ=\"\r\n");
	  while(1)
		{
		delay_ms(500);//�ȴ�100ms�ȴ�BC26��Ӧָ��
		//��֤�Ƿ񷵻�+QMTCONN: 0,0,0\r\n
		 if(research_str(Usart3CommandBuffer,200,QMTCONN,15)==1)
		 {
			   clear(Usart3CommandBuffer,200);
				 printf("����ƽ̨�ɹ�\r\n");
			   break;
		 }
		 else
		 {
			printf("����ƽ̨ʧ��\r\n");
		 }
	  }		
}
void BC26_Send(char *id,char *value)
{
	//�̶�����53
  //id��value�ĳ���Ϊ�ɱ䳤��
  //{"datastreams":[{"id":"T","datapoints":[{"value":21}]}]}
	int base_lenth=53;//�̶�����
	int variable_length=Get_CharPointer_Length(id)+Get_CharPointer_Length(value);//�ɱ䳤��
	int terminal_length=base_lenth+variable_length;//���ճ���
	delay_ms(100);
	Usart3_SendString(USART3,"AT+QMTPUB=0,0,0,0,\"$dp\"\r\n");
	delay_ms(100);
	//��ʮ�����Ʒ���01
	Usart3_SendByte(USART3,0x01);
	//��ʮ�����Ʒ���00
	delay_ms(100);
  Usart3_SendByte(USART3,0x00);
	delay_ms(100);
	//��ʮ�����Ʒ�������֡���ȣ�json��
	Usart3_SendByte(USART3,terminal_length);
	delay_ms(100);
	//����json�Ҳ�����
	Usart3_SendString(USART3,"{\"datastreams\":[{\"id\":\""); //23
	delay_ms(100);
	//���ַ�����ʽ����id
	Usart3_SendString(USART3,id);
	delay_ms(100);
	Usart3_SendString(USART3,"\",\"datapoints\":[{\"value\":");//25
	//���ַ�����ʽ����value
	delay_ms(100);
	Usart3_SendString(USART3,value);
	delay_ms(100);
	Usart3_SendString(USART3,"}]}]}");//5
	//��ʮ�����Ʒ���1A�Խ������ݰ�
	delay_ms(100);
	Usart3_SendByte(USART3,0x1A);
	int i=0;
	for( ;i<10;i++)
	{
		delay_ms(200);
		if(research_str(Usart3CommandBuffer,200,QMTPUB,14)==1)
	  {
		 send_success_flag=1;
		 printf("�����Ѿ��ɹ��ϴ�\r\n");
		 clear(Usart3CommandBuffer,200);
		 break;
	  }
	  else
	  {
			//printf("�����ϴ���\r\n");
			//clear(Usart2CommandBuffer,200);
	  }		
	}
	
	send_success_flag=0;	
}