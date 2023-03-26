#include "imu963.h"


#define M_PI 3.1415926f


float imura_buff[Filter_N];


int imura_zeroBiasFlag = 0;
arhs_source_param_t source_data;//原始数据
gyro_zero_param_t Gyro_Bias;//陀螺仪数据(校准使用)

float angle_x;
float angle_y;
float angle_z;

 
//void imura_filter()
//{
//	Movingaverage_filter(, imura_buff);
//}
 
void imura_zeroBias()
{
	
	
		Gyro_Bias.Xdata = 0;
    Gyro_Bias.Ydata = 0;
    Gyro_Bias.Zdata = 0;
//    Mag_Bias.Xdata = 0;
//    Mag_Bias.Ydata = 0;
//    Mag_Bias.Zdata = 0;
	
	if(imura_zeroBiasFlag == 0)
	{
		
			for (uint16_t i = 0; i < 100; ++i)
			{
					imu963ra_get_gyro();
//					imu963ra_get_mag();
					
					Gyro_Bias.Xdata += imu963ra_gyro_x;
					Gyro_Bias.Ydata += imu963ra_gyro_y;
					Gyro_Bias.Zdata += imu963ra_gyro_z;
	//        Mag_Bias.Xdata += imu963ra_mag_x;
	//        Mag_Bias.Ydata += imu963ra_mag_y;
	//        Mag_Bias.Zdata += imu963ra_mag_z;
				
					rt_thread_mdelay(10);
			}

			Gyro_Bias.Xdata /= 100;
			Gyro_Bias.Ydata /= 100;
			Gyro_Bias.Zdata /= 100;
			imura_zeroBiasFlag = 1;
	//    Mag_Bias.Xdata /= 100;
	//    Mag_Bias.Ydata /= 100;
	//    Mag_Bias.Zdata /= 100;
//			ips114_showfloat(120, 0, Gyro_Bias.Xdata,3,4); 
//			ips114_showfloat(120, 1, Gyro_Bias.Ydata,3,4); 
//			ips114_showfloat(120, 2, Gyro_Bias.Zdata,3,4);
//			
	}
//	
}

#define alpha 0.3f

void ARHS_getValues()
{  
    //一阶低通滤波，单位g/s
//		imura_zeroBias();
	
	
//		source_data.gyro_x = Movingaverage_filter(imu963ra_gyro_x, imura_buff);
//		source_data.gyro_y = Movingaverage_filter(imu963ra_gyro_y, imura_buff);
//		source_data.gyro_z = Movingaverage_filter(imu963ra_gyro_z, imura_buff);
	
    source_data.acc_x = (((float)imu963ra_acc_x) * alpha) * 8 / 4098 + source_data.acc_x * (1 - alpha);
    source_data.acc_y = (((float)imu963ra_acc_y) * alpha) * 8 / 4098 + source_data.acc_y * (1 - alpha);
    source_data.acc_z = (((float)imu963ra_acc_z) * alpha) * 8 / 4098 + source_data.acc_z * (1 - alpha);

    //陀螺仪角度转弧度
    source_data.gyro_x = ((float)imu963ra_gyro_x + Gyro_Bias.Xdata) * M_PI / 180 / 14.3f;
    source_data.gyro_y = ((float)imu963ra_gyro_y + Gyro_Bias.Ydata) * M_PI / 180 / 14.3f;
    source_data.gyro_z = ((float)imu963ra_gyro_z + Gyro_Bias.Zdata) * M_PI / 180 / 14.3f;
		
	
		angle_x += (source_data.gyro_x/3.33);
		angle_y += (source_data.gyro_y/3.33);
		angle_z += (source_data.gyro_z/3.33);
//		angle_x += (source_data.gyro_x);
//		angle_y += (source_data.gyro_y);
//		angle_z += (source_data.gyro_z);
		
		if(angle_z>=360)
		{
			angle_z-=360;
			
		}
		
		if(angle_z<=-360)
		{
			angle_z+=360;
			
		}
	
  
	
		
    //磁力计转换量程
//    source_data.mag_x = (float)imu963ra_mag_x / 3000.0f;
//    source_data.mag_y = (float)imu963ra_mag_y / 3000.0f;
//    source_data.mag_z = (float)imu963ra_mag_z / 3000.0f;

//    source_data.mag_x = ((float)imu963ra_mag_x - Mag_Bias.Xdata) / 3000.0f;
//    source_data.mag_y = ((float)imu963ra_mag_y - Mag_Bias.Ydata) / 3000.0f;
//    source_data.mag_z = ((float)imu963ra_mag_z - Mag_Bias.Zdata) / 3000.0f;
}

void Mahony_computeAngles()
{
    //采集陀螺仪数据
    imu963ra_get_acc();
    imu963ra_get_gyro();
//		imu963ra_get_mag();
		imura_zeroBias();

    ARHS_getValues();
//		rt_kprintf("%d, %d, %d\n",imu963ra_gyro_x, imu963ra_gyro_y, imu963ra_gyro_z);
//		rt_kprintf("%d,%d,%d\n",(int)angle_x,(int)angle_y,(int)angle_z);
}
    //六轴解算
    //ICM_AHRSupdate(source_data.gyro_x, source_data.gyro_y, source_data.gyro_z, source_data.acc_x, source_data.acc_y, source_data.acc_z);
