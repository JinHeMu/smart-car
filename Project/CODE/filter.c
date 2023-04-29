/***************************************************
*
* @file         Filter.c
* @brief        �˲�����
* @version      v1.0
* @date         2022/2/8
*
***************************************************/

#include "filter.h"

//-----------------------��-----------------------//


//--------------------ȫ�ֱ���---------------------//

//float Filter_buf[Filter_N + 1] = {0};	//������������

float Q_angle = 0;	//����������Э����
float Q_bias = 0;	//������Ư������Э����
float R_angle = 0;	//�ǶȲ�������Э����
float kal_P[2][2] = {0};
float kal_k[2] = {0};
float kal_angle;	//�˲�ֵ
float kal_rate;		//�˲�ֵ
float angle_bias;

float cpm_k = 0.85;	//�����˲�ϵ��
float cpm_angle;//�˲�ֵ

float yaw_Filter = 0;

//---------------------�ṹ��---------------------//



//-------------------------------------------------------------------------------------------------------------------
//  @brief      �������˲� | yaw��
//  @param      now_angle            Ӧ���ɼ��ٶȼƵõ�angle����ʹ�������ǽ��в��������Ǽ��ٶȼƵò���yaw�ǣ����ܻ��ǵ�ʹ�ô�����
//  @param      now_rate             �����ǵõ��Ľ��ٶ�
//  @param      dt      			 ����ʱ��
//  @return     kal_angle			 �˲�ֵ
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
float Kalmen_getAngle(float now_angle, float now_rate,float dt)
{
    //Ԥ�⵱ǰ�Ƕ�
    kal_angle = kal_angle - Q_bias * dt + now_rate * dt;

    //Ԥ�����Э����
    kal_P[0][0] += Q_angle - (kal_P[0][1] -kal_P[1][0]) * dt;
    kal_P[0][1] -= kal_P[0][1] * dt;
    kal_P[1][0] -= kal_P[1][0] * dt;
    kal_P[1][1] += Q_bias;

    //���㿨��������
    kal_k[0] = kal_P[0][0]/(kal_P[0][0] + R_angle);
    kal_k[1] = kal_P[1][0]/(kal_P[0][0] + R_angle);

    //�������Ź���ֵ
    kal_angle = kal_angle + kal_k[0] * (now_angle - kal_angle);
    Q_bias    = Q_bias    + kal_k[1] * (now_angle - kal_angle);

    //����Э�������
    kal_P[0][0] = kal_P[0][0] - kal_k[0] * kal_P[0][0];
    kal_P[0][1] = kal_P[0][1] - kal_k[0] * kal_P[0][1];
    kal_P[1][0] = kal_P[1][0] - kal_k[1] * kal_P[0][0];
    kal_P[1][1] = kal_P[1][1] - kal_k[1] * kal_P[0][1];

    return kal_angle;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���׻����˲� | yaw��
//  @param      now_angle            Ӧ���ɼ��ٶȼƵõ�angle����ʹ�������ǽ��в��������Ǽ��ٶȼƵò���yaw�ǣ����ܻ��ǵ�ʹ�ô�����
//  @param      now_rate             �����ǵõ��Ľ��ٶ�
//  @param      dt      			 ����ʱ��
//  @return     cpm_angle			 �˲�ֵ
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------


float complementary_filter2(float now_angle, float now_rate, float dt)
{
    float y1,x1,x2;
    x1 = (now_angle - cpm_angle) * cpm_k * cpm_k;
    y1 = y1 + x1 * dt;
    x2 = y1 + 2 * cpm_k *(now_angle - cpm_angle) + now_rate;
    cpm_angle = cpm_angle + x2 * dt;
    return cpm_angle;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����һ��ֵ����һ��ֵ������ֵȡƽ�� | �����˲�
//  @param		���˲���ֵ
//  @return
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------

float Movingaverage_filter(float value,float Filter_buff[])
{
    int8_t i = 0;//����
    float temp = value;
    float Filter_sum = 0;
    Filter_buff[Filter_N] = temp;

    for(i = 0; i < Filter_N; i++)
    {
        Filter_buff[i] = Filter_buff[i+1];		//��������
        Filter_sum += Filter_buff[i];
    }
    temp = Filter_sum / Filter_N;
    return temp;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ͨ�˲�
//  @param      ���˲���ֵ
//  @return     �˲�ֵ
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------

float RCFilter(float value,RC_Filter_pt Filter)
{
    Filter->temp = value;
    Filter->value = (1 - Filter->RC) * Filter->value + Filter->RC * Filter->temp;
//	temp = RC * value + (1 - RC) * temp;
    return Filter->value;
}

