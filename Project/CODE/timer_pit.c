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
        
//			rt_kprintf("x:%d", (int)car.MileageX);
////			rt_kprintf("y:%d\n", (int)car.MileageY);
			// rt_kprintf("x:%d", (int)car.Speed_X);
			// rt_kprintf("y:%d\n", (int)car.Speed_Y);
				

    }
		
		if(0 == (time%20))//每0.02s采集一次
    {
		
			encoder_get();
			motor_control(1);
			//rt_kprintf("%d,%d,%d\n",(int)car.MileageX,(int)car.MileageY, (int)sqrt(car.MileageX * car.MileageX + car.MileageY * car.MileageY));

			//rt_kprintf("%d,%d,%d,%d\n",RC_encoder1, RC_encoder2, RC_encoder3, RC_encoder4);
				
    }
		
		
		

    if(0 == (time%5))//0.005s采集一次
    {
			
			Mahony_computeAngles();//采集陀螺仪数据
			car.Speed_Z=angel_pid((int)angle_z, (int)car.current_angle);//omnimove模式下目标方向一直为0	
		//rt_kprintf("%d\n", (int)angle_z);
			car_omni(car.Speed_X,car.Speed_Y,car.Speed_Z); 
//						car_omni(car.Speed_X,car.Speed_Y,0); 
//			car_omni(5,0,car.Speed_Z); 
			//car_omni_angle(car.Speed_X,car.Speed_Y,car.Speed_Z);
//					x *= cos(car.current_angle);
//		y *= cos(car.current_angle);
//	car_omni(car.Speed_X,5,car.Speed_Z); 


			

			

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