/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2021,��ɿƼ�
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
 * @Software 		IAR 8.3 or MDK 5.33
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2021-08-27
 * @note		
					���߶��壺
					------------------------------------ 
					    ����ת����      ��Ƭ��                        
    					RX              �鿴SEEKFREE_BLUETOOTH_CH9141.h�ļ��е�BLUETOOTH_CH9141_UART_TX�궨��
    					TX              �鿴SEEKFREE_BLUETOOTH_CH9141.h�ļ��е�BLUETOOTH_CH9141_UART_RX�궨��
    					RTS             �鿴SEEKFREE_BLUETOOTH_CH9141.h�ļ��е�BLUETOOTH_CH9141_RTS_PIN�궨��
                        CTS             ����
    					CMD             ���ջ�������
					------------------------------------ 
 ********************************************************************************************************************/
 
#ifndef SEEKFREE_BLUETOOTH_CH9141_H
#define SEEKFREE_BLUETOOTH_CH9141_H

#include "common.h"

typedef struct
{
    uint8     *buffer;                                                          // ����ָ��
    uint32    head;                                                             // ����ͷָ�� ����ָ��յĻ���
    uint32    end;                                                              // ����βָ�� ����ָ��ǿջ��棨����ȫ�ճ��⣩
    uint32    size;                                                             // ����ʣ���С
    uint32    max;                                                              // �����ܴ�С
}fifo_struct;

typedef enum
{
    FIFO_SUCCESS,

    FIFO_BUFFER_NULL,
    FIFO_SPACE_NO_ENOUGH,
    FIFO_DATA_NO_ENOUGH,
}fifo_state_enum;

typedef enum
{
    FIFO_READ_AND_CLEAN,
    FIFO_READ_ONLY,
}fifo_operation_enum;

fifo_state_enum fifo_init           (fifo_struct *fifo, uint8 *buffer_addr, uint32 size);
void            fifo_head_offset    (fifo_struct *fifo, uint32 offset);
void            fifo_end_offset     (fifo_struct *fifo, uint32 offset);
void            fifo_clear          (fifo_struct *fifo);
uint32          fifo_used           (fifo_struct *fifo);

fifo_state_enum fifo_read_buffer    (fifo_struct *fifo, uint8 *dat, uint32 *length, fifo_operation_enum flag);
fifo_state_enum fifo_write_buffer   (fifo_struct *fifo, uint8 *dat, uint32 length);



#define BLUETOOTH_CH9141_INDEX              USART_8                             // ����ģ�� ��Ӧʹ�õĴ��ں�
#define BLUETOOTH_CH9141_BUAD_RATE          115200                              // ����ģ�� ��Ӧʹ�õĴ��ڲ�����
#define BLUETOOTH_CH9141_TX_PIN             UART8_RX_D17                        // ����ģ�� ��Ӧģ��� TX Ҫ�ӵ���Ƭ���� RX
#define BLUETOOTH_CH9141_RX_PIN             UART8_TX_D16                        // ����ģ�� ��Ӧģ��� RX Ҫ�ӵ���Ƭ���� TX
#define BLUETOOTH_CH9141_RTS_PIN            D26                                 // ����ģ�� ��Ӧģ��� RTS ����

#define BLUETOOTH_CH9141_BUFFER_SIZE        64
#define BLUETOOTH_CH9141_TIMEOUT_COUNT      500


uint8       bluetooth_ch9141_init               (void);

uint32      bluetooth_ch9141_send_buff          (uint8 *buff, uint32 len);
uint32      bluetooth_ch9141_read_buff          (uint8 *buff, uint32 len);





#endif
