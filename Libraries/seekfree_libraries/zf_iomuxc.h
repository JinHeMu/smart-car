/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		iomuxc
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/
 
 
#ifndef _zf_iomuxc_h
#define _zf_iomuxc_h

#include "MIMXRT1064.h"
#include "common.h"

typedef enum
{
    ALT0,
    ALT1,
    ALT2,
    ALT3,
    ALT4,
    ALT5,
    ALT6,
    ALT7,
    ALT8,
    ALT9,
    ALT_MAX,
}ALT_enum;



typedef enum
{
    HYS_EN          = 1<<IOMUXC_SW_PAD_CTL_PAD_HYS_SHIFT,   //�ͺ�ʹ��
    
    PULLDOWN_100K   = 0<<IOMUXC_SW_PAD_CTL_PAD_PUS_SHIFT,   //�˿�Ϊ����ʱ��Ч
    PULLUP_47K      = 1<<IOMUXC_SW_PAD_CTL_PAD_PUS_SHIFT,   //�˿�Ϊ����ʱ��Ч
    PULLUP_100K     = 2<<IOMUXC_SW_PAD_CTL_PAD_PUS_SHIFT,   //�˿�Ϊ����ʱ��Ч
    PULLUP_22K      = 3<<IOMUXC_SW_PAD_CTL_PAD_PUS_SHIFT,   //�˿�Ϊ����ʱ��Ч
    
    PULL_EN         = 1<<IOMUXC_SW_PAD_CTL_PAD_PKE_SHIFT | 1<<IOMUXC_SW_PAD_CTL_PAD_PUE_SHIFT,
    KEEPER_EN       = 1<<IOMUXC_SW_PAD_CTL_PAD_PKE_SHIFT,   //������ʹ��
    
    OD_EN           = 1<<IOMUXC_SW_PAD_CTL_PAD_ODE_SHIFT,   //��©ʹ��
    
    SPEED_50MHZ     = 0<<IOMUXC_SW_PAD_CTL_PAD_SPEED_SHIFT,
    SPEED_100MHZ    = 1<<IOMUXC_SW_PAD_CTL_PAD_SPEED_SHIFT,
    SPEED_200MHZ    = 3<<IOMUXC_SW_PAD_CTL_PAD_SPEED_SHIFT,
    
    DSE_DIS         = 0<<IOMUXC_SW_PAD_CTL_PAD_DSE_SHIFT,   //�������ǿ������
    DSE_R0          = 1<<IOMUXC_SW_PAD_CTL_PAD_DSE_SHIFT,   //3.3Vʱ150ohm  1.8Vʱ260ohm
    DSE_R0_2        = 2<<IOMUXC_SW_PAD_CTL_PAD_DSE_SHIFT,   //R/2
    DSE_R0_3        = 3<<IOMUXC_SW_PAD_CTL_PAD_DSE_SHIFT,   //R/3
    DSE_R0_4        = 4<<IOMUXC_SW_PAD_CTL_PAD_DSE_SHIFT,   //R/4
    DSE_R0_5        = 5<<IOMUXC_SW_PAD_CTL_PAD_DSE_SHIFT,   //R/5
    DSE_R0_6        = 6<<IOMUXC_SW_PAD_CTL_PAD_DSE_SHIFT,   //R/6
    DSE_R0_7        = 7<<IOMUXC_SW_PAD_CTL_PAD_DSE_SHIFT,   //R/7
        
    SRE_SLOW        = 0<<IOMUXC_SW_PAD_CTL_PAD_SRE_SHIFT,
    SRE_FAST        = 1<<IOMUXC_SW_PAD_CTL_PAD_SRE_SHIFT,
    
}PINCONF_enum;




void iomuxc_pinconf(PIN_enum pin, ALT_enum alt, uint32 pinconf);


#endif