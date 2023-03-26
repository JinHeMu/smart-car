/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		��ͫ����ͷ(SCC8660) RT CSI�ӿ�
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598 & QQ2380006440)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.32.4 or MDK 5.24 �����
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-09-04
 * @note		
					���߶��壺
					------------------------------------ 
						ģ��ܽ�            ��Ƭ���ܽ�
						SDA(����ͷ��RX)     �鿴SEEKFREE_SCC8660_CSI.h�ļ��е�SCC8660_CSI_COF_UART_TX�궨��
						SCL(����ͷ��TX)     �鿴SEEKFREE_SCC8660_CSI.h�ļ��е�SCC8660_CSI_COF_UART_RX�궨��
						���ж�(VSY)         �鿴SEEKFREE_SCC8660_CSI.h�ļ��е�SCC8660_CSI_VSYNC_PIN�궨��
						���ж�(HREF)        ������İ����ӣ����գ�
						�����ж�(PCLK)      �鿴SEEKFREE_SCC8660_CSI.h�ļ��е�SCC8660_CSI_PCLK_PIN�궨��
						���ݿ�(D0-D7)       B31-B24 B31��Ӧ����ͷ�ӿ�D0
					------------------------------------ 
	
					Ĭ�Ϸֱ���               160*120
					Ĭ��FPS                  50֡
 ********************************************************************************************************************/


#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_csi.h"
#include "zf_systick.h"
#include "zf_pit.h"
#include "zf_camera.h"
#include "zf_gpio.h"
#include "zf_iomuxc.h"
#include "zf_csi.h"
#include "fsl_cache.h"
#include "zf_usb_cdc.h"
#include "seekfree_iic.h"
#include "SEEKFREE_SCC8660_CSI.h"


//����ͼ�񻺳���  ����û���Ҫ����ͼ������ ���ͨ��scc8660_csi_image���������ݣ���ò�Ҫֱ�ӷ��ʻ�����
//����Ĭ�Ϸֱ���160*120��������С������Ĭ�Ͻ�ͼ�����������DTCM���򣬷����ٶȸ��졣
//����ע�͵������ǽ�ͼ�����鶨����SDRAM�ڣ����ͼ��ֱ��ʳ���160*120���뽫�������ע��ע�͵�λ��DTCM�����䡣
//AT_SDRAM_SECTION_ALIGN(uint16 scc8660_csi1_image[SCC8660_CSI_PIC_H][SCC8660_CSI_PIC_W],64);
//AT_SDRAM_SECTION_ALIGN(uint16 scc8660_csi2_image[SCC8660_CSI_PIC_H][SCC8660_CSI_PIC_W],64);
AT_DTCM_SECTION_ALIGN(uint16 scc8660_csi1_image[SCC8660_CSI_PIC_H][SCC8660_CSI_PIC_W],64);
AT_DTCM_SECTION_ALIGN(uint16 scc8660_csi2_image[SCC8660_CSI_PIC_H][SCC8660_CSI_PIC_W],64);

//�û�����ͼ������ֱ�ӷ������ָ������Ϳ���
//���ʷ�ʽ�ǳ��򵥣�����ֱ��ʹ���±�ķ�ʽ����
//������ʵ�10�� 50�еĵ㣬scc8660_csi_image[10][50]�Ϳ�����
uint16 (*scc8660_csi_image)[SCC8660_CSI_PIC_W];

uint8   scc8660_uart_receive[3];
uint8   scc8660_uart_receive_num = 0;
vuint8  scc8660_uart_receive_flag;

