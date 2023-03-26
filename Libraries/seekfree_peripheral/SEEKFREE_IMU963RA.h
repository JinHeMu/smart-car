/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2022,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_imu963ra
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ3184284598)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         IAR 8.32.4 or MDK 5.28
* @Target core      RT1064
* @Taobao           https://seekfree.taobao.com/
* @date             2022-04-14
* @note             ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�            ��Ƭ���ܽ�
*                   // Ӳ�� SPI ����
*                   SCL/SPC             �鿴 zf_device_imu963ra.h �� IMU963RA_SPC_PIN �궨��
*                   SDA/DSI             �鿴 zf_device_imu963ra.h �� IMU963RA_SDI_PIN �궨��
*                   SA0/SDO             �鿴 zf_device_imu963ra.h �� IMU963RA_SDO_PIN �궨��
*                   CS                  �鿴 zf_device_imu963ra.h �� IMU963RA_CS_PIN  �궨��
*                   VCC                 3.3V��Դ
*                   GND                 ��Դ��
*                   ������������
*
*                   // ��� IIC ����
*                   SCL/SPC             �鿴 SEEKFREE_IIC.h �� SEEKFREE_SCL �궨��
*                   SDA/DSI             �鿴 SEEKFREE_IIC.h �� SEEKFREE_SDA �궨��
*                   VCC                 3.3V��Դ
*                   GND                 ��Դ��
*                   ������������
*                   ------------------------------------
********************************************************************************************************************/

#ifndef _SEEKFREE_IMU963RA_H
#define _SEEKFREE_IMU963RA_H




#include "common.h"

#define IMU963RA_USE_SOFT_IIC                       (0)                 // Ĭ��ʹ��Ӳ�� SPI ��ʽ����

#if IMU963RA_USE_SOFT_IIC                                               // ������ ��ɫ�����Ĳ�����ȷ�� ��ɫ�ҵľ���û���õ�
//====================================================��� IIC ����====================================================
#define IMU963RA_SOFT_IIC_DELAY                     (60)              // ��� IIC ��ʱ����ʱ���� ��ֵԽС IIC ͨ������Խ��
//#define IMU963RA_SCL_PIN                          �鿴 SEEKFREE_IIC.h �� SEEKFREE_SCL �궨��
//#define IMU963RA_SDA_PIN                          �鿴 SEEKFREE_IIC.h �� SEEKFREE_SDA �궨��
//====================================================��� IIC ����====================================================
#else       
//====================================================Ӳ�� SPI ����====================================================
#define IMU963RA_SPI_SPEED                          (10*1000*1000)      // Ӳ�� SPI ����
#define IMU963RA_SPI                                (SPI_4        )     // Ӳ�� SPI ��
#define IMU963RA_SPC_PIN                            (SPI4_SCK_C23 )     // Ӳ�� SPI SCK ����
#define IMU963RA_SDI_PIN                            (SPI4_MOSI_C22)     // Ӳ�� SPI MOSI ����
#define IMU963RA_SDO_PIN                            (SPI4_MISO_C21)     // Ӳ�� SPI MISO ����
#define IMU963RA_CS_PIN                             (SPI4_CS0_C20)      // Ӳ�� CS Ƭѡ����
//====================================================Ӳ�� SPI ����====================================================
#endif
       
//#define IMU963RA_TIMEOUT_COUNT                      0x00FF              // IMU963RA ��ʱ����
        
//================================================���� IMU963RA �ڲ���ַ================================================
#define IMU963RA_DEV_ADDR                           0x6B                // SA0�ӵأ�0x6A SA0������0x6B ģ��Ĭ������
#define IMU963RA_SPI_W                              0x00
#define IMU963RA_SPI_R                              0x80

