#include "headfile.h"

void main()
{
	

	rt_thread_mdelay(8000);					// 延时150ms，等待主板其他外设上电成功
	gpio_init(B9, GPO, 1, GPIO_PIN_CONFIG); // 初始化B9灯

	arm_init();

	display_init(); // 显示屏初始化

	ART1_UART_Init();
	ART2_UART_Init();

	encoder_init();	  // 初始化编码器
	icm20602_init_spi();  // 初始化陀螺仪
	timer_pit_init(); // 周期定时器
	

	buzzer_init(); // 初始化蜂鸣器
	button_init(); // 初始化按键

	motor_init();

	

	car_start_init();

	car.MileageX = 0; car.MileageY = 0;
//	rt_sem_release(obj_detection_sem);
//	ART1_mode = 1;
//	ART2_mode = 1;
	
//	                arm_carry();
//                car_moveto_boundry(-3, 0);
//                arm_down();
//	
//	ART1_mode = 4;
//  uart_putchar(USART_4, 0x45); // 发送OPENART1告诉该识别边线了
	

	while (1)
	{
		gpio_toggle(B9);
		rt_thread_mdelay(500);

//		uart_putchar(USART_4, 0x43); // 发送OPENART1告诉该识别边线了
//		rt_thread_mdelay(3000);
//		uart_putchar(USART_4, 0x44); // 发送OPENART1告诉该识别边线了
//		rt_thread_mdelay(3000);
//		car.Speed_Y = 100;

	}
}
