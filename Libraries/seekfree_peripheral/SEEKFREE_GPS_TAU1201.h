/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2021,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				SEEKFREE_GPS_TAU1201.c
* @company			�ɶ���ɿƼ����޹�˾
* @author			��ɿƼ�(QQ3184284598)
* @version			�鿴doc��version�ļ� �汾˵��
* @Software			IAR 8.32.4 or MDK 5.28
* @Taobao			https://seekfree.taobao.com/
* @date				2021-12-30
* @note
* 					���߶��壺
* 					------------------------------------
* 					ģ��ܽ�			��Ƭ���ܽ�
* 					RX					�鿴 GPS_TAU1201_RX�궨�������
* 					TX                  �鿴 GPS_TAU1201_TX�궨�������
* 					GND					��Դ��
* 					VCC				    5V��Դ
* 					------------------------------------
********************************************************************************************************************/
#ifndef _SEEKFREE_GPS_TAU1201_h
#define _SEEKFREE_GPS_TAU1201_h

#include "common.h"


//--------------------------------------------------------------------------------------------------
//��������
//--------------------------------------------------------------------------------------------------
#define GPS_TAU1201_UART    USART_8
#define GPS_TAU1201_RX      UART8_TX_D16    // GPS RX�������ӵ���Ƭ����
#define GPS_TAU1201_TX      UART8_RX_D17    // GPS TX��������



#define ANGLE_TO_RAD(X)    (X * PI / 180.0)//�Ƕ�ת��Ϊ����
#define RAD_TO_ANGLE(X)    (X * 180.0 / PI)//����ת��Ϊ�Ƕ�
#define PI                  3.1415926535898



typedef struct{
	uint16 year;  
	uint8  month; 
	uint8  day;
	uint8  hour;
	uint8  minute;
	uint8  second;
}gps_time_struct;

typedef struct{
    gps_time_struct    time;        //ʱ��
    
    uint8       state;              //��Ч״̬  1����λ��Ч  0����λ��Ч
    
    uint16      latitude_degree;	//��
	uint16      latitude_cent;		//��
	uint16      latitude_second;    //��
	uint16      longitude_degree;	//��
	uint16      longitude_cent;		//��
	uint16      longitude_second;   //��
    
    double      latitude;           //����
    double      longitude;          //γ��
    
    int8 	    ns;                 //γ�Ȱ��� N�������򣩻� S���ϰ���
    int8 	    ew;                 //���Ȱ��� E���������� W��������
    
	float 	    speed;              //�ٶȣ�����/ÿСʱ��
    float 	    direction;          //���溽��000.0~359.9 �ȣ����汱��Ϊ�ο���׼��
    
    //������������Ϣ��GNGGA����л�ȡ
    uint8       satellite_used;     //���ڶ�λ����������
	float 	    height;             //�߶�   
}gps_info_struct;



extern gps_info_struct gps_tau1201;
extern uint8 gps_tau1201_flag;



double  get_two_points_distance(double lat1, double lng1, double lat2, double lng2);
double  get_two_points_azimuth(double lat1, double lon1, double lat2, double lon2);
void    gps_init(void);













#endif  
