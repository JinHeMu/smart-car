/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		camera
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/

#include "fsl_flexio_camera.h"
#include "zf_iomuxc.h"
#include "zf_camera.h"

#define FLEXIO_PIN_CONF         SPEED_100MHZ | KEEPER_EN | DSE_R0_6 //����FLEXIO����Ĭ������


#define DMA_CHN                 0u
#define DMA_MUX_SRC             kDmaRequestMuxFlexIO2Request2Request3



uint8 flexio_shift_count;
static FLEXIO_CAMERA_Type s_FlexioCameraDevice;


void flexio_iomuxc(FLEXIO_PIN_enum dat_pin_start, FLEXIO_PIN_enum pclk_pin, FLEXIO_PIN_enum href_pin)
{
    uint8 pin[32]={C0,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10,C11,C12,C13,C14,C15,C16,C17,C18,C19,C20,C21,C22,C23,C24,C25,C26,C27,C28,C29,C30,C31};
    
    if(24<dat_pin_start)                                        assert(0);  //�������Ų�������  ��ʼ���Ų��ô���24
    if(pclk_pin>=dat_pin_start && pclk_pin<=(dat_pin_start+7))  assert(0);  //����ʱ�����Ų�Ӧ�����������ŷ�Χ��
    if(href_pin>=dat_pin_start && href_pin<=(dat_pin_start+7))  assert(0);  //���ź����Ų�Ӧ�����������ŷ�Χ��
    if(href_pin == pclk_pin)                                    assert(0);  //���ź����Ų�Ӧ��������ʱ������һ��
    
    iomuxc_pinconf((PIN_enum)pin[pclk_pin],ALT4,FLEXIO_PIN_CONF);
    iomuxc_pinconf((PIN_enum)pin[href_pin],ALT4,FLEXIO_PIN_CONF);
    
    iomuxc_pinconf((PIN_enum)pin[dat_pin_start+0],ALT4,FLEXIO_PIN_CONF);
    iomuxc_pinconf((PIN_enum)pin[dat_pin_start+1],ALT4,FLEXIO_PIN_CONF);
    iomuxc_pinconf((PIN_enum)pin[dat_pin_start+2],ALT4,FLEXIO_PIN_CONF);
    iomuxc_pinconf((PIN_enum)pin[dat_pin_start+3],ALT4,FLEXIO_PIN_CONF);
    iomuxc_pinconf((PIN_enum)pin[dat_pin_start+4],ALT4,FLEXIO_PIN_CONF);
    iomuxc_pinconf((PIN_enum)pin[dat_pin_start+5],ALT4,FLEXIO_PIN_CONF);
    iomuxc_pinconf((PIN_enum)pin[dat_pin_start+6],ALT4,FLEXIO_PIN_CONF);
    iomuxc_pinconf((PIN_enum)pin[dat_pin_start+7],ALT4,FLEXIO_PIN_CONF);

}

