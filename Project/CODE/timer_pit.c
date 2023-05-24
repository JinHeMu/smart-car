#include "encoder.h"
#include "motor.h"
#include "timer_pit.h"

void timer1_pit_entry(void *parameter)//一个时钟节拍一毫秒
{
    static uint32 time;
    time++;
    
    //采集陀螺仪数据
    
    
    if(0 == (time%100))//每0.1s采集一次
    {
        //采集编码器数据
        encoder_get();
//			rt_kprintf("x:%d", (int)car.MileageX);
////			rt_kprintf("y:%d\n", (int)car.MileageY);
			// rt_kprintf("x:%d", (int)car.Speed_X);
			// rt_kprintf("y:%d\n", (int)car.Speed_Y);
    }
		
		if(0 == (time%20))//每0.02s采集一次
    {
			
			motor_control(1);
			rt_kprintf("%d\n",RC_encoder2);
				
    }
		
		
		

    if(0 == (time%5))//0.005s采集一次
    {
			
			Mahony_computeAngles();//采集陀螺仪数据
			
			car.Speed_Z=angel_pid((int)angle_z, (int)car.target_angle);//omnimove模式下目标方向一直为0
            // rt_kprintf("angle:%d\n", angle_z);
////			rt_kprintf("%d\n",(int)car.target_angle);
////			
////			//给予小车速度，通过解算自动输出到电机来形成相应的速度
////			// parm1 y轴速度 parm2 x轴速度
////			
//			car_omni(car.Speed_X,car.Speed_Y,car.Speed_Z); 
			car_omni(car.Speed_X,car.Speed_Y,car.Speed_Z); 
            //小车右为x，小车上为y
//				car_omni(20,0,0); 
//			rt_kprintf("%d\n",RC_encoder2);

			

			

    }
		
}


void timer_pit_init(void)
{
    rt_timer_t timer;
    
    //创建一个定时器 周期运行
    timer = rt_timer_create("timer1", timer1_pit_entry, RT_NULL, 1, RT_TIMER_FLAG_PERIODIC);
    
    //启动定时器
    if(RT_NULL != timer)
    {
        rt_timer_start(timer);
    }


    
}