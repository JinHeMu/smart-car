#include "display.h"

rt_mailbox_t display_mailbox; // 定义一个接受信息的邮箱



char taget_Big_category[10];

void (*current_operation_index)();


// 多级菜单界面
Menu_table table[30] =
	{
		{0, 1, 1, 0, (*home)}, // 一级界面（主页面） 索引，向下一个，确定，退出

		{1, 2, 6, 0, (*GUI_motor)},	 // 二级界面 电机相关参数
		{2, 3, 7, 0, (*GUI_pid)},	 // 二级界面 pid值
		{3, 4, 8, 0, (*GUI_imu_ra)}, // 二级界面 陀螺仪值
		{4, 5, 9, 0, (*GUI_route)},
		{5, 1, 10, 0, (*GUI_arm)},

		{6, 1, 1, 1, (*GUI_motor_value)},  // 三级界面：motor_value
		{7, 2, 2, 2, (*GUI_pid_value)},	   // 三级界面：pid_value
		{8, 3, 3, 3, (*GUI_imu_ra_value)}, // 三级界面
		{9, 4, 4, 4, (*GUI_route_value)}, // 三级界面
		{10, 5, 5, 5, (*GUI_arm_value)}, // 三级界面：
		


};

uint8_t func_index = 0; // 主程序此时所在程序的索引值

void Menu_key_set(void)
{

	rt_ubase_t mb_data; // 储存邮箱的数据

	rt_mb_recv(display_mailbox, &mb_data, RT_WAITING_NO); // 接受按键发送过来的邮件	

	if ((mb_data == 1)) // 按下按键1
	{
		
//		uart_putchar(USART_4, 0x41); // 发送OPENART1告诉该识别A4纸了
		car_move(100,100);
		rt_thread_mdelay(5000);
		car_move(40,40);

		mb_data = 0; // 邮箱数据清除
	}


	if ((mb_data == 2)) // 按下按键1
	{

		game_mode = 1;
		ips114_clear(WHITE);
		strcpy(taget_Big_category, "fruit");
		rt_kprintf("taget_Big_category is fruit!!!\n");
		ips114_showstr(0, 0, "fruit");
		
		car_move(200,200);
		rt_thread_mdelay(5000);
		car_move(40,40);
		
		mb_data = 0; // 邮箱数据清除
	}


	if ((mb_data == 3))
	{
		game_mode = 1;
		ips114_clear(WHITE);
		rt_kprintf("taget_Big_category is vegetable!!!\n");
		strcpy(taget_Big_category, "vegetable");
		ips114_showstr(0, 0, "vegetable");

		car_move(300,300);
		rt_thread_mdelay(5000);
		car_move(40,40);
		
		mb_data = 0; // 邮箱数据清除
	}

	if (mb_data == 4)
	{
//		game_mode = 1;
//		ips114_clear(WHITE);
//		rt_kprintf("taget_Big_category is food!!!\n");
//		strcpy(taget_Big_category, "food");
//		ips114_showstr(0, 0, "food");
		
		Position_kp += 0.1;
		ips114_showfloat(160, 0, Position_kp, 3, 2);
		
		
		mb_data = 0; // 邮箱数据清除
	}

}

void home() // 主界面
{
	ips114_showstr(0, 1, "DEBUG");
}

void GUI_motor() // 一级菜单，显示motor
{
	ips114_showstr(0, 0, "->> motor");
	ips114_showstr(0, 1, "    pid");
	ips114_showstr(0, 2, "    imu_ra");
	ips114_showstr(0, 3, "    route");
	ips114_showstr(0, 4, "		Arm");
}

void GUI_pid() // 一级菜单，显示pid
{
	ips114_showstr(0, 0, "    motor");
	ips114_showstr(0, 1, "->> pid");
	ips114_showstr(0, 2, "    imu_ra");
	ips114_showstr(0, 3, "    route");
	ips114_showstr(0, 4, "		Arm");
}

void GUI_imu_ra()
{

	ips114_showstr(0, 0, "    motor");
	ips114_showstr(0, 1, "    pid");
	ips114_showstr(0, 2, "->> imu_ra");
	ips114_showstr(0, 3, "    route");
	ips114_showstr(0, 4, "		Arm");
}

