#include "smotor.h"

/* BOX
1 - 0°
2 - 90°
3 - 180°
4 - 270°*/

uint8 cur_angle = 1;
uint32 prime = 1;

// 定义机械臂的PWM IO口
#define ARM_UP_PIN PWM1_MODULE3_CHA_B10
#define ARM_LOW_PIN PWM2_MODULE1_CHA_C8
#define ARM_LEFT_PIN PWM2_MODULE0_CHB_C7
#define ARM_MID_PIN PWM4_MODULE2_CHA_C30

#define ARM_CENTER (1.5 * 50000 / 20)

// 定义电磁铁控制IO口
#define MAGNET_FRONT C6
#define MAGNET_LEFT C5


uint8 storehouse_num[6] = {0,0,0,0,0,0};

// 电磁铁释放
void magnet_front_release(void)
{
	gpio_set(MAGNET_FRONT, 0);
}
void magnet_left_release(void)
{
	gpio_set(MAGNET_LEFT, 0);
}

// 电磁贴吸取
void magnet_front_appeal(void)
{
	gpio_set(MAGNET_FRONT, 1);
}
void magnet_left_appeal(void)
{
	gpio_set(MAGNET_LEFT, 1);
}

// 设置机械臂角度
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
	pwm_duty(ARM_MID_PIN, 3050 + 55.6548 * angle );
}

void arm_out(void) // 防止仓卡住
{
	ARM_LOW_angle(60);
	ARM_UP_angle(120);
	rt_thread_mdelay(200);
}

void arm_return(void) // 防止目标检测到舵机
{
	ARM_UP_angle(180); // 收回，防止目标检测识别到
	rt_thread_mdelay(500);
	ARM_LOW_angle(70);
}

/*
机械臂分为三种情况，或者是两种运动情况
1.放入盒子-先转动仓库，放入卡片

2.不放入盒子- 捡起并抬起，跑点，放下，抬起

注意机械臂抬起角度防止仓库碰到
*/

// 机械臂将卡片放下后并抬起
void arm_down(void)
{

	ARM_UP_angle(60);
	ARM_LOW_angle(60);
	rt_thread_mdelay(300);

	magnet_front_release();
	

	ARM_UP_angle(160); // 收回，防止目标检测识别到
	rt_thread_mdelay(400);
	ARM_UP_angle(180); // 收回，防止目标检测识别到
	rt_thread_mdelay(200);
	ARM_LOW_angle(70);
	rt_thread_mdelay(100);
		ARM_LOW_angle(80);
	rt_thread_mdelay(100);
		ARM_LOW_angle(90);
	rt_thread_mdelay(100);
}

// 机械臂将卡片捡起来
void arm_carry(void)
{
	ARM_UP_angle(5);
	rt_thread_mdelay(200);
	ARM_LOW_angle(53);
	rt_thread_mdelay(100);
	ARM_LOW_angle(48);
	rt_thread_mdelay(100);
	ARM_LOW_angle(43);
	rt_thread_mdelay(100);

	magnet_front_appeal();
	rt_thread_mdelay(500);

	
		ARM_LOW_angle(70);
		rt_thread_mdelay(100);
		ARM_UP_angle(170);
		rt_thread_mdelay(150);
		ARM_LOW_angle(40);
		rt_thread_mdelay(500);
		
				ARM_LOW_angle(50);
		rt_thread_mdelay(100);
				ARM_LOW_angle(60);
		rt_thread_mdelay(100);

	
}

// 将卡片放入盒子
//添加六分类 十二点钟方向为1，以顺时针方向逐渐增大
/*
1-上
2-右
3-下
4-左
5-大类
6-车载
*/




void arm_set_mid(uint8 angle)
{
	arm_out();
		switch(angle)
		{
			case 1:angle = 4;ARM_MID_angle(180);break;
			case 2:angle = 5;ARM_MID_angle(240);break;
			case 3:angle = 6;ARM_MID_angle(300);break;
			case 4:angle = 1;ARM_MID_angle(0);break;
			case 5:angle = 2;ARM_MID_angle(60);break;
			case 6:angle = 3;ARM_MID_angle(120);break;
		}
	arm_return();
}


