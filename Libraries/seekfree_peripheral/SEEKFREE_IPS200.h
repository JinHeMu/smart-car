/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		IPS 2.0寸液晶
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		IAR 8.3 or MDK 5.26
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-05-07
 * @note		
             接线定义：
    ------------------------------------------------------------------------
*                                                           模块管脚                                             单片机管脚
*                   // 双排排针 并口两寸屏 硬件引脚
*                   RD                  查看 SEEKFREE_IPS200.h 中 IPS200_RD_PIN_PARALLEL8     宏定义
*                   WR                  查看 SEEKFREE_IPS200.h 中 IPS200_WR_PIN_PARALLEL8     宏定义
*                   RS                  查看 SEEKFREE_IPS200.h 中 IPS200_RS_PIN_PARALLEL8     宏定义
*                   RST                 查看 SEEKFREE_IPS200.h 中 IPS200_RST_PIN_PARALLEL8    宏定义
*                   CS                  查看 SEEKFREE_IPS200.h 中 IPS200_CS_PIN_PARALLEL8     宏定义
*                   BL                  查看 SEEKFREE_IPS200.h 中 IPS200_BL_PIN_PARALLEL8     宏定义
*                   D0-D7               查看 SEEKFREE_IPS200.h 中 IPS200_Dx_PIN_PARALLEL8     宏定义
*                   VCC                 3.3V电源
*                   GND                 电源地
*                   // 单排排针 SPI 两寸屏 硬件引脚
*                   SCL                 查看 SEEKFREE_IPS200.h 中 IPS200_SCL_PIN_SPI  宏定义
*                   SDA                 查看 SEEKFREE_IPS200.h 中 IPS200_SDA_PIN_SPI  宏定义
*                   RST                 查看 SEEKFREE_IPS200.h 中 IPS200_RST_PIN_SPI  宏定义
*                   DC                  查看 SEEKFREE_IPS200.h 中 IPS200_DC_PIN_SPI   宏定义
*                   CS                  查看 SEEKFREE_IPS200.h 中 IPS200_CS_PIN_SPI   宏定义
*                   BL                  查看 SEEKFREE_IPS200.h 中 IPS200_BL_PIN_SPI  宏定义
*                   VCC                 3.3V电源
*                   GND                 电源地
*                                                           最大分辨率 320 * 240
*   ------------------------------------------------------------------------
 ********************************************************************************************************************/

#ifndef _SEEKFREE_IPS200_H
#define _SEEKFREE_IPS200_H

#include "common.h"
#include "SEEKFREE_FONT.h"
#include "zf_spi.h"
     
//-------常用颜色在SEEKFREE_FONT.h文件中定义----------
//#define RED          	    0xF800	//红色
//#define BLUE         	    0x001F  //蓝色
//#define YELLOW       	    0xFFE0	//黄色
//#define GREEN        	    0x07E0	//绿色
//#define WHITE        	    0xFFFF	//白色
//#define BLACK        	    0x0000	//黑色 
//#define GRAY  			0X8430 	//灰色
//#define BROWN 			0XBC40 	//棕色
//#define PURPLE    		0XF81F	//紫色
//#define PINK    		    0XFE19	//粉色


#define IPS200_BGCOLOR      WHITE	//背景颜色
#define IPS200_PENCOLOR     RED		//画笔颜色

// 如果使用的是单排排针的两寸屏幕 SPI 驱动控制引脚 可以修改
#define IPS200_SPIN             SPI_3           //定义使用的SPI号
#define IPS200_SCL_PIN_SPI      SPI3_SCK_B0 //定义SPI_SCK引脚
#define IPS200_SDA_PIN_SPI      SPI3_MOSI_B1 //定义SPI_MOSI引脚
#define IPS200_SDA_IN_PIN_SPI   SPI3_MISO_B2 //定义SPI_MISO引脚  IPS没有MISO引脚，但是这里任然需要定义，在spi的初始化时需要使用
#define IPS200_CS_PIN_SPI       SPI3_CS0_B3  //定义SPI_CS引脚

// 如果使用的是单排排针的两寸屏幕 SPI 驱动控制引脚 可以修改
#define IPS200_RST_PIN_SPI      B2   // 液晶复位引脚定义
#define IPS200_DC_PIN_SPI       C19
#define IPS200_BL_PIN_SPI       C18


//引脚只能选择B C D这三个端口中的引脚，便于使用快速GPIO以提高显示速度
#define IPS200_RD_PIN_PARALLEL8  		B0
#define IPS200_WR_PIN_PARALLEL8  		B1
#define IPS200_RS_PIN_PARALLEL8  		B2
#define IPS200_RST_PIN_PARALLEL8 		C19
#define IPS200_CS_PIN_PARALLEL8  		B3
#define IPS200_BL_PIN_PARALLEL8  		C18


