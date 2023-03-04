#include "encoder.h"
#include "motor.h"
#include "timer_pit.h"

void timer1_pit_entry(void *parameter)
{
    static uint32 time;
    time++;
    
    //采集陀螺仪数据
    
    
    if(0 == (time%100))
    {
        //采集编码器数据
        encoder_get();
    }

    if(0 == (time%5))
    {
			Mahony_computeAngles();
//			car.Speed_Z=angel_pid((int)angle_z, (int)car.Angel_Target);//omnimove模式下目标方向一直为0
			car.Speed_Z=angel_pid((int)angle_z, 0);//omnimove模式下目标方向一直为0
			car_omni(car.Speed_X,car.Speed_Y,car.Speed_Z); // parm1 y轴速度 parm2 x轴速度
			rt_kprintf("%d, %d, %d, %d\n", (int)car.Speed_Z, (int)angle_z,(int)(Angel_KP*10),(int)(Angel_KD*10));
//			rt_kprintf("%d, %d, %d\n",imu963ra_gyro_x, imu963ra_gyro_y, imu963ra_gyro_z);
//	
        //采集加速度数据
        //采集电磁信号
//        elec_get();
//        
//        //根据电磁信号计算车身位置
//        elec_calculate();
    }
		
    
    //控制电机转动
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
				rt_kprintf("rt_timer_create successed ...\n");
    }else
		{
			rt_kprintf("rt_timer_create falied ...\n");
		}

    
}