//    MahonyAHRSupdateIMU(source_data.gyro_x, source_data.gyro_y, source_data.gyro_z, source_data.acc_x, source_data.acc_y, source_data.acc_z);
//    //九轴解算
//    //MahonyAHRSupdate(source_data.gyro_x, source_data.gyro_y, source_data.gyro_z, source_data.acc_x, source_data.acc_y, source_data.acc_z, source_data.mag_x, source_data.mag_y, source_data.mag_z);

//    //四元数计算欧拉角
//    arhs_data.pitch = asinf(-2 * q1 * q3 + 2 * q0 * q2) * 180 / M_PI;                                // pitch
//    arhs_data.roll = atan2f(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 180 / M_PI; // roll
//    arhs_data.yaw = atan2f(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) * 180 / M_PI;  // yaw

//    /*   姿态限制*/
//    if (arhs_data.roll > 90 || arhs_data.roll < -90)
//    {
//        if (arhs_data.pitch > 0)
//        {
//            arhs_data.pitch = 180 - arhs_data.pitch;
//        }
//        if (arhs_data.pitch < 0)
//        {
//            arhs_data.pitch = -(180 + arhs_data.pitch);
//        }
//    }

//    if (arhs_data.yaw > 360)
//    {
//        arhs_data.yaw -= 360;
//    }
//    else if (arhs_data.yaw < 0)
//    {
//        arhs_data.yaw += 360;
//    }
//}


//void mahonyupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) {
//    float recipNorm;
//    float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
//    float hx, hy, bx, bz;
//    float halfvx, halfvy, halfvz, halfwx, halfwy, halfwz;
//    float halfex, halfey, halfez;
//    float qa, qb, qc;

//    // Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
//    if((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f)) {
//        MahonyAHRSupdateIMU(gx, gy, gz, ax, ay, az);
//        return;
//    }

//    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
//    if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

//        // Normalise accelerometer measurement
//        recipNorm = invSqrt(ax * ax + ay * ay + az * az);
//        ax *= recipNorm;
//        ay *= recipNorm;
//        az *= recipNorm;

//        // Normalise magnetometer measurement
//        recipNorm = invSqrt(mx * mx + my * my + mz * mz);
//        mx *= recipNorm;
//        my *= recipNorm;
//        mz *= recipNorm;


//        // Auxiliary variables to avoid repeated arithmetic
//        q0q0 = q0 * q0;
//        q0q1 = q0 * q1;
//        q0q2 = q0 * q2;
//        q0q3 = q0 * q3;
//        q1q1 = q1 * q1;
//        q1q2 = q1 * q2;
//        q1q3 = q1 * q3;
//        q2q2 = q2 * q2;
//        q2q3 = q2 * q3;
//        q3q3 = q3 * q3;

//        // Reference direction of Earth's magnetic field
//        hx = 2.0f * (mx * (0.5f - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
//        hy = 2.0f * (mx * (q1q2 + q0q3) + my * (0.5f - q1q1 - q3q3) + mz * (q2q3 - q0q1));
//        bx = sqrt(hx * hx + hy * hy);
//        bz = 2.0f * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5f - q1q1 - q2q2));

//        // Estimated direction of gravity and magnetic field
//        halfvx = q1q3 - q0q2;
//        halfvy = q0q1 + q2q3;
//        halfvz = q0q0 - 0.5f + q3q3;
//        halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
//        halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
//        halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2);

//        // Error is sum of cross product between estimated direction and measured direction of field vectors
//        halfex = (ay * halfvz - az * halfvy) + (my * halfwz - mz * halfwy);
//        halfey = (az * halfvx - ax * halfvz) + (mz * halfwx - mx * halfwz);
//        halfez = (ax * halfvy - ay * halfvx) + (mx * halfwy - my * halfwx);

//        // Compute and apply integral feedback if enabled
//        if(twoKi > 0.0f) {
//            integralFBx += twoKi * halfex * (1.0f / sampleFreq);	// integral error scaled by Ki
//            integralFBy += twoKi * halfey * (1.0f / sampleFreq);
//            integralFBz += twoKi * halfez * (1.0f / sampleFreq);
//            gx += integralFBx;	// apply integral feedback
//            gy += integralFBy;
//            gz += integralFBz;
//        }
//        else {
//            integralFBx = 0.0f;	// prevent integral windup
//            integralFBy = 0.0f;
//            integralFBz = 0.0f;
//        }

//        // Apply proportional feedback
//        gx += twoKp * halfex;
//        gy += twoKp * halfey;
//        gz += twoKp * halfez;
//    }

//    // Integrate rate of change of quaternion
//    gx *= (0.5f * (1.0f / sampleFreq));		// pre-multiply common factors
//    gy *= (0.5f * (1.0f / sampleFreq));
//    gz *= (0.5f * (1.0f / sampleFreq));
//    qa = q0;
//    qb = q1;
//    qc = q2;
//    q0 += (-qb * gx - qc * gy - q3 * gz);
//    q1 += (qa * gx + qc * gz - q3 * gy);
//    q2 += (qa * gy - qb * gz + q3 * gx);
//    q3 += (qa * gz + qb * gy - qc * gx);

//    // Normalise quaternion
//    recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
//    q0 *= recipNorm;
//    q1 *= recipNorm;
//    q2 *= recipNorm;
//    q3 *= recipNorm;
//}

 
