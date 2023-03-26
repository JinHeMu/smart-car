/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		common
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/
 
 
 
#ifndef _common_h
#define _common_h

#include "fsl_common.h"

//������������
typedef unsigned char       uint8;   //  8 bits 
typedef unsigned short int  uint16;  // 16 bits 
typedef unsigned long int   uint32;  // 32 bits 
typedef unsigned long long  uint64;  // 64 bits 

typedef char                int8;    //  8 bits 
typedef short int           int16;   // 16 bits 
typedef long  int           int32;   // 32 bits 
typedef long  long          int64;   // 64 bits 

typedef volatile int8       vint8;   //  8 bits 
typedef volatile int16      vint16;  // 16 bits 
typedef volatile int32      vint32;  // 32 bits 
typedef volatile int64      vint64;  // 64 bits 

typedef volatile uint8      vuint8;  //  8 bits 
typedef volatile uint16     vuint16; // 16 bits 
typedef volatile uint32     vuint32; // 32 bits 
typedef volatile uint64     vuint64; // 64 bits 


typedef enum //ö�ٶ˿ڷ���
{
    A0,  A1,  A2,  A3,  A4,  A5,  A6,  A7, //RT1064 û��GPIO0�˿� ����������ڱ���ռλ
    A8,  A9,  A10, A11, A12, A13, A14, A15,//RT1064 û��GPIO0�˿� ����������ڱ���ռλ
    A16, A17, A18, A19, A20, A21, A22, A23,//RT1064 û��GPIO0�˿� ����������ڱ���ռλ
    A24, A25, A26, A27, A28, A29, A30, A31,//RT1064 û��GPIO0�˿� ����������ڱ���ռλ
    
    B0,  B1,  B2,  B3,  B4,  B5,  B6,  B7,  //GPIO1�˿� 0-7
    B8,  B9,  B10, B11, B12, B13, B14, B15, //GPIO1�˿� 8-15
    B16, B17, B18, B19, B20, B21, B22, B23, //GPIO1�˿� 16-23
    B24, B25, B26, B27, B28, B29, B30, B31, //GPIO1�˿� 24-31
    
    C0,  C1,  C2,  C3,  C4,  C5,  C6,  C7,  //GPIO2�˿� 0-7
    C8,  C9,  C10, C11, C12, C13, C14, C15, //GPIO2�˿� 8-15
    C16, C17, C18, C19, C20, C21, C22, C23, //GPIO2�˿� 16-23
    C24, C25, C26, C27, C28, C29, C30, C31, //GPIO2�˿� 24-31
    
    D0,  D1,  D2,  D3,  D4,  D5,  D6,  D7,  //GPIO3�˿� 0-7
    D8,  D9,  D10, D11, D12, D13, D14, D15, //GPIO3�˿� 8-15
    D16, D17, D18, D19, D20, D21, D22, D23, //GPIO3�˿� 16-23
    D24, D25, D26, D27, D28, D29, D30, D31, //GPIO3�˿� 24-31
    
    E0,  E1,  E2,  E3,  E4,  E5,  E6,  E7,  //RT1064 û��GPIO4�˿� ����������ڱ���ռλ
    E8,  E9,  E10, E11, E12, E13, E14, E15, //RT1064 û��GPIO4�˿� ����������ڱ���ռλ
    E16, E17, E18, E19, E20, E21, E22, E23, //RT1064 û��GPIO4�˿� ����������ڱ���ռλ
    E24, E25, E26, E27, E28, E29, E30, E31, //RT1064 û��GPIO4�˿� ����������ڱ���ռλ
    
    F0,  F1,  F2,  F3,  F4,  F5,  F6,  F7,  //GPIO5�˿� 0-7
    F8,  F9,  F10, F11, F12, F13, F14, F15, //GPIO5�˿� 8-15
    F16, F17, F18, F19, F20, F21, F22, F23, //GPIO5�˿� 16-23
    F24, F25, F26, F27, F28, F29, F30, F31, //GPIO5�˿� 24-31
    
    G0,  G1,  G2,  G3,  G4,  G5,  G6,  G7,  //GPIO6�˿� 0-7   //����GPIO�˿�  ��Ӧ�˿�B
    G8,  G9,  G10, G11, G12, G13, G14, G15, //GPIO6�˿� 8-15  //����GPIO�˿�  ��Ӧ�˿�B
    G16, G17, G18, G19, G20, G21, G22, G23, //GPIO6�˿� 16-23 //����GPIO�˿�  ��Ӧ�˿�B
    G24, G25, G26, G27, G28, G29, G30, G31, //GPIO6�˿� 24-31 //����GPIO�˿�  ��Ӧ�˿�B
    
    H0,  H1,  H2,  H3,  H4,  H5,  H6,  H7,  //GPIO7�˿� 0-7   //����GPIO�˿�  ��Ӧ�˿�C
    H8,  H9,  H10, H11, H12, H13, H14, H15, //GPIO7�˿� 8-15  //����GPIO�˿�  ��Ӧ�˿�C
    H16, H17, H18, H19, H20, H21, H22, H23, //GPIO7�˿� 16-23 //����GPIO�˿�  ��Ӧ�˿�C
    H24, H25, H26, H27, H28, H29, H30, H31, //GPIO7�˿� 24-31 //����GPIO�˿�  ��Ӧ�˿�C
    
    I0,  I1,  I2,  I3,  I4,  I5,  I6,  I7,  //GPIO8�˿� 0-7   //����GPIO�˿�  ��Ӧ�˿�D
    I8,  I9,  I10, I11, I12, I13, I14, I15, //GPIO8�˿� 8-15  //����GPIO�˿�  ��Ӧ�˿�D
    I16, I17, I18, I19, I20, I21, I22, I23, //GPIO8�˿� 16-23 //����GPIO�˿�  ��Ӧ�˿�D
    I24, I25, I26, I27, I28, I29, I30, I31, //GPIO8�˿� 24-31 //����GPIO�˿�  ��Ӧ�˿�D
}PIN_enum;



