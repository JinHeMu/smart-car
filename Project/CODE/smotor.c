#include "smotor.h"


/* BOX 
1 - 0°
2 - 90°
3 - 180°
4 - 270°*/


//定义机械臂的PWM IO口
#define ARM_UP_PIN PWM1_MODULE3_CHA_B10
#define ARM_LOW_PIN PWM2_MODULE1_CHA_C8
#define ARM_LEFT_PIN PWM2_MODULE0_CHB_C7
#define ARM_MID_PIN PWM4_MODULE2_CHA_C30

#define ARM_CENTER (1.5 * 50000 / 20)

//定义电磁铁控制IO口
#define MAGNET_FRONT C5
#define MAGNET_LEFT C6


//电磁铁释放
void magnet_front_release(void)
{
	gpio_set(MAGNET_FRONT, 0);
}
void magnet_left_release(void)
{
	gpio_set(MAGNET_LEFT, 0);
}

//电磁贴吸取
void  magnet_front_appeal(void)
{
	gpio_set(MAGNET_FRONT, 1);
}
void magnet_left_appeal(void)
{
	gpio_set(MAGNET_LEFT, 1);
}

//设置机械臂角度
void ARM_UP_angle(int angle)
{
	pwm_duty(ARM_UP_PIN, 1000 + angle * 30);
}

void ARM_LOW_angle(int angle)
{
	pwm_duty(ARM_LOW_PIN, 1000 + angle * 30);
}

void ARM_LEFT_angle(int angle)
{
	pwm_duty(ARM_LEFT_PIN, 1000 + angle * 30);
}

void ARM_MID_angle(int angle)
{
	pwm_duty(ARM_MID_PIN, 1200 + +angle * 14);
}


/*
机械臂分为三种情况，或者是两种运动情况
1.放入盒子-先转动仓库，放入卡片

2.不放入盒子- 捡起并抬起，跑点，放下，抬起

注意机械臂抬起角度防止仓库碰到
*/


//机械臂将卡片放下后并抬起
void arm_down(void)
{	
	
	ARM_UP_angle(0);
	rt_thread_mdelay(300);
	ARM_LOW_angle(60);
	rt_thread_mdelay(300);

	magnet_front_release();
	rt_thread_mdelay(300);

	ARM_LOW_angle(80);
	ARM_UP_angle(150);

}

//机械臂将卡片捡起来
void arm_carry(void)
{
	ARM_UP_angle(90);
	rt_thread_mdelay(300);
	ARM_LOW_angle(43);
	rt_thread_mdelay(300);
	ARM_UP_angle(5);
	rt_thread_mdelay(300);

	magnet_front_appeal();
	rt_thread_mdelay(500);
	
	
	ARM_LOW_angle(60);
	rt_thread_mdelay(300);
	ARM_UP_angle(150);
	ARM_LOW_angle(90);
	rt_thread_mdelay(300);
	

}


//将卡片放入盒子
void arm_putbox(uint8 angle)
{
	//机械臂启动
	//盒子转动 1-0；2-90 3-180 4-270
	ARM_LOW_angle(80);
	ARM_UP_angle(180);
	rt_thread_mdelay(300);

	
	ARM_MID_angle(angle*90 - 90);
	rt_thread_mdelay(500);
	

	ARM_UP_angle(90);
	rt_thread_mdelay(300);
	ARM_LOW_angle(43);
	rt_thread_mdelay(300);
	ARM_UP_angle(5);
	rt_thread_mdelay(300);

	magnet_front_appeal();
	rt_thread_mdelay(500);
	
	
	ARM_LOW_angle(60);
	rt_thread_mdelay(300);
	ARM_UP_angle(180);
	rt_thread_mdelay(300);
	ARM_LOW_angle(90);
	rt_thread_mdelay(300);
	
	magnet_front_release();
	rt_thread_mdelay(300);
	
	ARM_LOW_angle(80);
	rt_thread_mdelay(300);
	
	
}

//机械臂打开盒子
void arm_openbox(uint8 angle)
{
switch (angle)
{
	case 1:	ARM_MID_angle(270);rt_thread_mdelay(1500);break;
	case 2:	ARM_MID_angle(0);rt_thread_mdelay(1500);break;
	case 3:	ARM_MID_angle(90);rt_thread_mdelay(1500);break;
	case 4:	ARM_MID_angle(180);rt_thread_mdelay(1500);break;
}

	ARM_LEFT_angle(90);
	rt_thread_mdelay(300);

	magnet_left_appeal();
	rt_thread_mdelay(300);

	ARM_LEFT_angle(0);
	rt_thread_mdelay(1000);

	ARM_LEFT_angle(90);
	rt_thread_mdelay(300);

	magnet_left_release();

	ARM_LEFT_angle(0);
	rt_thread_mdelay(300);
	

}

//机械臂合上盒子
void arm_closebox(void)
{
	ARM_LEFT_angle(90);
	rt_thread_mdelay(300);

	magnet_left_release();
	
	rt_thread_mdelay(300);
	ARM_LEFT_angle(0);

}

void arm_init(void)
{
	pwm_init(ARM_UP_PIN, 50, ARM_CENTER);
	pwm_init(ARM_LOW_PIN, 50, ARM_CENTER);
	pwm_init(ARM_LEFT_PIN, 50, ARM_CENTER);
	pwm_init(ARM_MID_PIN, 50, ARM_CENTER);

	gpio_init(MAGNET_FRONT, GPO, 0, GPIO_PIN_CONFIG);
	gpio_init(MAGNET_LEFT, GPO, 0, GPIO_PIN_CONFIG);

	ARM_LEFT_angle(0);
	ARM_LOW_angle(100);
	ARM_UP_angle(180);
	ARM_MID_angle(0);
	rt_thread_mdelay(200);


	
}
