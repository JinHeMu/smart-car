/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		adc
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/
 
#include "zf_iomuxc.h"
#include "zf_adc.h"


#define ADC_PIN_CONF  SPEED_100MHZ | DSE_R0_6  //����ADC����Ĭ������


ADC_Type * ADCN[] = ADC_BASE_PTRS;



void adc_iomuxc(ADCCH_enum ch)
{
    switch(ch)
    {
        case ADC1_CH0_B27:
        case ADC2_CH0_B27:  iomuxc_pinconf(B27 ,ALT5,ADC_PIN_CONF);break;
        
        case ADC1_CH1_B12:  iomuxc_pinconf(B12 ,ALT5,ADC_PIN_CONF);break;
        
        case ADC1_CH2_B13:  iomuxc_pinconf(B13 ,ALT5,ADC_PIN_CONF);break;
        
        case ADC1_CH3_B14:  iomuxc_pinconf(B14 ,ALT5,ADC_PIN_CONF);break;
        
        case ADC1_CH4_B15:  iomuxc_pinconf(B15 ,ALT5,ADC_PIN_CONF);break;
        
        case ADC1_CH5_B16:
        case ADC2_CH5_B16:  iomuxc_pinconf(B16 ,ALT5,ADC_PIN_CONF);break;
        
        case ADC1_CH6_B17:
        case ADC2_CH6_B17:  iomuxc_pinconf(B17 ,ALT5,ADC_PIN_CONF);break;
        
        case ADC2_CH7_B18:
        case ADC1_CH7_B18:  iomuxc_pinconf(B18 ,ALT5,ADC_PIN_CONF);break;
        
        case ADC1_CH8_B19:
        case ADC2_CH8_B19:  iomuxc_pinconf(B19 ,ALT5,ADC_PIN_CONF);break;
        
        case ADC1_CH9_B20:  
        case ADC2_CH9_B20:  iomuxc_pinconf(B20 ,ALT5,ADC_PIN_CONF);break;
        
        case ADC1_CH10_B21:  
        case ADC2_CH10_B21: iomuxc_pinconf(B21 ,ALT5,ADC_PIN_CONF);break;
                            
        case ADC1_CH11_B22: 
        case ADC2_CH11_B22: iomuxc_pinconf(B22 ,ALT5,ADC_PIN_CONF);break;
                            
        case ADC1_CH12_B23: 
        case ADC2_CH12_B23: iomuxc_pinconf(B23 ,ALT5,ADC_PIN_CONF);break;
                            
        case ADC1_CH13_B24: 
        case ADC2_CH13_B24: iomuxc_pinconf(B24 ,ALT5,ADC_PIN_CONF);break;
        
        case ADC1_CH14_B25:  
        case ADC2_CH14_B25: iomuxc_pinconf(B25 ,ALT5,ADC_PIN_CONF);break;
        
        case ADC1_CH15_B26:  
        case ADC2_CH15_B26: iomuxc_pinconf(B26 ,ALT5,ADC_PIN_CONF);break;
        
        case ADC2_CH1_B28:  iomuxc_pinconf(B28 ,ALT5,ADC_PIN_CONF);break;
        
        case ADC2_CH2_B29:  iomuxc_pinconf(B29 ,ALT5,ADC_PIN_CONF);break;
        
        case ADC2_CH3_B30:  iomuxc_pinconf(B30 ,ALT5,ADC_PIN_CONF);break;
        
        case ADC2_CH4_B31:  iomuxc_pinconf(B31 ,ALT5,ADC_PIN_CONF);break;
        
        default:assert(0);break;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC��ʼ��
//  @param      adcn            ѡ��ADCģ��(ADC_1��ADC_2)
//  @param      ch              ѡ��ADCͨ��
//  @param      resolution      ѡ��ѡ��ͨ���ֱ���(���ͬһ��ģ���ʼ��ʱ�����˲�ͬ�ķֱ��� �����һ����ʼ���ķֱ�����Ч)
//  @return     void
//  Sample usage:               adc_init(ADC_1,ADC1_CH0_B27,ADC_8BIT);//��ʼ��B27ΪADC���� �ֱ���Ϊ8λ
//-------------------------------------------------------------------------------------------------------------------
void adc_init(ADCN_enum adcn, ADCCH_enum ch, ADCRES_enum resolution)
{
    adc_config_t adcConfigStrcut;

    adc_iomuxc(ch);
    
    /*
    *  config->enableAsynchronousClockOutput = true;
     *  config->enableOverWrite =               false;
       *  config->enableContinuousConversion =    false;
       *  config->enableHighSpeed =               false;
       *  config->enableLowPower =                false;
       *  config->enableLongSample =              false;
       *  config->referenceVoltageSource =        kADC_ReferenceVoltageSourceVref;
       *  config->samplePeriodMode =              kADC_SamplePeriod2or12Clocks;
       *  config->clockSource =                   kADC_ClockSourceAD;
       *  config->clockDriver =                   kADC_ClockDriver1;
       */
    ADC_GetDefaultConfig(&adcConfigStrcut);
    adcConfigStrcut.resolution = (adc_resolution_t)(resolution);
    ADC_Init(ADCN[adcn], &adcConfigStrcut);
    if (kStatus_Success == ADC_DoAutoCalibration(ADCN[adcn]));
    else    assert(0);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADCת��һ��
//  @param      adcn            ѡ��ADCģ��(ADC_1��ADC_2)
//  @param      ch              ѡ��ADCͨ��
//  @return     void
//  Sample usage:               adc_convert(ADC_1,ADC1_CH0_B27);
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_convert(ADCN_enum adcn, ADCCH_enum ch)
{
    adc_channel_config_t adcChannelConfigStruct;
    
    adcChannelConfigStruct.channelNumber = ch&0xf;
    adcChannelConfigStruct.enableInterruptOnConversionCompleted = false;
    
    ADC_SetChannelConfig(ADCN[adcn], 0, &adcChannelConfigStruct);
    while (0U == ADC_GetChannelStatusFlags(ADCN[adcn], 0));
    return ADC_GetChannelConversionValue(ADCN[adcn], 0);
    
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC��ֵ�˲�
//  @param      adcn            ѡ��ADCģ��(ADC_1��ADC_2)
//  @param      ch              ѡ��ADCͨ��
//  @param      count           ��ֵ�˲�����
//  @return     void
//  Sample usage:               adc_convert(ADC_1,ADC1_CH0_B27,5);//�ɼ�5�� Ȼ�󷵻�ƽ��ֵ
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_mean_filter(ADCN_enum adcn, ADCCH_enum ch, uint8 count)
{
    uint8 i;
    uint32 sum;
    
    assert(count);//���Դ�������Ϊ0
    
    sum = 0;
    for(i=0; i<count; i++)
    {
        sum += adc_convert(adcn,ch);
    }
    
    sum = sum/count;
    return sum;
}
