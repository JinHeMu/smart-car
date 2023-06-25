#include "headfile.h"

void main()
{
	

	rt_thread_mdelay(800);					// 延时150ms，等待主板其他外设上电成功
	gpio_init(B9, GPO, 1, GPIO_PIN_CONFIG); // 初始化B9灯

	arm_init();

	display_init(); // 显示屏初始化

	ART1_UART_Init();
	ART2_UART_Init();

	encoder_init();	  // 初始化编码器
	imu963ra_init();  // 初始化陀螺仪
	timer_pit_init(); // 周期定时器

	buzzer_init(); // 初始化蜂鸣器
	button_init(); // 初始化按键

	motor_init();

	

	car_start_init();

	car.MileageX = 0; car.MileageY = -60;
	ART1_mode = 1;

	while (1)
	{
////		gpio_toggle(B9);
////		rt_thread_mdelay(500);
//		car.Speed_X = 150; 
//		rt_thread_mdelay(3000);
//		car.Speed_X = 0;
//		rt_thread_mdelay(3000);
//		car.Speed_Y = 150; 
//		rt_thread_mdelay(3000);
//		car.Speed_Y = 0;
//		rt_thread_mdelay(3000);
//		car.Speed_X = -150; 
//		rt_thread_mdelay(3000);
//		car.Speed_X = 0;
//		rt_thread_mdelay(3000);
//		car.Speed_Y = -150; 
//		rt_thread_mdelay(3000);
//		car.Speed_Y = 0;
		rt_thread_mdelay(3000);
	}
}