typedef enum //ö�ٶ˿ڷ���
{
    GPI = 0, //����ܽ����뷽��      
    GPO = 1, //����ܽ��������
}GPIODIR_enum;

typedef enum //ö�ٶ˿ڵ�ƽ
{
    GPIO_LOW = 0,  //����ܽ����뷽��      
    GPIO_HIGH = 1, //����ܽ��������
}GPIOLEVEL_enum;


typedef enum //ö�ٶ˿ڵ�ƽ
{
	CAMERA_BIN=1,		//С���
	CAMERA_BIN_UART,	//С��紮�ڰ汾
	CAMERA_GRAYSCALE,	//�����
	CAMERA_COLOR,		//��ͫ
}CAMERA_TYPE_enum;



// Compiler Related Definitions 
#ifdef __CC_ARM                         // ARM Compiler 
    #define ALIGN(n)                    __attribute__((aligned(n)))
#elif defined (__IAR_SYSTEMS_ICC__)     // for IAR Compiler 
    #define PRAGMA(x)                   _Pragma(#x)
    #define ALIGN(n)                    PRAGMA(data_alignment=n)
#elif defined (__GNUC__)                // GNU GCC Compiler 
    #define ALIGN(n)                    __attribute__((aligned(n)))
#endif // Compiler Related Definitions 