//定义屏幕数据引脚连接的端口数量
#define IPS200_PORT_NUM    2    //可设置参数1：仅与单片机的一个端口连接  2：与单片机的两个端口连接


//-------------------------数据端口都在同一个PORT上--------------------
#if(1==IPS200_PORT_NUM)

//引脚只能选择B C D这三个端口中的引脚，便于使用快速GPIO以提高显示速度
//请根据IPS200_PORT_NUM宏定义选着合适的引脚
//IPS200_PORT_NUM为1时，8个数据引脚必须连续 例如C0-C7,C6-C13等等。
//切换引脚后注意修改IPS200_DATA_PORT宏定义
#define IPS200_D0_PIN_PARALLEL8 		C8
#define IPS200_D1_PIN_PARALLEL8 		C9
#define IPS200_D2_PIN_PARALLEL8 		C10
#define IPS200_D3_PIN_PARALLEL8 		C11
#define IPS200_D4_PIN_PARALLEL8 		C12
#define IPS200_D5_PIN_PARALLEL8 		C13
#define IPS200_D6_PIN_PARALLEL8 		C14
#define IPS200_D7_PIN_PARALLEL8 		C15

//定义数据端口所在PORT，切换引脚后务必根据引脚所在PORT进行更改
//默认数据引脚使用的是C端口，因此这里默认设置为2
#define IPS200_DATA_PORT    2       //1：B端口  2：C端口  3：D端口
                            
#if  ( 1 == IPS200_DATA_PORT )
    #define IPS200_DATAPORT PORTB_FAST_DR	    //宏定义快速GPIO的输出寄存器
#elif( 2 == IPS200_DATA_PORT )
    #define IPS200_DATAPORT PORTC_FAST_DR	    //宏定义快速GPIO的输出寄存器
#elif( 3 == IPS200_DATA_PORT )
    #define IPS200_DATAPORT PORTD_FAST_DR	    //宏定义快速GPIO的输出寄存器
#endif

#define DATA_START_NUM		(IPS200_D0_PIN_PARALLEL8&0x1f)  //宏定义数据引脚的起始编号


//-------------------------数据端口都在两个个PORT上--------------------
//允许D0-D3在一个端口上，D4-D7在另外一个端口上
#elif(2==IPS200_PORT_NUM)

//IPS200_PORT_NUM为2时，数据引脚前四个与后四个可以接在单片机的不同端口上，
//D0-D3四个数据引脚必须连续 例如C0-C3,C1-C4等等，
//D4-D7四个数据引脚必须连续 例如B0-B3,B1-B4等等。
//可以连接到不同端口的意思就是屏幕的D0-D3与C1-C4连接，D4-D7与B2-B5连接。
//切换引脚后注意修改IPS200_DATA_PORT1和IPS200_DATA_PORT2宏定义
#define IPS200_D0_PIN_PARALLEL8 		B16
#define IPS200_D1_PIN_PARALLEL8 		B17
#define IPS200_D2_PIN_PARALLEL8 		B18
#define IPS200_D3_PIN_PARALLEL8 		B19
#define IPS200_D4_PIN_PARALLEL8 		D12
#define IPS200_D5_PIN_PARALLEL8 		D13
#define IPS200_D6_PIN_PARALLEL8 		D14
#define IPS200_D7_PIN_PARALLEL8 		D15

//定义数据端口所在PORT，切换引脚后务必根据引脚所在PORT进行更改
#define IPS200_DATA_PORT1    1       //1：B端口  2：C端口  3：D端口
                            
#if  ( 1 == IPS200_DATA_PORT1 )
    #define IPS200_DATAPORT1 PORTB_FAST_DR	    //宏定义快速GPIO的输出寄存器
#elif( 2 == IPS200_DATA_PORT1 )
    #define IPS200_DATAPORT1 PORTC_FAST_DR	    //宏定义快速GPIO的输出寄存器
#elif( 3 == IPS200_DATA_PORT1 )
    #define IPS200_DATAPORT1 PORTD_FAST_DR	    //宏定义快速GPIO的输出寄存器
#endif

#define DATA_START_NUM1		(IPS200_D0_PIN_PARALLEL8&0x1f)  //宏定义数据引脚的起始编号


#define IPS200_DATA_PORT2    3       //1：B端口  2：C端口  3：D端口
                            
