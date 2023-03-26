/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		��ͫ����ͷ(SCC8660) RT FLEXIO�ӿ�
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598 & QQ2380006440)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.32.4 or MDK 5.24 �����
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-09-04
 * @note		
					���߶��壺
					------------------------------------ 
						ģ��ܽ�            ��Ƭ���ܽ�
						SDA(����ͷ��RX)     �鿴SEEKFREE_SCC8660.h�ļ��е�SCC8660_COF_UART_TX�궨��
						SCL(����ͷ��TX)     �鿴SEEKFREE_SCC8660.h�ļ��е�SCC8660_COF_UART_RX�궨��
                        ���ж�(VSY)         �鿴SEEKFREE_SCC8660.h�ļ��е�SCC8660_VSYNC_PIN�궨��
						���ж�(HREF)		�鿴SEEKFREE_SCC8660.h�ļ��е�SCC8660_HREF_PIN�궨��
						�����ж�(PCLK)      �鿴SEEKFREE_SCC8660.h�ļ��е�SCC8660_PCLK_PIN�궨��
						���ݿ�(D0-D7)		�鿴SEEKFREE_SCC8660.h�ļ��е�SCC8660_DATA_PIN�궨��
					------------------------------------ 
	
					Ĭ�Ϸֱ���               160*120
					Ĭ��FPS                  25֡
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
#include "fsl_cache.h"
#include "seekfree_iic.h"
#include "SEEKFREE_SCC8660_CSI.h"
#include "SEEKFREE_SCC8660.h"


//����ͼ�񻺳���  ����û���Ҫ����ͼ������ ���ͨ��scc8660_image���������ݣ���ò�Ҫֱ�ӷ��ʻ�����
//AT_DTCM_SECTION_ALIGN(uint16 scc8660_flexio1_image[SCC8660_PIC_H][SCC8660_PIC_W],64);
//AT_DTCM_SECTION_ALIGN(uint16 scc8660_flexio2_image[SCC8660_PIC_H][SCC8660_PIC_W],64);
AT_OCRAM_SECTION_ALIGN(uint16 scc8660_flexio1_image[SCC8660_PIC_H][SCC8660_PIC_W],64);
AT_OCRAM_SECTION_ALIGN(uint16 scc8660_flexio2_image[SCC8660_PIC_H][SCC8660_PIC_W],64);

uint16 (*flexio_color_camera_buffer_addr1)[SCC8660_PIC_W];//����ͷ�����ַ
uint16 (*flexio_color_camera_buffer_addr2)[SCC8660_PIC_W];//����ͷ�����ַ

//�û�����ͼ������ֱ�ӷ������ָ������Ϳ���
//���ʷ�ʽ�ǳ��򵥣�����ֱ��ʹ���±�ķ�ʽ����
//������ʵ�10�� 50�еĵ㣬scc8660_image[10][50]�Ϳ�����
uint16 (*scc8660_image)[SCC8660_PIC_W];


