#include "icm20602.h"

#define M_PI 3.1415926f

uint8 icm_zeroBiasFlag = 0;
arhs_source_param_t source_data; // 原始数据
gyro_zero_param_t Gyro_Bias;	 // 陀螺仪数据(校准使用)

float angle_x;
float angle_y;
float angle_z;


#define WINDOW_SIZE 10

float slidingFilter(float newData)
{
    static float buffer[WINDOW_SIZE];
    static int currentIndex = 0;
    static float sum = 0;

    // 更新缓存数组
    sum -= buffer[currentIndex];
    buffer[currentIndex] = newData;
    sum += newData;

    // 更新当前索引
    currentIndex = (currentIndex + 1) % WINDOW_SIZE;

    // 计算平均值
    float average = sum / WINDOW_SIZE;

    return average;
}

void icm_zeroBias(void)//陀螺仪零漂
{
	Gyro_Bias.Zdata = 0;//初始化
		for (uint16_t i = 0; i < 1000; ++i)
		{
			get_icm20602_gyro_spi();//获取角速度
			Gyro_Bias.Zdata += icm_gyro_z;//累加陀螺仪
			//rt_kprintf("Zdata :%d\n", 	(int)icm_gyro_z);
			rt_thread_mdelay(5);
		}
		Gyro_Bias.Zdata /= 1000;//取平均数
		//rt_kprintf("Gyro_Bias.Zdata :%d\n", 	(int)Gyro_Bias.Zdata);
}

void ARHS_getValues()
{

	float filtered_gyro_z = slidingFilter(((float)icm_gyro_z - Gyro_Bias.Zdata)  / 16.4f);

	//rt_kprintf("%d\n", (int)(filtered_gyro_z * 10000));
	
	if(abs(filtered_gyro_z) < 3)
	{
		filtered_gyro_z = 0;
	}

    angle_z -= filtered_gyro_z * 0.005;
	
	if (angle_z >= 360)
	{
		angle_z -= 360;
	}

	if (angle_z <= -360)
	{
		angle_z += 360;
	}
}




void Mahony_computeAngles()
{
	// 采集陀螺仪数据
	get_icm20602_gyro_spi();//获取角速度
	ARHS_getValues();
}
