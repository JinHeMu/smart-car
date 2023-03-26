/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		flash(����) 
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/

#include "fsl_cache.h"
#include "zf_rom_api.h"
#include "zf_flash.h"

flexspi_nor_config_t config;
serial_nor_config_option_t option;
uint32 instance = 1;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      У��FLASH�Ƿ�������
//  @param      sector_num 		��Ҫд����������   ������Χ0-1023
//  @param      page_num	 	��ǰ����ҳ�ı��     ������Χ0-15
//  @return     				����1�����ݣ�����0û�����ݣ������Ҫ�������ݵ�����д���µ�������Ӧ�ö������������в�������
//  @since      v1.0
//  Sample usage:               flash_check(500,0);//У��500����������0ҳ�Ƿ�������
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_check(uint32 sector_num, uint32 page_num)
{
    uint8 i;
    
    assert(FLASH_SECTOR_NUM>sector_num);                //У������Ϸ���
    assert(FLASH_SECTOR_PAGE_NUM>page_num);             //У������Ϸ���
    
    DCACHE_CleanInvalidateByRange(FLASH_BASE_ADDR + sector_num * FLASH_SECTOR_SIZE, 256);//��ȡflashǰ���������ջ���
    for(i=0; i<FLASH_PAGE_SIZE/4; i++)
    {
        if(0xFFFFFFFF != flash_read(sector_num,page_num*FLASH_PAGE_SIZE/4+i,uint32))
        {
            return kStatus_Fail;//flash��Ҫ����
        }
    }
    
    return kStatus_Success;//flash����Ҫ����
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��������
//  @param      sector_num 		��Ҫд����������   ������Χ0-1023
//  @return     				����1�б�ʾʧ��  ����0��ʾ�ɹ�
//  @since      v1.0
//  Sample usage:               flash_erase_sector(500);
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_erase_sector(uint32 sector_num)
{
    status_t status;
    
    assert(FLASH_SECTOR_NUM>sector_num);                //У������Ϸ���
    
    status = rom_api_flexspi_nor_flash_erase(instance, &config, sector_num*FLASH_SECTOR_SIZE, FLASH_SECTOR_SIZE);
    if(status != kStatus_Success)
    {
        return kStatus_Fail;
    }
    
    
    
    return kStatus_Success;

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���һҳ
//  @param      sector_num 		��Ҫд����������   ������Χ0-1023
//  @param      page_num 		��Ҫд���ҳ���     ������Χ0-15
//  @param      buf 		    ��Ҫд������ݵ�ַ   ������������ͱ���Ϊuint32
//  @param      len 		    ��Ҫд������ݳ���   ������Χ1-64
//  @return     				����1�б�ʾʧ��  ����0��ʾ�ɹ�
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_page_program(uint32 sector_num, uint32 page_num, const uint32 *buf, uint8 len)
{
    uint16 i=0;
    status_t status;
    uint8 page_buffer[256];
    
    assert(FLASH_SECTOR_NUM>sector_num);                //У������Ϸ���
    assert(FLASH_SECTOR_PAGE_NUM>page_num);             //У������Ϸ���
    assert(FLASH_PAGE_SIZE>=(len*4));                   //У������Ϸ���
    
    while(i<(len*4))
    {
        page_buffer[i] = ((uint8 *)buf)[i];
        i++;
    }
    while(i<FLASH_PAGE_SIZE)
    {
        page_buffer[i] = 0xff;
        i++;
    }
    
    status = rom_api_flexspi_nor_flash_page_program(instance,&config,sector_num*FLASH_SECTOR_SIZE+page_num*FLASH_PAGE_SIZE,(const uint32_t *)page_buffer);
    if(status != kStatus_Success)
    {
        return kStatus_Fail;
    }
    
    
    
    return kStatus_Success;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��һҳ
//  @param      sector_num 		��Ҫ��ȡ���������       ������Χ0-1023
//  @param      page_num 		��Ҫ��ȡ��ҳ���         ������Χ0-15
//  @param      buf 		    ��Ҫ��ȡ�����ݵ�ַ       ������������ͱ���Ϊuint32
//  @param      len 		    ��Ҫ��ȡ�����ݳ���       ������Χ1-64
//  @return     				����1�б�ʾʧ��  ����0��ʾ�ɹ�
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void flash_read_page(uint32 sector_num, uint32 page_num, uint32 *buf, uint8 len)
{
    uint8 i;
    
    assert(FLASH_SECTOR_NUM>sector_num);                //У������Ϸ���
    assert(FLASH_SECTOR_PAGE_NUM>page_num);             //У������Ϸ���
    assert(FLASH_PAGE_SIZE>=(len*4));                   //У������Ϸ���
    DCACHE_CleanInvalidateByRange(FLASH_BASE_ADDR + sector_num * FLASH_SECTOR_SIZE, len*4);//��ȡflashǰ���������ջ���
    
    for(i=0; i<len; i++)
    {
        *(buf + i) = flash_read(sector_num,page_num*FLASH_PAGE_SIZE+i,uint32);
        
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      flash��ʼ��
//  @param      void 		
//  @return     				����1�б�ʾʧ��  ����0��ʾ�ɹ�
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_init(void)
{
    option.option0.U = 0xC0000008; // QuadSPI NOR, Frequency: 133MHz
    
    rom_api_init();
    if(rom_api_flexspi_nor_get_config(instance, &config, &option) !=  kStatus_Success)
    {
        return kStatus_Fail;
    }
    return kStatus_Success;
}


