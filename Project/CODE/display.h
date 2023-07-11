#ifndef _display_
#define _display_
#include "headfile.h"


extern rt_mailbox_t display_mailbox;
extern char taget_Big_category[10];



typedef struct
{
    uint8 current;	//��ǰ״̬������
    uint8 next; 		//����һ��
    uint8 enter; 	//ȷ��
		uint8 back; 		//�˳�
    void (*current_operation)(void); //��ǰ״̬Ӧ��ִ�еĲ���
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