//��Ҫ���õ�����ͷ������
uint16 SCC8660_CFG_CSI[SCC8660_CONFIG_FINISH][2]=
{
    {SCC8660_AUTO_EXP,          0},                     //�Զ��ع�     Ĭ�ϣ�0     	��ѡ����Ϊ��0 1��      0���ֶ��ع�  1���Զ��ع�
    {SCC8660_BRIGHT,            800},                   //��������     �ֶ��ع�Ĭ�ϣ�800	�ֶ��ع�ʱ��������Χ0-65535   �Զ��ع��Ƽ�ֵ��100 �Զ��ع�ʱ�������÷�Χ0-255
														//��Ҫע��SCC8660_BRIGHT�Ĳ����̶�ʱ����ͬ��SCC8660_PCLK_DIV������Ӱ��ͼ������ȡ�
														//����SCC8660_BRIGHT�Ĳ���Ϊ800��SCC8660_PCLK_DIV�Ĳ���Ϊ0ʱ �� SCC8660_PCLK_DIV�Ĳ���Ϊ2ʱ 
														//����Ϊ2��ʱ��ͼ������Ҫ��Ϊ0��ʱ��������ʹ��˫���ʱ��о���������ͷ���Ȳ�һ��ʱ��Ҫע��������⡣
														
    {SCC8660_FPS,               50},                    //ͼ��֡��     Ĭ�ϣ�50    	��ѡ����Ϊ��60 50 30 25�� ʵ��֡�ʻ���Ҫ��SCC8660_PCLK_DIV����������
    {SCC8660_SET_COL,           SCC8660_CSI_PIC_W},     //ͼ������     Ĭ�ϣ�160   	����.h�ĺ궨�崦�޸�
    {SCC8660_SET_ROW,           SCC8660_CSI_PIC_H},     //ͼ������     Ĭ�ϣ�120   	����.h�ĺ궨�崦�޸�
    {SCC8660_PCLK_DIV,          0},                     //PCLK��Ƶϵ�� Ĭ�ϣ�0     	��ѡ����Ϊ��0:1/1 1:2/3 2:1/2 3:1/3 4:1/4 5:1/8��  
                                                       //��Ƶϵ��Խ��PCLKƵ��Խ�ͣ�����PCLK���Լ���DVP�ӿڵĸ��ţ�������PCLKƵ�����Ӱ��֡�ʡ��������������뱣��Ĭ�ϡ�
                                                       //��������FPSΪ50֡������pclk��Ƶϵ��ѡ���Ϊ5��������ͷ�����֡��Ϊ50*��1/8��=6.25֡
													   //�����������������£�SCC8660_PCLK_DIV����Խ��ͼ���Խ��
    
    {SCC8660_PCLK_MODE,         0},                     //PCLKģʽ     Ĭ�ϣ�0		��ѡ����Ϊ��0 1��         0������������źţ�1����������źš�(ͨ��������Ϊ0�����ʹ��STM32��DCMI�ӿڲɼ���Ҫ����Ϊ1)
    {SCC8660_COLOR_MODE,        0},                     //ͼ��ɫ��ģʽ Ĭ�ϣ�0		��ѡ����Ϊ��0 1��         0��������ɫģʽ    1������ģʽ��ɫ�ʱ��Ͷ���ߣ�
    {SCC8660_DATA_FORMAT,       0},                     //������ݸ�ʽ Ĭ�ϣ�0		��ѡ����Ϊ��0 1 2 3��     0��RGB565 1��RGB565(�ֽڽ���) 2��YUV422(YUYV) 3��YUV422(UYVY)
    {SCC8660_MANUAL_WB,         0},                     //�ֶ���ƽ��   Ĭ�ϣ�0		��ѡ����Ϊ��0 0x65-0xa0�� 0���ر��ֶ���ƽ�⣬�����Զ���ƽ��    �������ֶ���ƽ�� �ֶ���ƽ��ʱ ������Χ0x65-0xa0
    
    {SCC8660_INIT,              0}                      //����ͷ��ʼ��ʼ��
};

