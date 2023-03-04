#include "display.h"

rt_mailbox_t display_mailbox;

void (*current_operation_index)();


//typedef struct
//{
//    uint8 current;	//当前状态索引号
//    uint8 next; 		//向下一个
//    uint8 enter; 	//确定
//		uint8 back; 		//退出
//    void (*current_operation)(void); //当前状态应该执行的操作
//} Menu_table;

Menu_table  table[30]=
{
    {0,0,1,0,(*home)},	//一级界面（主页面） 索引，向下一个，确定，退出
		
    {2,3,4,0,(*GUI_motor)},	//二级界面 编码器值
    {3,1,5,0,(*GUI_pid)},	//二级界面 pid值
		{1,2,6,0,(*GUI_imu_ra)},	//二级界面 陀螺仪值

		
		{4,1,1,1,(*GUI_motor_value)},		//三级界面：motor_value
		{5,2,2,2,(*GUI_pid_value)},				//三级界面：pid_value
		{6,3,3,3,(*GUI_imu_ra_value)},				//三级界面：imu_ra_value_value
		

 
};


uint8_t  func_index = 0;	//主程序此时所在程序的索引值

void  Menu_key_set(void)
{
	
	 rt_ubase_t mb_data;
   
   rt_mb_recv(display_mailbox, &mb_data,  RT_WAITING_NO);//接受按键发送过来的邮件
	

		if((mb_data == 1))
		{ 
			ips114_clear(WHITE);
			func_index=table[func_index].next;	//按键next按下后的索引号

			
		}
		
		if((mb_data == 2))
		{
			ips114_clear(WHITE);
			func_index=table[func_index].enter;	//按键enter按下后的索引号
			
		}
	 
		if(mb_data == 3)
		{
			ips114_clear(WHITE);
			func_index=table[func_index].back;	//按键back按下后的索引号

		}
		current_operation_index=table[func_index].current_operation;	//执行当前索引号所对应的功能函数
		(*current_operation_index)();//执行当前操作函数
		mb_data = 0;//邮箱数据清除
}





void home()
{
	ips114_showstr(0,1,"DEBUG");
	
}

void GUI_motor()
{
	ips114_showstr(0,1,"motor");
}

void GUI_pid()
{
	ips114_showstr(0,2,"pid");
}

void GUI_motor_value()
{
	while(1)
	{
		
				ips114_showstr(0,0,"ENCODER1:");
				ips114_showstr(0,1,"ENCODER2:");
				ips114_showstr(0,2,"ENCODER3:");
				ips114_showstr(0,3,"ENCODER4:");
	
				ips114_showstr(130,0,"SUM:");
				ips114_showstr(130,1,"SUM:");
				ips114_showstr(130,2,"SUM:");
				ips114_showstr(130,3,"SUM:");
	
	
				ips114_showint16(70, 0,RC_encoder1);
				ips114_showint16(70, 1,RC_encoder2);
				ips114_showint16(70, 2, RC_encoder3);
				ips114_showint16(70, 3, RC_encoder4);
				ips114_showstr(0,4,"MileX:");
//				ips114_showfloat(100,4,MileageKx,1,5);
				ips114_showfloat(60,4,car.MileageX,4,2);
				ips114_showstr(0,5,"MileY:");
//				ips114_showfloat(100,5,MileageKy,1,5);
				ips114_showfloat(60,5,car.MileageY,4,2);
	
	
	}
				
	
}


void GUI_pid_value()
{
				ips114_showstr(0,0,"kp:");
				ips114_showstr(0,1,"ki:");
				ips114_showstr(0,2,"kd:");
				
        ips114_showfloat(50, 0, kp, 2,3);
        ips114_showfloat(50, 1, ki, 2,3);
        ips114_showfloat(50, 2, kd, 2,3);
				
	
				ips114_showstr(0,3,"angleKp:");
				ips114_showstr(0,4,"angleKi:");
				ips114_showstr(0,5,"angleKd:");
				
        ips114_showfloat(80, 3, Angel_KP, 2,3);
        ips114_showfloat(80, 4, Angel_KI, 2,3);
        ips114_showfloat(80, 5, Angel_KD, 2,3);
				ips114_showfloat(80, 7, angle_z,3,2);
				rt_sem_take(key4_sem, RT_WAITING_FOREVER);
//				kp+=0.1;
////				ki+=0.1;
				Angel_KP+=0.1;
}


void GUI_imu_ra()
{
	
		ips114_showstr(0,0,"imu_ra");

}

void GUI_imu_ra_value()
{
	
	ips114_showstr(0, 0, "acc_x:"); ips114_showstr(0, 1, "acc_y:"); ips114_showstr(0, 2, "acc_z:");
	ips114_showint16(50, 0, imu963ra_acc_x); ips114_showint16(50, 1, imu963ra_acc_y); ips114_showint16(50, 2, imu963ra_acc_z);
	
	ips114_showstr(100,0, "gyro_x:"); ips114_showstr(100, 1, "gyro_y:"); ips114_showstr(100, 2, "gyro_z:");
	ips114_showfloat(160, 0, angle_x,3,2); ips114_showfloat(160, 1, angle_y,3,2); ips114_showfloat(160, 2, angle_z,3,2);
	
//	rt_kprintf("%d,%d,%d,%d,%d,%d\n", imu963ra_acc_x,imu963ra_acc_y, imu963ra_acc_z, imu963ra_gyro_x,imu963ra_gyro_y ,imu963ra_gyro_z);
}
void display_entry(void *parameter)
{
    //角度环测试
//		ips114_clear(BLACK);


    while(1)
    {

			Menu_key_set();
//			rt_thread_delay(1000);
//			car.Angel_Target=180;
//			rt_thread_delay(5000);
//			car.Angel_Target=-180;
//			rt_thread_delay(5000);

//			 if(mt9v03x_csi_finish_flag)
//        {
//					mt9v03x_csi_finish_flag = 0;
//					Binarization(mt9v03x_csi_image, output_img,image_threshold);
//					
//				
////					ips114_displayimage032_zoom(output_img, MT9V03X_CSI_W, MT9V03X_CSI_H, 240/2, 135/2);	//显示摄像头图像
//					ips114_displayimage032(output_img, MT9V03X_CSI_W, MT9V03X_CSI_H);
//			
//				}
		}
}





void display_init(void)
{

    rt_thread_t display_th;


    //初始化屏幕
    ips114_init();
    
    //创建显示线程 优先级设置为31
    display_th = rt_thread_create("display", display_entry, RT_NULL, 1024, 31, 1000);


    display_mailbox = rt_mb_create("display", 5, RT_IPC_FLAG_FIFO);
    
    //启动显示线程
    if(RT_NULL != display_th)
    {
        rt_thread_startup(display_th);
		}

}