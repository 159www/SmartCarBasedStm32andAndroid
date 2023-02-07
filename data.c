#include "data.h"
#include "stdio.h"
#include "stdlib.h"
#include "stm32f10x.h"

/*
将字符转换为数字
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
/*将单个数字转换成字符*/
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
计算十的n次方
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
取得数字的长度
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
将整型数据转换成字符数组
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
将数组清零
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
判断两字符数组是否完全一致
一致返回1
否则返回0
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
将字符串转换成对应的整数
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
/*将整数转换成对应的字符串*/
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
/*判断double型数小数部分是否为零*/
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
将带小数的数转换成对应的字符串
最多 保留小数点后8位且小数部分末尾的零会被抹去
小数部分
double 类型的变量运算会损失精度 
*/
char *Double_to_str(double number,char *buffer)
{
    int integer=number;//获得整数部分
    double decimal=number-integer;//获得小数部分
    int decimal_integer;//小数部分转换成的整数
    int length;//返回指针的长度
    int index=0;
    //将小数转换为整数
    decimal_integer=decimal*100000000;//零的个数决定了小数点后的位数不能超过double的范围
    while(decimal_integer%10==0)
    {
        decimal_integer/=10;
    }
    //得到小数的长度
    length=get_number_length(integer)+get_number_length(decimal_integer)+1;
    buffer[length]='\0';
    //放入整数部分
    for(int i=get_number_length(integer)-1;i>=0;i--)
    {
        buffer[index++]=getChar(integer%10);
        integer/=10;

    }
    //放入小数点
    buffer[index++]='.';
    //放入小数部分
    index=length-1;
    while (decimal_integer)
    {
        buffer[index--]=getChar(decimal_integer%10);
        decimal_integer/=10;


    }
    return buffer;
}
/*拼接字符串*/
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
/*获取char类型指针指向字符串的内存单元的长度*/
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

//在指定的字符串中寻找目标子串找到返回1没找到返回0
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
