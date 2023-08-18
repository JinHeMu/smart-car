#ifndef _CARMOVE2_H
#define _CARMOVE2_H

#include "headfile.h"

#define field_width 35
#define field_height 25


#define A 0x41
#define B 0x42
#define C 0x43
#define COLUMN 0x44
#define ROW 0x45

#define UP 1
#define RIGHT 2
#define LOW 3
#define LEFT 4

#define boundry_speed 300


typedef struct Pose_car {
   //�ѵ����Ŀ���
   int8_t Position_Pointer;
   //��ǰ�ٶ�
   double Speed_X;
   double Speed_Y;
   double Speed_Z;
   float correct_speed;
     
   //��ǰ��̬
   float current_angle;

   //Ŀ��λ��
   float target_x;
   float target_y;    
   //Ŀ����̬
   float target_angle;

   //ʶ������λ��
   float target_carry_x;
   float target_carry_y;


   //Ŀ�����
   float target_distanceX;
   float target_distanceY;
   //���
   float MileageX;
   float MileageY;
}Pose_car;


typedef struct point{
	uint8 x;
	uint8 y;
}point;

typedef struct card {
	
	char Big_category[10];
	char Small_category[10];
	uint8 Box_location;
	int16 Target_x;
	int16 Target_y;

}card;


typedef struct unknowcard{
	

	float Current_x;//发现卡片的x里程计坐标
	float Current_y;//发现卡片的y里程计坐标
  int16 Distance;//发现卡片距离镜头的距离

}unknowcard;



extern Pose_car car;


extern rt_sem_t back_sem;
extern rt_sem_t arrive_sem;        // 到达
extern rt_sem_t boundry_sem;        // 边线
extern rt_sem_t correct_sem;       // 矫正
extern rt_sem_t recognize_sem;     // 识别
extern rt_sem_t obj_detection_sem; // 目标检测


extern uint8 game_mode;
extern uint8 find_mode;


void car_start_init(void);
void car_move(float tar_x, float tar_y);
void car_turn(float angle);
void car_turnto(float angle);
void car_moveto_boundry(uint8 mode, uint16 speed);

void car_speed_x(int16 tar_speed);
void car_speed_y(int16 tar_speed);







#endif





