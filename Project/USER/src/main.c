#include "headfile.h"


void main()
{	
////		speed_tar= 2000;
		systick_delay_ms(300);	//延时300ms，等待主板其他外设上电成功
    gpio_init(B9, GPO, 1, GPIO_PIN_CONFIG);

//		display_init();
//		mt9v03x_csi_init();	
		systick_delay_ms(300);
//	
		openart_init();
		bluetooth_ch9141_init();

//	
//		encoder_init();		
//		imu963ra_init();
//		timer_pit_init();



//    buzzer_init();
//    button_init();


//		motor_init();
//		smotor_init();	引脚冲突


	while(1)
	{
		
		gpio_toggle(B9);		
//    image_threshold = otsuThreshold(mt9v03x_csi_image[0], MT9V03X_CSI_W, MT9V03X_CSI_H);
		
		motor_control(1);
		    //角度环测试
		rt_thread_mdelay(500);
		
		
	}
}








