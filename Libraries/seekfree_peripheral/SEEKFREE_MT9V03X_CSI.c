/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		����� RT CSI�ӿ�
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-02-18
 * @note		
					���߶��壺
					------------------------------------ 
						ģ��ܽ�            ��Ƭ���ܽ�
						SDA(51��RX)         �鿴SEEKFREE_MT9V03X_CSI.h�ļ��е�MT9V03X_CSI_COF_UART_TX�궨��
						SCL(51��TX)         �鿴SEEKFREE_MT9V03X_CSI.h�ļ��е�MT9V03X_CSI_COF_UART_RX�궨��
						���ж�(VSY)         �鿴SEEKFREE_MT9V03X_CSI.h�ļ��е�MT9V03X_CSI_VSYNC_PIN�궨��
						���ж�(HREF)	    ����Ҫʹ��
						�����ж�(PCLK)      �鿴SEEKFREE_MT9V03X_CSI.h�ļ��е�MT9V03X_CSI_PCLK_PIN�궨��
						���ݿ�(D0-D7)		B31-B24 B31��Ӧ����ͷ�ӿ�D0
					------------------------------------ 
	
					Ĭ�Ϸֱ�����            188*120
					Ĭ��FPS                 50֡
 ********************************************************************************************************************/


#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_csi.h"
#include "zf_systick.h"
#include "zf_pit.h"
#include "zf_camera.h"
#include "zf_gpio.h"
#include "zf_iomuxc.h"
#include "zf_csi.h"
#include "SEEKFREE_MT9V03X.h"
#include "SEEKFREE_MT9V03X_CSI.h"


//ͼ�񻺳���  ����û���Ҫ����ͼ������ ���ͨ��mt9v03x_csi_image���������ݣ���ò�Ҫֱ�ӷ��ʻ�����
AT_DTCM_SECTION_ALIGN(uint8 mt9v03x_csi1_image[MT9V03X_CSI_H][MT9V03X_CSI_W], 64);
AT_DTCM_SECTION_ALIGN(uint8 mt9v03x_csi2_image[MT9V03X_CSI_H][MT9V03X_CSI_W], 64);

//�û�����ͼ������ֱ�ӷ������ָ������Ϳ���
//���ʷ�ʽ�ǳ��򵥣�����ֱ��ʹ���±�ķ�ʽ����
//������ʵ�10�� 50�еĵ㣬mt9v03x_csi_image[10][50]�Ϳ�����
uint8 (*mt9v03x_csi_image)[MT9V03X_CSI_W];







//��Ҫ���õ�����ͷ������
int16 MT9V03X_CFG_CSI[CONFIG_FINISH][2]=
{
    {AUTO_EXP,          0},   //�Զ��ع�����      ��Χ1-63 0Ϊ�ر� ����Զ��ع⿪��  EXP_TIME�������õ����ݽ����Ϊ����ع�ʱ�䣬Ҳ�����Զ��ع�ʱ�������
                              //һ������ǲ���Ҫ����������ܣ���Ϊ�������ع���һ�㶼�ȽϾ��ȣ�����������߷ǳ������ȵ�������Գ������ø�ֵ������ͼ���ȶ���
    {EXP_TIME,          800}, //�ع�ʱ��          ����ͷ�յ�����Զ����������ع�ʱ�䣬������ù���������Ϊ�������������ع�ֵ
    {FPS,               50},  //ͼ��֡��          ����ͷ�յ�����Զ���������FPS���������������Ϊ������������FPS
    {SET_COL,           MT9V03X_CSI_W}, //ͼ��������        ��Χ1-752     K60�ɼ���������188
    {SET_ROW,           MT9V03X_CSI_H}, //ͼ��������        ��Χ1-480
    {LR_OFFSET,         0},   //ͼ������ƫ����    ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ188 376 752ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
    {UD_OFFSET,         0},   //ͼ������ƫ����    ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ120 240 480ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
    {GAIN,              32},  //ͼ������          ��Χ16-64     ����������ع�ʱ��̶�������¸ı�ͼ�������̶�
    {PCLK_MODE,         0},   //�������MT9V034 V2.0�Լ����ϰ汾֧�ָ����
                              //����ʱ��ģʽ���� PCLKģʽ     Ĭ�ϣ�0     ��ѡ����Ϊ��0 1��        0������������źţ�1����������źš�(ͨ��������Ϊ0�����ʹ��CH32V307��DVP�ӿڻ�STM32��DCMI�ӿڲɼ���Ҫ����Ϊ1)
    
    
    {INIT,              0}    //����ͷ��ʼ��ʼ��
};

