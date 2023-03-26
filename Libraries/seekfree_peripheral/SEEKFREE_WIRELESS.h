/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		��ɿƼ�����ת����ģ��
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-03-27
 * @note		
					���߶��壺
					------------------------------------ 
					    ����ת����       ��Ƭ��                        
    					RX              �鿴SEEKFREE_WIRELESS.h�ļ��е�WIRELESS_UART_TX�궨��
    					TX              �鿴SEEKFREE_WIRELESS.h�ļ��е�WIRELESS_UART_RX�궨��
    					RTS             �鿴SEEKFREE_WIRELESS.h�ļ��е�RTS_PIN�궨��
    					CMD             ���ջ�������
					------------------------------------ 
 ********************************************************************************************************************/

#ifndef _SEEKFREE_WIRELESS
#define _SEEKFREE_WIRELESS


#include "common.h"

#define WIRELESS_UART        USART_8         //����ת����ģ�� ��ʹ�õ��Ĵ���     
#define WIRELESS_UART_TX     UART8_TX_D16
#define WIRELESS_UART_RX     UART8_RX_D17
#define WIRELESS_UART_BAUD   230400//��ѡ������9600 57600 115200 230400 460800


//0���ر��Զ�������  
//1�������Զ������� �Զ������ʵ��������޸�WIRELESS_UART_BAUD֮����Ҫ��ģ��������ã�ģ����Զ�����Ϊ��Ӧ�Ĳ�����

//ע������1������ת����ģ��汾��V2.0���µ����޷������Զ������ʵġ�
//ע������2�������Զ��������������RTS���ţ�����Ὺ��ʧ�ܡ�
//ע������3��ģ���Զ�������ʧ�ܵĻ������Գ��Զϵ�����
#define WIRELESS_AUTO_UART_BAUD 0
//�����Զ�����������Ķ��������� ע������
//�����Զ�����������Ķ��������� ע������
//�����Զ�����������Ķ��������� ע������

//��������λ����  ָʾ��ǰģ���Ƿ���Խ�������  0���Լ�������  1�����Լ�������
//������ת����V2.0���ϵİ汾��Ҳ���ڿ���ģ������Զ�������ģʽ
#define RTS_PIN D26 

#define WIRELESS_BUFFER_SIZE 16

extern uint8 wireless_send_buffer[WIRELESS_BUFFER_SIZE];
extern uint16 wireless_rx_index;

void    seekfree_wireless_init(void);
uint32  seekfree_wireless_send_buff(uint8 *buff, uint32 len);

#endif 