void GUI_route()
{
	ips114_showstr(0, 0, "    motor");
	ips114_showstr(0, 1, "    pid");
	ips114_showstr(0, 2, "    imu_ra");
	ips114_showstr(0, 3, "->> route");
	ips114_showstr(0, 4, "		Arm");

}

void GUI_arm()
{
	ips114_showstr(0, 0, "    Motor");
	ips114_showstr(0, 1, "    Pid");
	ips114_showstr(0, 2, "    Imu_ra");
	ips114_showstr(0, 3, "		Route");
	ips114_showstr(0, 4, "->> Arm");

}

void GUI_motor_value() // 显示电机状态，和路程状态
{

	ips114_showstr(0, 0, "ENCODER1:");
	ips114_showstr(0, 1, "ENCODER2:");
	ips114_showstr(0, 2, "ENCODER3:");
	ips114_showstr(0, 3, "ENCODER4:");

	ips114_showstr(130, 0, "SUM:");
	ips114_showstr(130, 1, "SUM:");
	ips114_showstr(130, 2, "SUM:");
	ips114_showstr(130, 3, "SUM:");

	ips114_showint16(70, 0, RC_encoder1);
	ips114_showint16(70, 1, RC_encoder2);
	ips114_showint16(70, 2, RC_encoder3);
	ips114_showint16(70, 3, RC_encoder4);
	ips114_showstr(0, 4, "MileX:");
	ips114_showfloat(60, 4, car.MileageX, 4, 2);
	ips114_showstr(0, 5, "MileY:");
	ips114_showfloat(60, 5, car.MileageY, 4, 2);
}

void GUI_pid_value() // 显示pid的相应数据，并且可以做相应的修改
{

		ips114_showstr(0, 0, "kp:");
		ips114_showstr(0, 1, "ki:");
		ips114_showstr(0, 2, "kd:");

		ips114_showstr(0, 3, "angleKp:");
		ips114_showstr(0, 4, "angleKi:");
		ips114_showstr(0, 5, "angleKd:");

		ips114_showfloat(80, 3, Angel_kp, 2, 3);
		ips114_showfloat(80, 4, Angel_ki, 2, 3);
		ips114_showfloat(80, 5, Angel_kd, 2, 3);
		ips114_showfloat(80, 7, angle_z, 3, 2);
		
		if(rt_sem_take(key4_sem, RT_WAITING_NO) == RT_EOK)
		{
			Angel_kd+=0.1;
		}


	

}

void GUI_imu_ra_value() // 显示陀螺仪相关数据
{

	ips114_showstr(0, 0, "acc_x:");
	ips114_showstr(0, 1, "acc_y:");
	ips114_showstr(0, 2, "acc_z:");
	ips114_showint16(50, 0, imu963ra_acc_x);
	ips114_showint16(50, 1, imu963ra_acc_y);
	ips114_showint16(50, 2, imu963ra_acc_z);

	ips114_showstr(100, 0, "gyro_x:");
	ips114_showstr(100, 1, "gyro_y:");
	ips114_showstr(100, 2, "gyro_z:");
	ips114_showfloat(160, 0, angle_x, 3, 2);
	ips114_showfloat(160, 1, angle_y, 3, 2);
	ips114_showfloat(160, 2, angle_z, 3, 2);
}

void GUI_route_value()
{
	
	ips114_showstr(0,1, "target_point:");ips114_showint8(110, 1, (int)car.target_x);ips114_showstr(160,1, ":");ips114_showint8(170, 1, (int)car.target_y);

}

void GUI_arm_value()
{
	int angle = 0 ;
	while(1)
	{
		ips114_showstr(0,1, "angle:");ips114_showint8(110, 1, angle);
		
		if(rt_sem_take(key2_sem, RT_WAITING_FOREVER) == RT_EOK)
		{
			ARM_LOW_angle(angle);
			angle += 10;
		}		
	}
	

}
void display_entry(void *parameter)
{
	while(1)
{
	Menu_key_set();
}
	
}




//}

void display_init(void)
{

	rt_thread_t display_th;

	// 初始化屏幕
	ips114_init();

	// 创建显示线程 优先级设置为31
	display_th = rt_thread_create("display", display_entry, RT_NULL, 1024, 31, 1000);

	display_mailbox = rt_mb_create("display", 5, RT_IPC_FLAG_FIFO);

	// 启动显示线程
	if (RT_NULL != display_th)
	{
		rt_thread_startup(display_th);
	}
}