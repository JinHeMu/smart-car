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
* @date             2022-05-04
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

#include "zf_systick.h"
#include "zf_gpio.h"
#include "zf_iic.h"
#include "zf_iomuxc.h"  
#include "zf_spi.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_IMU963RA.h"
 
int16 imu963ra_gyro_x, imu963ra_gyro_y, imu963ra_gyro_z;
int16 imu963ra_acc_x,  imu963ra_acc_y,  imu963ra_acc_z;
int16 imu963ra_mag_x,  imu963ra_mag_y,  imu963ra_mag_z;

#if IMU963RA_USE_SOFT_IIC
#define imu963ra_write_acc_gyro_register(reg,data)       simiic_write_reg(IMU963RA_DEV_ADDR, reg, data)
#define imu963ra_read_acc_gyro_register(reg)             simiic_read_reg (IMU963RA_DEV_ADDR, reg, SIMIIC)
#define imu963ra_read_acc_gyro_registers(reg,data,len)   simiic_read_regs(IMU963RA_DEV_ADDR, reg, data, len, SIMIIC)
#else

//-------------------------------------------------------------------------------------------------------------------
// @brief       IMU963RA д�Ĵ��� �ڲ�����
// @param       reg             �Ĵ�����ַ
// @param       data            ����
// @return      void
//-------------------------------------------------------------------------------------------------------------------
static void imu963ra_write_acc_gyro_register(uint8 reg, uint8 data)
{
    uint8 dat[2];
    
    dat[0] = reg | IMU963RA_SPI_W;
    dat[1] = data;
    
    spi_mosi(IMU963RA_SPI,IMU963RA_CS_PIN,dat,dat,2,1);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IMU963RA ���Ĵ��� �ڲ�����
// @param       reg             �Ĵ�����ַ
// @return      uint8           ����
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu963ra_read_acc_gyro_register(uint8 reg)
{
    uint8 dat[2];

    dat[0] = reg | IMU963RA_SPI_R;
    dat[1] = 0;
    
    spi_mosi(IMU963RA_SPI,IMU963RA_CS_PIN,dat,dat,2,1);
        
    return dat[1];
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IMU963RA ������ �ڲ�����
// @param       reg             �Ĵ�����ַ
// @param       data            ���ݻ�����
// @param       len             ���ݳ���
// @return      void
//-------------------------------------------------------------------------------------------------------------------
static void imu963ra_read_acc_gyro_registers(uint8 *data, uint32 len)
{
    spi_mosi(IMU963RA_SPI, IMU963RA_CS_PIN, data, data, len, 1);
}
#endif

//-------------------------------------------------------------------------------------------------------------------
// @brief       IMU963RA��ΪIIC�����������д���ݣ��ڲ�����
// @param       void
// @return      void
// Sample usage:                �ڲ����ã��û��������
//-------------------------------------------------------------------------------------------------------------------
void imu963ra_write_mag_register(uint8 addr, uint8 reg, uint8 data)
{
    addr = addr << 1;
    imu963ra_write_acc_gyro_register(IMU963RA_SLV0_CONFIG, 0x00);    // �ӻ�0�������
    imu963ra_write_acc_gyro_register(IMU963RA_SLV0_ADD, addr | 0);   // ���õشżƵ�ַ��ע��������Ҫ����8λ��I2C��ַ�� 0x2C
    imu963ra_write_acc_gyro_register(IMU963RA_SLV0_SUBADD, reg);     // ��Ҫд��ļĴ�����ַ
    imu963ra_write_acc_gyro_register(IMU963RA_DATAWRITE_SLV0, data); // ��Ҫд�������
    imu963ra_write_acc_gyro_register(IMU963RA_MASTER_CONFIG, 0x4C);  // ���ڵ�һ����������ͨѶ �������� I2C����ʹ��
    
    //�ȴ�ͨѶ�ɹ�
    while(0 == (0x80 & imu963ra_read_acc_gyro_register(IMU963RA_STATUS_MASTER)))
    {
        systick_delay_ms(2);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IMU963RA��ΪIIC����������ƶ����ݣ��ڲ�����
// @param       void
// @return      void
// Sample usage:                �ڲ����ã��û��������
//-------------------------------------------------------------------------------------------------------------------
uint8 imu963ra_read_mag_register(uint8 addr, uint8 reg)
{
    addr = addr << 1;
    
    imu963ra_write_acc_gyro_register(IMU963RA_SLV0_ADD, addr | 1);   // ���õشżƵ�ַ��ע��������Ҫ����8λ��I2C��ַ�� 0x2C
    imu963ra_write_acc_gyro_register(IMU963RA_SLV0_SUBADD, reg);     // ��Ҫ��ȡ�ļĴ�����ַ
    imu963ra_write_acc_gyro_register(IMU963RA_SLV0_CONFIG, 0x01);    
    imu963ra_write_acc_gyro_register(IMU963RA_MASTER_CONFIG, 0x4C);  // ���ڵ�һ����������ͨѶ �������� I2C����ʹ��
    
    //�ȴ�ͨѶ�ɹ�
    while(0 == (0x01 & imu963ra_read_acc_gyro_register(IMU963RA_STATUS_MASTER)))
    {
        systick_delay_ms(2);
    }
    
    return (imu963ra_read_acc_gyro_register(IMU963RA_SENSOR_HUB_1)); // ���ض�ȡ��������
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IMU963RA��ΪIIC������������Զ�д���ݣ��ڲ�����
// @param       void
// @return      void
// Sample usage:                �ڲ����ã��û��������
//-------------------------------------------------------------------------------------------------------------------
void imu963ra_connect_mag(uint8 addr, uint8 reg)
{
    addr = addr << 1;
    
    imu963ra_write_acc_gyro_register(IMU963RA_SLV0_ADD, addr | 1);   // ���õشżƵ�ַ��ע��������Ҫ����8λ��I2C��ַ�� 0x2C
    imu963ra_write_acc_gyro_register(IMU963RA_SLV0_SUBADD, reg);     // ��Ҫ��ȡ�ļĴ�����ַ
    imu963ra_write_acc_gyro_register(IMU963RA_SLV0_CONFIG, 0x06);    
    imu963ra_write_acc_gyro_register(IMU963RA_MASTER_CONFIG, 0x6C);  // ���ڵ�һ����������ͨѶ �������� I2C����ʹ��
}   


//-------------------------------------------------------------------------------------------------------------------
// @brief       IMU963RA �����Լ� �ڲ�����
// @param       void
// @return      uint8           1-�Լ�ʧ�� 0-�Լ�ɹ�
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu963ra_acc_gyro_self_check (void)
{
    uint8 dat = 0;
//    uint16 timeout_count = 0;

    while(0x6B != dat)                                          // �ж� ID �Ƿ���ȷ
    {
//        if(timeout_count++ > IMU963RA_TIMEOUT_COUNT)
//            return 1;
        dat = imu963ra_read_acc_gyro_register(IMU963RA_WHO_AM_I);
        systick_delay_ms(1);
    }
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       IMU963RA �������Լ� �ڲ�����
// @param       void
// @return      uint8           1-�Լ�ʧ�� 0-�Լ�ɹ�
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu963ra_mag_self_check (void)
{
    uint8 dat = 0;
//    uint16 timeout_count = 0;

    while(0xff != dat)                                              // �ж� ID �Ƿ���ȷ
    {
//        if(timeout_count++ > IMU963RA_TIMEOUT_COUNT)
//            return 1;
        dat = imu963ra_read_mag_register(IMU963RA_MAG_ADDR, IMU963RA_MAG_CHIP_ID);
        systick_delay_ms(1);
    }
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��ȡ IMU963RA ���ٶȼ�����
// @param       void
// @return      void
// Sample usage:                ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void imu963ra_get_acc (void)
{
    struct
    {
        uint8 reg;
        uint8 dat[6];
    }buf;

#if IMU963RA_USE_SOFT_IIC
    imu963ra_read_acc_gyro_registers(IMU963RA_OUTX_L_A, &buf.dat[0], 7);
#else
    buf.reg = IMU963RA_OUTX_L_A | IMU963RA_SPI_R;
    imu963ra_read_acc_gyro_registers(&buf.reg, 7);
#endif
   
    imu963ra_acc_x = (int16_t)(((uint16_t)buf.dat[1]<<8 | buf.dat[0]));
    imu963ra_acc_y = (int16_t)(((uint16_t)buf.dat[3]<<8 | buf.dat[2]));
    imu963ra_acc_z = (int16_t)(((uint16_t)buf.dat[5]<<8 | buf.dat[4]));
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       ��ȡIMU963RA����������
// @param       void
// @return      void
// Sample usage:                ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void imu963ra_get_gyro (void)
{
    struct
    {
        uint8 reg;
        uint8 dat[6];
    }buf;

#if IMU963RA_USE_SOFT_IIC
    imu963ra_read_acc_gyro_registers(IMU963RA_OUTX_L_G, &buf.dat[0], 6);
#else
    buf.reg = IMU963RA_OUTX_L_G | IMU963RA_SPI_R;
    imu963ra_read_acc_gyro_registers(&buf.reg, 7);
#endif
    
    imu963ra_gyro_x = (int16_t)(((uint16_t)buf.dat[1]<<8 | buf.dat[0]));
    imu963ra_gyro_y = (int16_t)(((uint16_t)buf.dat[3]<<8 | buf.dat[2]));
    imu963ra_gyro_z = (int16_t)(((uint16_t)buf.dat[5]<<8 | buf.dat[4]));
}


//-------------------------------------------------------------------------------------------------------------------
// @brief       ��ȡ IMU963RA ����������
// @param       void
// @return      void
// Sample usage:                ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void imu963ra_get_mag (void)
{
    uint8 temp_status;
    struct
    {
        uint8 reg;
        uint8 dat[6];
    }buf;

    imu963ra_write_acc_gyro_register(IMU963RA_FUNC_CFG_ACCESS, 0x40);
    temp_status = imu963ra_read_acc_gyro_register(IMU963RA_STATUS_MASTER);
    if(0x01 & temp_status)
    {
        
    #if IMU963RA_USE_SOFT_IIC
        imu963ra_read_acc_gyro_registers(IMU963RA_SENSOR_HUB_1, &buf.dat[0], 6);
    #else
        buf.reg = IMU963RA_SENSOR_HUB_1 | IMU963RA_SPI_R;
        imu963ra_read_acc_gyro_registers(&buf.reg, 7);
    #endif
        
        imu963ra_mag_x = (int16_t)(((uint16_t)buf.dat[1]<<8 | buf.dat[0]));
        imu963ra_mag_y = (int16_t)(((uint16_t)buf.dat[3]<<8 | buf.dat[2]));
        imu963ra_mag_z = (int16_t)(((uint16_t)buf.dat[5]<<8 | buf.dat[4]));
    }
    imu963ra_write_acc_gyro_register(IMU963RA_FUNC_CFG_ACCESS, 0x00);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       ��ʼ�� IMU963RA
// @param       void
// @return      uint8           1-��ʼ��ʧ�� 0-��ʼ���ɹ�
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8 imu963ra_init (void)
{

    systick_delay_ms(10);                                                // �ϵ���ʱ

#if IMU963RA_USE_SOFT_IIC
    simiic_delay_set(IMU963RA_SOFT_IIC_DELAY);
    simiic_init();
#else
    spi_init(IMU963RA_SPI, IMU963RA_SPC_PIN, IMU963RA_SDI_PIN, IMU963RA_SDO_PIN, IMU963RA_CS_PIN, 0, 10*1000*1000);//Ӳ��SPI��ʼ��

#endif
    
    imu963ra_write_acc_gyro_register(IMU963RA_FUNC_CFG_ACCESS, 0x00);    // �ر�HUB�Ĵ�������
    imu963ra_write_acc_gyro_register(IMU963RA_CTRL3_C, 0x01);            // ��λ�豸
    systick_delay_ms(2);                             
    imu963ra_write_acc_gyro_register(IMU963RA_FUNC_CFG_ACCESS, 0x00);    // �ر�HUB�Ĵ�������
    imu963ra_acc_gyro_self_check();										// �����Լ�                
             
              
    imu963ra_write_acc_gyro_register(IMU963RA_INT1_CTRL, 0x03);         // ���������� ���ٶ����ݾ����ж�
    imu963ra_write_acc_gyro_register(IMU963RA_CTRL1_XL, 0x4C);          // ���ü��ٶȼ����̡�8G�Լ������������104hz �Լ����ٶ���Ϣ�ӵ�һ���˲������
    //IMU963RA_CTRL1_XL�Ĵ���
	//����Ϊ:0x40 ���ٶ�����Ϊ:��2G	    ��ȡ���ļ��ٶȼ����� ����16393������ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
	//����Ϊ:0x48 ���ٶ�����Ϊ:��4G      ��ȡ���ļ��ٶȼ����� ����8197�� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
	//����Ϊ:0x4C ���ٶ�����Ϊ:��8G      ��ȡ���ļ��ٶȼ����� ����4098�� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
	//����Ϊ:0x44 ���ٶ�����Ϊ:��16G     ��ȡ���ļ��ٶȼ����� ����2049�� ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
	
	imu963ra_write_acc_gyro_register(IMU963RA_CTRL2_G, 0x6C);           // ���������Ǽ����̡�2000dps�Լ������������416hz
	//ICM20602_GYRO_CONFIG�Ĵ���
	//����Ϊ:0x62 ����������Ϊ:��125dps  ��ȡ�������������ݳ���228.6��   ����ת��Ϊ������λ�����ݣ���λΪ����/s
	//����Ϊ:0x60 ����������Ϊ:��250dps	��ȡ�������������ݳ���114.3��   ����ת��Ϊ������λ�����ݣ���λΪ����/s
	//����Ϊ:0x64 ����������Ϊ:��500dps  ��ȡ�������������ݳ���57.1��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
	//����Ϊ:0x68 ����������Ϊ:��1000dps ��ȡ�������������ݳ���28.6��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
	//����Ϊ:0x6C ����������Ϊ:��2000dps ��ȡ�������������ݳ���14.3��    ����ת��Ϊ������λ�����ݣ���λΪ����/s
	//����Ϊ:0x61 ����������Ϊ:��4000dps ��ȡ�������������ݳ���7.1��     ����ת��Ϊ������λ�����ݣ���λΪ����/s
	
    imu963ra_write_acc_gyro_register(IMU963RA_CTRL3_C, 0x44);            // ʹ�����������ֵ�ͨ�˲���
    imu963ra_write_acc_gyro_register(IMU963RA_CTRL4_C, 0x02);            // ʹ�����ֵ�ͨ�˲���
    imu963ra_write_acc_gyro_register(IMU963RA_CTRL5_C, 0x00);            // ���ٶȼ�����������������
    imu963ra_write_acc_gyro_register(IMU963RA_CTRL6_C, 0x00);            // �������ٶȼƸ�����ģʽ �����ǵ�ͨ�˲� 133hz
    imu963ra_write_acc_gyro_register(IMU963RA_CTRL7_G, 0x00);            // ���������Ǹ�����ģʽ �رո�ͨ�˲�
    imu963ra_write_acc_gyro_register(IMU963RA_CTRL9_XL, 0x01);           // �ر�I3C�ӿ�

    imu963ra_write_acc_gyro_register(IMU963RA_FUNC_CFG_ACCESS, 0x40);    // ����HUB�Ĵ������� �������õشż�
    imu963ra_write_acc_gyro_register(IMU963RA_MASTER_CONFIG, 0x80);      // ��λI2C����
    systick_delay_ms(2);                             
    imu963ra_write_acc_gyro_register(IMU963RA_MASTER_CONFIG, 0x00);      // �����λ��־
    systick_delay_ms(2);
    
    imu963ra_write_mag_register(IMU963RA_MAG_ADDR, IMU963RA_MAG_CONTROL2, 0x80);    // ��λ���ӵ�����
    systick_delay_ms(2);
    imu963ra_write_mag_register(IMU963RA_MAG_ADDR, IMU963RA_MAG_CONTROL2, 0x00);
    systick_delay_ms(2);
	
    imu963ra_mag_self_check();												// �������Լ�
    
    imu963ra_write_mag_register(IMU963RA_MAG_ADDR, IMU963RA_MAG_CONTROL1, 0x19);    // ���ô���������8G �������100hz ����ģʽ
	//IMU963RA_MAG_ADDR�Ĵ���
	//����Ϊ:0x19 ����������Ϊ:8G     ��ȡ���ļ��ٶȼ����� ����3000�� ����ת��Ϊ������λ�����ݣ���λ��G(��˹)
	//����Ϊ:0x09 ����������Ϊ:2G     ��ȡ���ļ��ٶȼ����� ����12000������ת��Ϊ������λ�����ݣ���λ��G(��˹)
	
    imu963ra_write_mag_register(IMU963RA_MAG_ADDR, IMU963RA_MAG_FBR, 0x01);
    imu963ra_connect_mag(IMU963RA_MAG_ADDR, IMU963RA_MAG_OUTX_L);
    
    imu963ra_write_acc_gyro_register(IMU963RA_FUNC_CFG_ACCESS, 0x00);                            // �ر�HUB�Ĵ�������

	systick_delay_ms(20);	// �ȴ������ƻ�ȡ����
	
    return 0;
}
