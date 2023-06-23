#include "imu963.h"

#define M_PI 3.1415926f

float imura_buff[Filter_N];

int imura_zeroBiasFlag = 0;
arhs_source_param_t source_data; // 原始数据
gyro_zero_param_t Gyro_Bias;	 // 陀螺仪数据(校准使用)

float angle_x;
float angle_y;
float angle_z;


void imura_zeroBias()
{

	Gyro_Bias.Xdata = 0;
	Gyro_Bias.Ydata = 0;
	Gyro_Bias.Zdata = 0;

	if (imura_zeroBiasFlag == 0)
	{

		for (uint16_t i = 0; i < 100; ++i)
		{
			imu963ra_get_gyro();

			Gyro_Bias.Xdata += imu963ra_gyro_x;
			Gyro_Bias.Ydata += imu963ra_gyro_y;
			Gyro_Bias.Zdata += imu963ra_gyro_z;

			rt_thread_mdelay(10);
		}
		Gyro_Bias.Xdata /= 100;
		Gyro_Bias.Ydata /= 100;
		Gyro_Bias.Zdata /= 100;
		imura_zeroBiasFlag = 1;
	}
}

void ARHS_getValues()
{
	// 陀螺仪角度转弧度
	source_data.gyro_x = ((float)imu963ra_gyro_x + Gyro_Bias.Xdata) * M_PI / 180 / 14.3f;
	source_data.gyro_y = ((float)imu963ra_gyro_y + Gyro_Bias.Ydata) * M_PI / 180 / 14.3f;
	source_data.gyro_z = ((float)imu963ra_gyro_z + Gyro_Bias.Zdata) * M_PI / 180 / 14.3f;
	
	source_data.acc_x = imu963ra_acc_x;
	source_data.acc_y = imu963ra_acc_y;
	source_data.acc_z = imu963ra_acc_z;
	
	if(abs((int)imu963ra_gyro_z + Gyro_Bias.Zdata)<10)
	{
		source_data.gyro_z = 0;
	}

	angle_x += (source_data.gyro_x / 3.33);
	angle_y += (source_data.gyro_y / 3.33);
	angle_z += (source_data.gyro_z / 3.33);

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
	imu963ra_get_gyro();
	imu963ra_get_acc();
	imura_zeroBias();
	ARHS_getValues();
}
