/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		common
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/
 
#include "common.h"


CAMERA_TYPE_enum flexio_camera_type;     //����ͷ�ͺ�
uint8 *flexio_camera_buffer_addr;        //����ͷ�����ַ


void NMI_Handler(void)
{
	while(1);
}
//void HardFault_Handler(void)
//{
//	//Ӳ���Ϸ�  ����������Խ�����ʹ����δ��ʼ�����豸
//	//������ʾ���һ�����⣬��������˵�ҵ������ʼ������ô����Ҫ��HardFault��
//	//�ܶ�����Ϊ�Լ������жϣ�Ȼ�����ж�����ʹ�������裬Ȼ�����ĳ�ʼ��˳�����ȳ�ʼ���ж�Ȼ���ٳ�ʼ������
//	//��ô��������ˣ��жϳ�ʼ�����֮��Ჿ���ж�ֱ�ӽ����жϺ��������������Ҳ������жϺ�������ô�ͻ�����ж�����ʹ�õ������軹û����ʼ��
//	//���Դ����Ҫע���������
//	while(1);
//}
//void MemManage_Handler(void)
//{
//	while(1);
//}
void BusFault_Handler(void)
{
	while(1);
}
void UsageFault_Handler(void)
{
	while(1);
}
void SVC_Handler(void)
{
	while(1);
}
void DebugMon_Handler(void)
{
	while(1);
}
//void PendSV_Handler(void)
//{
//	while(1);
//}

//�˺����Ѿ���fsl_sdmmc_event.c�ļ��ж���
//void SysTick_Handler(void)
//{
//	while(1);
//}