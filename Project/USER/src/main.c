#include "headfile.h"

void main()
{
	

	rt_thread_mdelay(150);					// 延时150ms，等待主板其他外设上电成功
	gpio_init(B9, GPO, 1, GPIO_PIN_CONFIG); // 初始化B9灯

	arm_init();

	display_init(); // 显示屏初始化	
	
	
	buzzer_init(); // 初始化蜂鸣器
	button_init(); // 初始化按键

	ART1_UART_Init();
	ART2_UART_Init();
	ART3_UART_Init();

	encoder_init();	  // 初始化编码器
	icm20602_init_spi();  // 初始化陀螺仪
	icm_zeroBias();
	timer_pit_init(); // 周期定时器
	


	motor_init();

	car_start_init();
//	rt_sem_release(obj_detection_sem);
	
//	uart_putchar(USART_4, A); // 发送OPENART1告诉该识别A4纸了
//	
//	
//	
//	car.MileageY = -20;
//	car.MileageX = 10;
//	
//				  ART1_mode = 3;
//        uart_putchar(USART_4, 0x43);
//        rt_thread_mdelay(1000);


	while (1)
	{
		gpio_toggle(B9);
		rt_thread_mdelay(500);
	}
}
