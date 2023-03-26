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


#ifndef _SEEKFREE_W25QXXJV_h
#define _SEEKFREE_W25QXXJV_h


#include "common.h"

#define W25QXXJV_FLEXSPI                FLEXSPI
#define W25QXXJV_FLEXSPI_AMBA_BASE      FlexSPI_AMBA_BASE
#define W25QXXJV_FLEXSPI_CLOCK          kCLOCK_FlexSpi

#define W25QXXJV_BASE_ADDR              0x60000000
#define W25QXXJV_SIZE                   8192    //64Mbit 
#define W25QXXJV_SECTOR_SIZE            4096    //4KB
#define W25QXXJV_SECTOR_NUM             2048
    
#define W25QXXJV_PAGE_SIZE              256
#define W25QXXJV_SECTOR_PAGE_NUM        (W25QXXJV_SECTOR_SIZE/W25QXXJV_PAGE_SIZE)






//-------------------------------------------------------------------------------------------------------------------
//  @brief      ʹ�ú궨���flash�������ݶ�ȡ
//  @param      sector_num 		��Ҫд����������
//  @param      offset	 		��ַƫ�� ƫ��1���� Ϊ�ĸ��ֽ�
//  @param      type		 	��ȡ����������
//  @return     				���ظ�����ַ������
//  @since      v1.0
//  Sample usage:               flash_read(20,0,uint32);//��ȡ20������ƫ��0��������Ϊuint32
//  @note       ��Ҫ�ر�ע����� RTϵ����cache�������ÿ�ζ�ȡflash֮ǰ����SCB_InvalidateDCache();ִ�и�������cache
//              ������˵û��һ���������һ�£�
//-------------------------------------------------------------------------------------------------------------------
#define     w25qxxjv_read(sector_num,offset,type)      (*(type *)((uint32)((W25QXXJV_BASE_ADDR + (sector_num)*W25QXXJV_SECTOR_SIZE) + (offset*4))))


//����ת���궨����zf_flash.h�ļ���






void   w25qxxjv_init(void);
uint8  w25qxxjv_check(uint32 sector_num, uint32 page_num);
uint8  w25qxxjv_erase_sector(uint32 sector_num);
uint8  w25qxxjv_write_page(uint32 sector_num, uint32 page_num, const void *buf, uint8 len);
void   w25qxxjv_read_page(uint32 sector_num, uint32 page_num, void *buf, uint8 len);


#endif

