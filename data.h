#ifndef __DATA_H__
#define __DATA_H__

int getnumber(char a);
int pow1(int j);
int get_number_length(int number);
void clear(char a[],int length);
int isEqual(char a[],char b[],int length);
int strtoint(char a[],int start,int end);
char *int_to_str(int number,char *buffer);
char *Str_Cat(char *front,int front_length,char *back,int back_length,char *ReturnPointer);
int Get_CharPointer_Length( char *p);
int isInteger(double number);//判断小数部分为0
char *Double_to_str(double number,char *buffer);
int research_str(char * buffer,int buffer_len,char *str,int str_len);
void ClearNull(char *p,int len);

#endif

