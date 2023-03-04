#include "openart.h"


uint8               openart_rx_buffer;
lpuart_transfer_t   openart_receivexfer;
lpuart_handle_t     openart_g_lpuartHandle;


uint8 point[64];//前一半x后一半y
uint8 point_x[32];
uint8 point_y[32];
uint8 recive_flag = 0;
int i = 0; //数据个数












void openart_uart4_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
		static uint8 rxstate = 0;
		
    if(kStatus_LPUART_RxIdle == status)
    {
			if (rxstate == 0) 
			{
				if(openart_rx_buffer == 99)// 接受到帧头
				{
					rxstate = 1;
				}
			}else if (rxstate == 1)// 接受数据
			{
				if(openart_rx_buffer != 111)// 
				{
					point[i] = openart_rx_buffer;
					rt_kprintf("%d\n", point[i]);
					i++; //统计接到数据的个数  前二分之一为x坐标， 后二分之一为y坐标
				}else
				{
					rxstate = 2;
				}
			
			}else if (rxstate == 2)
			{
				rxstate = 0;
				recive_flag = 1; //接受标志位，说明已经接受完毕
				static int a;
				for(a = 0; a <= i/2; a++)
				{
					point_x[a] = point[a];
					rt_kprintf("%d\n", point_x[a]);
				}
				
				for(a = i/2; a <= i; a++ )
				{
					point_y[a] = point[a];
					rt_kprintf("%d\n", point_y[a]);
				}

			}	
			
    }
    
    handle->rxDataSize = openart_receivexfer.dataSize;  //还原缓冲区长度
    handle->rxData = openart_receivexfer.data;          //还原缓冲区地址
}



void openart_init(void)
{
    uart_init(USART_4, 115200, UART4_TX_C16, UART4_RX_C17);
    
    //配置串口接收的缓冲区及缓冲区长度
    openart_receivexfer.dataSize = 1;
    openart_receivexfer.data = &openart_rx_buffer;
    
    //设置中断函数及其参数
    uart_set_handle(USART_4, &openart_g_lpuartHandle, openart_uart4_callback, NULL, 0, openart_receivexfer.data, 1);
    
    NVIC_SetPriority(LPUART4_IRQn, 0);         //设置串口中断优先级 范围0-15 越小优先级越高
    uart_rx_irq(USART_4, 1);
}