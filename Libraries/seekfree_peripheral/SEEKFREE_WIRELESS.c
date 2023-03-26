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

#include "fsl_common.h"
#include "fsl_gpio.h"
#include "zf_systick.h"
#include "zf_gpio.h"
#include "zf_iomuxc.h"
#include "zf_uart.h"
#include "SEEKFREE_WIRELESS.h"

uint8 wireless_send_buffer[WIRELESS_BUFFER_SIZE];
uint16 wireless_rx_index = 0;

vuint8 wireless_auto_baud_num;
vuint8 wireless_auto_baud_flag;
vuint8 wireless_auto_baud_data[3] = {0x00, 0x01, 0x03};

uint8               wireless_rx_buffer;
lpuart_transfer_t   wireless_receivexfer;
lpuart_handle_t     wireless_g_lpuartHandle;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�� �����жϺ���
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       �ú�����ISR�ļ� ����8�жϳ��򱻵���
//-------------------------------------------------------------------------------------------------------------------
void wireless_uart_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
    if(kStatus_LPUART_RxIdle == status)
    {
        //�ӵ�һ���ֽں�Ƭ��������봮���жϣ�ͨ���ڴ˴���ȡwireless_rx_buffer����ȡ������
        
        //�Զ�ƥ�䲨�����ж�
        if(wireless_auto_baud_flag && (wireless_auto_baud_num<3))
        {
            wireless_auto_baud_data[wireless_auto_baud_num] = wireless_rx_buffer;
            wireless_auto_baud_num++;
            if(3 == wireless_auto_baud_num)
            {
                wireless_auto_baud_flag = 2;
            }
        }
        //����״̬��ȡ��������wireless_send_buffer
        else 
        {
            wireless_send_buffer[wireless_rx_index++] = wireless_rx_buffer;
            if(wireless_rx_index == WIRELESS_BUFFER_SIZE)wireless_rx_index=0;
        }
    }
    handle->rxDataSize = wireless_receivexfer.dataSize;  //��ԭ����������
    handle->rxData = wireless_receivexfer.data;          //��ԭ��������ַ
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ���ʼ��
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
void seekfree_wireless_init(void)
{
#if(0 == WIRELESS_AUTO_UART_BAUD)//�ر��Զ�������

    gpio_init(RTS_PIN, GPI, 0, GPIO_PIN_CONFIG);//��ʼ����������

    uart_init (WIRELESS_UART, WIRELESS_UART_BAUD, WIRELESS_UART_TX, WIRELESS_UART_RX);	//��ʼ������    
    uart_rx_irq(WIRELESS_UART, 1);
    //���ô��ڽ��յĻ�����������������
    wireless_receivexfer.dataSize = 1;
    wireless_receivexfer.data = &wireless_rx_buffer;
    
    uart_set_handle(WIRELESS_UART, &wireless_g_lpuartHandle, wireless_uart_callback, NULL, 0, wireless_receivexfer.data, 1);
    //NVIC_SetPriority((IRQn_Type)((IRQn_Type)(WIRELESS_UART) + LPUART1_IRQn), 0);  //�����ж����ȼ� ��Χ0-15 ԽС���ȼ�Խ��

#elif(1 == WIRELESS_AUTO_UART_BAUD)//�����Զ�������
    uint32 temp_isr;
    gpio_init(RTS_PIN, GPI, 0, GPIO_PIN_CONFIG);//��ʼ����������
    uint8 rts_init_status;
    rts_init_status = gpio_get(RTS_PIN);
    
    temp_isr = DisableGlobalIRQ();
    EnableGlobalIRQ(0);
    gpio_init(RTS_PIN, GPO, rts_init_status, GPIO_PIN_CONFIG);//��ʼ����������

    uart_init (WIRELESS_UART, WIRELESS_UART_BAUD, WIRELESS_UART_TX, WIRELESS_UART_RX);	//��ʼ������    
    uart_rx_irq(WIRELESS_UART, 1);
    //���ô��ڽ��յĻ�����������������
    wireless_receivexfer.dataSize = 1;
    wireless_receivexfer.data = &wireless_rx_buffer;
    
    uart_set_handle(WIRELESS_UART, &wireless_g_lpuartHandle, wireless_uart_callback, NULL, 0, wireless_receivexfer.data, 1);
    //NVIC_SetPriority((IRQn_Type)((IRQn_Type)(WIRELESS_UART) + LPUART1_IRQn), 0);  //�����ж����ȼ� ��Χ0-15 ԽС���ȼ�Խ��
    systick_delay_ms(5);  //ģ���ϵ�֮����Ҫ��ʱ�ȴ�
    gpio_set(RTS_PIN, !rts_init_status);   //RTS�������ߣ������Զ�������ģʽ
    systick_delay_ms(100);   //RTS����֮�������ʱ20ms
    gpio_toggle(RTS_PIN);    //RTS����ȡ��
    //gpio_set(RTS_PIN, 0);   //RTS����
    
    wireless_auto_baud_flag = 1;
    
    //�����ض����� ����ģ���Զ��жϲ�����
    uart_putchar(WIRELESS_UART, wireless_auto_baud_data[0]);
    uart_putchar(WIRELESS_UART, wireless_auto_baud_data[1]);
    uart_putchar(WIRELESS_UART, wireless_auto_baud_data[2]);

    systick_delay_ms(20);
    //�����Զ��������Ƿ����
    if(2 != wireless_auto_baud_flag)
    {
        //���������뵽������ڣ�˵���Զ�������ʧ����
        systick_delay_ms(500);
        while(1);
    }

    //�����Զ��������Ƿ���ȷ
    if(0xa5 != wireless_auto_baud_data[0] &&
       0xff != wireless_auto_baud_data[1] &&
       0xff != wireless_auto_baud_data[2] )
    {
        //���������뵽������ڣ�˵���Զ�������ʧ����
        systick_delay_ms(500);
        while(1);
    }

    gpio_init(RTS_PIN, GPI, 0, GPIO_PIN_CONFIG);//��ʼ����������
    systick_delay_ms(10);//��ʱ�ȴ� ģ��׼������
    EnableGlobalIRQ(temp_isr);
#endif

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�� ���ͺ���
//  @param      buff        ��Ҫ���͵����ݵ�ַ
//  @param      len         ���ͳ���
//  @return     uint32      ʣ��δ���͵��ֽ���   
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
uint32 seekfree_wireless_send_buff(uint8 *buff, uint32 len)
{
    while(len>30)
    {
//        if(gpio_get(RTS_PIN))  
//        {
//            return len;//ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
//        }
        while(gpio_get(RTS_PIN));  //���RTSΪ�͵�ƽ���������������
        uart_putbuff(WIRELESS_UART,buff,30);

        buff += 30; //��ַƫ��
        len -= 30;//����
    }
    
//    if(gpio_get(RTS_PIN))  
//    {
//        return len;//ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
//    }
    while(gpio_get(RTS_PIN));  //���RTSΪ�͵�ƽ���������������
    uart_putbuff(WIRELESS_UART,buff,len);//������������
    
    return 0;
}