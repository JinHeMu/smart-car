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

#include "fsl_gpt.h"
#include "zf_systick.h"
#include "rtthread.h"

static uint8 gpt_init_flag;


//-------------------------------------------------------------------------------------------------------------------
//  @brief      gpt��ʼ��
//  @param      void            
//  @return     void
//  Sample usage:               �����û����ã��û���ʹ��h�ļ��еĺ궨��
//-------------------------------------------------------------------------------------------------------------------
void gpt_init(void)
{
    if(!gpt_init_flag)
    {
        gpt_config_t gptConfig;
        
        gpt_init_flag = 1;
        GPT_GetDefaultConfig(&gptConfig);           //��ȡĬ������
        GPT_Init(DELAY_GPT, &gptConfig);            //GPT��ʼ�� ���ڴ�ʱ��
        GPT_Deinit(DELAY_GPT);                      //GPT����ʼ��
        GPT_Init(DELAY_GPT, &gptConfig);            //GPT��ʼ��
        GPT_SetClockDivider(DELAY_GPT, GPT_DIV);    //���÷�Ƶϵ��
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      systick��ʱ����
//  @param      time            ��Ҫ��ʱ��ʱ��
//  @return     void
//  Sample usage:               �����û����ã��û���ʹ��h�ļ��еĺ궨��
//-------------------------------------------------------------------------------------------------------------------
void systick_delay(uint32 time)
{
#if(0==DELAY_TIMER_TYPE)
    if(time == 0)   return;
    assert(SysTick_LOAD_RELOAD_Msk >= time);//����   ��ʱʱ�����С�ڻ����SysTick_LOAD_RELOAD_Msk
    SysTick->CTRL = 0x00;                   //�ȹ��� systick ,���־λ
    SysTick->LOAD = time;                   //������ʱʱ��
    SysTick->VAL = 0x00;                    //��ռ�����
    SysTick->CTRL = ( 0 | SysTick_CTRL_ENABLE_Msk     //ʹ�� systick
                  //| SysTick_CTRL_TICKINT_Msk        //ʹ���ж� (ע���˱�ʾ�ر��ж�)
                    | SysTick_CTRL_CLKSOURCE_Msk      //ʱ��Դѡ�� (core clk)
                );
    while( !(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));//�ȴ�ʱ�䵽
    
#elif(1==DELAY_TIMER_TYPE)
    
    
    gpt_init();
    GPT_SetOutputCompareValue(DELAY_GPT, kGPT_OutputCompare_Channel1, time);//���ö�ʱʱ��
    GPT_StartTimer(DELAY_GPT);                      //������ʱ��
    while(!GPT_GetStatusFlags(DELAY_GPT,kGPT_OutputCompare1Flag));
    GPT_ClearStatusFlags(DELAY_GPT,kGPT_OutputCompare1Flag);
    GPT_StopTimer(DELAY_GPT);
#endif
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���뼶systick��ʱ����
//  @param      ms              ��ʱ���ٺ���
//  @return     void
//  Sample usage:               systick_delay_ms(1000);   //��ʱ1000����
//-------------------------------------------------------------------------------------------------------------------
void systick_delay_ms(uint32 ms)
{
    //get_clk();//��ȡ�ں�ʱ�ӱ��ں�������
//	while(ms--) systick_delay(MSEC_TO_COUNT(1, DELAY_CLK));
	rt_thread_mdelay(ms);
	
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      systick��ʱ��
//  @param      time            ��ʱʱ��(0-0x00ffffff)
//  @return     void
//  Sample usage:               �����û����ã��û���ʹ��h�ļ��еĺ궨��
//-------------------------------------------------------------------------------------------------------------------
void systick_timing(uint32 time)
{
#if(0==DELAY_TIMER_TYPE)
    assert(SysTick_LOAD_RELOAD_Msk >= time);//����   ��ʱʱ�����С�ڻ����SysTick_LOAD_RELOAD_Msk
    SysTick->LOAD = time;                   //������ʱʱ��
    SysTick->VAL = 0x00;       	            //��ռ�����
    SysTick->CTRL = ( 0 
					| SysTick_CTRL_ENABLE_Msk       //ʹ�� systick
                    | SysTick_CTRL_TICKINT_Msk      //ʹ���ж�
                    | SysTick_CTRL_CLKSOURCE_Msk    //ʱ��Դѡ�� (core clk) 
                );
#elif(1==DELAY_TIMER_TYPE)
    
    gpt_init();
    GPT_SetOutputCompareValue(DELAY_GPT, kGPT_OutputCompare_Channel1, time);//���ö�ʱʱ��
    GPT_StartTimer(DELAY_GPT);                      //������ʱ��
    GPT_EnableInterrupts(DELAY_GPT,GPT_IR_OF1IE_MASK);//ʹ���ж�
    EnableIRQ(GPT2_IRQn);
#endif
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      systick��ʱ������
//  @param      void
//  @return     void
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void systick_start(void)
{
#if(0==DELAY_TIMER_TYPE)
    SysTick->LOAD = SysTick_LOAD_RELOAD_Msk;//������ʱʱ��
    SysTick->VAL = 0x00;       	            //��ռ�����
    SysTick->CTRL = ( 0 
					| SysTick_CTRL_ENABLE_Msk       //ʹ�� systick
                    //| SysTick_CTRL_TICKINT_Msk      //ʹ���ж�
                    | SysTick_CTRL_CLKSOURCE_Msk    //ʱ��Դѡ�� (core clk) 
                );
#elif(1==DELAY_TIMER_TYPE)
    
    gpt_init();
    GPT_SetOutputCompareValue(DELAY_GPT, kGPT_OutputCompare_Channel1, 0xffffffff);//���ö�ʱʱ��
    GPT_StartTimer(DELAY_GPT);                      //������ʱ��
#endif
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��õ�ǰSystem tick timer��ֵ
//  @return     ���ص�ǰSystem tick timer��ֵ
//  Sample usage:               uint32 tim = systick_getval();   
//-------------------------------------------------------------------------------------------------------------------
uint32 systick_getval(void)
{
#if(0==DELAY_TIMER_TYPE)
    return (SysTick_LOAD_RELOAD_Msk - SysTick->VAL);
#elif(1==DELAY_TIMER_TYPE)
    return (GPT_GetCurrentTimerCount(DELAY_GPT));
#endif
}