#ifndef _CARMOVE_H
#define _CARMOVE_H

#include "headfile.h"


typedef struct Pose_car {
    //已到达的目标点
    int8_t Position_Pointer;
    //当前速度
    double Speed_X;
    double Speed_Y;
    double Speed_Z;
    //当前位置
    float current_x;
    float current_y;
    //目标位置
    float target_x;
    float target_y;
    //当前姿态
    float current_angle;
    //目标姿态
    double target_angle;
    //目标距离
    float target_distanceX;
    float target_distanceY;
    //距上次转向之后前进的距离(里程）
    float MileageX;
    float MileageY;
}Pose_car;


struct point{
	uint8 x;
	uint8 y;
};

//extern struct point cur_point;//仅在路径规划函数里用
//extern struct point tar_point[point_num];
//extern struct point TAR_POINT[point_num];

extern Pose_car car;


void route_planning_init();

void uart_coordinate_transforming(uint8 ART1_POINT_X[15], uint8 ART1_POINT_Y[15], uint8 num);
void nearest_neighbor_sort(struct point* arr, int size);







#endif





