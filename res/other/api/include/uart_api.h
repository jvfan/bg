#ifndef __UART_API_H
#define __UART_API_H

//串口初始化函数
extern int set_opt(int,int,int,char,int);
//检测函数：打开串口并发送一个字符串
extern int uart_check(char *uart,char *uart_buffer);

#endif

