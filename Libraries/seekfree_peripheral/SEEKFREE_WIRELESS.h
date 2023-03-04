/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		逐飞科技无线转串口模块
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		IAR 8.3 or MDK 5.28
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-03-27
 * @note		
					接线定义：
					------------------------------------ 
					    无线转串口       单片机                        
    					RX              查看SEEKFREE_WIRELESS.h文件中的WIRELESS_UART_TX宏定义
    					TX              查看SEEKFREE_WIRELESS.h文件中的WIRELESS_UART_RX宏定义
    					RTS             查看SEEKFREE_WIRELESS.h文件中的RTS_PIN宏定义
    					CMD             悬空或者上拉
					------------------------------------ 
 ********************************************************************************************************************/

#ifndef _SEEKFREE_WIRELESS
#define _SEEKFREE_WIRELESS


#include "common.h"

#define WIRELESS_UART        USART_8         //无线转串口模块 所使用到的串口     
#define WIRELESS_UART_TX     UART8_TX_D16
#define WIRELESS_UART_RX     UART8_RX_D17
#define WIRELESS_UART_BAUD   230400//可选参数有9600 57600 115200 230400 460800


//0：关闭自动波特率  
//1：开启自动波特率 自动波特率的作用是修改WIRELESS_UART_BAUD之后不需要对模块进行配置，模块会自动设置为对应的波特率

//注意事项1：无线转串口模块版本是V2.0以下的是无法开启自动波特率的。
//注意事项2：开启自动波特率务必连接RTS引脚，否则会开启失败。
//注意事项3：模块自动波特率失败的话，可以尝试断电重启
#define WIRELESS_AUTO_UART_BAUD 0
//开启自动波特率务必阅读上面两条 注意事项
//开启自动波特率务必阅读上面两条 注意事项
//开启自动波特率务必阅读上面两条 注意事项

//定义流控位引脚  指示当前模块是否可以接受数据  0可以继续接收  1不可以继续接收
//在无线转串口V2.0以上的版本中也用于控制模块进入自动波特率模式
#define RTS_PIN D26 

#define WIRELESS_BUFFER_SIZE 16

extern uint8 wireless_send_buffer[WIRELESS_BUFFER_SIZE];
extern uint16 wireless_rx_index;

void    seekfree_wireless_init(void);
uint32  seekfree_wireless_send_buff(uint8 *buff, uint32 len);

#endif 
