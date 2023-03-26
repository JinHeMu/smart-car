/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		CSI
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/

#ifndef _zf_csi_h
#define _zf_csi_h

#include "common.h"
#include "fsl_csi.h"

extern uint32 fullCameraBufferAddr;    //�ɼ���ɵĻ�������ַ    �û��������
extern csi_handle_t csi_handle;        //csi����ṹ��

//��ö�ٶ��岻�����û��޸�
typedef enum //ö�ٴ�������
{
    CSI_VSYNC_B14,
    CSI_VSYNC_B22,
    CSI_VSYNC_C29,
}VSYNCPIN_enum;

//��ö�ٶ��岻�����û��޸�
typedef enum //ö�ٴ�������
{
    CSI_PIXCLK_B20,
    CSI_PIXCLK_C28,
}PIXCLKPIN_enum;


void csi_init(uint16 width, uint16 height, csi_handle_t *handle, csi_transfer_callback_t callback, VSYNCPIN_enum vsync, PIXCLKPIN_enum pixclk);
void csi_add_empty_buffer(csi_handle_t *handle, uint8 *buff);
uint8 csi_get_full_buffer(csi_handle_t *handle, uint32 *buffaddr);
void csi_start(csi_handle_t *handle);
void csi_stop(csi_handle_t *handle);







#endif