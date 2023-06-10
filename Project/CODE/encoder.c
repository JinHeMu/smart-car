#include "encoder.h"

//---------------------�ṹ��---------------------//
struct RC_Para Encoder1_Para = {0, 0, 0.25}; // ��ͨ�˲�
struct RC_Para Encoder2_Para = {0, 0, 0.25};
struct RC_Para Encoder3_Para = {0, 0, 0.25};
struct RC_Para Encoder4_Para = {0, 0, 0.25};

RC_Filter_pt RC_Encoder1 = &Encoder1_Para;
RC_Filter_pt RC_Encoder2 = &Encoder2_Para;
RC_Filter_pt RC_Encoder3 = &Encoder3_Para;
RC_Filter_pt RC_Encoder4 = &Encoder4_Para;

float MileageKx = 0.008632f; // ��Ӧʵ��x�������ֵ
float MileageKy = 0.00888f;
float MileageKz = 0.01f;

int16 encoder1=0,encoder2=0,encoder3=0,encoder4=0;//��������ֵ
int16 RC_encoder1, RC_encoder2, RC_encoder3, RC_encoder4; // �˲�֮��encoder��ֵ

void encoder_init(void)
{
  // ��ʼ�� QTIMER_1 A��ʹ��QTIMER1_TIMER2_C2 B��ʹ��QTIMER1_TIMER3_C24
  qtimer_quad_init(QTIMER_1, QTIMER1_TIMER0_C0, QTIMER1_TIMER1_C1);
  qtimer_quad_init(QTIMER_1, QTIMER1_TIMER2_C2, QTIMER1_TIMER3_C24);
  qtimer_quad_init(QTIMER_2, QTIMER2_TIMER0_C3, QTIMER2_TIMER3_C25);
  qtimer_quad_init(QTIMER_3, QTIMER3_TIMER2_B18, QTIMER3_TIMER3_B19);
}

void omni_mileage() // ����ȫ��λ�ƶ�����
{
  float detax = 0, detay = 0, detaz = 0;
  detay = (float)(RC_encoder1 + RC_encoder2 + RC_encoder3 + RC_encoder4) / 4;  // y���ƶ��ٶ�
  detax = (float)(-RC_encoder1 + RC_encoder2 + RC_encoder3 - RC_encoder4) / 4; // x���ƶ��ٶ�
  detaz = (float)(RC_encoder1 - RC_encoder2 + RC_encoder3 - RC_encoder4) / 4;
  car.MileageX += (float)(detax * MileageKx); // ��λcm
  car.MileageY += (float)(detay * MileageKy); // ��λcm
  // car.MileageZ += (float)(detaz*MileageKz);
}


//void omni_mileage_angle() // ����ȫ��λ�ƶ�����
//{
//  float detax = 0, detay = 0, detaz = 0;
//  detay = (float)(RC_encoder1 + RC_encoder2 + RC_encoder3 + RC_encoder4) / 4;  // y���ƶ��ٶ�
//  detax = (float)(-RC_encoder1 + RC_encoder2 + RC_encoder3 - RC_encoder4) / 4; // x���ƶ��ٶ�
//  detaz = (float)(RC_encoder1 - RC_encoder2 + RC_encoder3 - RC_encoder4) / 4;
//  car.MileageX += (float)(detax * MileageKx); // ��λcm
//  car.MileageY += (float)(detay * MileageKy); // ��λcm
//  // car.MileageZ += (float)(detaz*MileageKz);
//}

void encoder_get(void)
{

  encoder1 = qtimer_quad_get(QTIMER_1, QTIMER1_TIMER0_C0); // ������Ҫע��ڶ������������дA������
  encoder2 = -qtimer_quad_get(QTIMER_1, QTIMER1_TIMER2_C2);
  encoder4 = -qtimer_quad_get(QTIMER_2, QTIMER2_TIMER0_C3);
  encoder3 = qtimer_quad_get(QTIMER_3, QTIMER3_TIMER2_B18);

  RC_encoder1 = (int16_t)RCFilter(encoder1, RC_Encoder1); // ��ͨ�˲�
  RC_encoder2 = (int16_t)RCFilter(encoder2, RC_Encoder2);
  RC_encoder3 = (int16_t)RCFilter(encoder3, RC_Encoder3);
  RC_encoder4 = (int16_t)RCFilter(encoder4, RC_Encoder4);

  // ����λ��(��λ��m)
  // Car.mileage=(Encoder/1024)*(45/104)*2*PI*0.03;
  omni_mileage();

  qtimer_quad_clear(QTIMER_1, QTIMER1_TIMER0_C0);
  qtimer_quad_clear(QTIMER_1, QTIMER1_TIMER2_C2);
  qtimer_quad_clear(QTIMER_2, QTIMER2_TIMER0_C3);
  qtimer_quad_clear(QTIMER_3, QTIMER3_TIMER2_B18);
}