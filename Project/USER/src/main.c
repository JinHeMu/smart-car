#include "headfile.h"


void main()
{

	systick_delay_ms(300);					// 延时300ms，等待主板其他外设上电成功
	gpio_init(B9, GPO, 1, GPIO_PIN_CONFIG); // 初始化B9灯

	display_init(); // 显示屏初始化

//		mt9v03x_csi_init();	//初始化总钻风摄像头
//		systick_delay_ms(300);

	ART1_UART_Init();

//	bluetooth_ch9141_init(); // 初始化蓝牙串口

	encoder_init();	  // 初始化编码器
	imu963ra_init();  // 初始化陀螺仪
	timer_pit_init(); // 周期定时器

	buzzer_init();//初始化蜂鸣器
	button_init();//初始化按键

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
