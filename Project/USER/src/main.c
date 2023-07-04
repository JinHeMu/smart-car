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
	uart_putchar(USART_4, 0x41); // 发送OPENART1告诉该识别A4纸了
	
//	
	rt_sem_release(obj_detection_sem);

	while (1)
	{
		gpio_toggle(B9);
		rt_thread_mdelay(500);
	}
}
