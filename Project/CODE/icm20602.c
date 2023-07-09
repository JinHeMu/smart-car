#include "icm20602.h"

#define M_PI 3.1415926f

uint8 icm_zeroBiasFlag = 0;
arhs_source_param_t source_data; // ԭʼ����
gyro_zero_param_t Gyro_Bias;	 // ����������(У׼ʹ��)

float angle_x;
float angle_y;
float angle_z;


#define WINDOW_SIZE 10

float slidingFilter(float newData)
{
    static float buffer[WINDOW_SIZE];
    static int currentIndex = 0;
    static float sum = 0;

    // ���»�������
    sum -= buffer[currentIndex];
    buffer[currentIndex] = newData;
    sum += newData;

    // ���µ�ǰ����
    currentIndex = (currentIndex + 1) % WINDOW_SIZE;

    // ����ƽ��ֵ
    float average = sum / WINDOW_SIZE;

    return average;
}

void icm_zeroBias(void)//��������Ư
{
	Gyro_Bias.Zdata = 0;//��ʼ��
		for (uint16_t i = 0; i < 200; ++i)
		{
			get_icm20602_gyro_spi();//��ȡ���ٶ�
			Gyro_Bias.Zdata += icm_gyro_z;//�ۼ�������
			rt_kprintf("Zdata :%d\n", 	(int)icm_gyro_z);
			rt_thread_mdelay(5);
		}
		Gyro_Bias.Zdata /= 200;//ȡƽ����
		rt_kprintf("Gyro_Bias.Zdata :%d\n", 	(int)Gyro_Bias.Zdata);
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
	// �ɼ�����������
	get_icm20602_gyro_spi();//��ȡ���ٶ�
	ARHS_getValues();
}
