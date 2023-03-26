/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		uart
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/
 
#ifndef _zf_uart_h
#define _zf_uart_h


#include "common.h"
#include "fsl_lpuart.h"

//��ö�ٶ��岻�����û��޸�
typedef enum //ö�ٴ�������
{
    UART1_TX_B12,
    UART1_RX_B13, 
    
    UART2_TX_B18,   UART2_TX_D11,    
    UART2_RX_B19,   UART2_RX_D10,    
    
    UART3_TX_B22,   UART3_TX_C8,    UART3_TX_E13,
    UART3_RX_B23,   UART3_RX_C9,    UART3_RX_E14,
        
    UART4_TX_C16,   UART4_TX_D0 ,   UART4_TX_E19, 
    UART4_RX_C17,   UART4_RX_D1 ,   UART4_RX_E20, 
        
    UART5_TX_C28,   UART5_TX_E23, 
    UART5_RX_C29,   UART5_RX_E24, 
        
    UART6_TX_B2 ,   UART6_TX_E25,
    UART6_RX_B3 ,   UART6_RX_E26,
    
    UART7_TX_D8 ,   UART7_TX_E31,
    UART7_RX_D9 ,   UART7_RX_D18,
                  
    UART8_TX_B26,   UART8_TX_D16,   UART8_TX_D24,
    UART8_RX_B27,   UART8_RX_D17,   UART8_RX_D25,
    
}UARTPIN_enum;


//��ö�ٶ��岻�����û��޸�
typedef enum //ö�ٴ��ں�
{
    USART_0,//RT1064û�д���0 ���������ռλ
    USART_1,
    USART_2,
    USART_3,
    USART_4,
    USART_5,
    USART_6,
    USART_7,
    USART_8,
}UARTN_enum;


void uart_init(UARTN_enum uartn, uint32 baud, UARTPIN_enum tx_pin, UARTPIN_enum rx_pin);
void uart_putchar(UARTN_enum uartn, uint8 dat);
void uart_putbuff(UARTN_enum uartn, uint8 *buff, uint32 len);
void uart_putstr(UARTN_enum uartn, const int8 *str);
void uart_getchar(UARTN_enum uartn, uint8 *dat);
uint8 uart_query(UARTN_enum uartn, uint8 *dat);
void uart_tx_irq(UARTN_enum uartn,uint8 status);
void uart_rx_irq(UARTN_enum uartn,uint8 status);
void uart_set_handle(UARTN_enum uartn, lpuart_handle_t *handle, lpuart_transfer_callback_t callback, uint8 *tx_buff, uint32 tx_count, uint8 *rx_buff, uint32 rx_count);

#endif