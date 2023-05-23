//#include "icm20602.h"

//float icm20602_buff[Filter_N];

//int icm20602_zeroBiasFlag = 0;


//gyro_zero_param_t Gyro_Bias;	 // 陀螺仪数据(校准使用)
//arhs_source_param_t source_data; // 原始数据

//float angle_x;
//float angle_y;
//float angle_z;

//void icm20602_zeroBias()
//{
//	Gyro_Bias.Xdata = 0;
//	Gyro_Bias.Ydata = 0;
//	Gyro_Bias.Zdata = 0;
//	if (icm20602_zeroBiasFlag == 0)
//	{

//		for (uint16_t i = 0; i < 500; ++i)
//		{
//			get_icm20602_gyro_spi();
//			//imu963ra_get_mag();

//			Gyro_Bias.Xdata += icm_gyro_x;
//			Gyro_Bias.Ydata += icm_gyro_y;
//			Gyro_Bias.Zdata += icm_gyro_z;
//			//        Mag_Bias.Xdata += imu963ra_mag_x;
//			//        Mag_Bias.Ydata += imu963ra_mag_y;
//			//        Mag_Bias.Zdata += imu963ra_mag_z;

//			rt_thread_mdelay(5);
//		}
//		Gyro_Bias.Xdata /= 500;
//		Gyro_Bias.Ydata /= 500;
//		Gyro_Bias.Zdata /= 500;
//		icm20602_zeroBiasFlag = 1;
//}