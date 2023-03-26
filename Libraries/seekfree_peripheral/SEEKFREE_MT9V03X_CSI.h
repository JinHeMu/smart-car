/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		����� RT CSI�ӿ�
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
						SDA(51��RX)         �鿴SEEKFREE_MT9V03X_CSI.h�ļ��е�MT9V03X_CSI_COF_UART_TX�궨��
						SCL(51��TX)         �鿴SEEKFREE_MT9V03X_CSI.h�ļ��е�MT9V03X_CSI_COF_UART_RX�궨��
						���ж�(VSY)         �鿴SEEKFREE_MT9V03X_CSI.h�ļ��е�MT9V03X_CSI_VSYNC_PIN�궨��
						���ж�(HREF)	    ����Ҫʹ��
						�����ж�(PCLK)      �鿴SEEKFREE_MT9V03X_CSI.h�ļ��е�MT9V03X_CSI_PCLK_PIN�궨��
						���ݿ�(D0-D7)		B31-B24 B31��Ӧ����ͷ�ӿ�D0
					------------------------------------ 
	
					Ĭ�Ϸֱ�����            188*120
					Ĭ��FPS                 50֡
 ********************************************************************************************************************/



#ifndef _SEEKFREE_MT9V03X_CSI_h
#define _SEEKFREE_MT9V03X_CSI_h


#include "common.h"
#include "zf_uart.h"

//��������ͷ����
#define MT9V03X_CSI_W               188             //ͼ����  ��Χ1-752      RT105X RT106X �ɼ�ʱ�п�ȱ���Ϊ4�ı���
#define MT9V03X_CSI_H               120             //ͼ��߶�	��Χ1-480



//--------------------------------------------------------------------------------------------------
//��������
//--------------------------------------------------------------------------------------------------
#define MT9V03X_CSI_COF_UART        USART_5         //��������ͷ��ʹ�õ��Ĵ���     
#define MT9V03X_CSI_COF_UART_TX     UART5_TX_C28
#define MT9V03X_CSI_COF_UART_RX     UART5_RX_C29


#define MT9V03X_CSI_PCLK_PIN        CSI_PIXCLK_B20  //��������ʱ������
#define MT9V03X_CSI_VSYNC_PIN       CSI_VSYNC_B22   //���峡�ź�����
//���������������ﲻ�ṩ���õ��������ŵĶ���
//���ڵڶ��������ɲ���û���������Գ���ֱ�ӹ̶�ʹ��B31-B24��������ͷ����������

                            

extern uint8 mt9v03x_csi_finish_flag;               //һ��ͼ��ɼ���ɱ�־λ
extern uint8 (*mt9v03x_csi_image)[MT9V03X_CSI_W];          //ͼ������

void mt9v03x_csi_init(void);
void csi_seekfree_sendimg_03x(UARTN_enum uartn, uint8 *image, uint16 width, uint16 height);










#endif
