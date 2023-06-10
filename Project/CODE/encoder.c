#include "encoder.h"

//---------------------结构体---------------------//
struct RC_Para Encoder1_Para = {0, 0, 0.25}; // 低通滤波
struct RC_Para Encoder2_Para = {0, 0, 0.25};
struct RC_Para Encoder3_Para = {0, 0, 0.25};
struct RC_Para Encoder4_Para = {0, 0, 0.25};

RC_Filter_pt RC_Encoder1 = &Encoder1_Para;
RC_Filter_pt RC_Encoder2 = &Encoder2_Para;
RC_Filter_pt RC_Encoder3 = &Encoder3_Para;
RC_Filter_pt RC_Encoder4 = &Encoder4_Para;

float MileageKx = 0.008632f; // 对应实际x距离比例值
float MileageKy = 0.00888f;
float MileageKz = 0.01f;

int16 encoder1=0,encoder2=0,encoder3=0,encoder4=0;//编码器的值
int16 RC_encoder1, RC_encoder2, RC_encoder3, RC_encoder4; // 滤波之后encoder的值

void encoder_init(void)
{
  // 初始化 QTIMER_1 A相使用QTIMER1_TIMER2_C2 B相使用QTIMER1_TIMER3_C24
  qtimer_quad_init(QTIMER_1, QTIMER1_TIMER0_C0, QTIMER1_TIMER1_C1);
  qtimer_quad_init(QTIMER_1, QTIMER1_TIMER2_C2, QTIMER1_TIMER3_C24);
  qtimer_quad_init(QTIMER_2, QTIMER2_TIMER0_C3, QTIMER2_TIMER3_C25);
  qtimer_quad_init(QTIMER_3, QTIMER3_TIMER2_B18, QTIMER3_TIMER3_B19);
}

void omni_mileage() // 计算全方位移动距离
{
  float detax = 0, detay = 0, detaz = 0;
  detay = (float)(RC_encoder1 + RC_encoder2 + RC_encoder3 + RC_encoder4) / 4;  // y轴移动速度
  detax = (float)(-RC_encoder1 + RC_encoder2 + RC_encoder3 - RC_encoder4) / 4; // x轴移动速度
  detaz = (float)(RC_encoder1 - RC_encoder2 + RC_encoder3 - RC_encoder4) / 4;
  car.MileageX += (float)(detax * MileageKx); // 单位cm
  car.MileageY += (float)(detay * MileageKy); // 单位cm
  // car.MileageZ += (float)(detaz*MileageKz);
}


//void omni_mileage_angle() // 计算全方位移动距离
//{
//  float detax = 0, detay = 0, detaz = 0;
//  detay = (float)(RC_encoder1 + RC_encoder2 + RC_encoder3 + RC_encoder4) / 4;  // y轴移动速度
//  detax = (float)(-RC_encoder1 + RC_encoder2 + RC_encoder3 - RC_encoder4) / 4; // x轴移动速度
//  detaz = (float)(RC_encoder1 - RC_encoder2 + RC_encoder3 - RC_encoder4) / 4;
//  car.MileageX += (float)(detax * MileageKx); // 单位cm
//  car.MileageY += (float)(detay * MileageKy); // 单位cm
//  // car.MileageZ += (float)(detaz*MileageKz);
//}

void encoder_get(void)
{

  encoder1 = qtimer_quad_get(QTIMER_1, QTIMER1_TIMER0_C0); // 这里需要注意第二个参数务必填写A相引脚
  encoder2 = -qtimer_quad_get(QTIMER_1, QTIMER1_TIMER2_C2);
  encoder4 = -qtimer_quad_get(QTIMER_2, QTIMER2_TIMER0_C3);
  encoder3 = qtimer_quad_get(QTIMER_3, QTIMER3_TIMER2_B18);

  RC_encoder1 = (int16_t)RCFilter(encoder1, RC_Encoder1); // 低通滤波
  RC_encoder2 = (int16_t)RCFilter(encoder2, RC_Encoder2);
  RC_encoder3 = (int16_t)RCFilter(encoder3, RC_Encoder3);
  RC_encoder4 = (int16_t)RCFilter(encoder4, RC_Encoder4);

  // 计算位移(单位：m)
  // Car.mileage=(Encoder/1024)*(45/104)*2*PI*0.03;
  omni_mileage();

  qtimer_quad_clear(QTIMER_1, QTIMER1_TIMER0_C0);
  qtimer_quad_clear(QTIMER_1, QTIMER1_TIMER2_C2);
  qtimer_quad_clear(QTIMER_2, QTIMER2_TIMER0_C3);
  qtimer_quad_clear(QTIMER_3, QTIMER3_TIMER2_B18);
}