//������ͷ�ڲ���ȡ������������
uint16 SCC8660_GET_CFG_CSI[SCC8660_CONFIG_FINISH-1][2]= 
{
    {SCC8660_AUTO_EXP,          0},
    {SCC8660_BRIGHT,            0}, //��������          
    {SCC8660_FPS,               0}, //ͼ��֡��           
    {SCC8660_SET_COL,           0}, //ͼ������           
    {SCC8660_SET_ROW,           0}, //ͼ������          
    {SCC8660_PCLK_DIV,          0}, //PCLK��Ƶϵ��      
    {SCC8660_PCLK_MODE,         0}, //PCLKģʽ      
    {SCC8660_COLOR_MODE,        0}, //ͼ��ɫ��ģʽ
    {SCC8660_DATA_FORMAT,       0}, //������ݸ�ʽ 	
    {SCC8660_MANUAL_WB,         0}, //��ƽ������
};

uint8               scc8660_csi_rx_buffer;
lpuart_transfer_t   scc8660_csi_receivexfer;
lpuart_handle_t     scc8660_csi_g_lpuartHandle;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCC8660(��ͫ����ͷ)�����жϺ���
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       �ú�����SDK�ײ�fsl_lpuart�ļ��еĴ���3�жϺ����ڵ���
//-------------------------------------------------------------------------------------------------------------------
void csi_scc8660_uart_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
    if(kStatus_LPUART_RxIdle == status)
    {
        scc8660_uart_receive[scc8660_uart_receive_num] = scc8660_csi_rx_buffer;
        scc8660_uart_receive_num++;
    
        if(1==scc8660_uart_receive_num && 0XA5!=scc8660_uart_receive[0])  scc8660_uart_receive_num = 0;
        if(3 == scc8660_uart_receive_num)
        {
            scc8660_uart_receive_num = 0;
            scc8660_uart_receive_flag = 1;
        }
    }
    handle->rxDataSize = scc8660_csi_receivexfer.dataSize;  //��ԭ����������
    handle->rxData = scc8660_csi_receivexfer.data;          //��ԭ��������ַ
}

uint8 scc8660_csi_finish_flag;  //ͼ��ɼ���ɵı�־λ    


