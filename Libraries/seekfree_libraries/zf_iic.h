/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		iic
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/
 
 
#ifndef _zf_iic_h
#define _zf_iic_h

#include "common.h"
#include "fsl_lpi2c.h"

//��ö�ٶ��岻�����û��޸�
typedef enum //ö��IIC����
{
    IIC1_SDA_B17,  IIC1_SDA_D5 ,//ע����İ岢δ����D5
    IIC1_SCL_B16,  IIC1_SCL_D4 ,
                       
    IIC2_SDA_C5 ,  IIC2_SDA_D10,
    IIC2_SCL_C4 ,  IIC2_SCL_D11,
                       
    IIC3_SDA_B22,  IIC3_SDA_D13,    IIC3_SDA_E21,//ע����İ岢δ����E21
    IIC3_SCL_B23,  IIC3_SCL_D12,    IIC3_SCL_E22,//ע����İ岢δ����E22
                      
    IIC4_SDA_B13,  IIC4_SDA_E11,//ע����İ岢δ����E11
    IIC4_SCL_B12,  IIC4_SCL_E12,//ע����İ岢δ����E12
    
}IIC_PIN_enum;

//��ö�ٶ��岻�����û��޸�
typedef enum //ö��IIC���
{
    IIC_0,//RT1064û��IIC_0 ���������ռλ
    IIC_1,
    IIC_2,
    IIC_3,
    IIC_4,
}IICN_enum;



void iic_init(IICN_enum iic_n, IIC_PIN_enum sda_pin, IIC_PIN_enum scl_pin, uint32 baud);
uint8 iic_write_reg(IICN_enum iic_n, uint8 slaveid, uint8 reg, uint8 data);
uint8 iic_read_reg(IICN_enum iic_n, uint8 slaveid, uint8 reg, uint8 *data);
uint8 iic_read_reg_bytes(IICN_enum iic_n, uint8 slaveid, uint8 reg, uint8 *data, uint8 num);


#endif