#if (defined(__ICCARM__))
	//���彫�������ITCM�ķ�ʽ
    #define AT_ITCM_SECTION_INIT(var) var @"ITCM_NonCacheable.init"
	//���彫������߱�������DTCM�ķ�ʽ
	#if ((!(defined(FSL_FEATURE_HAS_NO_NONCACHEABLE_SECTION) && FSL_FEATURE_HAS_NO_NONCACHEABLE_SECTION)) && defined(FSL_FEATURE_L1ICACHE_LINESIZE_BYTE))
        #define AT_DTCM_SECTION(var) var @"NonCacheable"
        #define AT_DTCM_SECTION_ALIGN(var, alignbytes) SDK_PRAGMA(data_alignment = alignbytes) var @"NonCacheable"
        #define AT_DTCM_SECTION_INIT(var) var @"NonCacheable.init"
        #define AT_DTCM_SECTION_ALIGN_INIT(var, alignbytes) SDK_PRAGMA(data_alignment = alignbytes) var @"NonCacheable.init"
    #endif 
	//���彫������߱�������OCRAM�ķ�ʽ
	#define AT_OCRAM_SECTION(var) var @"OCRAM_CACHE"
    #define AT_OCRAM_SECTION_ALIGN(var, alignbytes) SDK_PRAGMA(data_alignment = alignbytes) var @"OCRAM_CACHE"
    #define AT_OCRAM_SECTION_INIT(var) var @"OCRAM_CACHE.init"
    #define AT_OCRAM_SECTION_ALIGN_INIT(var, alignbytes) SDK_PRAGMA(data_alignment = alignbytes) var @"OCRAM_CACHE.init"
	//���彫������߱�������SDRAM�ķ�ʽ
	#define AT_SDRAM_SECTION(var) var @"SDRAM_CACHE"
    #define AT_SDRAM_SECTION_ALIGN(var, alignbytes) SDK_PRAGMA(data_alignment = alignbytes) var @"SDRAM_CACHE"
    #define AT_SDRAM_SECTION_INIT(var) var @"SDRAM_CACHE.init"
    #define AT_SDRAM_SECTION_ALIGN_INIT(var, alignbytes) SDK_PRAGMA(data_alignment = alignbytes) var @"SDRAM_CACHE.init"
	//���彫������߱�������SDRAM_NONCACHE�ķ�ʽ
	#define AT_SDRAM_NONCACHE_SECTION(var) var @"SDRAM_NonCacheable"
    #define AT_SDRAM_NONCACHE_SECTION_ALIGN(var, alignbytes) SDK_PRAGMA(data_alignment = alignbytes) var @"SDRAM_NonCacheable"
    #define AT_SDRAM_NONCACHE_SECTION_INIT(var) var @"SDRAM_NonCacheable.init"
    #define AT_SDRAM_NONCACHE_SECTION_ALIGN_INIT(var, alignbytes) SDK_PRAGMA(data_alignment = alignbytes) var @"SDRAM_NonCacheable.init"
	
	
