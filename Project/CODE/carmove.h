#ifndef _CARMOVE_H
#define _CARMOVE_H

#include "headfile.h"


typedef struct Pose_car {
    //�ѵ����Ŀ���
    int8_t Position_Pointer;
    //��ǰ�ٶ�
    double Speed_X;
    double Speed_Y;
    double Speed_Z;
    float correct_speed;
    //��ǰλ��
    float current_x;
    float current_y;
    //Ŀ��λ��
    float target_x;
    float target_y;
    //ʶ������λ��
    float target_carry_x;
    float target_carry_y;
    //��ǰ��̬
    float current_angle;
    //Ŀ����̬
    double target_angle;
    //Ŀ�����
    float target_distanceX;
    float target_distanceY;
    //���
    float MileageX;
    float MileageY;
    float MileageZ;
}Pose_car;


struct point{
	uint8 x;
	uint8 y;
};

//extern struct point cur_point;//����·���滮��������
//extern struct point tar_point[point_num];
//extern struct point TAR_POINT[point_num];

extern Pose_car car;
extern uint8 running_mode;//С������ģʽ
extern rt_sem_t recognize_sem;

void route_planning_init();

void uart_coordinate_transforming(uint8 ART1_POINT_X[15], uint8 ART1_POINT_Y[15], uint8 num);
void static_planning(struct point* arr, int size);







#endif





