#include "headfile.h"


void main()
{

	systick_delay_ms(300);					// ��ʱ300ms���ȴ��������������ϵ�ɹ�
	gpio_init(B9, GPO, 1, GPIO_PIN_CONFIG); // ��ʼ��B9��

	display_init(); // ��ʾ����ʼ��

//		mt9v03x_csi_init();	//��ʼ�����������ͷ
//		systick_delay_ms(300);

	ART1_UART_Init();

//	bluetooth_ch9141_init(); // ��ʼ����������

	encoder_init();	  // ��ʼ��������
	imu963ra_init();  // ��ʼ��������
	timer_pit_init(); // ���ڶ�ʱ��

	buzzer_init();//��ʼ��������
	button_init();//��ʼ������

	motor_init();
	route_planning_init();

	arm_init();

	while (1)
	{
		gpio_toggle(B9);		
		rt_thread_mdelay(500);
		motor_control(1);
	}
}
