/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		pit
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/
 
#ifndef _zf_pit_h
#define _zf_pit_h


#include "common.h"
#include "fsl_pit.h"

typedef enum
{
    PIT_CH0 = kPIT_Chnl_0,
    PIT_CH1,
    PIT_CH2,
    PIT_CH3,
    PIT_MAX,
}PIT_enum;


#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_PerClk)//����PIT��ʱ��������ʱ��



void pit_init(void);
void pit_interrupt(PIT_enum pit_ch, uint32 count);
void pit_delay(PIT_enum pit_ch, uint32 count);
void pit_start(PIT_enum pit_ch);
uint32 pit_get(PIT_enum pit_ch);
void pit_close(PIT_enum pit_ch);

//��ȡPIT��ʱ���ı�־λ
#define PIT_FLAG_GET(pit_ch)            PIT_GetStatusFlags(PIT, (pit_chnl_t)pit_ch)

//���PIT��ʱ���ı�־λ
#define PIT_FLAG_CLEAR(pit_ch)          PIT_ClearStatusFlags(PIT, (pit_chnl_t)pit_ch, kPIT_TimerFlag)

//------------------------------------���´�������PIT�ж�------------------------------------
//-----------------���ñ��꺯��ǰ������ȵ���pit_init�������Գ�ʼ��PIT����-------------------
#define pit_interrupt_ms(pit_ch, time)  pit_interrupt(pit_ch, MSEC_TO_COUNT(time, PIT_SOURCE_CLOCK))    //(��λΪ ����)
#define pit_interrupt_us(pit_ch, time)  pit_interrupt(pit_ch, USEC_TO_COUNT(time, PIT_SOURCE_CLOCK))    //(��λΪ ΢��)

//------------------------------------���´�������PIT��ʱ------------------------------------
//-----------------���ñ��꺯��ǰ������ȵ���pit_init�������Գ�ʼ��PIT����-------------------
#define pit_delay_ms(pit_ch, time)      pit_delay(pit_ch, MSEC_TO_COUNT(time, PIT_SOURCE_CLOCK))        //(��λΪ ����)
#define pit_delay_us(pit_ch, time)      pit_delay(pit_ch, USEC_TO_COUNT(time, PIT_SOURCE_CLOCK))        //(��λΪ ΢��)
#define pit_delay_ns(pit_ch, time)      pit_delay(pit_ch, USEC_TO_COUNT(time, PIT_SOURCE_CLOCK/1000))   //(��λΪ ����)

//------------------------------------���´������ڻ�ȡPIT��ʱʱ��------------------------------------
//-----------------���ñ��꺯��ǰ������ȵ���pit_init�������Գ�ʼ��PIT����-------------------
#define pit_get_ms(pit_ch)              COUNT_TO_MSEC(pit_get(pit_ch),PIT_SOURCE_CLOCK)                 //(��λΪ ����)
#define pit_get_us(pit_ch)              COUNT_TO_USEC(pit_get(pit_ch),PIT_SOURCE_CLOCK)                 //(��λΪ ΢��)
#define pit_get_ns(pit_ch)              COUNT_TO_USEC((uint64)pit_get(pit_ch)*1000,PIT_SOURCE_CLOCK)    //(��λΪ ����)


#endif