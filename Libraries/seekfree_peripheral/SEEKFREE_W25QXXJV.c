/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		W25QXXJV
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/

#include "fsl_common.h"
#include "fsl_flexspi.h"
#include "fsl_cache.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "SEEKFREE_W25QXXJV.h"



#define NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD 0
#define NOR_CMD_LUT_SEQ_IDX_READSTATUSREG 1
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE 2
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR 3
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD 4
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE 5
#define NOR_CMD_LUT_SEQ_IDX_READ_NORMAL 6
#define NOR_CMD_LUT_SEQ_IDX_READID 7
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST 8


#define W25QXXJV_BUSY_STATUS_POL       1           //����æ��־λ����  
#define W25QXXJV_BUSY_STATUS_OFFSET    0           //����æ��־λƫ��λ��

#define CUSTOM_LUT_LENGTH           36

const uint32_t customLUT[CUSTOM_LUT_LENGTH] = {
            
        /* Fast read quad mode - SDR */
        [4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xEB, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_4PAD, 0x18),
        [4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD + 1] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_4PAD, 0x06, kFLEXSPI_Command_READ_SDR, kFLEXSPI_4PAD, 0x04),
        
        /* Read status register */
        [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUSREG] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),
            
        /* Write Enable */
        [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x06, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
            
        /* Erase Sector  */
        [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x20, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
        
        /* Page Program - quad mode */
        [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x32, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
        [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD + 1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
            
        /* Page Program - single mode */
        [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x02, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
        [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE + 1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

        /* Normal read mode -SDR */
        /* Normal read mode -SDR */
        [4 * NOR_CMD_LUT_SEQ_IDX_READ_NORMAL] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x03, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
        [4 * NOR_CMD_LUT_SEQ_IDX_READ_NORMAL + 1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
            
        /* Read ID */
        [4 * NOR_CMD_LUT_SEQ_IDX_READID] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xAB, kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_1PAD, 0x18),
        [4 * NOR_CMD_LUT_SEQ_IDX_READID + 1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

        /* Fast read mode - SDR */
        [4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x0B, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
        [4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST + 1] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_1PAD, 0x08, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),
};

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ڲ����ã��û��������
//  @param      void 		    
//  @return     				
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void flash_iomuxc(void)
{
    CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03u */
    
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_SD_B1_05_FLEXSPIA_DQS,      /* GPIO_SD_B1_05 is configured as FLEXSPIA_DQS */
        1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_05 */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_SD_B1_06_FLEXSPIA_SS0_B,    /* GPIO_SD_B1_06 is configured as FLEXSPIA_SS0_B */
        1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_06 */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_SD_B1_07_FLEXSPIA_SCLK,     /* GPIO_SD_B1_07 is configured as FLEXSPIA_SCLK */
        1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_07 */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_SD_B1_08_FLEXSPIA_DATA00,   /* GPIO_SD_B1_08 is configured as FLEXSPIA_DATA00 */
        1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_08 */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_SD_B1_09_FLEXSPIA_DATA01,   /* GPIO_SD_B1_09 is configured as FLEXSPIA_DATA01 */
        1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_09 */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_SD_B1_10_FLEXSPIA_DATA02,   /* GPIO_SD_B1_10 is configured as FLEXSPIA_DATA02 */
        1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_10 */
    IOMUXC_SetPinMux(
        IOMUXC_GPIO_SD_B1_11_FLEXSPIA_DATA03,   /* GPIO_SD_B1_11 is configured as FLEXSPIA_DATA03 */
        1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_11 */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_AD_B0_12_LPUART1_TX,        /* GPIO_AD_B0_12 PAD functional properties : */
        0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                Drive Strength Field: R0/6
                                                Speed Field: medium(100MHz)
                                                Open Drain Enable Field: Open Drain Disabled
                                                Pull / Keep Enable Field: Pull/Keeper Enabled
                                                Pull / Keep Select Field: Keeper
                                                Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_AD_B0_13_LPUART1_RX,        /* GPIO_AD_B0_13 PAD functional properties : */
        0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                Drive Strength Field: R0/6
                                                Speed Field: medium(100MHz)
                                                Open Drain Enable Field: Open Drain Disabled
                                                Pull / Keep Enable Field: Pull/Keeper Enabled
                                                Pull / Keep Select Field: Keeper
                                                Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_SD_B1_05_FLEXSPIA_DQS,      /* GPIO_SD_B1_05 PAD functional properties : */
        0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                Drive Strength Field: R0/6
                                                Speed Field: max(200MHz)
                                                Open Drain Enable Field: Open Drain Disabled
                                                Pull / Keep Enable Field: Pull/Keeper Enabled
                                                Pull / Keep Select Field: Keeper
                                                Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_SD_B1_06_FLEXSPIA_SS0_B,    /* GPIO_SD_B1_06 PAD functional properties : */
        0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                Drive Strength Field: R0/6
                                                Speed Field: max(200MHz)
                                                Open Drain Enable Field: Open Drain Disabled
                                                Pull / Keep Enable Field: Pull/Keeper Enabled
                                                Pull / Keep Select Field: Keeper
                                                Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_SD_B1_07_FLEXSPIA_SCLK,     /* GPIO_SD_B1_07 PAD functional properties : */
        0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                Drive Strength Field: R0/6
                                                Speed Field: max(200MHz)
                                                Open Drain Enable Field: Open Drain Disabled
                                                Pull / Keep Enable Field: Pull/Keeper Enabled
                                                Pull / Keep Select Field: Keeper
                                                Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_SD_B1_08_FLEXSPIA_DATA00,   /* GPIO_SD_B1_08 PAD functional properties : */
        0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                Drive Strength Field: R0/6
                                                Speed Field: max(200MHz)
                                                Open Drain Enable Field: Open Drain Disabled
                                                Pull / Keep Enable Field: Pull/Keeper Enabled
                                                Pull / Keep Select Field: Keeper
                                                Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_SD_B1_09_FLEXSPIA_DATA01,   /* GPIO_SD_B1_09 PAD functional properties : */
        0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                Drive Strength Field: R0/6
                                                Speed Field: max(200MHz)
                                                Open Drain Enable Field: Open Drain Disabled
                                                Pull / Keep Enable Field: Pull/Keeper Enabled
                                                Pull / Keep Select Field: Keeper
                                                Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_SD_B1_10_FLEXSPIA_DATA02,   /* GPIO_SD_B1_10 PAD functional properties : */
        0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                Drive Strength Field: R0/6
                                                Speed Field: max(200MHz)
                                                Open Drain Enable Field: Open Drain Disabled
                                                Pull / Keep Enable Field: Pull/Keeper Enabled
                                                Pull / Keep Select Field: Keeper
                                                Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                Hyst. Enable Field: Hysteresis Disabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_SD_B1_11_FLEXSPIA_DATA03,   /* GPIO_SD_B1_11 PAD functional properties : */
        0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                Drive Strength Field: R0/6
                                                Speed Field: max(200MHz)
                                                Open Drain Enable Field: Open Drain Disabled
                                                Pull / Keep Enable Field: Pull/Keeper Enabled
                                                Pull / Keep Select Field: Keeper
                                                Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                Hyst. Enable Field: Hysteresis Disabled */
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ڲ����ã��û��������
//  @param      void 		    
//  @return     				
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
status_t flexspi_nor_write_enable(FLEXSPI_Type *base, uint32_t baseAddr)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write enable */
    flashXfer.deviceAddress = baseAddr;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Command;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ڲ����ã��û��������
//  @param      void 		    
//  @return     				
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
status_t flexspi_nor_wait_bus_busy(FLEXSPI_Type *base)
{
    /* Wait status ready. */
    bool isBusy;
    uint32_t readValue;
    status_t status;
    flexspi_transfer_t flashXfer;

    flashXfer.deviceAddress = 0;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Read;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_READSTATUSREG;
    flashXfer.data = &readValue;
    flashXfer.dataSize = 4;

    do
    {
        status = FLEXSPI_TransferBlocking(base, &flashXfer);
        if (status != kStatus_Success)
        {
            return status;
        }
        if (W25QXXJV_BUSY_STATUS_POL)
        {
            if (readValue & (1U << W25QXXJV_BUSY_STATUS_OFFSET))
            {
                isBusy = true;
            }
            else
            {
                isBusy = false;
            }
        }
        else
        {
            if (readValue & (1U << W25QXXJV_BUSY_STATUS_OFFSET))
            {
                isBusy = false;
            }
            else
            {
                isBusy = true;
            }
        }

    } while (isBusy);
	
		
    return status;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ڲ����ã��û��������
//  @param      void 		    
//  @return     				
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
status_t flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address)
{
    status_t status;
    flexspi_transfer_t flashXfer;

    /* Write enable */
    status = flexspi_nor_write_enable(base, address);

    if (status != kStatus_Success)
    {
        return status;
    }

    flashXfer.deviceAddress = address;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Command;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_ERASESECTOR;
    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nor_wait_bus_busy(base);

    return status;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ڲ����ã��û��������
//  @param      void 		    
//  @return     				
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
status_t flexspi_nor_flash_page_program(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src, uint16 num)
{
    status_t status;
    flexspi_transfer_t flashXfer;

    /* Write enable */
    status = flexspi_nor_write_enable(base, dstAddr);

    if (status != kStatus_Success)
    {
        return status;
    }

    /* Prepare page program command */
    flashXfer.deviceAddress = dstAddr;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Write;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD;
    flashXfer.data = (uint32_t *)src;
    flashXfer.dataSize = num;
    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nor_wait_bus_busy(base);

    return status;
}

////-------------------------------------------------------------------------------------------------------------------
////  @brief      �ڲ����ã��û��������
////  @param      void 		    
////  @return     				
////  @since      v1.0
////  Sample usage:               
////-------------------------------------------------------------------------------------------------------------------
//status_t flexspi_nor_flash_read_sector(FLEXSPI_Type *base, uint32_t address,const uint32_t *src,size_t leng))
//{
//	//uint32_t temp;
//    flexspi_transfer_t flashXfer;
//    flashXfer.deviceAddress = address;
//    flashXfer.port = kFLEXSPI_PortA1;
//    flashXfer.cmdType = kFLEXSPI_Read;
//    flashXfer.SeqNumber = 1;
//    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_READ_NORMAL1;
//    flashXfer.data = (uint32_t *)src;
//    flashXfer.dataSize = leng;
//
//    status_t status = FLEXSPI_TransferBlocking(base, &flashXfer);
//
//   // *vendorId = temp;
//
//    return status;
//}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ڲ����ã��û��������
//  @param      void 		    
//  @return     				
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
status_t flexspi_nor_get_vendor_id(FLEXSPI_Type *base, uint8_t *vendorId)
{
    uint32_t temp;
    status_t status;
        
    flexspi_transfer_t flashXfer;
    flashXfer.deviceAddress = 0;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Read;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_READID;
    flashXfer.data = &temp;
    flashXfer.dataSize = 1;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    *vendorId = temp;

    return status;
}


flexspi_device_config_t deviceconfig = {
    .flexspiRootClk = 133000000,
    .flashSize = W25QXXJV_SIZE,
    .CSIntervalUnit = kFLEXSPI_CsIntervalUnit1SckCycle,
    .CSInterval = 2,
    .CSHoldTime = 3,
    .CSSetupTime = 3,
    .dataValidTime = 0,
    .columnspace = 0,
    .enableWordAddress = 0,
    .AWRSeqIndex = 0,
    .AWRSeqNumber = 0,
    .ARDSeqIndex = NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD,
    .ARDSeqNumber = 1,
    .AHBWriteWaitUnit = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
    .AHBWriteWaitInterval = 0,
};
uint8 vendorID;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASH��ʼ������
//  @param      void 		    
//  @return     				
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void w25qxxjv_init(void)
{
    status_t status;
    flexspi_config_t config;

    flash_iomuxc();
    
    CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* ѡ�� PLL3 PFD0 ʱ����Ϊflexspiʱ��Դ. */
    CLOCK_SetDiv(kCLOCK_FlexspiDiv, 5);   /* ����ʱ��Ϊ133M */
    
    FLEXSPI_GetDefaultConfig(&config);

    config.ahbConfig.enableAHBPrefetch = true;
    config.ahbConfig.enableAHBBufferable = true;
    config.ahbConfig.enableReadAddressOpt = true;
    config.ahbConfig.enableAHBCachable = true;
    config.rxSampleClock = kFLEXSPI_ReadSampleClkLoopbackFromDqsPad;
    FLEXSPI_Init(W25QXXJV_FLEXSPI, &config);

    FLEXSPI_SetFlashConfig(W25QXXJV_FLEXSPI, &deviceconfig, kFLEXSPI_PortA1);

    FLEXSPI_UpdateLUT(W25QXXJV_FLEXSPI,0,customLUT, CUSTOM_LUT_LENGTH);

    FLEXSPI_SoftwareReset(W25QXXJV_FLEXSPI);

    status = flexspi_nor_get_vendor_id(W25QXXJV_FLEXSPI, &vendorID);
    
    if (status != kStatus_Success)  assert(0);//��ȡIDʧ�� ��������
    if (0x16 != vendorID)           assert(0);//IDУ��ʧ�� ��������
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      У��FLASH�Ƿ�������
//  @param      sector_num 		��Ҫд����������		W25Q64JV ������Χ��0-2047
//  @param      page_num	 	��ǰ����ҳ�ı��		������Χ0-15
//  @return     				����1�����ݣ�����0û�����ݣ������Ҫ�������ݵ�����д���µ�������Ӧ�ö������������в�������
//  @since      v1.0
//  Sample usage:               w25qxxjv_check(500,0);//У��500����������=��0ҳ�Ƿ�������
//-------------------------------------------------------------------------------------------------------------------
uint8 w25qxxjv_check(uint32 sector_num, uint32 page_num)
{
    uint8 i;
    
    assert(W25QXXJV_SECTOR_NUM>sector_num);                //У������Ϸ���
    assert(W25QXXJV_SECTOR_PAGE_NUM>page_num);             //У������Ϸ���
    
    DCACHE_CleanInvalidateByRange(W25QXXJV_BASE_ADDR + sector_num * W25QXXJV_SECTOR_SIZE, 256);//��ȡflashǰ���������ջ���
    for(i=0; i<W25QXXJV_PAGE_SIZE/4; i++)
    {
        if(0xFFFFFFFF != w25qxxjv_read(sector_num,page_num*W25QXXJV_PAGE_SIZE/4+i,uint32))
        {
            return 1;//flash��Ҫ����
        }
    }
    
    return 0;//flash����Ҫ����
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASH��������
//  @param      sector_num 		��Ҫ�������������		W25Q64JV ������Χ��0-2047
//  @return     				������������ʧ�ܣ�����0�����ɹ�
//  @since      v1.0
//  Sample usage:               uint32 dat = w25qxxjv_erase_sector(1000);
//-------------------------------------------------------------------------------------------------------------------
uint8 w25qxxjv_erase_sector(uint32 sector_num)
{
    status_t status;
    uint8 isr;
    
    assert(W25QXXJV_SECTOR_NUM>sector_num);//У������Ϸ���
    isr = DisableGlobalIRQ();
    status = flexspi_nor_flash_erase_sector(W25QXXJV_FLEXSPI, sector_num * W25QXXJV_SECTOR_SIZE);
    EnableGlobalIRQ(isr);
    return status;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASHҳ���
//  @param      sector_num 		��Ҫд����������			W25Q64JV ������Χ��0-2047
//  @param      page_num		д�뽫����д��������ڼ�ҳ	������Χ0-15
//  @param      *buf	 		�����׵�ַ
//  @param      len		 		д����ֽ��� ˵������len=1���ʾд��len*4���ֽ�	������Χ1-64
//  @return     				��������д��ʧ�ܣ�����0д��ɹ�
//  @since      v1.0
//  Sample usage:               w25qxxjv_write_page(500,0,(const void *)buf,64);//��buf������������� ǰ64���֣�������ֱ�ʾ�ĸ��ֽڣ� д��flash500��������0ҳ
//  һ��flash���� ��Ϊ��n��������Ȼ��һ���ж��ҳ��ÿһ��ҳ�й̶���С
//  ���İ��Դ���flash ÿ������Ϊ4KB��ÿ��ҳ��256�ֽڣ����һ��������16ҳ
//-------------------------------------------------------------------------------------------------------------------
uint8 w25qxxjv_write_page(uint32 sector_num, uint32 page_num, const void *buf, uint8 len)
{
    uint8 isr;
    status_t status;

    
    assert(W25QXXJV_SECTOR_NUM>sector_num);                //У������Ϸ���
    assert(W25QXXJV_SECTOR_PAGE_NUM>page_num);             //У������Ϸ���
    assert(W25QXXJV_PAGE_SIZE>=(len*4));                   //У������Ϸ���
    
    isr = DisableGlobalIRQ();
    status = flexspi_nor_flash_page_program(W25QXXJV_FLEXSPI, sector_num * W25QXXJV_SECTOR_SIZE + W25QXXJV_PAGE_SIZE*page_num, buf, len*4);
    EnableGlobalIRQ(isr);
    
    return status;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASHҳ��ȡ
//  @param      sector_num 		��Ҫд����������			W25Q64JV ������Χ��0-2047
//  @param      page_num		д�뽫����д��������ڼ�ҳ	������Χ0-15
//  @param      *buf	 		�����׵�ַ
//  @param      len		 		��ȡ���ֽ��� ˵������len=1���ʾд��len*4���ֽ�	������Χ1-64
//  @return     void
//  @since      v1.0
//  Sample usage:               w25qxxjv_read_page(500,0,(void *)buf,64);//��flash500��������0ҳ��������� ǰ64���֣�������ֱ�ʾ�ĸ��ֽڣ� д��buf����
//  һ��flash���� ��Ϊ��n��������Ȼ��һ���ж��ҳ��ÿһ��ҳ�й̶���С
//  ���İ��Դ���flash ÿ������Ϊ4KB��ÿ��ҳ��256�ֽڣ����һ��������16ҳ
//-------------------------------------------------------------------------------------------------------------------
void w25qxxjv_read_page(uint32 sector_num, uint32 page_num, void *buf, uint8 len)
{
    uint8 i;
    
    assert(W25QXXJV_SECTOR_NUM>sector_num);                //У������Ϸ���
    assert(W25QXXJV_SECTOR_PAGE_NUM>page_num);             //У������Ϸ���
    assert(W25QXXJV_PAGE_SIZE>=(len*4));                   //У������Ϸ���
    DCACHE_CleanInvalidateByRange(W25QXXJV_BASE_ADDR + sector_num * W25QXXJV_SECTOR_SIZE, len*4);//��ȡflashǰ���������ջ���
    
    for(i=0; i<len; i++)
    {
        *((uint32 *)buf + i) = w25qxxjv_read(sector_num,page_num*W25QXXJV_PAGE_SIZE+i,uint32);
        
    }
}




