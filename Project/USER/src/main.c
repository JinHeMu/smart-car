#include "headfile.h"


void main()
{

	rt_thread_mdelay(7000);					// 延时300ms，等待主板其他外设上电成功
	gpio_init(B9, GPO, 1, GPIO_PIN_CONFIG); // 初始化B9灯
	arm_init();
	
	display_init(); // 显示屏初始化

	ART1_UART_Init();
	ART1_mode = 1;
	ART2_UART_Init();


	encoder_init();	  // 初始化编码器
	imu963ra_init();  // 初始化陀螺仪
	timer_pit_init(); // 周期定时器

	buzzer_init();	//初始化蜂鸣器
	button_init();	//初始化按键

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
