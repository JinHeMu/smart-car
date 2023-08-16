#include "openart3.h"

uint8 ART3_uart_rx_buffer;
lpuart_transfer_t ART3_receivexfer;
lpuart_handle_t ART3_g_lpuartHandle;


int8 ART3_dat[10];
uint8 ART3_mode = 1;//模式1为目标检测模式

uint8	ART3_DETECT_DISTANCE = 0;//记录卡片距离
uint8	ART3_DETECT_Flag = 0;//记录是否发现卡片

uint16 Last_MileageX = 0;
uint16 Last_Distance = 0;


void ART3_uart_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{

	static uint8 rxstate = 0;
	static uint8 count = 0;
	
	
	
	if (kStatus_LPUART_RxIdle == status)
	{
//		rt_kprintf("%d\n", ART3_uart_rx_buffer);
//		if (ART3_mode == 1 && car.MileageX > 0 && car.MileageX < 0)
		if (ART3_mode == 1)
		{
			if (rxstate == 0)
			{
				if (ART3_uart_rx_buffer == 'C') // 接受到帧头
				{
					rxstate = 1;
					count = 0;
				}
			}
			else if (rxstate == 1)
			{
				if (ART3_uart_rx_buffer == 'D') // 接收到帧尾
				{

					ART3_DETECT_DISTANCE = (int)ART3_dat[0];
					ART3_DETECT_Flag = ART3_dat[1];
					

					if(ART3_DETECT_Flag)
					{
						//防止原地多次发送误判
						if(abs(Last_MileageX - (int)car.MileageX) > 40)
						{

							Last_MileageX = (int)car.MileageX;
							Last_Distance = ART3_DETECT_DISTANCE;		
							ART3_DETECT_Flag = 0;
							
							rt_sem_release(obj_detection_sem);



						}else if (abs(Last_MileageX - (int)car.MileageX) <= 40 && abs(ART3_DETECT_DISTANCE - Last_Distance) >= 20)
						{
							
							Last_Distance = ART3_DETECT_DISTANCE;
							Last_MileageX = (int)car.MileageX;
							ART3_DETECT_Flag = 0;
							
							rt_sem_release(obj_detection_sem);
							

						}

					}
					
					rxstate = 0;
				}
				else // 没有接收到帧尾
				{
					ART3_dat[count] = ART3_uart_rx_buffer;
					count++;
				}
			}
		}
		

		handle->rxDataSize = ART3_receivexfer.dataSize; // 还原缓冲区长度
		handle->rxData = ART3_receivexfer.data;			// 还原缓冲区地址
	}
}

void ART3_UART_Init(void)
{
	uart_init(USART_8, 115200, UART8_TX_D16, UART8_RX_D17);
	NVIC_SetPriority(LPUART1_IRQn, 1); // 设置串口中断优先级 范围0-15 越小优先级越高
	uart_rx_irq(USART_8, 1);
	uart_tx_irq(USART_8, 1);
	// 配置串口接收的缓冲区及缓冲区长度
	ART3_receivexfer.dataSize = 1;
	ART3_receivexfer.data = &ART3_uart_rx_buffer;
	// 设置中断函数及其参数
	uart_set_handle(USART_8, &ART3_g_lpuartHandle, ART3_uart_callback, NULL, 0, ART3_receivexfer.data, 1);
}