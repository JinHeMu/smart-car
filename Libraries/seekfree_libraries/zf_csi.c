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

#include "zf_iomuxc.h"
#include "zf_csi.h"

#define CSI_PIN_CONF        SPEED_100MHZ | KEEPER_EN | DSE_R0_6 //����CSI����Ĭ������   

csi_handle_t csi_handle;        //csi����ṹ��
uint32 fullCameraBufferAddr;    //�ɼ���ɵĻ�������ַ    �û��������

void csi_iomuxc(VSYNCPIN_enum vsync, PIXCLKPIN_enum pixclk)
{
    //���ݶ˿����Ÿ�������
    iomuxc_pinconf(B24,ALT4,CSI_PIN_CONF);
    iomuxc_pinconf(B25,ALT4,CSI_PIN_CONF);
    iomuxc_pinconf(B26,ALT4,CSI_PIN_CONF);
    iomuxc_pinconf(B27,ALT4,CSI_PIN_CONF);
    iomuxc_pinconf(B28,ALT4,CSI_PIN_CONF);
    iomuxc_pinconf(B29,ALT4,CSI_PIN_CONF);
    iomuxc_pinconf(B30,ALT4,CSI_PIN_CONF);
    iomuxc_pinconf(B31,ALT4,CSI_PIN_CONF);
    
    if      (CSI_VSYNC_B14 == vsync)    iomuxc_pinconf(B14,ALT4,CSI_PIN_CONF);
    else if (CSI_VSYNC_B22 == vsync)    iomuxc_pinconf(B22,ALT4,CSI_PIN_CONF);
    else if (CSI_VSYNC_C29 == vsync)    iomuxc_pinconf(C29,ALT2,CSI_PIN_CONF);
    
    if      (CSI_PIXCLK_B20 == pixclk)  iomuxc_pinconf(B20,ALT4,CSI_PIN_CONF);
    else if (CSI_PIXCLK_C28 == pixclk)  iomuxc_pinconf(C28,ALT2,CSI_PIN_CONF);
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      CSI��ʼ��
//  @param      width       ͼ�����
//  @param      height      ͼ�����
//  @param      handle      CSI��handle�ṹ�� ��MT9V032_CSI�Ѿ��������
//  @param      callback    �жϵ�ʱ��Ļص�����
//  @return     void			
//  @since      v1.0
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
void csi_init(uint16 width, uint16 height, csi_handle_t *handle, csi_transfer_callback_t callback, VSYNCPIN_enum vsync, PIXCLKPIN_enum pixclk)
{
    uint16 i;
    uint32 pixel_num;
    uint16 temp_width;
    uint16 temp_height=0;
    csi_config_t csi_config;

    csi_iomuxc(vsync,pixclk);
    
    //CSI �ɼ���ʼ��
    CLOCK_SetMux(kCLOCK_CsiMux, 2);
    CLOCK_SetDiv(kCLOCK_CsiDiv, 0);
        
    //����ͷ�ɼ���ʼ��
    DisableGlobalIRQ();
    pixel_num = width*height;
    if(pixel_num%8) assert(0);//���ص㲻��8�ı��� �޷��ɼ�
    if(width%8)
    {
        temp_width = width - width%8;//������ʱ���
        i=1;
        while(pixel_num>(temp_width+(i*8)))
        {
            if(!(pixel_num%((temp_width+(i*8))))) 
            {
                temp_width += (i*8);//������ʱ���
                temp_height = pixel_num/temp_width;
                break;
            }
            i++;
        }
        if(!temp_height)
        {
            //���û���ҵ������ʱ���������
            i=1;
            while((temp_width-(i*8)))
            {
                if(!(pixel_num%((temp_width-(i*8))))) 
                {
                    temp_width -= (i*8);//������ʱ���
                    temp_height = pixel_num/temp_width;
                    break;
                }
                i++;
            }
        }
        if(!temp_height)
        {
            //�������û���ҵ�
            temp_width = pixel_num;
            temp_height = 1;
        }
    }
    else
    {
        temp_width = width;
        temp_height = height;
    }
    
    CSI_GetDefaultConfig(&csi_config);
    csi_config.width = temp_width;
    csi_config.height = temp_height;
    csi_config.polarityFlags = kCSI_DataLatchOnRisingEdge;
    csi_config.bytesPerPixel = 1U;
    csi_config.linePitch_Bytes = temp_width;
    csi_config.workMode = kCSI_NonGatedClockMode;
    csi_config.dataBus = kCSI_DataBus8Bit;
    csi_config.useExtVsync = true;
    CSI_Init(CSI, &csi_config);
    CSI_TransferCreateHandle(CSI,handle,callback,NULL);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      CSI��ӿջ�������ַ
//  @param      handle      CSI��handle�ṹ�� ��MT9V032_CSI�Ѿ��������
//  @param      *buff       ���������׵�ַ
//  @return     void			
//  @since      v1.0
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
void csi_add_empty_buffer(csi_handle_t *handle, uint8 *buff)
{
    CSI_TransferSubmitEmptyBuffer(CSI,handle,(uint32_t)buff);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      CSI��ȡ�ɼ���ɵĻ�������ַ
//  @param      handle      CSI��handle�ṹ�� ��MT9V032_CSI�Ѿ��������
//  @param      buffaddr    ��ȡ���ĵ�ַ
//  @return     void			
//  @since      v1.0
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
uint8 csi_get_full_buffer(csi_handle_t *handle, uint32 *buffaddr)
{
    if(kStatus_Success == CSI_TransferGetFullBuffer(CSI,handle,(uint32_t *)buffaddr))
    {
        return 1;//��ȡ���ɼ���ɵ�BUFFER
    }
    return 0;    //δ�ɼ����
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      CSI����
//  @param      handle      CSI��handle�ṹ�� ��MT9V032_CSI�Ѿ��������
//  @return     void			
//  @since      v1.0
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
void csi_start(csi_handle_t *handle)
{
    CSI_TransferStart(CSI,handle);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      CSIֹͣ
//  @param      handle      CSI��handle�ṹ�� ��MT9V032_CSI�Ѿ��������
//  @return     void			
//  @since      v1.0
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
void csi_stop(csi_handle_t *handle)
{
    CSI_TransferStop(CSI,handle);
}