#define IMU963RA_FUNC_CFG_ACCESS 			        0x01
#define IMU963RA_PIN_CTRL 					        0x02
#define IMU963RA_S4S_TPH_L 					        0x04
#define IMU963RA_S4S_TPH_H 					        0x05
#define IMU963RA_S4S_RR 					        0x06
#define IMU963RA_FIFO_CTRL1 				        0x07
#define IMU963RA_FIFO_CTRL2 				        0x08
#define IMU963RA_FIFO_CTRL3 				        0x09
#define IMU963RA_FIFO_CTRL4 				        0x0A
#define IMU963RA_COUNTER_BDR_REG1 			        0x0B
#define IMU963RA_COUNTER_BDR_REG2 			        0x0C
#define IMU963RA_INT1_CTRL 					        0x0D
#define IMU963RA_INT2_CTRL 					        0x0E
#define IMU963RA_WHO_AM_I 					        0x0F
#define IMU963RA_CTRL1_XL 					        0x10
#define IMU963RA_CTRL2_G					        0x11
#define IMU963RA_CTRL3_C					        0x12
#define IMU963RA_CTRL4_C					        0x13
#define IMU963RA_CTRL5_C					        0x14
#define IMU963RA_CTRL6_C					        0x15
#define IMU963RA_CTRL7_G					        0x16
#define IMU963RA_CTRL8_XL 					        0x17
#define IMU963RA_CTRL9_XL 					        0x18
#define IMU963RA_CTRL10_C 					        0x19
#define IMU963RA_ALL_INT_SRC 				        0x1A
#define IMU963RA_WAKE_UP_SRC 				        0x1B
#define IMU963RA_TAP_SRC 					        0x1C
#define IMU963RA_D6D_SRC 					        0x1D
#define IMU963RA_STATUS_REG 				        0x1E
#define IMU963RA_OUT_TEMP_L 				        0x20
#define IMU963RA_OUT_TEMP_H 				        0x21
#define IMU963RA_OUTX_L_G 					        0x22
#define IMU963RA_OUTX_H_G 					        0x23
#define IMU963RA_OUTY_L_G 					        0x24
#define IMU963RA_OUTY_H_G 					        0x25
#define IMU963RA_OUTZ_L_G 					        0x26
#define IMU963RA_OUTZ_H_G 					        0x27
#define IMU963RA_OUTX_L_A 					        0x28
#define IMU963RA_OUTX_H_A 					        0x29
#define IMU963RA_OUTY_L_A 					        0x2A
#define IMU963RA_OUTY_H_A 					        0x2B
#define IMU963RA_OUTZ_L_A 					        0x2C
#define IMU963RA_OUTZ_H_A 					        0x2D
#define IMU963RA_EMB_FUNC_STATUS_MAINPAGE 	        0x35
#define IMU963RA_FSM_STATUS_A_MAINPAGE 		        0x36
#define IMU963RA_FSM_STATUS_B_MAINPAGE 		        0x37
#define IMU963RA_STATUS_MASTER_MAINPAGE 	        0x39
#define IMU963RA_FIFO_STATUS1 				        0x3A
#define IMU963RA_FIFO_STATUS2 				        0x3B
#define IMU963RA_TIMESTAMP0 				        0x40
#define IMU963RA_TIMESTAMP1 				        0x41
#define IMU963RA_TIMESTAMP2 				        0x42
#define IMU963RA_TIMESTAMP3 				        0x43
#define IMU963RA_TAP_CFG0 					        0x56
#define IMU963RA_TAP_CFG1 					        0x57
#define IMU963RA_TAP_CFG2 					        0x58
#define IMU963RA_TAP_THS_6D 				        0x59
#define IMU963RA_INT_DUR2 					        0x5A
#define IMU963RA_WAKE_UP_THS 				        0x5B
#define IMU963RA_WAKE_UP_DUR 				        0x5C
#define IMU963RA_FREE_FALL   				        0x5D
#define IMU963RA_MD1_CFG     				        0x5E
#define IMU963RA_MD2_CFG     				        0x5F
#define IMU963RA_S4S_ST_CMD_CODE 			        0x60
#define IMU963RA_S4S_DT_REG 				        0x61
#define IMU963RA_I3C_BUS_AVB 				        0x62
#define IMU963RA_INTERNAL_FREQ_FINE 		        0x63
#define IMU963RA_INT_OIS   					        0x6F
#define IMU963RA_CTRL1_OIS 					        0x70
#define IMU963RA_CTRL2_OIS 					        0x71
#define IMU963RA_CTRL3_OIS 					        0x72
#define IMU963RA_X_OFS_USR 					        0x73
#define IMU963RA_Y_OFS_USR 					        0x74
#define IMU963RA_Z_OFS_USR 					        0x75
#define IMU963RA_FIFO_DATA_OUT_TAG 			        0x78
#define IMU963RA_FIFO_DATA_OUT_X_L 			        0x79
#define IMU963RA_FIFO_DATA_OUT_X_H 			        0x7A
#define IMU963RA_FIFO_DATA_OUT_Y_L 			        0x7B
#define IMU963RA_FIFO_DATA_OUT_Y_H 			        0x7C
#define IMU963RA_FIFO_DATA_OUT_Z_L 			        0x7D
#define IMU963RA_FIFO_DATA_OUT_Z_H 			        0x7E

