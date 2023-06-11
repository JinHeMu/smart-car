#include "headfile.h"


void main()
{

	rt_thread_mdelay(7000);					// ��ʱ300ms���ȴ��������������ϵ�ɹ�
	gpio_init(B9, GPO, 1, GPIO_PIN_CONFIG); // ��ʼ��B9��
	arm_init();
	
	display_init(); // ��ʾ����ʼ��

	ART1_UART_Init();
	ART1_mode = 1;
	ART2_UART_Init();


	encoder_init();	  // ��ʼ��������
	imu963ra_init();  // ��ʼ��������
	timer_pit_init(); // ���ڶ�ʱ��

	buzzer_init();	//��ʼ��������
	button_init();	//��ʼ������

	motor_init();

	car_start_init();

	
	
	
	
	
	
	while (1)
	{
		gpio_toggle(B9);		
		rt_thread_mdelay(500);
//		rt_kprintf("woshishabi\n");
//		car_move(0,200);
	}
}