//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCC8660(��ͫ����ͷ)CSI�жϺ���
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       �ú�����isr.c�е�CSI_IRQHandler��������
//-------------------------------------------------------------------------------------------------------------------
void scc8660_csi_isr(CSI_Type *base, csi_handle_t *handle, status_t status, void *userData)
{
    if(csi_get_full_buffer(&csi_handle,&fullCameraBufferAddr))
    {
        csi_add_empty_buffer(&csi_handle,(uint8 *)fullCameraBufferAddr);
        if(fullCameraBufferAddr == (uint32)scc8660_csi1_image[0])
        {
            scc8660_csi_image = scc8660_csi1_image;//�ɼ����
            L1CACHE_CleanInvalidateDCacheByRange((uint32)scc8660_csi1_image[0],SCC8660_CSI_W*SCC8660_CSI_H);
        }
        else if(fullCameraBufferAddr == (uint32)scc8660_csi2_image[0])
        {
            scc8660_csi_image = scc8660_csi2_image;//�ɼ����
            L1CACHE_CleanInvalidateDCacheByRange((uint32)scc8660_csi2_image[0],SCC8660_CSI_W*SCC8660_CSI_H);
        }
        scc8660_csi_finish_flag = 1;//�ɼ���ɱ�־λ��һ
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ͷ���ô��ڳ�ʼ��
//  @param      NULL
//  @return     void                    
//  @since      v1.0
//  Sample usage:       �ڲ����ã������û����á�
//-------------------------------------------------------------------------------------------------------------------
void scc8660_csi_cof_uart_init(void)
{
    //��ʼ������ ��������ͷ
    uart_init (SCC8660_CSI_COF_UART, 9600,SCC8660_CSI_COF_UART_TX,SCC8660_CSI_COF_UART_RX);      
    uart_rx_irq(SCC8660_CSI_COF_UART,1);
    //���ô��ڽ��յĻ�����������������
    scc8660_csi_receivexfer.dataSize = 1;
    scc8660_csi_receivexfer.data = &scc8660_csi_rx_buffer;
    //���ô����ж�
    uart_set_handle(SCC8660_CSI_COF_UART, &scc8660_csi_g_lpuartHandle, csi_scc8660_uart_callback, NULL, 0, scc8660_csi_receivexfer.data, 1);
    
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��������ͷ�ڲ�������Ϣ
//  @param      uartn       ѡ��ʹ�õĴ���
//  @param      buff        ����������Ϣ�ĵ�ַ
//  @return     void
//  @since      v1.0
//  Sample usage:           ���øú���ǰ���ȳ�ʼ������
//-------------------------------------------------------------------------------------------------------------------
void scc8660_set_all_config(UARTN_enum uartn, uint16 buff[SCC8660_CONFIG_FINISH-1][2])
{
    uint16 temp, i;
    uint8  send_buffer[4];

    scc8660_uart_receive_flag = 0;
    
    //���ò���  ������ο���������ֲ�
    //��ʼ��������ͷ�����³�ʼ��
    for(i=0; i<SCC8660_CONFIG_FINISH; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = buff[i][0];
        temp           = buff[i][1];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8)temp;
        
        uart_putbuff(uartn,send_buffer,4);
        systick_delay_ms(2);
    }
    
    //�ȴ�����ͷ��ʼ���ɹ�
    while(!scc8660_uart_receive_flag);
    scc8660_uart_receive_flag = 0;
    while((0xff != scc8660_uart_receive[1]) || (0xff != scc8660_uart_receive[2]));
    //���ϲ��ֶ�����ͷ���õ�����ȫ�����ᱣ��������ͷ��51��Ƭ����eeprom��
    //����set_exposure_time�����������õ��ع����ݲ��洢��eeprom��
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ����ͷ�ڲ�ȫ��������Ϣ
//  @param      uartn       ѡ��ʹ�õĴ���
//  @param      buff        ����������Ϣ�ĵ�ַ
//  @return     void
//  @since      v1.0
//  Sample usage:           ���øú���ǰ���ȳ�ʼ������ͷ���ô���
//-------------------------------------------------------------------------------------------------------------------
void scc8660_get_all_config(UARTN_enum uartn, uint16 buff[SCC8660_CONFIG_FINISH-1][2])
{
    uint16 temp, i;
    uint8  send_buffer[4];
    
    for(i=0; i<SCC8660_CONFIG_FINISH-1; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = SCC8660_GET_STATUS;
        temp           = buff[i][0];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8)temp;
        
        uart_putbuff(uartn,send_buffer,4);
        
        //�ȴ����ܻش�����
        while(!scc8660_uart_receive_flag);
        scc8660_uart_receive_flag = 0;
        
        buff[i][1] = scc8660_uart_receive[1]<<8 | scc8660_uart_receive[2];
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ����ͷ�̼��汾
//  @param      uartn               ѡ��ʹ�õĴ���
//  @return     (uint16)camera_id   ��������ͷID
//  @since      v1.0
//  Sample usage:           ���øú���ǰ���ȳ�ʼ������ͷ���ô���
//-------------------------------------------------------------------------------------------------------------------
uint16 scc8660_get_id(UARTN_enum uartn)
{
    uint16 temp;
    uint8  send_buffer[4];
	
    send_buffer[0] = 0xA5;
    send_buffer[1] = SCC8660_GET_WHO_AM_I;
    temp           = 0;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;
    
    uart_putbuff(uartn,send_buffer,4);
        
    //�ȴ����ܻش�����
    while(!scc8660_uart_receive_flag);
    scc8660_uart_receive_flag = 0;
    
    return ((uint16)(scc8660_uart_receive[1]<<8) | scc8660_uart_receive[2]);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ����ͷĳһ������������Ϣ
//  @param      uartn               ѡ��ʹ�õĴ���
//  @return     (uint16)config      ���ز���ֵ
//  @since      v1.0
//  Sample usage:           ���øú���ǰ���ȳ�ʼ������ͷ���ô���
//-------------------------------------------------------------------------------------------------------------------
uint16 scc8660_get_config(UARTN_enum uartn, uint8 config)
{
    uint8  send_buffer[4];
    send_buffer[0] = 0xA5;
    send_buffer[1] = SCC8660_GET_WHO_AM_I;
    send_buffer[2] = 0x00;
    send_buffer[3] = config;
    
    uart_putbuff(uartn,send_buffer,4);
        
    //�ȴ����ܻش�����
    while(!scc8660_uart_receive_flag);
    scc8660_uart_receive_flag = 0;
    
    return ((uint16)(scc8660_uart_receive[1]<<8) | scc8660_uart_receive[2]);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ��ɫ����ͷ�̼��汾
//  @param      uartn       ѡ��ʹ�õĴ���
//  @return     uint16      �汾��
//  @since      v1.0
//  Sample usage:           ���øú���ǰ���ȳ�ʼ������ͷ���ô���
//-------------------------------------------------------------------------------------------------------------------
uint16 scc8660_get_version(UARTN_enum uartn)
{
    uint16 temp;
    uint8  send_buffer[4];
    send_buffer[0] = 0xA5;
    send_buffer[1] = SCC8660_GET_STATUS;
    temp           = SCC8660_GET_VERSION;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;
    
    uart_putbuff(uartn,send_buffer,4);
        
    //�ȴ����ܻش�����
    while(!scc8660_uart_receive_flag);
    scc8660_uart_receive_flag = 0;
    
    return ((uint16)(scc8660_uart_receive[1]<<8) | scc8660_uart_receive[2]);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��������ͼ������
//  @param      uartn       ѡ��ʹ�õĴ���
//  @param      data        ��Ҫ���õ�����ֵ
//  @return     uint16      �������õ�����ֵ
//  @since      v1.0
//  Sample usage:           ���øú���ǰ���ȳ�ʼ������ͷ���ô���
//  @note                   ͨ���ú������õĲ��������ᱻ51��Ƭ������
//-------------------------------------------------------------------------------------------------------------------
uint16  scc8660_set_bright(UARTN_enum uartn, uint16 data)
{
    uint8  send_buffer[4];

    send_buffer[0] = 0xA5;
    send_buffer[1] = SCC8660_SET_BRIGHT;
    send_buffer[2] = data>>8;
    send_buffer[3] = (uint8)data;
    
    uart_putbuff(uartn,send_buffer,4);
    
    //�ȴ����ܻش�����
    while(!scc8660_uart_receive_flag);
    scc8660_uart_receive_flag = 0;
    
    data = scc8660_uart_receive[1]<<8 | scc8660_uart_receive[2];
    return data;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �������ð�ƽ��
//  @param      uartn       ѡ��ʹ�õĴ���
//  @param      data        ��Ҫ���õ�����ֵ
//  @return     uint16      �������õ�����ֵ
//  @since      v1.0
//  Sample usage:           ���øú���ǰ���ȳ�ʼ������ͷ���ô���
//  @note                   ͨ���ú������õĲ��������ᱻ51��Ƭ������
//-------------------------------------------------------------------------------------------------------------------
uint16  scc8660_set_maunal_wb(UARTN_enum uartn, uint16 data)
{
    uint8  send_buffer[4];

    send_buffer[0] = 0xA5;
    send_buffer[1] = SCC8660_SET_MANUAL_WB;
    send_buffer[2] = data>>8;
    send_buffer[3] = (uint8)data;
    
    uart_putbuff(uartn,send_buffer,4);
    
    //�ȴ����ܻش�����
    while(!scc8660_uart_receive_flag);
    scc8660_uart_receive_flag = 0;
    
    data = scc8660_uart_receive[1]<<8 | scc8660_uart_receive[2];
    return data;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ������������ͷ�Ĵ���
//  @param      uartn       ѡ��ʹ�õĴ���
//  @param      reg         �Ĵ�����ַ
//  @param      data        ��Ҫд��Ĵ�������ֵ
//  @return     uint16      �ش�д��Ĵ�����ֵ
//  @since      v1.0
//  Sample usage:           ���øú���ǰ���ȳ�ʼ������
//-------------------------------------------------------------------------------------------------------------------
uint16 scc8660_set_reg_addr(UARTN_enum uartn, uint8 reg, uint16 data)
{
    uint16 temp;
    uint8  send_buffer[4];

    send_buffer[0] = 0xA5;
    send_buffer[1] = SCC8660_SET_REG_ADDR;
    send_buffer[2] = 0x00;
    send_buffer[3] = (uint8)reg;
    
    uart_putbuff(uartn,send_buffer,4);
    
    systick_delay_ms(2);
    
    send_buffer[0] = 0xA5;
    send_buffer[1] = SCC8660_SET_REG_DATA;
    temp           = data;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;
    
    uart_putbuff(uartn,send_buffer,4);
    
    //�ȴ����ܻش�����
    while(!scc8660_uart_receive_flag);
    scc8660_uart_receive_flag = 0;
    
    
    temp = scc8660_uart_receive[1]<<8 | scc8660_uart_receive[2];

    return temp;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCC8660(��ͫ����ͷ)��ʼ�� ʹ��CSI�ӿ�
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:		
//-------------------------------------------------------------------------------------------------------------------
void scc8660_csi_init(void)
{
    //����ͷ��ʼ��ʼ��֮ǰ��ؽ����ź�����
    if      (SCC8660_CSI_VSYNC_PIN == CSI_VSYNC_B14)  gpio_init(B14,GPO,1,GPIO_PIN_CONFIG);
    else if (SCC8660_CSI_VSYNC_PIN == CSI_VSYNC_B22)  gpio_init(B22,GPO,1,GPIO_PIN_CONFIG);
    else if (SCC8660_CSI_VSYNC_PIN == CSI_VSYNC_C29)  gpio_init(C29,GPO,1,GPIO_PIN_CONFIG);
    
    //��ʼ������ͷ���ô���
    scc8660_csi_cof_uart_init();
	//�����ж�
	EnableGlobalIRQ(0);
    //�ȴ�����ͷ�ϵ��ʼ���ɹ� ��ʽ�����֣���ʱ����ͨ����ȡ���õķ�ʽ ��ѡһ
    //systick_delay_ms(500);//��ʱ��ʽ
    scc8660_get_all_config(SCC8660_CSI_COF_UART,SCC8660_GET_CFG_CSI);//��ȡ���õķ�ʽ
    
    scc8660_uart_receive_flag = 0;
    //������ͷ����������Ϣ
    scc8660_set_all_config(SCC8660_CSI_COF_UART,SCC8660_CFG_CSI);
    //��ȡ���ñ��ڲ鿴�����Ƿ���ȷ
    scc8660_get_all_config(SCC8660_CSI_COF_UART,SCC8660_GET_CFG_CSI);
    //Ϊ����ʹ�ã���ȡ������Ϣ��δ�����ݽ���У�飬�����Ҫȷ�������Ƿ�ɹ��������н������ݱȶԡ�
	//�����ж�
    DisableGlobalIRQ();
    //CSI �ɼ���ʼ��
    csi_init(SCC8660_CSI_W, SCC8660_CSI_H, &csi_handle, scc8660_csi_isr, SCC8660_CSI_VSYNC_PIN, SCC8660_CSI_PCLK_PIN);
    csi_add_empty_buffer(&csi_handle, (uint8 *)&scc8660_csi1_image[0][0]);
	csi_add_empty_buffer(&csi_handle, (uint8 *)&scc8660_csi2_image[0][0]);
    csi_start(&csi_handle);
    //����ͼ���ַ��ֵ����ر���
    scc8660_csi_image = scc8660_csi1_image;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCC8660(��ͫ����ͷ)ͼ���ϴ�����λ��
//  @param      uartn	��Ҫ���͵Ĵ���
//  @param      image	ͼ�������ַ
//  @param      width	ͼ��߶�
//  @param      height	ͼ����
//  @return     void                    
//  @since      v1.0
//  Sample usage:       csi_seekfree_sendimg_scc8660(USART_1,(uint8 *)scc8660_csi_image[0],SCC8660_CSI_PIC_W,SCC8660_CSI_PIC_H);
//-------------------------------------------------------------------------------------------------------------------
void csi_seekfree_sendimg_scc8660(UARTN_enum uartn, uint8 *image, uint16 width, uint16 height)
{
    uart_putchar(uartn,0x00);uart_putchar(uartn,0xff);uart_putchar(uartn,0x01);uart_putchar(uartn,0x01);//��������
    uart_putbuff(uartn, image, 2*width*height);  //����ͼ�� ��Ϊ��ɫͼ�� һ�����ص�ռ�������ֽ� ��������ֽ�����Ҫ����2
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCC8660(��ͫ����ͷ)ͼ���ϴ�����λ����ʹ��USB���⴮�ڷ��ͣ�
//  @param      uartn	��Ҫ���͵Ĵ���
//  @param      image	ͼ�������ַ
//  @param      width	ͼ��߶�
//  @param      height	ͼ����
//  @return     void                    
//  @since      v1.0
//  Sample usage:       seekfree_sendimg_scc8660_usb_cdc((uint8 *)scc8660_csi_image[0],SCC8660_CSI_PIC_W,SCC8660_CSI_PIC_H);
//-------------------------------------------------------------------------------------------------------------------
void seekfree_sendimg_scc8660_usb_cdc(uint8 *image, uint16 width, uint16 height)
{
	uint8 cmd[4];
	cmd[0] = 0x00; cmd[1] = 0xff; cmd[2] = 0x01; cmd[3] = 0x01; 
	usb_cdc_send_buff(cmd,4);					//��������
    usb_cdc_send_buff(image, width*height*2);  	//����ͼ�� ��Ϊ��ɫͼ�� һ�����ص�ռ�������ֽ� ��������ֽ�����Ҫ����2
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCC8660(��ͫ����ͷ)��ȡ���ص�RGB����
//  @param      *dat    ͼ������ĵ�ַ
//  @param      x       ��Ҫ��ȡ������������
//  @param      y       ��Ҫ��ȡ������������
//  @param      z       ͼ����
//  @param      *r      ����r������ַ ����ֵ��Χ0-31
//  @param      *g      ����g������ַ ����ֵ��Χ0-63
//  @param      *b      ����b������ַ ����ֵ��Χ0-31
//  @return     void					
//  @since      v1.0
//  Sample usage:		color_camera_take_point(scc8660_csi_image[0],0, 0, SCC8660_CSI_PIC_W, &r_value,&g_value,&b_value);//��ȡ��0�� ��0�����ص�RGB����  ����ͷ�����ݸ�ʽ��������Ϊ0
//	@note				�˺�����ҪĿ����Ϊ���ô���ܹ��������˽�RGB��������δ洢��
//-------------------------------------------------------------------------------------------------------------------
void inline color_camera_take_point(uint16 *dat, uint16 x, uint16 y, uint16 z, uint8 *r, uint8 *g, uint8 *b)
{
    uint16 pixel;
    
    //��ȡָ���������������
    pixel = dat[x+y*z];
    
    //��ΪR5G3�Ǵ洢�ڵͰ�λ G3B5�Ǵ洢�ڸ߰�λ
    //��Ϊ�����Ƚ�λ�ý��н��������ڻ�ȡÿ������������
    pixel = ((pixel&0xff)<<8) |(pixel>>8);
    
    *r = pixel>>11;
    *g = (pixel>>5)&0x3f;
    *b = pixel&0x1f;
}



