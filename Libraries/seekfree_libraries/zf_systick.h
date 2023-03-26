/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		systick
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/
 
 
 
 
#ifndef _zf_systick_h
#define _zf_systick_h

#include "common.h"
#include "fsl_lpspi.h"

#define DELAY_TIMER_TYPE            1		//0������systick������ʱ   1������GPT������ʱ
    
    
#define DELAY_GPT 					GPT2	//ѡ��ʹ�õ�GPT��ʱ��
#define GPT_DIV   					2		//����GPT����ʱ�ӷ�Ƶ
    
#if(0==DELAY_TIMER_TYPE)    
    #define DELAY_CLK               CLOCK_GetFreq(kCLOCK_CpuClk)//����SYSTICK��ʱ��������ʱ��
#else   
    #define DELAY_CLK               (CLOCK_GetFreq(kCLOCK_PerClk)/(GPT_DIV))//����GPTģ��ʱ��
#endif



void systick_delay(uint32 time);
void systick_timing(uint32 time);
void systick_start(void);
uint32 systick_getval(void);


//------------------------------------���º궨��������ʱ------------------------------------
void systick_delay_ms(uint32 ms);                                                        //���뼶��ʱ���� ���ڵδ�ʱ�����ֻ��24λ������ú���ʵ�֣�������ʱʱ�䷶Χ�Ͽ�
#define systick_delay_us(time)      systick_delay(USEC_TO_COUNT(time, DELAY_CLK))        //������ʱʱ��  ��λus   ��Χ0  -  16777215(0xffffff)/(DELAY_CLK*1000)ms   ��Ƶ500M systick�����ʱʱ���ԼΪ33ms
#define systick_delay_ns(time)      systick_delay(USEC_TO_COUNT(time, DELAY_CLK/1000))   //������ʱʱ��  ��λns   ��Χ0  -  16777215(0xffffff)*1000/(DELAY_CLK)ns   ��Ƶ500M systick�����ʱʱ���ԼΪ33ms
//������ʱ��������DELAY_TIMER_TYPE�궨��ѡ��  ��ʱ��ʹ�õĶ�ʱ�����ͣ���ʹ�ÿ�������ļ�ϵͳʱ�뽫�궨������Ϊ1����Ϊ�ļ�ϵͳ��Ҫռ��systick��ʱ��


    
//------------------------------------���º궨�����ڶ�ʱ------------------------------------  
#define systick_timing_ms(time)     systick_timing(MSEC_TO_COUNT(time, DELAY_CLK))       //���ö�ʱʱ��  ��λms   ��Χ0  -  16777215(0xffffff)/(DELAY_CLK*1000)ms   ��Ƶ500M ���ʱʱ���ԼΪ33ms
#define systick_timing_us(time)     systick_timing(USEC_TO_COUNT(time, DELAY_CLK))       //���ö�ʱʱ��  ��λus   ��Χ0  -  16777215(0xffffff)/(DELAY_CLK)us        ��Ƶ500M ���ʱʱ���ԼΪ33ms
#define systick_timing_ns(time)     systick_timing(USEC_TO_COUNT(time, DELAY_CLK/1000))  //���ö�ʱʱ��  ��λns   ��Χ0  -  16777215(0xffffff)*1000/(DELAY_CLK)ns   ��Ƶ500M ���ʱʱ���ԼΪ33ms


//------------------------------------���º궨�����ڻ�ȡ��ǰʱ��------------------------------------
#define systick_getval_ms()         COUNT_TO_MSEC(systick_getval(),DELAY_CLK)            //��ȡ��ǰ��ʱʱ��  ��λms
#define systick_getval_us()         COUNT_TO_USEC(systick_getval(),DELAY_CLK)            //��ȡ��ǰ��ʱʱ��  ��λus
#define systick_getval_ns()         COUNT_TO_USEC(systick_getval()*1000,DELAY_CLK)       //��ȡ��ǰ��ʱʱ��  ��λns

#endif