#elif(defined(__CC_ARM) || defined(__ARMCC_VERSION))
	//���彫�������ITCM�ķ�ʽ
    #define AT_ITCM_SECTION_INIT(var) __attribute__((section("ITCM_NonCacheable.init"))) var
	//���彫������߱�������DTCM�ķ�ʽ
	#if ((!(defined(FSL_FEATURE_HAS_NO_NONCACHEABLE_SECTION) && FSL_FEATURE_HAS_NO_NONCACHEABLE_SECTION)) && defined(FSL_FEATURE_L1ICACHE_LINESIZE_BYTE))
        #if(defined(__CC_ARM))
			#define AT_DTCM_SECTION(var) __attribute__((section("NonCacheable"), zero_init)) var
			#define AT_DTCM_SECTION_ALIGN(var, alignbytes) \
				__attribute__((section("NonCacheable"), zero_init)) __attribute__((aligned(alignbytes))) var
		#else
			#define AT_DTCM_SECTION(var) __attribute__((section(".bss.NonCacheable"))) var
			#define AT_DTCM_SECTION_ALIGN(var, alignbytes) \
				__attribute__((section(".bss.NonCacheable"))) __attribute__((aligned(alignbytes))) var
		#endif

        #define AT_DTCM_SECTION_INIT(var) __attribute__((section("NonCacheable.init"))) var
        #define AT_DTCM_SECTION_ALIGN_INIT(var, alignbytes) \
            __attribute__((section("NonCacheable.init"))) __attribute__((aligned(alignbytes))) var
    #endif
	
	//���彫������߱�������OCRAM�ķ�ʽ
    #if(defined(__CC_ARM))
		#define AT_OCRAM_SECTION(var) __attribute__((section("OCRAM_CACHE"), zero_init)) var
		#define AT_OCRAM_SECTION_ALIGN(var, alignbytes) \
			__attribute__((section("OCRAM_CACHE"), zero_init)) __attribute__((aligned(alignbytes))) var
	#else
		#define AT_OCRAM_SECTION(var) __attribute__((section(".bss.OCRAM_CACHE"))) var
		#define AT_OCRAM_SECTION_ALIGN(var, alignbytes) \
			__attribute__((section(".bss.OCRAM_CACHE"))) __attribute__((aligned(alignbytes))) var
	#endif

    #define AT_OCRAM_SECTION_INIT(var) __attribute__((section("OCRAM_CACHE.init"))) var
    #define AT_OCRAM_SECTION_ALIGN_INIT(var, alignbytes) \
        __attribute__((section("OCRAM_CACHE.init"))) __attribute__((aligned(alignbytes))) var
	
	
	//���彫������߱�������SDRAM�ķ�ʽ
	#if(defined(__CC_ARM))
		#define AT_SDRAM_SECTION(var) __attribute__((section("SDRAM_CACHE"), zero_init)) var
		#define AT_SDRAM_SECTION_ALIGN(var, alignbytes) \
			__attribute__((section("SDRAM_CACHE"), zero_init)) __attribute__((aligned(alignbytes))) var
	#else
		#define AT_SDRAM_SECTION(var) __attribute__((section(".bss.SDRAM_CACHE"))) var
		#define AT_SDRAM_SECTION_ALIGN(var, alignbytes) \
			__attribute__((section(".bss.SDRAM_CACHE"))) __attribute__((aligned(alignbytes))) var
	#endif
	
    #define AT_SDRAM_SECTION_INIT(var) __attribute__((section("SDRAM_CACHE.init"))) var
    #define AT_SDRAM_SECTION_ALIGN_INIT(var, alignbytes) \
        __attribute__((section("SDRAM_CACHE.init"))) __attribute__((aligned(alignbytes))) var
	//���彫������߱�������SDRAM_NONCACHE�ķ�ʽ
	#if(defined(__CC_ARM))
		#define AT_SDRAM_NONCACHE_SECTION(var) __attribute__((section("SDRAM_NonCacheable"), zero_init)) var
		#define AT_SDRAM_NONCACHE_SECTION_ALIGN(var, alignbytes) \
			__attribute__((section("SDRAM_NonCacheable"), zero_init)) __attribute__((aligned(alignbytes))) var
	#else
		#define AT_SDRAM_NONCACHE_SECTION(var) __attribute__((section(".bss.SDRAM_NonCacheable"))) var
		#define AT_SDRAM_NONCACHE_SECTION_ALIGN(var, alignbytes) \
			__attribute__((section(".bss.SDRAM_NonCacheable"))) __attribute__((aligned(alignbytes))) var
	#endif
	
    #define AT_SDRAM_NONCACHE_SECTION_INIT(var) __attribute__((section("SDRAM_NonCacheable.init"))) var
    #define AT_SDRAM_NONCACHE_SECTION_ALIGN_INIT(var, alignbytes) \
        __attribute__((section("SDRAM_NonCacheable.init"))) __attribute__((aligned(alignbytes))) var
	
#endif




#include <math.h>
#include <string.h>

            
            
extern CAMERA_TYPE_enum flexio_camera_type;     //����ͷ�ͺ�
extern uint8 *flexio_camera_buffer_addr;        //����ͷ�����ַ
            

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
	



#endif
