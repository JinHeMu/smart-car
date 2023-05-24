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
        encoder_get();
//			rt_kprintf("x:%d", (int)car.MileageX);
////			rt_kprintf("y:%d\n", (int)car.MileageY);
			// rt_kprintf("x:%d", (int)car.Speed_X);
			// rt_kprintf("y:%d\n", (int)car.Speed_Y);
    }
		
		if(0 == (time%20))//ÿ0.02s�ɼ�һ��
    {
			
			motor_control(1);
			rt_kprintf("%d\n",RC_encoder2);
				
    }
		
		
		

    if(0 == (time%5))//0.005s�ɼ�һ��
    {
			
			Mahony_computeAngles();//�ɼ�����������
			
			car.Speed_Z=angel_pid((int)angle_z, (int)car.target_angle);//omnimoveģʽ��Ŀ�귽��һֱΪ0
            // rt_kprintf("angle:%d\n", angle_z);
////			rt_kprintf("%d\n",(int)car.target_angle);
////			
////			//����С���ٶȣ�ͨ�������Զ������������γ���Ӧ���ٶ�
////			// parm1 y���ٶ� parm2 x���ٶ�
////			
//			car_omni(car.Speed_X,car.Speed_Y,car.Speed_Z); 
			car_omni(car.Speed_X,car.Speed_Y,car.Speed_Z); 
            //С����Ϊx��С����Ϊy
//				car_omni(20,0,0); 
//			rt_kprintf("%d\n",RC_encoder2);

			

			

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