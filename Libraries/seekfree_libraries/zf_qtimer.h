/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		qtimer
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/
 
#ifndef _zf_qtimer_h
#define _zf_qtimer_h



#include "common.h"
#include "fsl_qtmr.h"

//��ö�ٶ��岻�����û��޸�
typedef enum    //QTIMER����ö��
{
    QTIMER1_TIMER0_C0 =1*12+3*0,											//��ʱ��0 ͬһʱ��ֻ����һ�������붨ʱ�������
    QTIMER1_TIMER1_C1 =1*12+3*1,                                            //��ʱ��1 ͬһʱ��ֻ����һ�������붨ʱ�������
    QTIMER1_TIMER2_C2 =1*12+3*2,                                            //��ʱ��2 ͬһʱ��ֻ����һ�������붨ʱ�������
    QTIMER1_TIMER3_C24=1*12+3*3,                                            //��ʱ��3 ͬһʱ��ֻ����һ�������붨ʱ�������
    
    QTIMER2_TIMER0_C3 =2*12+3*0, QTIMER2_TIMER0_E19,                        //��ʱ��0 ͬһʱ��ֻ����һ�������붨ʱ�������
    QTIMER2_TIMER1_C4 =2*12+3*1, QTIMER2_TIMER1_E20,                        //��ʱ��1 ͬһʱ��ֻ����һ�������붨ʱ�������
    QTIMER2_TIMER2_C5 =2*12+3*2, QTIMER2_TIMER2_E21,                        //��ʱ��2 ͬһʱ��ֻ����һ�������붨ʱ�������
    QTIMER2_TIMER3_C25=2*12+3*3, QTIMER2_TIMER3_E22,                        //��ʱ��3 ͬһʱ��ֻ����һ�������붨ʱ�������
    
    QTIMER3_TIMER0_B16=3*12+3*0, QTIMER3_TIMER0_C6,  QTIMER3_TIMER0_E15,    //��ʱ��0 ͬһʱ��ֻ����һ�������붨ʱ�������
    QTIMER3_TIMER1_B17=3*12+3*1, QTIMER3_TIMER1_C7,  QTIMER3_TIMER1_E16,    //��ʱ��1 ͬһʱ��ֻ����һ�������붨ʱ�������
    QTIMER3_TIMER2_B18=3*12+3*2, QTIMER3_TIMER2_C8,  QTIMER3_TIMER2_E17,    //��ʱ��2 ͬһʱ��ֻ����һ�������붨ʱ�������
    QTIMER3_TIMER3_B19=3*12+3*3, QTIMER3_TIMER3_C26, QTIMER3_TIMER3_E18,    //��ʱ��3 ͬһʱ��ֻ����һ�������붨ʱ�������
    
    QTIMER4_TIMER0_C9 =4*12+3*0,                                            //��ʱ��0 ͬһʱ��ֻ����һ�������붨ʱ�������
    QTIMER4_TIMER1_C10=4*12+3*1,                                            //��ʱ��1 ͬһʱ��ֻ����һ�������붨ʱ�������
    QTIMER4_TIMER2_C11=4*12+3*2,                                            //��ʱ��2 ͬһʱ��ֻ����һ�������붨ʱ�������
    QTIMER4_TIMER3_C27=4*12+3*3,                                            //��ʱ��3 ͬһʱ��ֻ����һ�������붨ʱ�������
    
}QTIMER_PIN_enum;

//��ö�ٶ��岻�����û��޸�
typedef enum    //QTIMER���ö��
{
    QTIMER_0,//RT1064û��QTIMER0 ���������ռλ
    QTIMER_1,
    QTIMER_2,
    QTIMER_3,
    QTIMER_4,
}QTIMERN_enum;


void qtimer_quad_init(QTIMERN_enum qtimern, QTIMER_PIN_enum phaseA, QTIMER_PIN_enum phaseB);
int16 qtimer_quad_get(QTIMERN_enum qtimern, QTIMER_PIN_enum phaseA);
void qtimer_quad_clear(QTIMERN_enum qtimern, QTIMER_PIN_enum phaseA);

#endif
 
