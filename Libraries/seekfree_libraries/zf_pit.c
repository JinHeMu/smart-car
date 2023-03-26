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
 
#include "fsl_common.h"
#include "zf_pit.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���ڶ�ʱ��ģ���ʼ��
//  @param      void
//  @param      void     
//  @return     void
//  Sample usage:           �����Ҫʹ�������жϡ���ʱ����ʱ�ȹ��ܶ���Ҫ��PIT���г�ʼ��֮����ܵ�����Ӧ���ܺ���
//-------------------------------------------------------------------------------------------------------------------
void pit_init(void)
{
    pit_config_t pitConfig;
    
    PIT_GetDefaultConfig(&pitConfig);   //Ĭ������Ϊfalse
              
    PIT_Init(PIT, &pitConfig);          //��һ�γ�ʼ�����ڴ�ʱ��
    PIT_Deinit(PIT);                    //��λ����
    PIT_Init(PIT, &pitConfig);          //���³�ʼ��������ȷ�Ĳ���
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����ж� ʱ������
//  @param      pit_ch      ѡ��ģ���ͨ�� (ѡ��Χ ��PIT_enumö��ֵ������ȷ��)
//  @param      count       ������ֵ
//  @return     void
//  Sample usage:           ������ͨ���û�������ģ������Ҫʹ�������жϹ��ܣ�Ӧ�õ���h�ļ��ĺ궨�庯��������ʱ�䵥λ��
//-------------------------------------------------------------------------------------------------------------------
void pit_interrupt(PIT_enum pit_ch, uint32 count)
{
    PIT_SetTimerPeriod(PIT, (pit_chnl_t)pit_ch, count);
    PIT_EnableInterrupts(PIT, (pit_chnl_t)pit_ch, kPIT_TimerInterruptEnable);//��PITͨ��0�ж�
	PIT_SetTimerChainMode(PIT, (pit_chnl_t)pit_ch,false);
    PIT_StartTimer(PIT, (pit_chnl_t)pit_ch);
    EnableIRQ(PIT_IRQn);
    
    //NVIC_SetPriority(PIT_IRQn,0);//���ȼ����� ��Χ0-15 ԽС���ȼ�Խ��
    
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʱ ʱ������
//  @param      pit_ch      ѡ��ģ���ͨ�� (ѡ��Χ ��PIT_enumö��ֵ������ȷ��)
//  @param      count       ������ֵ
//  @return     void
//  Sample usage:           ������ͨ���û�������ģ������Ҫʹ����ʱ���ܣ�Ӧ�õ���h�ļ��ĺ궨�庯��������ʱ�䵥λ��
//-------------------------------------------------------------------------------------------------------------------
void pit_delay(PIT_enum pit_ch, uint32 count)
{
    PIT_SetTimerPeriod(PIT, (pit_chnl_t)pit_ch, count);
	PIT_SetTimerChainMode(PIT, (pit_chnl_t)pit_ch,false);
    PIT_StartTimer(PIT, (pit_chnl_t)pit_ch);
    
    while(!PIT_GetStatusFlags(PIT, (pit_chnl_t)pit_ch));//�ȴ���ʱ����
    PIT_StopTimer(PIT, (pit_chnl_t)pit_ch);
    PIT_ClearStatusFlags(PIT, (pit_chnl_t)pit_ch, kPIT_TimerFlag);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʱ����ʼ
//  @param      pit_ch      ѡ��ģ���ͨ�� (ѡ��Χ ��PIT_enumö��ֵ������ȷ��)
//  @return     void
//  Sample usage:           pit_start(PIT_CH0); //0ͨ�� ��ʼ����
//-------------------------------------------------------------------------------------------------------------------
void pit_start(PIT_enum pit_ch)
{
    PIT_SetTimerPeriod(PIT, (pit_chnl_t)pit_ch, 0xFFFFFFFF);
	PIT_SetTimerChainMode(PIT, (pit_chnl_t)pit_ch,false);
    PIT_StartTimer(PIT, (pit_chnl_t)pit_ch);
    
    
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ��ʱ��ֵ
//  @param      pit_ch      ѡ��ģ���ͨ�� (ѡ��Χ ��PIT_enumö��ֵ������ȷ��)
//  @return     uint32      ����ֵ
//  Sample usage:           ������ͨ���û�������ģ������Ҫ��ȡ����ֵ��Ӧ�õ���h�ļ��ĺ궨�庯��������ʱ�䵥λ��
//-------------------------------------------------------------------------------------------------------------------
uint32 pit_get(PIT_enum pit_ch)
{
    if(PIT_GetStatusFlags(PIT, (pit_chnl_t)pit_ch))
    {
        PIT_ClearStatusFlags(PIT, (pit_chnl_t)pit_ch, kPIT_TimerFlag);
        return 0XFFFFFFFF;
        //��ʱ��� �������ֵ
    }
    return (0XFFFFFFFF - PIT_GetCurrentTimerCount(PIT, (pit_chnl_t)pit_ch));
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ر�PIT��ʱ��
//  @param      pit_ch      ѡ��ģ���ͨ�� (ѡ��Χ ��PIT_enumö��ֵ������ȷ��)
//  @return     void      
//  Sample usage:           �رն�ʱ���������־λ
//-------------------------------------------------------------------------------------------------------------------
void pit_close(PIT_enum pit_ch)
{
    PIT_StopTimer(PIT, (pit_chnl_t)pit_ch);
    PIT_ClearStatusFlags(PIT, (pit_chnl_t)pit_ch, kPIT_TimerFlag);
}

