#include "openart1.h"

// �����շ�����ĸ�ĺ���
// ��'G' ART1��ʼʶ��
// ��'A' ART1����֡ͷ
// ��'Y' ART1����֡β
// ��'I' ART1ͼƬ����֡ͷ
// ��'J' ART1ͼƬ����֡β
// ��'C' ART1�����Ƕ�֡ͷ
// ��'D' ART1�����Ƕ�֡β Ҳ��ʶ��ɹ���־λ



uint8 ART1_uart_rx_buffer;
lpuart_transfer_t ART1_receivexfer;
lpuart_handle_t ART1_g_lpuartHandle;

uint8 ART1_mode=0;//ģʽ1λʶ������� ģʽ2����λ�� ģʽ3ʶ��ͼƬ ģʽ4���߽���
uint8 ART1_dat[82]; // ǰһ��x��һ��y
uint8 ART1_POINT_X[40];
uint8 ART1_POINT_Y[40];

int8 ART1_CORRECT_X;
int8 ART1_CORRECT_Y;
int8 ART1_CORRECT_Angle;
int8 ART1_CORRECT_Boundary_Angle;

char classified[10];

uint8 point_num = 0; // ���ݸ���

rt_sem_t uart_corrdinate_sem;//ʶ���������ź���
//rt_sem_t key2_sem;
//rt_sem_t key3_sem;
//rt_sem_t key4_sem;



void ART1_uart_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{

	static uint8 rxstate = 0;
	static uint8 recive_flag = 0;
	static uint8 ART1_POINT_i,ART1_POINT_j=0;
	static uint8 count = 0;	
	
	memset(classified, 0, sizeof(classified));
	ART1_CORRECT_X = 0;  
	ART1_CORRECT_Y = 0;

	if (kStatus_LPUART_RxIdle == status)
	{
		if(ART1_mode==1)
		{
			if(rxstate == 0)
			{
				if (ART1_uart_rx_buffer == 'A') // ���ܵ�֡ͷ
				{
					rxstate = 1;
					point_num = 0;
				}
			}
			else if(rxstate == 1)
			{
				if(ART1_uart_rx_buffer == 'Y')//���յ�֡β
				{
					for (int temp = 0; temp<point_num; temp++)
					{
						if(temp < point_num/2)
						{
							ART1_POINT_X[temp] = ART1_dat[temp];
						}else
						{
							ART1_POINT_Y[temp-point_num/2] = ART1_dat[temp];
						}			 
					}
					rt_sem_release(uart_corrdinate_sem);//�����Ѿ�ʶ���������ֽ���ź���
					rxstate = 0;
				}
				else//û�н��յ�֡β����ȡ�����
				{
					ART1_dat[point_num] = ART1_uart_rx_buffer;
					point_num++; // ͳ�ƽӵ����ݵĸ���  ǰ����֮һΪx���꣬ �����֮һΪy����
				}
			}
		}else if (ART1_mode == 2)
		{
			if(rxstate == 0)
			{
				if (ART1_uart_rx_buffer == 'C') // ���ܵ�֡ͷ
				{
					rxstate = 1;
					count = 0;
				}
			}
			else if(rxstate == 1)
			{
				if(ART1_uart_rx_buffer == 'D')//���յ�֡β
				{
					
					ART1_CORRECT_X = ART1_dat[0];
					ART1_CORRECT_Y = ART1_dat[1];
					ART1_CORRECT_Angle = ART1_dat[2];
//					rt_kprintf("x:%d", ART1_CORRECT_X);
//					rt_kprintf("y:%d\n", ART1_CORRECT_Y);
//					rt_kprintf("Angle:%d\n", ART1_CORRECT_Angle);

					rxstate = 0;
				}
				else//û�н��յ�֡β����ȡ�����
				{
					ART1_dat[count] = ART1_uart_rx_buffer;
					count++; // ͳ�ƽӵ����ݵĸ���  ǰ����֮һΪx���꣬ �����֮һΪy����
				}
			}
		}else if (ART1_mode == 3)
		{
			if(rxstate == 0)
			{
				if (ART1_uart_rx_buffer == 'I') // ���ܵ�֡ͷ
				{
					rxstate = 1;
					count = 0;
				}
			}
			else if(rxstate == 1)
			{
				if(ART1_uart_rx_buffer == 'J')//���յ�֡β
				{
					for (int temp = 0; temp<count; temp++)
					{
						classified[temp] = ART1_dat[temp];
					}
					
					//rt_sem_release(recognize_sem);//ʶ��Ƭ����ʶ���ź���
					rxstate = 0;
				}
				else//û�н��յ�֡β����ȡ�����
				{
					ART1_dat[count] = ART1_uart_rx_buffer;
					count++; // ͳ�ƽӵ����ݵĸ���  ǰ����֮һΪx���꣬ �����֮һΪy����
				}
			}
		}else if (ART1_mode == 4)
		{
			
			ART1_CORRECT_Boundary_Angle = ART1_uart_rx_buffer;
			rt_kprintf("Angle: %d\n", ART1_CORRECT_Boundary_Angle);


		}
	handle->rxDataSize = ART1_receivexfer.dataSize; // ��ԭ����������
	handle->rxData = ART1_receivexfer.data;			// ��ԭ��������ַ
	}
}

void ART1_UART_Init(void)
{
	uart_init(USART_4, 115200, UART4_TX_C16, UART4_RX_C17);
	NVIC_SetPriority(LPUART4_IRQn, 0); // ���ô����ж����ȼ� ��Χ0-15 ԽС���ȼ�Խ��
	uart_rx_irq(USART_4, 1);
	uart_tx_irq(USART_4, 1);
	// ���ô��ڽ��յĻ�����������������
	ART1_receivexfer.dataSize = 1;
	ART1_receivexfer.data = &ART1_uart_rx_buffer;
	// �����жϺ����������
	uart_set_handle(USART_4, &ART1_g_lpuartHandle, ART1_uart_callback, NULL, 0, ART1_receivexfer.data, 1);
}