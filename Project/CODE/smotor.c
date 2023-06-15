#include "smotor.h"

//定义机械臂的PWM IO口
#define ARM_UP_PIN PWM1_MODULE3_CHA_B10
#define ARM_LOW_PIN PWM4_MODULE2_CHA_C30
#define ARM_LEFT_PIN PWM2_MODULE0_CHB_C7
#define ARM_MID_PIN PWM2_MODULE1_CHA_C8

#define ARM_CENTER (1.5 * 50000 / 20)

//定义电磁铁控制IO口
#define MAGNET_PIN1 C30
#define MAGNET_PIN2 C6

//电磁铁释放
void magnet1_release(void)
{
	gpio_set(MAGNET_PIN1, 0);
}
void magnet2_release(void)
{
	gpio_set(MAGNET_PIN2, 0);
}

//电磁贴吸取
void magnet1_appeal(void)
{
	gpio_set(MAGNET_PIN1, 1);
}
void magnet2_appeal(void)
{
	gpio_set(MAGNET_PIN2, 1);
}

//设置机械臂角度
void ARM_UP_angle(int angle)
{
	pwm_duty(ARM_UP_PIN, 1000 + angle * 30);
}

void ARM_LOW_angle(int angle)
{
	pwm_duty(ARM_LOW_PIN, 1100 + +angle * 14);
}

void ARM_LEFT_angle(int angle)
{
	pwm_duty(ARM_LEFT_PIN, 1000 + angle * 30);
}

void ARM_MID_angle(int angle)
{
	pwm_duty(ARM_LOW_PIN, 1100 + +angle * 14);
}

//机械臂抬起
void arm_up(void)
{
	ARM_LOW_angle(100);
	ARM_UP_angle(120);
}

//机械臂放下
void arm_down(void)
{
	ARM_LOW_angle(30);
	rt_thread_mdelay(300);
	ARM_UP_angle(0);
}


void arm_carry(void)
{
	ARM_UP_angle(0);
	rt_thread_mdelay(500);
	ARM_LOW_angle(30);
}
void arm_pick(void)
{
	ARM_LOW_angle(30);
	rt_thread_mdelay(300);
	ARM_UP_angle(0);
	rt_thread_mdelay(1000);
	ARM_LOW_angle(100);
	ARM_UP_angle(120);
}

//将卡片放入盒子
void arm_box(void)
{

}

void arm_init(void)
{
	pwm_init(ARM_UP_PIN, 50, ARM_CENTER);
	pwm_init(ARM_LOW_PIN, 50, ARM_CENTER);
	//	  pwm_init(ARM_LEFT_PIN, 50, ARM_CENTER);
	//		pwm_init(ARM_MID_PIN, 50, ARM_CENTER);

	gpio_init(MAGNET_PIN1, GPO, 0, GPIO_PIN_CONFIG);
	gpio_init(MAGNET_PIN2, GPO, 0, GPIO_PIN_CONFIG);

	ARM_UP_angle(90);
	ARM_LOW_angle(90);	
	//		ARM_LEFT_angle(80);
	//		ARM_MID_angle(120);
	rt_thread_mdelay(500);


	
}
