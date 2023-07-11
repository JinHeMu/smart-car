#ifndef _display_
#define _display_
#include "headfile.h"


extern rt_mailbox_t display_mailbox;
extern char taget_Big_category[10];



typedef struct
{
    uint8 current;	//当前状态索引号
    uint8 next; 		//向下一个
    uint8 enter; 	//确定
		uint8 back; 		//退出
    void (*current_operation)(void); //当前状态应该执行的操作
} Menu_table;



void home();
void GUI_motor();
void GUI_pid();
void GUI_route();
void GUI_motor_value();
void GUI_pid_value();
void GUI_imu_ra_value();
void GUI_route_value();
void GUI_imu_ra();
void GUI_arm();
void GUI_arm_value();
 

void  Menu_key_set(void);
 
void display_init(void);

#endif




