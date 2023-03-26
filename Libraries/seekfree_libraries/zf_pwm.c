/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		pwm
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/


#include "zf_iomuxc.h"
#include "zf_pwm.h"


#define PWM_SRC_CLK_FREQ    CLOCK_GetFreq(kCLOCK_IpgClk)        //����PWM����ʱ��ԴƵ��
#define PWM_PIN_CONF        SPEED_100MHZ | KEEPER_EN | DSE_R0_6 //����PWM����Ĭ������   

static PWM_Type * PWMPTR[] = PWM_BASE_PTRS;


void pwm_iomuxc(PWMCH_enum pwmch)
{
    switch(pwmch)
    {
        case PWM1_MODULE0_CHA_D12:  iomuxc_pinconf(D12,ALT1,PWM_PIN_CONF); break;
        case PWM1_MODULE0_CHA_E23:  iomuxc_pinconf(E23,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM1_MODULE0_CHB_D13:  iomuxc_pinconf(D13,ALT1,PWM_PIN_CONF); break;
        case PWM1_MODULE0_CHB_E24:  iomuxc_pinconf(E24,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM1_MODULE1_CHA_D14:  iomuxc_pinconf(D14,ALT1,PWM_PIN_CONF); break;
        case PWM1_MODULE1_CHA_E25:  iomuxc_pinconf(E25,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM1_MODULE1_CHB_D15:  iomuxc_pinconf(D15,ALT1,PWM_PIN_CONF); break;
        case PWM1_MODULE1_CHB_E26:  iomuxc_pinconf(E26,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM1_MODULE2_CHA_D16:  iomuxc_pinconf(D16,ALT1,PWM_PIN_CONF); break;
        case PWM1_MODULE2_CHA_E27:  iomuxc_pinconf(E27,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM1_MODULE2_CHB_D17:  iomuxc_pinconf(D17,ALT1,PWM_PIN_CONF); break;
        case PWM1_MODULE2_CHB_E28:  iomuxc_pinconf(E28,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM1_MODULE3_CHA_B10:  iomuxc_pinconf(B10,ALT1,PWM_PIN_CONF); break;
        case PWM1_MODULE3_CHA_C16:  iomuxc_pinconf(C16,ALT6,PWM_PIN_CONF); break;
        case PWM1_MODULE3_CHA_D0 :  iomuxc_pinconf(D0 ,ALT2,PWM_PIN_CONF); break;
        case PWM1_MODULE3_CHA_D24:  iomuxc_pinconf(D24,ALT1,PWM_PIN_CONF); break;
        case PWM1_MODULE3_CHA_E12:  iomuxc_pinconf(E12,ALT4,PWM_PIN_CONF); break;
                                                   
                                                   
        case PWM1_MODULE3_CHB_B11:  iomuxc_pinconf(B11,ALT1,PWM_PIN_CONF); break;
        case PWM1_MODULE3_CHB_C17:  iomuxc_pinconf(C17,ALT6,PWM_PIN_CONF); break;
        case PWM1_MODULE3_CHB_D1 :  iomuxc_pinconf(D1 ,ALT2,PWM_PIN_CONF); break;
        case PWM1_MODULE3_CHB_D25:  iomuxc_pinconf(D25,ALT1,PWM_PIN_CONF); break;
        case PWM1_MODULE3_CHB_E13:  iomuxc_pinconf(E13,ALT4,PWM_PIN_CONF); break;
                                                   
                                                   
                                                   
        case PWM2_MODULE0_CHA_C6 :  iomuxc_pinconf(C6 ,ALT2,PWM_PIN_CONF); break;
        case PWM2_MODULE0_CHA_E6 :  iomuxc_pinconf(E6 ,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM2_MODULE0_CHB_C7 :  iomuxc_pinconf(C7 ,ALT2,PWM_PIN_CONF); break;
        case PWM2_MODULE0_CHB_E7 :  iomuxc_pinconf(E7 ,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM2_MODULE1_CHA_C8 :  iomuxc_pinconf(C8 ,ALT2,PWM_PIN_CONF); break;
        case PWM2_MODULE1_CHA_E8 :  iomuxc_pinconf(E8 ,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM2_MODULE1_CHB_C9 :  iomuxc_pinconf(C9 ,ALT2,PWM_PIN_CONF); break;
        case PWM2_MODULE1_CHB_E9 :  iomuxc_pinconf(E9 ,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM2_MODULE2_CHA_C10:  iomuxc_pinconf(C10,ALT2,PWM_PIN_CONF); break;
        case PWM2_MODULE2_CHA_E10:  iomuxc_pinconf(E10,ALT1,PWM_PIN_CONF); break;

        case PWM2_MODULE2_CHB_C11:  iomuxc_pinconf(C11,ALT2,PWM_PIN_CONF); break;
        case PWM2_MODULE2_CHB_E11:  iomuxc_pinconf(E11,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM2_MODULE3_CHA_B0 :  iomuxc_pinconf(B0 ,ALT0,PWM_PIN_CONF); break;
        case PWM2_MODULE3_CHA_C18:  iomuxc_pinconf(C18,ALT6,PWM_PIN_CONF); break;
        case PWM2_MODULE3_CHA_D2 :  iomuxc_pinconf(D2 ,ALT2,PWM_PIN_CONF); break;
        case PWM2_MODULE3_CHA_E19:  iomuxc_pinconf(E19,ALT1,PWM_PIN_CONF); break;
        case PWM2_MODULE3_CHA_B9 :  iomuxc_pinconf(B9 ,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM2_MODULE3_CHB_B1 :  iomuxc_pinconf(B1 ,ALT0,PWM_PIN_CONF); break;
        case PWM2_MODULE3_CHB_C19:  iomuxc_pinconf(C19,ALT6,PWM_PIN_CONF); break;
        case PWM2_MODULE3_CHB_D3 :  iomuxc_pinconf(D3 ,ALT2,PWM_PIN_CONF); break;
        case PWM2_MODULE3_CHB_E20:  iomuxc_pinconf(E20,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM3_MODULE0_CHA_E29:  iomuxc_pinconf(E29,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM3_MODULE0_CHB_E30:  iomuxc_pinconf(E30,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM3_MODULE1_CHA_E31:  iomuxc_pinconf(E31,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM3_MODULE1_CHB_D18:  iomuxc_pinconf(D18,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM3_MODULE2_CHA_D19:  iomuxc_pinconf(D19,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM3_MODULE2_CHB_D20:  iomuxc_pinconf(D20,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM3_MODULE3_CHA_E21:  iomuxc_pinconf(E21,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM3_MODULE3_CHB_E22:  iomuxc_pinconf(E22,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM4_MODULE0_CHA_B24:  iomuxc_pinconf(B24,ALT1,PWM_PIN_CONF); break;
        case PWM4_MODULE0_CHA_E0 :  iomuxc_pinconf(E0 ,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM4_MODULE0_CHB_E1 :  iomuxc_pinconf(E1 ,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM4_MODULE1_CHA_B25:  iomuxc_pinconf(B25,ALT1,PWM_PIN_CONF); break;
        case PWM4_MODULE1_CHA_E2 :  iomuxc_pinconf(E2 ,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM4_MODULE1_CHB_E3 :  iomuxc_pinconf(E3 ,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM4_MODULE2_CHA_C30:  iomuxc_pinconf(C30,ALT1,PWM_PIN_CONF); break;
        case PWM4_MODULE2_CHA_E4 :  iomuxc_pinconf(E4 ,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM4_MODULE2_CHB_E5 :  iomuxc_pinconf(E5 ,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM4_MODULE3_CHA_C31:  iomuxc_pinconf(C31,ALT1,PWM_PIN_CONF); break;
        case PWM4_MODULE3_CHA_E17:  iomuxc_pinconf(E17,ALT1,PWM_PIN_CONF); break;
                                                   
        case PWM4_MODULE3_CHB_E18:  iomuxc_pinconf(E18,ALT1,PWM_PIN_CONF); break;

        default :assert(0);break;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM��ʼ��
//  @param      pwmch       PWMͨ���ż�����
//  @param      freq        PWMƵ��
//  @param      duty        PWMռ�ձ�
//  @return     void
//  Sample usage:           pwm_init(PWM1_MODULE0_CHB_D13, 50, 5000);     //��ʼ��PWM1  ��ģ��0 ͨ��B ʹ������D13  ���PWMƵ��50HZ   ռ�ձ�Ϊ�ٷ�֮ 5000/PWM_DUTY_MAX*100
//							PWM_DUTY_MAX��fsl_pwm.h�ļ��� Ĭ��Ϊ50000 
//-------------------------------------------------------------------------------------------------------------------
void pwm_init(PWMCH_enum pwmch, uint32 freq, uint32 duty)
{
    uint8  pwm_num;
    uint8  pwm_module;
    uint8  pwm_module_ch;
    uint32 temp_prsc;
    pwm_config_t pwmConfig;
    
    pwm_iomuxc(pwmch);

    //pwmConfig.enableDebugMode = false;
    //pwmConfig.enableWait = false;
    //pwmConfig.reloadSelect = kPWM_LocalReload;
    //pwmConfig.faultFilterCount = 0;
    //pwmConfig.faultFilterPeriod = 0;
    //pwmConfig.clockSource = kPWM_BusClock;
    //pwmConfig.prescale = kPWM_Prescale_Divide_1;
    //pwmConfig.initializationControl = kPWM_Initialize_LocalSync;
    //pwmConfig.forceTrigger = kPWM_Force_Local;
    //pwmConfig.reloadFrequency = kPWM_LoadEveryOportunity;
    //pwmConfig.reloadLogic = kPWM_ReloadImmediate;
    //pwmConfig.pairOperation = kPWM_Independent;

    PWM_GetDefaultConfig(&pwmConfig);

    pwmConfig.reloadLogic = kPWM_ReloadPwmFullCycle;
    pwmConfig.pairOperation = kPWM_Independent;
    pwmConfig.enableDebugMode = true;
    

    //�����Ƶϵ��
    temp_prsc = (PWM_SRC_CLK_FREQ/freq)>>16;
	if(PWM_SRC_CLK_FREQ%(freq<<16))	temp_prsc++;

    if      (1   >= temp_prsc)  pwmConfig.prescale = kPWM_Prescale_Divide_1;
    else if (2   >= temp_prsc)  pwmConfig.prescale = kPWM_Prescale_Divide_2;
    else if (4   >= temp_prsc)  pwmConfig.prescale = kPWM_Prescale_Divide_4;
    else if (8   >= temp_prsc)  pwmConfig.prescale = kPWM_Prescale_Divide_8;
    else if (16  >= temp_prsc)  pwmConfig.prescale = kPWM_Prescale_Divide_16;
    else if (32  >= temp_prsc)  pwmConfig.prescale = kPWM_Prescale_Divide_32;
    else if (64  >= temp_prsc)  pwmConfig.prescale = kPWM_Prescale_Divide_64;
    else if (128 >= temp_prsc)  pwmConfig.prescale = kPWM_Prescale_Divide_128;
    else
    {
        assert(0) ;//Ƶ�ʹ�С ����IPGƵ�ʹ���
    }

    pwm_num = pwmch/40;             //����ģ����
    pwm_module = pwmch%40/10;       //������ģ����
    pwm_module_ch = pwmch%40%10/5;  //������ģ���ͨ��
    //��ʼ��PWMģ�� 
    if (PWM_Init(PWMPTR[pwm_num], (pwm_submodule_t)pwm_module, &pwmConfig) == kStatus_Fail)//��һ�γ�ʼ�����ڴ�ʱ��
    {
        assert(0) ;//��ʼ��ʧ��
    }
    PWM_Deinit(PWMPTR[pwm_num], (pwm_submodule_t)pwm_module);
    
    if (PWM_Init(PWMPTR[pwm_num], (pwm_submodule_t)pwm_module, &pwmConfig) == kStatus_Fail)//���³�ʼ��������ȷ�Ĳ���
    {
        assert(0) ;//��ʼ��ʧ��
    }

    //����Ƶ��ռ�ձȵȲ���
    pwm_signal_param_t pwmSignal;

    pwmSignal.pwmChannel = (pwm_channels_t)(pwm_module_ch); 
    pwmSignal.level = kPWM_HighTrue;
    pwmSignal.dutyCyclePercent = duty;  //PWM_DUTY_MAX
    pwmSignal.deadtimeValue = 0;//((uint64_t)PWM_SRC_CLK_FREQ * 650) / 1000000000;

    //���LOAD OKAYλ  �������µĲ���
    PWM_SetPwmLdok(PWMPTR[pwm_num], (pwm_module_control_t)(1<<(pwm_module)), false);
    PWM_SetupPwm(PWMPTR[pwm_num], (pwm_submodule_t)pwm_module, &pwmSignal, 1, kPWM_EdgeAligned, freq, PWM_SRC_CLK_FREQ);     
    //����LOAD OKAYλ  �Ը�������
    PWM_SetPwmLdok(PWMPTR[pwm_num], (pwm_module_control_t)(1<<(pwm_module)), true);

    //������ʱ��
    PWM_StartTimer(PWMPTR[pwm_num], (pwm_module_control_t)(1<<(pwm_module)));
    
    PWMPTR[pwm_num]->SM[pwm_module].DISMAP[0]=0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWMռ�ձ�����
//  @param      pwmch       PWMͨ���ż�����
//  @param      freq        PWMƵ��
//  @param      duty        PWMռ�ձ�
//  @return     void
//  Sample usage:           pwm_duty(PWM1_MODULE0_CHB_D13, 5000);     //����ռ�ձ�Ϊ�ٷ�֮ 5000/PWM_DUTY_MAX*100
//							PWM_DUTY_MAX��fsl_pwm.h�ļ��� Ĭ��Ϊ50000
//-------------------------------------------------------------------------------------------------------------------
void pwm_duty(PWMCH_enum pwmch, uint32 duty)
{
    uint8  pwm_num;
    uint8  pwm_module;
    uint8  pwm_module_ch;
    
    pwm_num = pwmch/40;             //����ģ����
    pwm_module = pwmch%40/10;       //������ģ����
    pwm_module_ch = pwmch%40%10/5;  //������ģ���ͨ��
    
    //���LOAD OKAYλ  �������µĲ���
    PWM_SetPwmLdok(PWMPTR[pwm_num], (pwm_module_control_t)(1<<(pwm_module)), false);
    PWM_UpdatePwmDutycycle(PWMPTR[pwm_num], (pwm_submodule_t)pwm_module, (pwm_channels_t)(pwm_module_ch), kPWM_EdgeAligned, duty);
    //����LOAD OKAYλ  �Ը�������
    PWM_SetPwmLdok(PWMPTR[pwm_num], (pwm_module_control_t)(1<<(pwm_module)), true);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWMƵ������
//  @param      pwmch       PWMͨ���ż�����
//  @param      freq        PWMƵ��
//  @param      duty        PWMռ�ձ�
//  @return     void
//  Sample usage:           pwm_freq(PWM1_MODULE0_CHB_D13, 50);     //����Ƶ��Ϊ50HZ   
//-------------------------------------------------------------------------------------------------------------------
void pwm_freq(PWMCH_enum pwmch, uint32 freq, uint32 duty)
{
    uint8  pwm_num;
    uint8  pwm_module;
    uint8  pwm_module_ch;

    uint16 temp_prsc;
    pwm_config_t pwmConfig;

    PWM_GetDefaultConfig(&pwmConfig);

    pwmConfig.reloadLogic = kPWM_ReloadPwmFullCycle;
    pwmConfig.pairOperation = kPWM_Independent;
    pwmConfig.enableDebugMode = true;
    
    //�����Ƶϵ��
    temp_prsc = (PWM_SRC_CLK_FREQ/freq)>>16;
    if      (1   >= temp_prsc)  pwmConfig.prescale = kPWM_Prescale_Divide_1;
    else if (2   >= temp_prsc)  pwmConfig.prescale = kPWM_Prescale_Divide_2;
    else if (4   >= temp_prsc)  pwmConfig.prescale = kPWM_Prescale_Divide_4;
    else if (8   >= temp_prsc)  pwmConfig.prescale = kPWM_Prescale_Divide_8;
    else if (16  >= temp_prsc)  pwmConfig.prescale = kPWM_Prescale_Divide_16;
    else if (32  >= temp_prsc)  pwmConfig.prescale = kPWM_Prescale_Divide_32;
    else if (64  >= temp_prsc)  pwmConfig.prescale = kPWM_Prescale_Divide_64;
    else if (128 >= temp_prsc)  pwmConfig.prescale = kPWM_Prescale_Divide_128;
    else
    {
        assert(0) ;//Ƶ�ʹ�С ����IPGƵ�ʹ���
    }
    
    pwm_num = pwmch/40;             //����ģ����
    pwm_module = pwmch%40/10;       //������ģ����
    pwm_module_ch = pwmch%40%10/5;  //������ģ���ͨ��
    
    //��ʼ��PWMģ�� 
    if (PWM_Init(PWMPTR[pwm_num], (pwm_submodule_t)pwm_module, &pwmConfig) == kStatus_Fail)
    {
        assert(0) ;//��ʼ��ʧ��
    }

    //����Ƶ��ռ�ձȵȲ���
    pwm_signal_param_t pwmSignal;

    pwmSignal.pwmChannel = (pwm_channels_t)(pwm_module_ch); 
    pwmSignal.level = kPWM_HighTrue;
    pwmSignal.dutyCyclePercent = duty;  //PWM_DUTY_MAX
    pwmSignal.deadtimeValue = 0;//((uint64_t)PWM_SRC_CLK_FREQ * 650) / 1000000000;
    
    //���LOAD OKAYλ  �������µĲ���
    PWM_SetPwmLdok(PWMPTR[pwm_num], (pwm_module_control_t)(1<<(pwm_module)), false);
    PWM_SetupPwm(PWMPTR[pwm_num], (pwm_submodule_t)pwm_module, &pwmSignal, 1, kPWM_EdgeAligned, freq, PWM_SRC_CLK_FREQ);     
    //����LOAD OKAYλ  �Ը�������
    PWM_SetPwmLdok(PWMPTR[pwm_num], (pwm_module_control_t)(1<<(pwm_module)), true);

    //������ʱ��
    PWM_StartTimer(PWMPTR[pwm_num], (pwm_module_control_t)(1<<(pwm_module)));
    
    PWMPTR[pwm_num]->SM[pwm_module].DISMAP[0]=0;
}