static flexio_camera_config_t s_FlexioCameraConfig;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLEXIO��ʼ��ΪCSI(����ͷ)�ӿ�
//  @param      dat_pin_start   ����ͷ���ݽӿڵ���ʼλ
//  @param      pclk_pin        ����ʱ�ӵ�����
//  @param      href_pin        ���źŵ�����
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void flexio_camera(FLEXIO_PIN_enum dat_pin_start, FLEXIO_PIN_enum pclk_pin, FLEXIO_PIN_enum href_pin, uint16 col, uint16 row)
{
    flexio_iomuxc(dat_pin_start,pclk_pin,href_pin);
    
    if(col*row>=(376*240))  flexio_shift_count = 2;
    else                    flexio_shift_count = 1;
    
    s_FlexioCameraDevice.flexioBase = FLEXIO2;                  //���û���ַ
    s_FlexioCameraDevice.datPinStartIdx = dat_pin_start;        //��������������ʼλ
    s_FlexioCameraDevice.pclkPinIdx = pclk_pin;                 //��������ʱ������
    s_FlexioCameraDevice.hrefPinIdx = href_pin;                 //�������ź�����
    s_FlexioCameraDevice.shifterStartIdx = 4-flexio_shift_count;//���û�������ʼ���  ����1064�Ļ�����ֻ���ĸ��������д����4-
    s_FlexioCameraDevice.shifterCount = flexio_shift_count;     //���û���������
    s_FlexioCameraDevice.timerIdx = 3;                          //���ö�ʱ�����
    
    CLOCK_EnableClock(kCLOCK_Flexio2);
    FLEXIO_Reset(FLEXIO2);
    FLEXIO_CAMERA_GetDefaultConfig(&s_FlexioCameraConfig);
    FLEXIO_CAMERA_Init(&s_FlexioCameraDevice, &s_FlexioCameraConfig);
    FLEXIO_CAMERA_ClearStatusFlags(&s_FlexioCameraDevice, kFLEXIO_CAMERA_RxDataRegFullFlag | kFLEXIO_CAMERA_RxErrorFlag);
    FLEXIO_CAMERA_Enable(&s_FlexioCameraDevice, true);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLEXIO ʹ��
//  @param      enable          1��ʹ��FLEXIO   0���ر�FLEXIO
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void flexio_enable(uint8 enable)
{
    if(enable)
    {
        s_FlexioCameraDevice.flexioBase->CTRL |= FLEXIO_CTRL_FLEXEN_MASK;
    }
    else
    {
        s_FlexioCameraDevice.flexioBase->CTRL &= ~FLEXIO_CTRL_FLEXEN_MASK;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLEXIO RX����DMA����
//  @param      void
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void flexio_enable_rxdma(void)
{
    FLEXIO_CAMERA_EnableRxDMA(&s_FlexioCameraDevice, true);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLEXIO ��־λ���
//  @param      void
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void flexio_flag_clear(void)
{
    FLEXIO_CAMERA_ClearStatusFlags(&s_FlexioCameraDevice, kFLEXIO_CAMERA_RxDataRegFullFlag | kFLEXIO_CAMERA_RxErrorFlag);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      DMAMUX���ó�ʼ��
//  @param      void
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void dma_mux_init(void)
{
    DMAMUX_Deinit(DMAMUX);
    DMAMUX_Init(DMAMUX);
    DMAMUX_SetSource(DMAMUX, DMA_CHN, DMA_MUX_SRC);
    DMAMUX_EnableChannel(DMAMUX, DMA_CHN);
}


edma_transfer_config_t transferConfig;
edma_config_t edma_config;
edma_handle_t g_EDMA_Handle;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      DMA��ʼ��
//  @param      dest_addr       Ŀ�ĵ�ַ
//  @param      count           DMA�������
//  @param      callback        DMA�жϻص�����
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void flexio_dma_init(uint8 *dest_addr, uint32 count, edma_callback callback)
{
    edma_modulo_t s_addr_modulo;
    
    EDMA_GetDefaultConfig(&edma_config);
    EDMA_Deinit(DMA0);
    EDMA_Init(DMA0, &edma_config);
    DMA0->CERR = DMA_CERR_CERR_MASK | DMA_CERR_CAEI_MASK;
    
    EDMA_CreateHandle(&g_EDMA_Handle, DMA0, DMA_CHN);
    EDMA_SetCallback(&g_EDMA_Handle, callback, NULL);
    EDMA_PrepareTransfer(&transferConfig, 
    (void *)FLEXIO_CAMERA_GetRxBufferAddress(&s_FlexioCameraDevice), 
    4,
    (void *)(dest_addr), 
    4,
    4*flexio_shift_count,
    count,
    kEDMA_MemoryToMemory);
    
    
    EDMA_SubmitTransfer(&g_EDMA_Handle, &transferConfig);
    
    switch(4*flexio_shift_count)
    {
        case 4:     s_addr_modulo = kEDMA_Modulo4bytes;break;
        case 8:     s_addr_modulo = kEDMA_Modulo8bytes;break;
        case 16:    s_addr_modulo = kEDMA_Modulo16bytes;break;
        case 32:    s_addr_modulo = kEDMA_Modulo32bytes;break;
        default:assert(0);  //��������
    }
    
    
    EDMA_SetModulo(DMA0,DMA_CHN,s_addr_modulo,kEDMA_ModuloDisable);
    EDMA_StartTransfer(&g_EDMA_Handle);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      DMA����
//  @param      dest_addr       Ŀ�ĵ�ַ
//  @return     void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void dma_restart(uint8 *dest_addr)
{
    DMA0->TCD[DMA_CHN].DADDR = (uint32_t)(dest_addr);
    flexio_flag_clear();
    DMA0->SERQ = DMA_SERQ_SERQ(DMA_CHN);
}

