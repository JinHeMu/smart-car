#include "headfile.h"


void main()
{

	rt_thread_mdelay(8000);					// ��ʱ300ms���ȴ��������������ϵ�ɹ�
	gpio_init(B9, GPO, 1, GPIO_PIN_CONFIG); // ��ʼ��B9��
	arm_init();
	
	display_init(); // ��ʾ����ʼ��

//	
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
	//arm_carry();

	
	



		while(1)
		{
//			car_move(200, 0);
//			rt_thread_mdelay(3000);
//			car_move(200, 200);
//			rt_thread_mdelay(3000);
//			car_move(0, 200);
//			rt_thread_mdelay(3000);
//			car_move(0, 0);
//			rt_thread_mdelay(3000);
//			car.current_angle = 90;
//			rt_thread_mdelay(3000);
//			car.current_angle = 180;
//			rt_thread_mdelay(3000);
//			car.current_angle = 270;
//			rt_thread_mdelay(3000);
//			car.current_angle = 0;
//			car.current_angle = 20;
//			rt_thread_mdelay(3000);
//						car.current_angle = 40;
//			rt_thread_mdelay(3000);
//						car.current_angle = 220;
//			rt_thread_mdelay(3000);
//						car.current_angle = 0;
//			rt_thread_mdelay(3000);
//						car.current_angle = -90;
			rt_thread_mdelay(3000);
			
		}			
}
