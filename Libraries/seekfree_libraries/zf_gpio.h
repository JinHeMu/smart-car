/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		gpio
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/
 
#ifndef _zf_gpio_h
#define _zf_gpio_h

#include "common.h"
#include "MIMXRT1064.h"
#include "fsl_gpio.h"
#include "zf_iomuxc.h"

extern GPIO_Type * PORTPTR[];

#define GPIO_PIN_CONFIG         SPEED_100MHZ | DSE_R0 | PULLUP_47K | PULL_EN	//�궨��GPIO���ŵ�Ĭ�����ã����ڳ�ʼ��GPIOʱ������д�����������Ҫ���������������޸�
#define FAST_GPIO_PIN_CONFIG    SPEED_200MHZ | DSE_R0 | PULLUP_47K | PULL_EN    //�궨�����GPIO���ŵ�Ĭ�����ã����ڳ�ʼ��GPIOʱ������д�����������Ҫ���������������޸�
#define GPIO_INT_CONFIG         SPEED_100MHZ | HYS_EN | PULLUP_22K | PULL_EN    //�궨��GPIO�ж����ŵ�Ĭ�����ã����ڳ�ʼ��GPIO�ж�ʱ������д�����������Ҫ���������������޸�

typedef enum    // ö�ٴ�����ʽ
{
    NO_INT,     //�ر��ж�ģʽ
    LOW,
    HIGH,
    RISING,
    FALLING,
    BOTH,       //��PINTģ�� ���ô˴�����ʽ
}TRIGGER_enum;



//------------------------------------------------------
//ͨ��GPIO����
void gpio_init(PIN_enum pin, GPIODIR_enum dir, uint8 dat, uint32 pinconf);
void gpio_set(PIN_enum pin, uint8 dat);
uint8 gpio_get(PIN_enum pin);
void gpio_dir(PIN_enum pin, GPIODIR_enum dir);
void gpio_toggle(PIN_enum pin);

#define PORTB_DR           		GPIO1->DR                   //B�˿���������Ĵ���
#define PORTC_DR           		GPIO2->DR                   //C�˿���������Ĵ���
#define PORTD_DR           		GPIO3->DR                   //D�˿���������Ĵ���



//GPIO�жϳ�ʼ��
void gpio_interrupt_init(PIN_enum pin, TRIGGER_enum trigger, uint32 pinconf);


//�жϱ�־λ��ȡ 
#define GET_GPIO_FLAG(pin)      ((GPIO_GetPinsInterruptFlags(PORTPTR[pin>>5]) >> (pin&0x1f)) & 0x01)
//�жϱ�־λ���   
#define CLEAR_GPIO_FLAG(pin)    GPIO_ClearPinsInterruptFlags(PORTPTR[pin>>5], (uint32)1<<(pin&0x1f));


//------------------------------------------------------
//���º�������ʵ�ֿ���GPIO�Ĳ���������GPIO��߿ɴ�150M��IO��ת�ٶ�

void fast_gpio_init(PIN_enum pin, GPIODIR_enum dir, uint8 dat, uint32 pinconf);
void fast_gpio_set(PIN_enum pin, uint8 dat);
uint8 fast_gpio_get(PIN_enum pin);
void fast_gpio_dir(PIN_enum pin, GPIODIR_enum dir);
void fast_gpio_toggle(PIN_enum pin);

#define PORTB_FAST_DR           GPIO6->DR                   //B�˿���������Ĵ���
#define PORTC_FAST_DR           GPIO7->DR                   //C�˿���������Ĵ���
#define PORTD_FAST_DR           GPIO8->DR                   //D�˿���������Ĵ���

#define PORTB_FAST_PSR          GPIO6->PSR                  //B�˿���������Ĵ���
#define PORTC_FAST_PSR          GPIO7->PSR                  //C�˿���������Ĵ���
#define PORTD_FAST_PSR          GPIO8->PSR                  //D�˿���������Ĵ���

#define PORTB_FAST_SET(X)       GPIO6->DR_SET    = 1<<X     //B�˿ڿ���GPIO��һ�Ĵ���     Xѡ��˿ں� 5����ʾB5
#define PORTC_FAST_SET(X)       GPIO7->DR_SET    = 1<<X     //C�˿ڿ���GPIO��һ�Ĵ���     Xѡ��˿ں� 5����ʾC5
#define PORTD_FAST_SET(X)       GPIO8->DR_SET    = 1<<X     //D�˿ڿ���GPIO��һ�Ĵ���     Xѡ��˿ں� 5����ʾD5
                                                                                          
#define PORTB_FAST_CLR(X)       GPIO6->DR_CLEAR  = 1<<X     //B�˿ڿ���GPIO�õͼĴ���     Xѡ��˿ں� 5����ʾB5
#define PORTC_FAST_CLR(X)       GPIO7->DR_CLEAR  = 1<<X     //C�˿ڿ���GPIO�õͼĴ���     Xѡ��˿ں� 5����ʾC5
#define PORTD_FAST_CLR(X)       GPIO8->DR_CLEAR  = 1<<X     //D�˿ڿ���GPIO�õͼĴ���     Xѡ��˿ں� 5����ʾD5
                                                                                          
#define PORTB_FAST_TOGGLE(X)    GPIO6->DR_TOGGLE = 1<<X     //B�˿ڿ���GPIOȡ���Ĵ���     Xѡ��˿ں� 5����ʾB5
#define PORTC_FAST_TOGGLE(X)    GPIO7->DR_TOGGLE = 1<<X     //C�˿ڿ���GPIOȡ���Ĵ���     Xѡ��˿ں� 5����ʾC5
#define PORTD_FAST_TOGGLE(X)    GPIO8->DR_TOGGLE = 1<<X     //D�˿ڿ���GPIOȡ���Ĵ���     Xѡ��˿ں� 5����ʾD5






#endif
