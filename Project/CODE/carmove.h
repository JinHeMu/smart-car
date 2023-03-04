#ifndef _CARMOVE_H
#define _CARMOVE_H

#include "headfile.h"


typedef struct location_goal {
    //已到达的目标点
    int8_t Position_Pointer;
    //当前速度
    double Speed_X;
    double Speed_Y;
    double Speed_Z;
    //当前位置
    float x;
    float y;
    //目标位置
    float x1;
    float y1;
    //当前姿态
    float Angel;
    //目标姿态
    double Angel_Target;
    //目标距离
    float DistanceX;
    float DistanceY;
    //距上次转向之后前进的距离(里程）
    float MileageX;
    float MileageY;
}location_goal;

extern location_goal car;







#endif





