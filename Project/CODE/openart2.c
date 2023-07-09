#include "openart2.h"

uint8 ART2_uart_rx_buffer;
lpuart_transfer_t ART2_receivexfer;
lpuart_handle_t ART2_g_lpuartHandle;

int8 ART2_center_x = 0;
int8 ART2_center_y = 0;
int8 ART2_angle = 0;
int8 ART2_dat[10];
uint8 ART2_mode = 0;
uint8 detect_flag = 0;
uint8 detect_arrive_flag = 0;

void ART2_uart_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{

	static uint8 rxstate = 0;
	static uint8 count = 0;
	
	if (kStatus_LPUART_RxIdle == status)
	{
		if (ART2_mode && car.MileageX > 40 && car.MileageX < 310)
		{
			if (rxstate == 0)
			{
				if (ART2_uart_rx_buffer == 'C') // 接受到帧头
				{
					rxstate = 1;
					count = 0;
				}
			}
			else if (rxstate == 1)
			{
				if (ART2_uart_rx_buffer == 'D') // 接收到帧尾
				{

					ART2_center_x = ART2_dat[0];
					ART2_center_y = ART2_dat[1];
					detect_arrive_flag = ART2_dat[2];//是否到达点位
					detect_flag = 1;
					rt_kprintf("x:%d, y:%d,flag:%d\n", ART2_center_x, ART2_center_y, detect_arrive_flag);
					
					rxstate = 0;
				}
				else // 没有接收到帧尾
				{
					ART2_dat[count] = ART2_uart_rx_buffer;
					count++;
				}
			}
		}

		handle->rxDataSize = ART2_receivexfer.dataSize; // 还原缓冲区长度
		handle->rxData = ART2_receivexfer.data;			// 还原缓冲区地址
	}
}

void ART2_UART_Init(void)
{
	uart_init(USART_1, 115200, UART1_TX_B12, UART1_RX_B13);
	NVIC_SetPriority(LPUART1_IRQn, 1); // 设置串口中断优先级 范围0-15 越小优先级越高
	uart_rx_irq(USART_1, 1);
	uart_tx_irq(USART_1, 1);
	// 配置串口接收的缓冲区及缓冲区长度
	ART2_receivexfer.dataSize = 1;
	ART2_receivexfer.data = &ART2_uart_rx_buffer;
	// 设置中断函数及其参数
	uart_set_handle(USART_1, &ART2_g_lpuartHandle, ART2_uart_callback, NULL, 0, ART2_receivexfer.data, 1);
}