#if  ( 1 == IPS200_DATA_PORT2 )
    #define IPS200_DATAPORT2 PORTB_FAST_DR	    //宏定义快速GPIO的输出寄存器
#elif( 2 == IPS200_DATA_PORT2 )
    #define IPS200_DATAPORT2 PORTC_FAST_DR	    //宏定义快速GPIO的输出寄存器
#elif( 3 == IPS200_DATA_PORT2 )
    #define IPS200_DATAPORT2 PORTD_FAST_DR	    //宏定义快速GPIO的输出寄存器
#endif

#define DATA_START_NUM2		(IPS200_D4_PIN_PARALLEL8&0x1f)  //宏定义数据引脚的起始编号


#endif 







#define IPS200_BL(x)		fast_gpio_set(IPS200_BL_PIN_PARALLEL8,x)
#define IPS200_CS(x)       	fast_gpio_set(IPS200_CS_PIN_PARALLEL8,x)
#define IPS200_RD(x)       	fast_gpio_set(IPS200_RD_PIN_PARALLEL8,x)
#define IPS200_WR(x)       	fast_gpio_set(IPS200_WR_PIN_PARALLEL8,x)
#define IPS200_RS(x)       	fast_gpio_set(IPS200_RS_PIN_PARALLEL8,x)
#define IPS200_RST(x)      	fast_gpio_set(ips_rst_pin,x)

#define IPS200_DC(x)        fast_gpio_set(IPS200_DC_PIN_SPI,x)

#define IPS200_W 240
#define IPS200_H 320


//定义显示方向
//0 竖屏模式
//1 竖屏模式  旋转180°
//2 横屏模式
//3 横屏模式  旋转180°
#define IPS200_DISPLAY_DIR 0

#if (0==IPS200_DISPLAY_DIR || 1==IPS200_DISPLAY_DIR)
#define	IPS200_X_MAX	IPS200_W	//液晶X方宽度
#define IPS200_Y_MAX	IPS200_H   //液晶Y方宽度
     
#elif (2==IPS200_DISPLAY_DIR || 3==IPS200_DISPLAY_DIR)
#define	IPS200_X_MAX	IPS200_H	//液晶X方宽度
#define IPS200_Y_MAX	IPS200_W   //液晶Y方宽度
     
#else
#error "IPS200_DISPLAY_DIR 定义错误"
     
#endif

typedef enum
{
    IPS200_TYPE_SPI,                                                            // SPI 驱动
    IPS200_TYPE_PARALLEL8,                                                      // 并口驱动
}ips200_type_enum;

void ips200_init(ips200_type_enum type_select); //初始化硬件
void ips200_w_data(uint8 dat);
void ips200_wr_reg(uint8 command);
void ips200_wr_data(uint8 dat);
void ips200_wr_data16(uint16 dat);
void ips200_w_reg(uint8 com,uint8 dat);
void ips200_address_set(uint16 x1,uint16 y1,uint16 x2,uint16 y2);
void ips200_clear(uint16 color);
void ips200_drawpoint(uint16 x,uint16 y,uint16 color);
void ips200_showchar(uint16 x,uint16 y,const int8 dat);
void ips200_showstr(uint16 x,uint16 y,const int8 dat[]);


void ips200_showint8(uint16 x,uint16 y,int8 dat);
void ips200_showuint8(uint16 x,uint16 y,uint8 dat);
void ips200_showint16(uint16 x,uint16 y,int16 dat);
void ips200_showuint16(uint16 x,uint16 y,uint16 dat);
void ips200_showint32(uint16 x,uint16 y,int dat,uint8 num);
void ips200_showfloat(uint16 x,uint16 y,double dat,int8 num,int8 pointnum);
void ips200_showimage(uint16 x,uint16 y,uint16 w,uint16 l,const unsigned char *p);

void ips200_displayimage032(uint8 *p, uint16 width, uint16 height);
void ips200_displayimage032_zoom(uint8 *p, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height);
void ips200_displayimage032_zoom1(uint8 *p, uint16 width, uint16 height, uint16 start_x, uint16 start_y, uint16 dis_width, uint16 dis_height);
void ips200_displayimage8660_zoom(uint16 *p, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height);
void ips200_displayimage8660_zoom1(uint16 *p, uint16 width, uint16 height, uint16 start_x, uint16 start_y, uint16 dis_width, uint16 dis_height);
void ips200_displayimage8660_grayscale_zoom(uint16 *p, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height);
void ips200_displayimage7725(uint8 *p, uint16 width, uint16 height);
void ips200_display_chinese(uint16 x, uint16 y, uint8 size, const uint8 *p, uint8 number, uint16 color);

#endif

