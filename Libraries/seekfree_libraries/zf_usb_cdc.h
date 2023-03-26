/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		USB���⴮��
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-11-04
 ********************************************************************************************************************/
 
#ifndef _zf_usb_cdc_h
#define _zf_usb_cdc_h

#include "common.h"


void  usb_cdc_init(void);
uint8 usb_check_busy(void);
void  usb_cdc_send_char(uint8 dat);
void  usb_cdc_send_str(const int8 *str);
void  usb_cdc_send_buff(uint8 *p, uint32 length);

#endif
