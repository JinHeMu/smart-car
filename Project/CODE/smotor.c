#include "smotor.h"

#define SMOTOR1_PIN   PWM4_MODULE2_CHA_C30       //定义车模寻迹舵机引脚
//#define SMOTOR2_PIN   PWM1_MODULE0_CHA_D12       //定义云台舵机1引脚
//#define SMOTOR3_PIN   PWM1_MODULE0_CHB_D13       //定义云台舵机2引脚

#define SMOTOR1_CENTER  (1.5*50000/20)
//#define SMOTOR2_CENTER  (1.5*50000/20)
//#define SMOTOR3_CENTER  (1.5*50000/20)
#define Maxduty            (6000)

#define MAGNET_PIN B10

// 死区 duty1000
// duty 4000 - 90° 每3000duty - 90° 1°= 33duty



void magnet_release(void)
{
	gpio_set(MAGNET_PIN, 0);
	
}

void magnet_appeal(void)
{
	gpio_set(MAGNET_PIN, 1);
	
}

void arm_init(void)
{		
    pwm_init(SMOTOR1_PIN, 50, SMOTOR1_CENTER);
		gpio_init(MAGNET_PIN, GPO, 0, GPIO_PIN_CONFIG);      //单片机端口D0 初始化DIR_1          GPIO
		smotor1_angle(90);
		rt_thread_mdelay(500);
//    pwm_init(SMOTOR2_PIN, 50, 0);
//    pwm_init(SMOTOR3_PIN, 50, 0);
}

 void smotor1_angle(int angle)
 {
     pwm_duty(SMOTOR1_PIN, 1100 + angle*33);
 }

 void arm_carry(void)
 {
	
	smotor1_angle(0);
	magnet_appeal();
	rt_thread_mdelay(1000);
	smotor1_angle(90);
	rt_thread_mdelay(1000);
 }


 void arm_down(void)
 {
	smotor1_angle(0);
	rt_thread_mdelay(1000);
	magnet_release();
	smotor1_angle(90);
	rt_thread_mdelay(1000);
 }

 void arm_box(void)
 {

	
 }

//void smotor2_control(int16 angle)
//{
//    pwm_duty(SMOTOR2_PIN, Maxduty/180*angle+777);
//}

//void smotor3_control(int16 angle)
//{
//    pwm_duty(SMOTOR3_PIN, Maxduty/180*angle+777);
//}