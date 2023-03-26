/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		�����
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-02-18
 * @note		
					���߶��壺
					------------------------------------ 
						ģ��ܽ�            ��Ƭ���ܽ�
						SDA(51��RX)         �鿴SEEKFREE_MT9V03X.h�ļ��е�MT9V03X_COF_UART_TX�궨��
						SCL(51��TX)         �鿴SEEKFREE_MT9V03X.h�ļ��е�MT9V03X_COF_UART_RX�궨��
						���ж�(VSY)         �鿴SEEKFREE_MT9V03X.h�ļ��е�MT9V03X_VSYNC_PIN�궨��
						���ж�(HREF)		�鿴SEEKFREE_MT9V03X.h�ļ��е�MT9V03X_HREF_PIN�궨��
						�����ж�(PCLK)      �鿴SEEKFREE_MT9V03X.h�ļ��е�MT9V03X_PCLK_PIN�궨��
						���ݿ�(D0-D7)		�鿴SEEKFREE_MT9V03X.h�ļ��е�MT9V03X_DATA_PIN�궨��
					------------------------------------ 
	
					Ĭ�Ϸֱ�����            188*120
					Ĭ��FPS                 50֡
 ********************************************************************************************************************/



#ifndef _SEEKFREE_MT9V03X_h
#define _SEEKFREE_MT9V03X_h

#include "common.h"
#include "fsl_edma.h"
#include "zf_uart.h"

//��������ͷ����
#define MT9V03X_DMA_CH          DMA_CH0         //��������ͷ��DMA�ɼ�ͨ��
#define MT9V03X_W               188             //ͼ����  ��Χ1-752       RT102X RT105X RT106X �ɼ�ʱ�п�ȱ���Ϊ4�ı���
#define MT9V03X_H               120             //ͼ��߶�	��Χ1-480



//--------------------------------------------------------------------------------------------------
//��������
//--------------------------------------------------------------------------------------------------
#define MT9V03X_COF_UART        USART_4         //��������ͷ��ʹ�õ��Ĵ���     
#define MT9V03X_COF_UART_TX     UART4_TX_C16
#define MT9V03X_COF_UART_RX     UART4_RX_C17

#define MT9V03X_VSYNC_PIN       C7              //���ж�����
#define MT9V03X_VSYNC_IRQN      GPIO2_Combined_0_15_IRQn   //�жϺ�


#define MT9V03X_DATA_PIN        FLEXIO2_D08_C8  //����D0��������  ����D0����ΪFLEXIO2_D08_C8 ��ôD1��ʹ�õ�������ΪFLEXIO2_D09_C9����������
#define MT9V03X_PCLK_PIN        FLEXIO2_D05_C5  //��������ʱ������
#define MT9V03X_HREF_PIN        FLEXIO2_D06_C6  //�������ź�����

     //��ʼ����ʱ����
#define MT9V03X_INIT_TIMEOUT    0x0080
                        
//����ͷ����ö��
typedef enum
{
    INIT = 0,               //����ͷ��ʼ������
    AUTO_EXP,               //�Զ��ع�����
    EXP_TIME,               //�ع�ʱ������
    FPS,                    //����ͷ֡������
    SET_COL,                //ͼ��������
    SET_ROW,                //ͼ��������
    LR_OFFSET,              //ͼ������ƫ������
    UD_OFFSET,              //ͼ������ƫ������
    GAIN,                   //ͼ��ƫ������
    PCLK_MODE,              //����ʱ��ģʽ
    CONFIG_FINISH,          //������λ����Ҫ����ռλ����
    
    COLOR_GET_WHO_AM_I = 0xEF,
    SET_EXP_TIME = 0XF0,    //���������ع�ʱ������
    GET_STATUS,             //��ȡ����ͷ��������
    GET_VERSION,            //�̼��汾������
	
    SET_ADDR = 0XFE,        //�Ĵ�����ַ����
    SET_DATA                //�Ĵ�����������
}CMD;
     
     
     
extern uint8    receive[3];         //�����ڽ���������Ϣʱ����  ��ֹ�û�ʹ�øñ���
extern uint8    receive_num;        //�����ڽ���������Ϣʱ����  ��ֹ�û�ʹ�øñ���
extern vuint8   uart_receive_flag;  //�����ڽ���������Ϣʱ����  ��ֹ�û�ʹ�øñ���


extern uint8    mt9v03x_finish_flag;//һ��ͼ��ɼ���ɱ�־λ
extern uint8    mt9v03x_image[MT9V03X_H][MT9V03X_W];


void set_config(UARTN_enum uartn, int16 buff[CONFIG_FINISH-1][2]);
void get_config(UARTN_enum uartn, int16 buff[CONFIG_FINISH-1][2]);
uint16 get_version(UARTN_enum uartn);
uint16 set_exposure_time(UARTN_enum uartn, uint16 light);

void mt9v03x_init(void);
void mt9v03x_vsync(void);
void mt9v03x_dma(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds);
void seekfree_sendimg_03x(UARTN_enum uartn, uint8 *image, uint16 width, uint16 height);
void seekfree_sendimg_03x_usb_cdc(uint8 *image, uint16 width, uint16 height);



#endif

