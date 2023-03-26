/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		spi
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/
 

#include "zf_iomuxc.h"
#include "zf_spi.h"


#define LPSPI_PIN_CONF  SPEED_100MHZ | KEEPER_EN | DSE_R0_6 //����SPI����Ĭ������
#define LPSPI_CLK_DIV   3
#define LPSPI_CLK_SRC   2



LPSPI_Type * SPIN[] = LPSPI_BASE_PTRS;

void spi_iomuxc(SPIN_enum spi_n, SPI_PIN_enum sck_pin, SPI_PIN_enum mosi_pin, SPI_PIN_enum miso_pin, SPI_PIN_enum cs_pin)
{
    switch(spi_n)
    {
        case SPI_1:
        {
            if      (SPI1_SCK_D12  == sck_pin)  iomuxc_pinconf(D12,ALT4,LPSPI_PIN_CONF);
            else if (SPI1_SCK_E27  == sck_pin)  iomuxc_pinconf(E27,ALT3,LPSPI_PIN_CONF);
            else                                assert(0);//���Ŵ��� �������ʧ��
            
            if      (SPI1_MOSI_D14 == mosi_pin) iomuxc_pinconf(D14,ALT4,LPSPI_PIN_CONF);
            else if (SPI1_MOSI_E28 == mosi_pin) iomuxc_pinconf(E28,ALT3,LPSPI_PIN_CONF);
            else                                assert(0);//���Ŵ��� �������ʧ��
                                                               
            if      (SPI1_MISO_D15 == miso_pin) iomuxc_pinconf(D15,ALT4,LPSPI_PIN_CONF);
            else if (SPI1_MISO_E29 == miso_pin) iomuxc_pinconf(E29,ALT3,LPSPI_PIN_CONF);
            else                                assert(0);//���Ŵ��� �������ʧ��
            
            if      (SPI1_CS0_D13  == cs_pin)   iomuxc_pinconf(D13,ALT4,LPSPI_PIN_CONF);
            else if (SPI1_CS0_E30  == cs_pin)   iomuxc_pinconf(E30,ALT3,LPSPI_PIN_CONF);
            else if (SPI1_CS1_E31  == cs_pin)   iomuxc_pinconf(E31,ALT3,LPSPI_PIN_CONF);
            else if (SPI1_CS2_D26  == cs_pin)   iomuxc_pinconf(D26,ALT2,LPSPI_PIN_CONF);
            else if (SPI1_CS3_D27  == cs_pin)   iomuxc_pinconf(D27,ALT2,LPSPI_PIN_CONF);
            else if (SPI_CS_NOT_USE  == cs_pin)   ;//��ʹ��Ƭѡ����
            else                                assert(0);//���Ŵ��� �������ʧ��
        }break;
        
        case SPI_2:
        {
            if      (SPI2_SCK_D7   == sck_pin)  iomuxc_pinconf(D7 ,ALT4,LPSPI_PIN_CONF);
            else if (SPI2_SCK_E0   == sck_pin)  iomuxc_pinconf(E0 ,ALT2,LPSPI_PIN_CONF);
            else                                assert(0);//���Ŵ��� �������ʧ��
            
            if      (SPI2_MOSI_D8  == mosi_pin) iomuxc_pinconf(D8 ,ALT4,LPSPI_PIN_CONF);
            else if (SPI2_MOSI_E2  == mosi_pin) iomuxc_pinconf(E2 ,ALT2,LPSPI_PIN_CONF);
            else                                assert(0);//���Ŵ��� �������ʧ��
                                                               
            if      (SPI2_MISO_D9  == miso_pin) iomuxc_pinconf(D9 ,ALT4,LPSPI_PIN_CONF);
            else if (SPI2_MISO_E3  == miso_pin) iomuxc_pinconf(E3 ,ALT2,LPSPI_PIN_CONF);
            else                                assert(0);//���Ŵ��� �������ʧ��
            
            if      (SPI2_CS0_D6   == cs_pin)   iomuxc_pinconf(D6 ,ALT4,LPSPI_PIN_CONF);
            else if (SPI2_CS0_E1   == cs_pin)   iomuxc_pinconf(E1 ,ALT2,LPSPI_PIN_CONF);
            else if (SPI2_CS1_E14  == cs_pin)   iomuxc_pinconf(E14,ALT4,LPSPI_PIN_CONF);
            else if (SPI2_CS2_D10  == cs_pin)   iomuxc_pinconf(D10,ALT4,LPSPI_PIN_CONF);
            else if (SPI2_CS3_D11  == cs_pin)   iomuxc_pinconf(D11,ALT4,LPSPI_PIN_CONF);
            else if (SPI_CS_NOT_USE  == cs_pin)   ;//��ʹ��Ƭѡ����
            else                                assert(0);//���Ŵ��� �������ʧ��
        }break;
        
        case SPI_3:
        {
            if      (SPI3_SCK_B0   == sck_pin)  iomuxc_pinconf(B0 ,ALT7,LPSPI_PIN_CONF);
            else if (SPI3_SCK_B31  == sck_pin)  iomuxc_pinconf(B31,ALT2,LPSPI_PIN_CONF);
            else                                assert(0);//���Ŵ��� �������ʧ��
            
            if      (SPI3_MOSI_B1  == mosi_pin) iomuxc_pinconf(B1 ,ALT7,LPSPI_PIN_CONF);
            else if (SPI3_MOSI_B30 == mosi_pin) iomuxc_pinconf(B30,ALT2,LPSPI_PIN_CONF);
            else                                assert(0);//���Ŵ��� �������ʧ��
                                                               
            if      (SPI3_MISO_B2  == miso_pin) iomuxc_pinconf(B2 ,ALT7,LPSPI_PIN_CONF);
            else if (SPI3_MISO_B29 == miso_pin) iomuxc_pinconf(B29,ALT2,LPSPI_PIN_CONF);
            else                                assert(0);//���Ŵ��� �������ʧ��
            
            if      (SPI3_CS0_B3   == cs_pin)   iomuxc_pinconf(B3 ,ALT7,LPSPI_PIN_CONF);
            else if (SPI3_CS0_B28  == cs_pin)   iomuxc_pinconf(B28,ALT2,LPSPI_PIN_CONF);
            else if (SPI3_CS1_B4   == cs_pin)   iomuxc_pinconf(B4 ,ALT7,LPSPI_PIN_CONF);
            else if (SPI3_CS2_B5   == cs_pin)   iomuxc_pinconf(B5 ,ALT7,LPSPI_PIN_CONF);
            else if (SPI3_CS3_B6   == cs_pin)   iomuxc_pinconf(B6 ,ALT7,LPSPI_PIN_CONF);
            else if (SPI_CS_NOT_USE  == cs_pin)   ;//��ʹ��Ƭѡ����
            else                                assert(0);//���Ŵ��� �������ʧ��
        }break;
        
        case SPI_4:
        {
            if      (SPI4_SCK_C3   == sck_pin)  iomuxc_pinconf(C3 ,ALT3,LPSPI_PIN_CONF);
            else if (SPI4_SCK_C23  == sck_pin)  iomuxc_pinconf(C23,ALT1,LPSPI_PIN_CONF);
            else                                assert(0);//���Ŵ��� �������ʧ��
            
            if      (SPI4_MOSI_C2  == mosi_pin) iomuxc_pinconf(C2 ,ALT3,LPSPI_PIN_CONF);
            else if (SPI4_MOSI_C22 == mosi_pin) iomuxc_pinconf(C22,ALT1,LPSPI_PIN_CONF);
            else                                assert(0);//���Ŵ��� �������ʧ��
                                                               
            if      (SPI4_MISO_C1  == miso_pin) iomuxc_pinconf(C1 ,ALT3,LPSPI_PIN_CONF);
            else if (SPI4_MISO_C21 == miso_pin) iomuxc_pinconf(C21,ALT1,LPSPI_PIN_CONF);
            else                                assert(0);//���Ŵ��� �������ʧ��
            
            if      (SPI4_CS0_C0   == cs_pin)   iomuxc_pinconf(C0 ,ALT3,LPSPI_PIN_CONF);
            else if (SPI4_CS0_C20  == cs_pin)   iomuxc_pinconf(C20,ALT1,LPSPI_PIN_CONF);
            else if (SPI4_CS1_C19  == cs_pin)   iomuxc_pinconf(C19,ALT2,LPSPI_PIN_CONF);
            else if (SPI4_CS2_C18  == cs_pin)   iomuxc_pinconf(C18,ALT2,LPSPI_PIN_CONF);
            else if (SPI4_CS3_C27  == cs_pin)   iomuxc_pinconf(C27,ALT6,LPSPI_PIN_CONF);
            else if (SPI_CS_NOT_USE  == cs_pin)   ;//��ʹ��Ƭѡ����
            else                                assert(0);//���Ŵ��� �������ʧ��
        }break;
        
        default:assert(0);break;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      SPI��ʼ��
//  @param      spi_n           ѡ��SPIģ��(SPI_1-SPI_4)
//  @param      cs_pin          ѡ��SPIƬѡ����
//  @param      sck_pin         ѡ��SPIʱ������
//  @param      mosi_pin        ѡ��SPI MOSI����
//  @param      miso_pin        ѡ��SPI MISO����
//  @param      mode            SPIģʽ 0��CPOL=0 CPHA=0    1��CPOL=0 CPHA=1   2��CPOL=1 CPHA=0   3��CPOL=1 CPHA=1 //����ϸ�ڿ����аٶ�
//  @param      baud            ����SPI�Ĳ�����
//  @return     void
//  Sample usage:               spi_init(SPI_1, SPI1_SCK_D12, SPI1_MOSI_D14, SPI1_MISO_D15, SPI1_CS0_D13, 0, 1*1000*1000);//Ӳ��SPI��ʼ��  ģʽ0 ������Ϊ1Mhz   
//-------------------------------------------------------------------------------------------------------------------
void spi_init(SPIN_enum spi_n, SPI_PIN_enum sck_pin, SPI_PIN_enum mosi_pin, SPI_PIN_enum miso_pin, SPI_PIN_enum cs_pin, uint8 mode, uint32 baud)
{
    lpspi_master_config_t masterConfig;
    uint32 src_clock;
    
    spi_iomuxc(spi_n, sck_pin, mosi_pin, miso_pin, cs_pin);

    CLOCK_SetMux(kCLOCK_LpspiMux, LPSPI_CLK_SRC);    //ѡ��PLL2��ΪLPSPIʱ��Դ
    CLOCK_SetDiv(kCLOCK_LpspiDiv, LPSPI_CLK_DIV);
    
    LPSPI_MasterGetDefaultConfig(&masterConfig);

    masterConfig.baudRate = baud;
    masterConfig.bitsPerFrame = 8;
    masterConfig.whichPcs = (lpspi_which_pcs_t)(cs_pin%14/2-3);
    
    switch(mode)
    {
        case 0:
        {
            masterConfig.cpol = kLPSPI_ClockPolarityActiveHigh; 
            masterConfig.cpha = kLPSPI_ClockPhaseFirstEdge; 
        }break;
        
        case 1:
        {
            masterConfig.cpol = kLPSPI_ClockPolarityActiveHigh; 
            masterConfig.cpha = kLPSPI_ClockPhaseSecondEdge; 
        }break;
        
        case 2:
        {
            masterConfig.cpol = kLPSPI_ClockPolarityActiveLow; 
            masterConfig.cpha = kLPSPI_ClockPhaseFirstEdge; 
        }break;
        
        case 3:
        {
            masterConfig.cpol = kLPSPI_ClockPolarityActiveLow; 
            masterConfig.cpha = kLPSPI_ClockPhaseSecondEdge; 
        }break;
    }

    masterConfig.pcsToSckDelayInNanoSec = 1000000000 / masterConfig.baudRate;
    masterConfig.lastSckToPcsDelayInNanoSec = 1000000000 / masterConfig.baudRate;
    masterConfig.betweenTransferDelayInNanoSec = 1000000000 / masterConfig.baudRate;
    src_clock = (CLOCK_GetFreq(kCLOCK_SysPllClk) / (LPSPI_CLK_DIV + 1U));
    
    LPSPI_MasterInit(SPIN[spi_n], &masterConfig, src_clock);//��һ�γ�ʼ�����ڴ�ʱ��
    LPSPI_Reset(SPIN[spi_n]);                               //��λ����
    LPSPI_MasterInit(SPIN[spi_n], &masterConfig, src_clock);//���³�ʼ��������ȷ�Ĳ���
    
    LPSPI_Enable(SPIN[spi_n], false);
    SPIN[spi_n]->CFGR1 &= (~LPSPI_CFGR1_NOSTALL_MASK);
    LPSPI_Enable(SPIN[spi_n], true);

    LPSPI_FlushFifo(SPIN[spi_n], true, true);                       //ˢ��FIFO
    LPSPI_ClearStatusFlags(SPIN[spi_n], kLPSPI_AllStatusFlag);      //���״̬��־
    LPSPI_DisableInterrupts(SPIN[spi_n], kLPSPI_AllInterruptEnable);//�ر��ж�
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      SPI���ͽ��պ���
//  @param      spi_n           ѡ��SPIģ��   (SPI_1-SPI_4)
//  @param      cs_pin          ѡ��SPIƬѡ����
//  @param      modata          ���͵����ݻ�������ַ
//  @param      midata          ��������ʱ���յ������ݵĴ洢��ַ(����Ҫ������ NULL)
//  @param      len             ���͵��ֽ���
//  @param      continuous      ����ͨ����CS�Ƿ����������Ч״̬ 1:��������  0:ÿ������һ���ֽڹر�CS(һ������Ϊ1 ����)
//  @return     void				
//  @since      v2.0
//  Sample usage:       		spi_mosi(SPI_1,SPI1_CS0_D13,buf,buf,1,1);    //����buff�����ݣ������յ�buf�����Ϊ1�ֽ� ͨ���ڼ�CS��������
//-------------------------------------------------------------------------------------------------------------------
void spi_mosi(SPIN_enum spi_n, SPI_PIN_enum cs_pin, uint8 *modata, uint8 *midata, uint32 len, uint8 continuous)
{
    lpspi_transfer_t transfer;

    assert(0 != len);               //�����ֽ�����Ϊ0
    
    if(continuous)  transfer.configFlags = ((cs_pin%14/2-3)<<LPSPI_MASTER_PCS_SHIFT) | kLPSPI_MasterPcsContinuous;
    else            transfer.configFlags = ((cs_pin%14/2-3)<<LPSPI_MASTER_PCS_SHIFT);
    transfer.txData = modata;
    transfer.rxData = midata;
    transfer.dataSize = len;
    LPSPI_MasterTransferBlocking(SPIN[spi_n], &transfer);
}





