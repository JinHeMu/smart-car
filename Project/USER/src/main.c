#include "headfile.h"

void main()
{
	

	rt_thread_mdelay(800);					// ��ʱ150ms���ȴ��������������ϵ�ɹ�
	gpio_init(B9, GPO, 1, GPIO_PIN_CONFIG); // ��ʼ��B9��

	arm_init();

	display_init(); // ��ʾ����ʼ��

	ART1_UART_Init();
	ART2_UART_Init();

	encoder_init();	  // ��ʼ��������
	imu963ra_init();  // ��ʼ��������
	timer_pit_init(); // ���ڶ�ʱ��
	
	rt_thread_mdelay(200);

	buzzer_init(); // ��ʼ��������
	button_init(); // ��ʼ������

	motor_init();

	

	car_start_init();

	car.MileageX = 0; car.MileageY = 0;
	ART1_mode = 1;
	ART2_mode = 1;
	
//	ART1_mode = 4;
//  uart_putchar(USART_4, 0x45); // ����OPENART1���߸�ʶ�������

	while (1)
	{
		gpio_toggle(B9);
		rt_thread_mdelay(500);
//		car.Speed_Y = 100;

	}
}
