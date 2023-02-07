#include "data.h"
#include "stdio.h"
#include "stdlib.h"
#include "stm32f10x.h"

/*
���ַ�ת��Ϊ����
*/
int getnumber(char a)
{
	
	int temp=0 ;
			if(a=='0')
            {
				temp = 0;
            }
			else if(a=='1')
            {
				temp = 1;

			}
			else if (a == '2')
			{
				temp = 2;

			}
			else if (a == '3')
			{
				temp = 3;

			}
			else if (a == '4')
			{
				temp = 4;

			}
			else if (a == '5')
			{
				temp = 5;

			}
			else if (a == '6')
			{
				temp = 6;

			}
			else if (a == '7')
			{
				temp = 7;

			}
			else if (a == '8')
			{
				temp = 8;

			}
			else if (a == '9')
			{
				temp = 9;

			}
			return temp;
}
/*����������ת�����ַ�*/
char getChar(int a)
{
	char temp=' ' ;
			if(a==0)
            {
				temp ='0';
            }
			else if(a==1)
            {
				temp = '1';

			}
			else if (a == 2)
			{
				temp = '2';

			}
			else if (a == 3)
			{
				temp = '3';

			}
			else if (a == 4)
			{
				temp = '4';

			}
			else if (a == 5)
			{
				temp = '5';

			}
			else if (a == 6)
			{
				temp = '6';

			}
			else if (a == 7)
			{
				temp = '7';

			}
			else if (a == 8)
			{
				temp = '8';

			}
			else if (a == 9)
			{
				temp = '9';

			}
			return temp;
	
}
/*
����ʮ��n�η�
*/
int pow1(int j)
{
	int temp=1;
	
	if(j==0)	
	{
		return 1;
		
	}
	else
	{
		int i=0;
		for(;i<j;i++)
	{
		
		temp*=10;
		
	}
		
	}
	
	return temp;
		
}
/*
ȡ�����ֵĳ���
*/
int get_number_length(int number)
{ 
	int length=0;
	int temp=number;
	while(temp/10)
	{
		temp/=10;
		length++;
	}
	
	return length+1;
}

/*
����������ת�����ַ�����
*/
char * inttochar(int number)
{
	int  size=get_number_length(number);
	int num=number;
  char temp[10];
	char *p=temp;
	int i=0;
	for(;i<size;i++)
  {
		int temp1=num/pow1(size-i);
		temp[i]=getnumber(temp1);
		num=num-temp1*(size-i);
		
		
	
	
  }	
   return p;
  


}
/*
����������
*/

void clear(char a[],int length)
{
	int i=0;
	for(;i<length;i++)
	{
		a[i]=0;
		
	}
	
}
/*
�ж����ַ������Ƿ���ȫһ��
һ�·���1
���򷵻�0
*/
int isEqual(char a[],char b[],int length)
{ 
	int flag=1;
	int i=0;
	for(;i<length;i++)
	{
		if(a[i]==b[i])
		{
			continue;
		}
		else
		{
			flag=0;
			break;
			
		}
		
	}
	
	return flag;
	
}
/*
���ַ���ת���ɶ�Ӧ������
*/

int strtoint(char a[],int start,int end)
{
	int total=0;
	int temp=0;
	int len=end-start+1;
	int i=start;
	int j=len-1;
	for(;i<=end;i++)
	{
		temp=getnumber(a[i])*pow1(j);
		j--;
		total+=temp;
		temp=0;	
	}
	
	return total;
}
/*������ת���ɶ�Ӧ���ַ���*/
char *int_to_str(int number,char *buffer)
{     
	 int length=get_number_length(number); 
	 int temp=number;
	 buffer[length]='\0';
	 for(int i=length-1;i>=0;i--)
	{
		
		buffer[i]=getChar(temp%pow1(1));
		temp /=10;	
		
	}
	
	return buffer;	
	
}
/*�ж�double����С�������Ƿ�Ϊ��*/
int isInteger(double number)
{
	 int integer=number;
	 if(number-integer>0)
	 {
		 
		 return 0;
	 }
	 else
	 {
		 
		 return 1;
		 
	 }
	
	
}
/*
����С������ת���ɶ�Ӧ���ַ���
��� ����С�����8λ��С������ĩβ����ᱻĨȥ
С������
double ���͵ı����������ʧ���� 
*/
char *Double_to_str(double number,char *buffer)
{
    int integer=number;//�����������
    double decimal=number-integer;//���С������
    int decimal_integer;//С������ת���ɵ�����
    int length;//����ָ��ĳ���
    int index=0;
    //��С��ת��Ϊ����
    decimal_integer=decimal*100000000;//��ĸ���������С������λ�����ܳ���double�ķ�Χ
    while(decimal_integer%10==0)
    {
        decimal_integer/=10;
    }
    //�õ�С���ĳ���
    length=get_number_length(integer)+get_number_length(decimal_integer)+1;
    buffer[length]='\0';
    //������������
    for(int i=get_number_length(integer)-1;i>=0;i--)
    {
        buffer[index++]=getChar(integer%10);
        integer/=10;

    }
    //����С����
    buffer[index++]='.';
    //����С������
    index=length-1;
    while (decimal_integer)
    {
        buffer[index--]=getChar(decimal_integer%10);
        decimal_integer/=10;


    }
    return buffer;
}
/*ƴ���ַ���*/
char *Str_Cat(char *front,int front_length,char *back,int back_length,char *ReturnPointer)
{
	  ReturnPointer[front_length+back_length]='\0';
		for(int i=0;i<front_length+back_length;i++)
	 {
		 if(i>=0&&i<front_length)
		 {
				ReturnPointer[i]=front[i];
		 }
		 else
		 {
				ReturnPointer[i]=back[i-front_length];
		 }
		 
	 }
		
		
	
		 return ReturnPointer;
	
}
/*��ȡchar����ָ��ָ���ַ������ڴ浥Ԫ�ĳ���*/
int Get_CharPointer_Length( char *p)
{
	int length=0;
	char *c=p;
	while(*c!='\0')
	{
			length++;
		  c++;
	}
	
	return length;
}

//��ָ�����ַ�����Ѱ��Ŀ���Ӵ��ҵ�����1û�ҵ�����0
int research_str(char * buffer,int buffer_len,char *str,int str_len)
{
    int result=0;
    int j;
    for(int i=0;i<buffer_len;i++)
    {
       for( j=0;j<str_len;j++)
       {
           if(buffer[i+j]==str[j])
           {
               continue;
           }
           if(buffer[i+j]!=str[j])
           {
               break;
           }

       }
        if(j==str_len)
        {
            result=1;
            break;
        }
    }
    return result;

}
void ClearNull(char *p,int len)
{
	 for(int i=0;i<len;i++)
	 {
			p[i]=NULL;
		 
	 }
}