//��Ҫ���õ�����ͷ������
uint16 SCC8660_CFG[SCC8660_CONFIG_FINISH][2]=
{
    {SCC8660_AUTO_EXP,          0},                     //�Զ��ع�     Ĭ�ϣ�0     ��ѡ����Ϊ��0 1��      0���ֶ��ع�  1���Զ��ع�
    {SCC8660_BRIGHT,            800},                   //��������     �ֶ��ع�Ĭ�ϣ�800	�ֶ��ع�ʱ��������Χ0-65535   �Զ��ع��Ƽ�ֵ��100 �Զ��ع�ʱ�������÷�Χ0-255
														//��Ҫע��SCC8660_BRIGHT�Ĳ����̶�ʱ����ͬ��SCC8660_PCLK_DIV������Ӱ��ͼ������ȡ�
														//����SCC8660_BRIGHT�Ĳ���Ϊ800��SCC8660_PCLK_DIV�Ĳ���Ϊ0ʱ �� SCC8660_PCLK_DIV�Ĳ���Ϊ2ʱ 
														//����Ϊ2��ʱ��ͼ������Ҫ��Ϊ0��ʱ��������ʹ��˫���ʱ��о���������ͷ���Ȳ�һ��ʱ��Ҫע��������⡣
														
    {SCC8660_FPS,               50},                    //ͼ��֡��     Ĭ�ϣ�50    ��ѡ����Ϊ��60 50 30 25�� ʵ��֡�ʻ���Ҫ��SCC8660_PCLK_DIV����������
    {SCC8660_SET_COL,           SCC8660_PIC_W},         //ͼ������     Ĭ�ϣ�160   ����.h�ĺ궨�崦�޸�
    {SCC8660_SET_ROW,           SCC8660_PIC_H},         //ͼ������     Ĭ�ϣ�120   ����.h�ĺ궨�崦�޸�
    {SCC8660_PCLK_DIV,          2},                     //PCLK��Ƶϵ�� Ĭ�ϣ�0     ��ѡ����Ϊ��0:1/1 1:2/3 2:1/2 3:1/3 4:1/4 5:1/8��  
                                                       //��Ƶϵ��Խ��PCLKƵ��Խ�ͣ�����PCLK���Լ���DVP�ӿڵĸ��ţ�������PCLKƵ�����Ӱ��֡�ʡ��������������뱣��Ĭ�ϡ�
                                                       //��������FPSΪ50֡������pclk��Ƶϵ��ѡ���Ϊ5��������ͷ�����֡��Ϊ50*��1/8��=6.25֡
                                                       //�����������������£�SCC8660_PCLK_DIV����Խ��ͼ���Խ��
													   //FLEXIO���ܽ��ܹ����Ƶ�ʣ���������������Ϊ2��������
													   
    
    {SCC8660_PCLK_MODE,         0},                     //PCLKģʽ     Ĭ�ϣ�0     ��ѡ����Ϊ��0 1��        0������������źţ�1����������źš�(ͨ��������Ϊ0�����ʹ��STM32��DCMI�ӿڲɼ���Ҫ����Ϊ1)
    {SCC8660_COLOR_MODE,        0},                     //ͼ��ɫ��ģʽ Ĭ�ϣ�0     ��ѡ����Ϊ��0 1��        0��������ɫģʽ  1������ģʽ��ɫ�ʱ��Ͷ���ߣ�
    {SCC8660_DATA_FORMAT,       0},                     //������ݸ�ʽ Ĭ�ϣ�0	   ��ѡ����Ϊ��0 1 2 3��    0:RGB565 1:RGB565(�ֽڽ���) 2:YUV422(YUYV) 3:YUV422(UYVY)
    {SCC8660_MANUAL_WB,         0},                     //�ֶ���ƽ��   Ĭ�ϣ�0     ��ѡ����Ϊ��0 0x65-0xa0��0:�ر��ֶ���ƽ�⣬�����Զ���ƽ��    �������ֶ���ƽ�� �ֶ���ƽ��ʱ ������Χ0x65-0xa0
    
    {SCC8660_INIT,              0}                      //����ͷ��ʼ��ʼ��
};



//������ͷ�ڲ���ȡ������������
uint16 SCC8660_GET_CFG[SCC8660_CONFIG_FINISH-1][2]=
{
    {SCC8660_AUTO_EXP,          0},
    {SCC8660_BRIGHT,            0}, //��������          
    {SCC8660_FPS,               0}, //ͼ��֡��           
    {SCC8660_SET_COL,           0}, //ͼ������           
    {SCC8660_SET_ROW,           0}, //ͼ������          
    {SCC8660_PCLK_DIV,          0}, //PCLK��Ƶϵ��      
    {SCC8660_PCLK_MODE,         0}, //PCLKģʽ      
    {SCC8660_COLOR_MODE,        0}, //ͼ��ɫ��ģʽ
    {SCC8660_DATA_FORMAT,       0}, //������ݸ�ʽ 	
    {SCC8660_MANUAL_WB,         0}, //��ƽ������
};