//������ͷ�ڲ���ȡ������������
int16 GET_CFG_CSI[CONFIG_FINISH-1][2]=
{
    {AUTO_EXP,          0},   //�Զ��ع�����      
    {EXP_TIME,          0},   //�ع�ʱ��          
    {FPS,               0},   //ͼ��֡��          
    {SET_COL,           0},   //ͼ��������        
    {SET_ROW,           0},   //ͼ��������        
    {LR_OFFSET,         0},   //ͼ������ƫ����    
    {UD_OFFSET,         0},   //ͼ������ƫ����    
    {GAIN,              0},   //ͼ������          
};


uint8               csi_rx_buffer;
lpuart_transfer_t   csi_receivexfer;
lpuart_handle_t     csi_g_lpuartHandle;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V03X����ͷ�����жϺ���
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       �ú�����ISR�ļ� ����3�жϳ��򱻵���
//-------------------------------------------------------------------------------------------------------------------
void csi_mt9v03x_uart_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
    if(kStatus_LPUART_RxIdle == status)
    {
        receive[receive_num] = csi_rx_buffer;
        receive_num++;
    
        if(1==receive_num && 0XA5!=receive[0])  receive_num = 0;
        if(3 == receive_num)
        {
            receive_num = 0;
            uart_receive_flag = 1;
        }
    }
    handle->rxDataSize = csi_receivexfer.dataSize;  //��ԭ����������
    handle->rxData = csi_receivexfer.data;          //��ԭ��������ַ
}






uint8 mt9v03x_csi_finish_flag;  //ͼ��ɼ���ɵı�־λ    



void csi_isr(CSI_Type *base, csi_handle_t *handle, status_t status, void *userData)
{
    if(csi_get_full_buffer(&csi_handle,&fullCameraBufferAddr))
    {
        csi_add_empty_buffer(&csi_handle,(uint8 *)fullCameraBufferAddr);
        if(fullCameraBufferAddr == (uint32)mt9v03x_csi1_image[0])
        {
            mt9v03x_csi_image = mt9v03x_csi1_image;//image_csi1�ɼ����
        }
        else if(fullCameraBufferAddr == (uint32)mt9v03x_csi2_image[0])
        {
            mt9v03x_csi_image = mt9v03x_csi2_image;//image_csi2�ɼ����
        }
        mt9v03x_csi_finish_flag = 1;//�ɼ���ɱ�־λ��һ
    }
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V03X����ͷ��ʼ�� ʹ��CSI�ӿ�
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:		ʹ��CSI�ӿڲɼ�����ͷ
//-------------------------------------------------------------------------------------------------------------------
void mt9v03x_csi_init(void)
{
    uart_init (MT9V03X_CSI_COF_UART, 9600,MT9V03X_CSI_COF_UART_TX,MT9V03X_CSI_COF_UART_RX);	//��ʼ������ ��������ͷ    
    uart_rx_irq(MT9V03X_CSI_COF_UART,1);
    //���ô��ڽ��յĻ�����������������
    csi_receivexfer.dataSize = 1;
    csi_receivexfer.data = &csi_rx_buffer;
    
    uart_set_handle(MT9V03X_CSI_COF_UART, &csi_g_lpuartHandle, csi_mt9v03x_uart_callback, NULL, 0, csi_receivexfer.data, 1);
    EnableGlobalIRQ(0);
    //�ȴ�����ͷ�ϵ��ʼ���ɹ� ��ʽ�����֣���ʱ����ͨ����ȡ���õķ�ʽ ��ѡһ
    //systick_delay_ms(1000);//��ʱ��ʽ
    get_config(MT9V03X_CSI_COF_UART,GET_CFG_CSI);//��ȡ���õķ�ʽ
    
	uart_receive_flag = 0;
    set_config(MT9V03X_CSI_COF_UART,MT9V03X_CFG_CSI);
    //��ȡ���ñ��ڲ鿴�����Ƿ���ȷ
    get_config(MT9V03X_CSI_COF_UART,GET_CFG_CSI);
    DisableGlobalIRQ();
    
    //CSI �ɼ���ʼ��
    csi_init(MT9V03X_CSI_W, MT9V03X_CSI_H, &csi_handle, csi_isr, MT9V03X_CSI_VSYNC_PIN, MT9V03X_CSI_PCLK_PIN);
    csi_add_empty_buffer(&csi_handle, mt9v03x_csi1_image[0]);
    csi_add_empty_buffer(&csi_handle, mt9v03x_csi2_image[0]);
    csi_start(&csi_handle);
    mt9v03x_csi_image = mt9v03x_csi1_image;//���ó�ֵ
}




void csi_seekfree_sendimg_03x(UARTN_enum uartn, uint8 *image, uint16 width, uint16 height)
{
    uart_putchar(uartn,0x00);uart_putchar(uartn,0xff);uart_putchar(uartn,0x01);uart_putchar(uartn,0x01);//��������
    uart_putbuff(uartn, image, width*height);  //����ͼ��
    
}

