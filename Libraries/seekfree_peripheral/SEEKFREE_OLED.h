/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		OLED
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 * @note		
					���߶��壺
					------------------------------------ 
					    OLEDҺ��    ��Ƭ��                        
    					D0          �鿴SEEKFREE_OLED�ļ��ڵ�OLED_SCL_PIN   �궨��
    					D1          �鿴SEEKFREE_OLED�ļ��ڵ�OLED_SDA_PIN   �궨��            
    					RES         �鿴SEEKFREE_OLED�ļ��ڵ�OLED_RST_PIN   �궨��    
    					DC          �鿴SEEKFREE_OLED�ļ��ڵ�OLED_DC_PIN    �궨��
						CS          �鿴SEEKFREE_OLED�ļ��ڵ�OLED_CS_PIN    �궨��
					------------------------------------ 
 ********************************************************************************************************************/



#ifndef _SEEKFREE_OLED_h
#define _SEEKFREE_OLED_h


#include "common.h"




//----�궨��OLED����----	 
#define  OLED_SCL_PIN	B0
#define  OLED_SDA_PIN	B1
#define  OLED_RST_PIN	B2
#define  OLED_DC_PIN	C19
#define  OLED_CS_PIN	B3

                            
                            
//������ʾ����
//0 ����ģʽ
//1 ����ģʽ  ��ת180
#define OLED_DISPLAY_DIR 0

#if (0==OLED_DISPLAY_DIR || 1==OLED_DISPLAY_DIR)
#define X_WIDTH         128
#define Y_WIDTH         64

#else
#error "OLED_DISPLAY_DIR �������"
     
#endif
                            
#define	Brightness	    0x7f    //����OLED����  Խ��Խ��    ��Χ0-0XFF                            
#define XLevelL		    0x00
#define XLevelH		    0x10
#define XLevel		    ((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	    128
#define Max_Row		    64




void oled_init(void);     
void oled_fill(uint8 dat);
void oled_set_pos(uint8 x, uint8 y);
void oled_putpixel(uint8 x,uint8 y,uint8 data1);
void oled_clrpixel(uint8 x,uint8 y);
void oled_p6x8str(uint8 x,uint8 y,const int8 ch[]);
void oled_p8x16str(uint8 x,uint8 y,const int8 ch[]);
void oled_uint16(uint8 x, uint8 y, uint16 num);
void oled_int16(uint8 x, uint8 y, int16 num);
void oled_printf_int32(uint16 x,uint16 y,int32 dat,uint8 num);
void oled_printf_float(uint16 x,uint16 y,double dat,uint8 num,uint8 pointnum);
void oled_dis_bmp(uint16 high, uint16 width, uint8 *p,uint8 value);
void oled_print_chinese(uint8 x, uint8 y, uint8 size, const uint8 *p, uint8 len);


/********************************************************************/

#endif
