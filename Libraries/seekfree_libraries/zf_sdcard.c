/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		SDCARD
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-11-19
 ********************************************************************************************************************/

#include "fsl_debug_console.h"
#include "fsl_sd_disk.h"
#include "fsl_sd.h"
#include "zf_iomuxc.h"
#include "zf_sdcard.h"


#define SDHC_PIN_CONF   SPEED_100MHZ | PULL_EN | PULLUP_47K | DSE_R0 | SRE_FAST

static const sdmmchost_detect_card_t sd_card_detect = {
#ifndef BOARD_SD_DETECT_TYPE
    .cdType = kSDMMCHOST_DetectCardByGpioCD,
#else
    .cdType = BOARD_SD_DETECT_TYPE,
#endif
    .cdTimeOut_ms = (~0U),
};


/*! @brief SDMMC card power control configuration */
#if defined DEMO_SDCARD_POWER_CTRL_FUNCTION_EXIST
static const sdmmchost_pwr_card_t s_sdCardPwrCtrl = {
    .powerOn          = BOARD_PowerOnSDCARD,
    .powerOnDelay_ms  = 500U,
    .powerOff         = BOARD_PowerOffSDCARD,
    .powerOffDelay_ms = 0U,
};
#endif


void sdhc_iomuxc(void)
{
    
    iomuxc_pinconf(B23,ALT5,SPEED_100MHZ | KEEPER_EN | PULLDOWN_100K | DSE_R0_6);//��Դʹ������
    
    iomuxc_pinconf(C30,ALT6,SDHC_PIN_CONF);//��ѹ�л�����
    
    iomuxc_pinconf(D4 ,ALT5,SDHC_PIN_CONF);//����������
    
    iomuxc_pinconf(D12,ALT0,SDHC_PIN_CONF);//CMD
    iomuxc_pinconf(D13,ALT0,SDHC_PIN_CONF);//CLK
    iomuxc_pinconf(D14,ALT0,SDHC_PIN_CONF);//DATA0
    iomuxc_pinconf(D15,ALT0,SDHC_PIN_CONF);//DATA1
    iomuxc_pinconf(D16,ALT0,SDHC_PIN_CONF);//DATA2
    iomuxc_pinconf(D17,ALT0,SDHC_PIN_CONF);//DATA3
	
	
}


void sdcard_clk_init(void)
{
    CLOCK_InitSysPll(&sysPllConfig_BOARD_BootClockRUN);
    /*configure system pll PFD0 fractional divider to 24, output clock is 528MHZ * 18 / 24 = 396 MHZ*/
    CLOCK_InitSysPfd(kCLOCK_Pfd0, 24U);
    /* Configure USDHC clock source and divider */
    CLOCK_SetDiv(kCLOCK_Usdhc1Div, 0U);
    CLOCK_SetMux(kCLOCK_Usdhc1Mux, 1U);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ȴ�SD������
//  @param      void
//  @return     uint32          0:�ɹ�  1��ʧ��
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
uint32 sdcard_wait_card_insert(void)
{
    g_sd.usrParam.cd = &sd_card_detect;
    
    g_sd.host.base           = SD_HOST_BASEADDR;    //�������������Ϣ
    g_sd.host.sourceClock_Hz = SD_HOST_CLK_FREQ;
    g_sd.usrParam.cd = &sd_card_detect;             //���ü�⿨��ʹ�õķ�ʽ
#if defined DEMO_SDCARD_POWER_CTRL_FUNCTION_EXIST
    g_sd.usrParam.pwr = &s_sdCardPwrCtrl;
#endif
    
    sdhc_iomuxc();
    
    if (SD_HostInit(&g_sd) != kStatus_Success)      //��ʼ��SD����
    {
        //PRINTF("\r\nSD host init fail\r\n");
        return kStatus_Fail;
    }
    SD_PowerOffCard(g_sd.host.base, g_sd.usrParam.pwr);//�ر�SD����Դ
    
    //�ȴ�SD������
    if (SD_WaitCardDetectStatus(SD_HOST_BASEADDR, &sd_card_detect, true) == kStatus_Success)
    {
        //PRINTF("\r\nCard inserted.\r\n");
        /* power on the card */
        SD_PowerOnCard(g_sd.host.base, g_sd.usrParam.pwr);
    }
    else
    {
        //PRINTF("\r\nCard detect fail.\r\n");
        return kStatus_Fail;
    }

    return kStatus_Success;
}