void arm_putbox(uint8 angle)
{

	arm_out();
	uint8 once_flag = 0;
	
	switch (abs(angle - cur_angle))
	{
	case 0:
		ARM_MID_angle(angle * 60 - 60);
		rt_thread_mdelay(0);
		cur_angle = angle;
		if(storehouse_num[0] == 0)
		{
			once_flag = 1;
		}
		break;

	case 1:
		ARM_MID_angle(angle * 60 - 60);
		rt_thread_mdelay(0);
		cur_angle = angle;
		if(storehouse_num[0] == 0)
		{
			storehouse_num[0] = 1;
			once_flag = 1;
		}
		break;

	case 2:
		ARM_MID_angle(angle * 60 -60);
		rt_thread_mdelay(0);
		cur_angle = angle;
		if(storehouse_num[1] == 0)
		{
			storehouse_num[1] = 1;
			once_flag = 1;
		}
		break;

	case 3:
		ARM_MID_angle(angle * 60 - 60);
		rt_thread_mdelay(0);
		cur_angle = angle;
			if(storehouse_num[2] == 0)
		{
			storehouse_num[2] = 1;
			once_flag = 1;
		}
		break;

	case 4:
		ARM_MID_angle(angle * 60 - 60);
		rt_thread_mdelay(100);
		cur_angle = angle;
			if(storehouse_num[3] == 0)
		{
			storehouse_num[3] = 1;
			once_flag = 1;
		}
		break;

	case 5:
		ARM_MID_angle(angle * 60 - 60);
		rt_thread_mdelay(200);
		cur_angle = angle;
			if(storehouse_num[4] == 0)
		{
			storehouse_num[4] = 1;
			once_flag = 1;
		}
		break;

	case 6:
		ARM_MID_angle(angle * 60 - 60);
		rt_thread_mdelay(300);
		cur_angle = angle;
			if(storehouse_num[5] == 0)
		{
			storehouse_num[5] = 1;
			once_flag = 1;
		}
		break;
	}

	ARM_UP_angle(0);
	rt_thread_mdelay(100);
	ARM_LOW_angle(40);
	rt_thread_mdelay(200);
		ARM_LOW_angle(34);
	rt_thread_mdelay(50);
		ARM_LOW_angle(33);
	rt_thread_mdelay(50);
		ARM_LOW_angle(32);
	rt_thread_mdelay(50);
		ARM_LOW_angle(31);
	rt_thread_mdelay(50);
	ARM_LOW_angle(30);
	rt_thread_mdelay(50);
	ARM_LOW_angle(25);
	rt_thread_mdelay(50);

	magnet_front_appeal();
	rt_thread_mdelay(200);

  rt_sem_release(arrive_sem);
//	select_mode();

//	switch (angle)
//	{
//	case 1:
//	{
//		if (prime % 2 != 0)
//		{
//			prime *= 2;
//			once_flag = 1;
//			//rt_kprintf(" 1-first!!!\n");
//		}
//		break;
//	}
//	case 2:
//	{
//		if (prime % 3 != 0)
//		{
//			prime *= 3;
//			once_flag = 1;
//			//rt_kprintf(" 2-first!!!\n");
//		}
//		break;
//	}
//	case 3:
//	{
//		if (prime % 5 != 0)
//		{
//			prime *= 5;
//			once_flag = 1;
//			//rt_kprintf(" 3-first!!!\n");
//		}
//		break;
//	}
//	case 4:
//	{
//		if (prime % 7 != 0)
//		{
//			prime *= 7;
//			once_flag = 1;
//			//rt_kprintf(" 4-first!!!\n");
//		}
//		break;
//	}
//	}
	
	//rt_kprintf("prime:%d\n", prime);


	if(once_flag)
	{
		ARM_LOW_angle(70);
		rt_thread_mdelay(100);
		ARM_UP_angle(170);
		rt_thread_mdelay(150);
		ARM_LOW_angle(40);
		rt_thread_mdelay(500);
		
				ARM_LOW_angle(50);
		rt_thread_mdelay(100);
				ARM_LOW_angle(60);
		rt_thread_mdelay(100);
				ARM_LOW_angle(70);
		rt_thread_mdelay(100);
				ARM_LOW_angle(80);
		rt_thread_mdelay(100);
	}else
	{
		ARM_LOW_angle(70);
		rt_thread_mdelay(100);
		ARM_UP_angle(170);
		rt_thread_mdelay(150);
		ARM_LOW_angle(40);
		rt_thread_mdelay(500);
		
				ARM_LOW_angle(50);
		rt_thread_mdelay(100);
				ARM_LOW_angle(60);
		rt_thread_mdelay(100);
				ARM_LOW_angle(70);
		rt_thread_mdelay(100);
				ARM_LOW_angle(80);
		rt_thread_mdelay(100);
		
		
		ARM_UP_angle(170);
						ARM_LOW_angle(85);
		rt_thread_mdelay(100);
		ARM_LOW_angle(86);
		rt_thread_mdelay(50);
		ARM_LOW_angle(87);
		rt_thread_mdelay(50);
		ARM_LOW_angle(88);
		rt_thread_mdelay(50);
		ARM_LOW_angle(89);
		rt_thread_mdelay(50);
		ARM_LOW_angle(90);
		rt_thread_mdelay(50);

	}
		once_flag = 0;
	
		rt_thread_mdelay(500);
		magnet_front_release();

		arm_return();
	}

	// 机械臂打开盒子
	void arm_openbox(uint8 angle, uint8 location)
	{

		arm_out();

		switch(angle)
		{
			case 1:angle = 4;ARM_MID_angle(180);break;
			case 2:angle = 5;ARM_MID_angle(240);break;
			case 3:angle = 6;ARM_MID_angle(295);break;
			case 4:angle = 1;ARM_MID_angle(0);break;
			case 5:angle = 2;ARM_MID_angle(59);break;
			case 6:angle = 3;ARM_MID_angle(118);break;
		}
		
		switch (abs(angle - cur_angle))
		{
		case 0:rt_thread_mdelay(0);cur_angle = angle;break;
		case 1:rt_thread_mdelay(500);cur_angle = angle;break;
		case 2:rt_thread_mdelay(700);cur_angle = angle;break;
		case 3:rt_thread_mdelay(900);cur_angle = angle;break;
		case 4:rt_thread_mdelay(1100);cur_angle = angle;break;
		case 5:rt_thread_mdelay(1300);cur_angle = angle;break;
		}

		
		ARM_LEFT_angle(100);
		rt_thread_mdelay(300);
		ARM_LEFT_angle(105);
		rt_thread_mdelay(200);


		magnet_left_appeal();
		rt_thread_mdelay(500);

		ARM_LEFT_angle(20);
		rt_thread_mdelay(500);

		//	ARM_LEFT_angle(100);
		//	rt_thread_mdelay(300);

		magnet_left_release();
		//ARM_MID_angle(300);

		switch(location)
		{
			case 1:ARM_MID_angle(240);break;
			case 2:ARM_MID_angle(180);break;
			case 3:ARM_MID_angle(240);break;
			case 4:ARM_MID_angle(240);break;
			case 5:ARM_MID_angle(120);break;
			case 6:ARM_MID_angle(180);break;
		}
		rt_thread_mdelay(500);


		

//		car.Speed_X = 300;
//		rt_thread_mdelay(500);
//		car.Speed_X = 0;
//		rt_thread_mdelay(500);

	}

	// 机械臂合上盒子
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
		pwm_init(ARM_MID_PIN, 200, ARM_CENTER);

		gpio_init(MAGNET_FRONT, GPO, 0, GPIO_PIN_CONFIG);
		gpio_init(MAGNET_LEFT, GPO, 0, GPIO_PIN_CONFIG);

		ARM_LEFT_angle(40);
		ARM_LOW_angle(60);
		ARM_UP_angle(120); // 收回，防止目标检测识别到
		ARM_MID_angle(0);
		rt_thread_mdelay(2000);
		ARM_LOW_angle(80);
	}