uint8               scc8660_rx_buffer;
lpuart_transfer_t   scc8660_receivexfer;
lpuart_handle_t     scc8660_g_lpuartHandle;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCC8660(��ͫ����ͷ)�����жϺ���
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       �ú�����SDK�ײ�fsl_lpuart�ļ��еĴ���3�жϺ����ڵ���
//-------------------------------------------------------------------------------------------------------------------
void scc8660_uart_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
    if(kStatus_LPUART_RxIdle == status)
    {
        scc8660_uart_receive[scc8660_uart_receive_num] = scc8660_rx_buffer;
        scc8660_uart_receive_num++;
    
        if(1==scc8660_uart_receive_num && 0XA5!=scc8660_uart_receive[0])  scc8660_uart_receive_num = 0;
        if(3 == scc8660_uart_receive_num)
        {
            scc8660_uart_receive_num = 0;
            scc8660_uart_receive_flag = 1;
        }
    }
    handle->rxDataSize = scc8660_receivexfer.dataSize;  //��ԭ����������
    handle->rxData = scc8660_receivexfer.data;          //��ԭ��������ַ
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCC8660(��ͫ����ͷ)���ô��ڳ�ʼ��
//  @param      NULL
//  @return     void                    
//  @since      v1.0
//  Sample usage:       �ڲ����ã������û����á�
//-------------------------------------------------------------------------------------------------------------------
void scc8660_cof_uart_init(void)
{
    //��ʼ������ ��������ͷ
    uart_init (SCC8660_COF_UART, 9600,SCC8660_COF_UART_TX,SCC8660_COF_UART_RX);      
    uart_rx_irq(SCC8660_COF_UART,1);
    //���ô��ڽ��յĻ�����������������
    scc8660_receivexfer.dataSize = 1;
    scc8660_receivexfer.data = &scc8660_rx_buffer;
    //���ô����ж�
    uart_set_handle(SCC8660_COF_UART, &scc8660_g_lpuartHandle, scc8660_uart_callback, NULL, 0, scc8660_receivexfer.data, 1);
    
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCC8660(��ͫ����ͷ)��ʼ��
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	ʹ��FLEXIO�ӿڲɼ�����ͷ	
//-------------------------------------------------------------------------------------------------------------------
void scc8660_init(void)
{
    flexio_camera_type = CAMERA_COLOR;//����flexio�ӿ���������ͷ����
    flexio_color_camera_buffer_addr1 = scc8660_flexio1_image;
    flexio_color_camera_buffer_addr2 = scc8660_flexio2_image;
    //����ͷ��ʼ��ʼ��֮ǰ��ؽ����ź�����
    gpio_init(SCC8660_VSYNC_PIN,GPO,1,GPIO_PIN_CONFIG);
    
    //��ʼ������ͷ���ô���
    scc8660_cof_uart_init();
	//�����ж�
	EnableGlobalIRQ(0);
    //�ȴ�����ͷ�ϵ��ʼ���ɹ� ��ʽ�����֣���ʱ����ͨ����ȡ���õķ�ʽ ��ѡһ
    //systick_delay_ms(500);//��ʱ��ʽ
    scc8660_get_all_config(SCC8660_COF_UART,SCC8660_GET_CFG);//��ȡ���õķ�ʽ
    
    scc8660_uart_receive_flag = 0;
    //������ͷ����������Ϣ
    scc8660_set_all_config(SCC8660_COF_UART,SCC8660_CFG);
    //��ȡ���ñ��ڲ鿴�����Ƿ���ȷ
    scc8660_get_all_config(SCC8660_COF_UART,SCC8660_GET_CFG);
    //Ϊ����ʹ�ã���ȡ������Ϣ��δ�����ݽ���У�飬�����Ҫȷ�������Ƿ�ɹ��������н������ݱȶԡ�
	//�����ж�
    DisableGlobalIRQ();

    scc8660_image = scc8660_flexio2_image;//����˫�����׵�ַ
    
    dma_mux_init();
    flexio_camera(SCC8660_DATA_PIN, SCC8660_PCLK_PIN, SCC8660_HREF_PIN, SCC8660_W, SCC8660_H);
    flexio_dma_init((uint8 *)scc8660_image[0], SCC8660_W*SCC8660_H, scc8660_dma);
    flexio_enable_rxdma();
    NVIC_SetPriority(DMA0_DMA16_IRQn,1);            //����DMA�ж����ȼ� ��Χ0-15 ԽС���ȼ�Խ��
    
    //���ó��ж�
    gpio_interrupt_init(SCC8660_VSYNC_PIN, FALLING, GPIO_INT_CONFIG);
    NVIC_SetPriority(SCC8660_VSYNC_IRQN,0);         //���ó��ж����ȼ� ��Χ0-15 ԽС���ȼ�Խ��
	//EnableGlobalIRQ(0);
}


uint8 scc8660_finish_flag;       //һ��ͼ��ɼ���ɱ�־λ
//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCC8660(��ͫ����ͷ)���ж�
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:       ��isr.c�����ȴ�����Ӧ���жϺ�����Ȼ����øú���(֮�����������жϱ�־λ)
//-------------------------------------------------------------------------------------------------------------------
void scc8660_vsync(void)
{
    CLEAR_GPIO_FLAG(SCC8660_VSYNC_PIN);
    
    if(scc8660_image==flexio_color_camera_buffer_addr1)
    {
        dma_restart((uint8 *)flexio_color_camera_buffer_addr2[0]);
    }
    else if(scc8660_image==flexio_color_camera_buffer_addr2)
    {
        dma_restart((uint8 *)flexio_color_camera_buffer_addr1[0]);
    }
    

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      SCC8660(��ͫ����ͷ)DMA����ж�
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void scc8660_dma(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    if(scc8660_image==flexio_color_camera_buffer_addr1)
    {
        scc8660_image = flexio_color_camera_buffer_addr2;
    }
    else if(scc8660_image==flexio_color_camera_buffer_addr2)
    {
        scc8660_image = flexio_color_camera_buffer_addr1;
    }
    L1CACHE_CleanInvalidateDCacheByRange((uint32)scc8660_image[0],SCC8660_W*SCC8660_H);//������ݴ����TCM����Բ���Ҫ��仰
	scc8660_finish_flag = 1;//һ��ͼ��Ӳɼ���ʼ���ɼ�������ʱ18MS����(50FPS)
}