//������������ؼĴ��� ��Ҫ��FUNC_CFG_ACCESS��SHUB_REG_ACCESSλ����Ϊ1������ȷ����
#define IMU963RA_SENSOR_HUB_1 				        0x02 
#define IMU963RA_SENSOR_HUB_2 				        0x03 
#define IMU963RA_SENSOR_HUB_3 				        0x04 
#define IMU963RA_SENSOR_HUB_4 				        0x05 
#define IMU963RA_SENSOR_HUB_5 				        0x06 
#define IMU963RA_SENSOR_HUB_6 				        0x07 
#define IMU963RA_SENSOR_HUB_7 				        0x08 
#define IMU963RA_SENSOR_HUB_8 				        0x09 
#define IMU963RA_SENSOR_HUB_9 				        0x0A 
#define IMU963RA_SENSOR_HUB_10 				        0x0B 
#define IMU963RA_SENSOR_HUB_11 				        0x0C 
#define IMU963RA_SENSOR_HUB_12 				        0x0D 
#define IMU963RA_SENSOR_HUB_13 				        0x0E 
#define IMU963RA_SENSOR_HUB_14 				        0x0F 
#define IMU963RA_SENSOR_HUB_15 				        0x10 
#define IMU963RA_SENSOR_HUB_16 				        0x11 
#define IMU963RA_SENSOR_HUB_17 				        0x12 
#define IMU963RA_SENSOR_HUB_18 				        0x13 
#define IMU963RA_MASTER_CONFIG 				        0x14 
#define IMU963RA_SLV0_ADD 					        0x15 
#define IMU963RA_SLV0_SUBADD 				        0x16 
#define IMU963RA_SLV0_CONFIG 				        0x17 
#define IMU963RA_SLV1_ADD 					        0x18 
#define IMU963RA_SLV1_SUBADD 				        0x19 
#define IMU963RA_SLV1_CONFIG 				        0x1A 
#define IMU963RA_SLV2_ADD 					        0x1B 
#define IMU963RA_SLV2_SUBADD 				        0x1C 
#define IMU963RA_SLV2_CONFIG 				        0x1D 
#define IMU963RA_SLV3_ADD 					        0x1E 
#define IMU963RA_SLV3_SUBADD 				        0x1F 
#define IMU963RA_SLV3_CONFIG 				        0x20 
#define IMU963RA_DATAWRITE_SLV0 			        0x21 
#define IMU963RA_STATUS_MASTER 				        0x22

#define IMU963RA_MAG_ADDR 			                0x0D            //7λIIC��ַ
#define IMU963RA_MAG_OUTX_L 			            0x00
#define IMU963RA_MAG_CONTROL1 			            0x09
#define IMU963RA_MAG_CONTROL2 			            0x0A
#define IMU963RA_MAG_FBR 			                0x0B
#define IMU963RA_MAG_CHIP_ID 			            0x0D


extern int16 imu963ra_acc_x,  imu963ra_acc_y,  imu963ra_acc_z;
extern int16 imu963ra_gyro_x, imu963ra_gyro_y, imu963ra_gyro_z;
extern int16 imu963ra_mag_x,  imu963ra_mag_y,  imu963ra_mag_z;

void imu963ra_get_acc (void);
void imu963ra_get_gyro (void);
void imu963ra_get_mag (void);

uint8 imu963ra_init (void);




#endif
