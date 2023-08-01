#include "encoder.h"
#include "motor.h"
#include "timer_pit.h"

void timer1_pit_entry(void *parameter)//һ��ʱ�ӽ���һ����
{
    static uint32 time;
    time++;
    
    //�ɼ�����������
    
    
    if(0 == (time%100))//ÿ0.1s�ɼ�һ��
    {
        //�ɼ�����������
        
//			rt_kprintf("x:%d", (int)car.MileageX);
////			rt_kprintf("y:%d\n", (int)car.MileageY);
			// rt_kprintf("x:%d", (int)car.Speed_X);
			// rt_kprintf("y:%d\n", (int)car.Speed_Y);
				

    }
		
		if(0 == (time%20))//ÿ0.02s�ɼ�һ��
    {
		
			encoder_get();
			motor_control(1);
			//rt_kprintf("%d,%d,%d\n",(int)car.MileageX,(int)car.MileageY, (int)sqrt(car.MileageX * car.MileageX + car.MileageY * car.MileageY));

			//rt_kprintf("%d,%d,%d,%d\n",RC_encoder1, RC_encoder2, RC_encoder3, RC_encoder4);
				
    }
		
		
		

    if(0 == (time%5))//0.005s�ɼ�һ��
    {
			
			Mahony_computeAngles();//�ɼ�����������
			car.Speed_Z=angel_pid((int)angle_z, (int)car.current_angle);//omnimoveģʽ��Ŀ�귽��һֱΪ0	
		rt_kprintf("%d\n", (int)angle_z);
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
    
    //����һ����ʱ�� ��������
    timer = rt_timer_create("timer1", timer1_pit_entry, RT_NULL, 1, RT_TIMER_FLAG_PERIODIC);
    
    //������ʱ��
    if(RT_NULL != timer)
    {
        rt_timer_start(timer);
    }


    
}