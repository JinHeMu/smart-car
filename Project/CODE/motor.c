#include "motor.h"

#define PWM_LIMIT 25000// 限幅，必须注意安全

#define DIR_1 D0
#define DIR_2 D1
#define PWM_1 PWM2_MODULE3_CHA_D2
#define PWM_2 PWM2_MODULE3_CHB_D3

#define DIR_3 D14
#define DIR_4 D15
#define PWM_3 PWM1_MODULE0_CHA_D12
#define PWM_4 PWM1_MODULE0_CHB_D13

int32 duty1 = 0, duty2 = 0, duty3 = 0, duty4 = 0; // 电机PWM值

//float Incremental_kp[4] = {6.5, 6.5, 6.5, 6.5};
//float Incremental_ki[4] = {0.20, 0.20, 0.20, 0.20};
//float Incremental_kd[4] = {0,0,0,0};      // 增量式PID，控制电机输出值



//float Incremental_kp[4] = {270, 270, 270, 270};
//float Incremental_ki[4] = {8, 8 , 8, 8};
//float Incremental_kd[4] = {0,0,0,0}; 

//float Incremental_kp[4] = {20, 20, 20, 20};
//float Incremental_ki[4] = {2, 2 , 2, 2};
//float Incremental_kd[4] = {0,0,0,0}; 

float Incremental_kp[4] = {70, 70, 70, 70};
float Incremental_ki[4] = {4, 4 , 4, 4};
float Incremental_kd[4] = {0,0,0,0}; 

//float Angel_kp = 6, Angel_ki = 0, Angel_kd = 24; // 角度环

//float Angel_kp = 30, Angel_ki = 0, Angel_kd = 360; // 角度环
//float Angel_kp = 30, Angel_ki = 0, Angel_kd = 400; // 角度环

float Angel_kp = 10, Angel_ki = 0, Angel_kd = 90; // 角度环

//float Position_kp = 0.05, Position_ki = 0, Position_kd = 0;
//float Position_kp = 2.5, Position_ki = 0, Position_kd = 0.5;

float Position_kp = 10, Position_ki = 0, Position_kd = 0;

float correct_kp = 1, correct_ki = 0.002, correct_kd = 0;

// 积分法计算位移参数

float vy1 = 0;float vy2 = 0;
float vx1 = 0;float vx2 = 0;
float vy = 0;float vx = 0;


// 电机目标速度
double speed_tar_1 = 0;
double speed_tar_2 = 0;
double speed_tar_3 = 0;
double speed_tar_4 = 0;

double speed_tar = 0; // 单位m 目标速度 0.0086*encoder encoder = speed * 116.279 cm/s

void motor_init(void)
{
    gpio_init(DIR_1, GPO, 0, GPIO_PIN_CONFIG); // 单片机端口D0 初始化DIR_1			GPIO
    gpio_init(DIR_2, GPO, 0, GPIO_PIN_CONFIG); // 单片机端口D1 初始化DIR_2			GPIO
    pwm_init(PWM_1, 17000, 0);                 // 单片机端口D2 初始化PWM_1周期10K 占空比0
    pwm_init(PWM_2, 17000, 0);                 // 单片机端口D3 初始化PWM_2周期10K 占空比0

    gpio_init(DIR_3, GPO, 0, GPIO_PIN_CONFIG); // 单片机端口D0 初始化DIR_1          GPIO
    gpio_init(DIR_4, GPO, 0, GPIO_PIN_CONFIG); // 单片机端口D1 初始化DIR_2          GPIO
    pwm_init(PWM_3, 17000, 0);                 // 单片机端口D2 初始化PWM_1周期10K 占空比0
    pwm_init(PWM_4, 17000, 0);                 // 单片机端口D3 初始化PWM_2周期10K 占空比0
}

void car_omni(float x, float y, float z) // 全向移动逆运算
{
    speed_tar_1 = y - x + z;
    speed_tar_2 = y + x - z;
    speed_tar_3 = y + x + z;
    speed_tar_4 = y - x - z;
	
}

void car_omni_angle(float x, float y, float z) // 全向移动逆运算
{

		vy1 = y *cos(abs(car.current_angle));
		vy2 = y *sin(abs(car.current_angle));
		
		vx1 = x *sin(abs(car.current_angle));
		vx2 = x *cos(abs(car.current_angle));
	
		vy = vy1 + vx1;
		vx = -vy2 +vx2;
	
    speed_tar_1 = vy - vx + z;
    speed_tar_2 = vy + vx - z;
    speed_tar_3 = vy + vx + z;
    speed_tar_4 = vy - vx - z;
	
}

void car_ahead()
{
    speed_tar_1 = speed_tar;
    speed_tar_2 = speed_tar;
    speed_tar_3 = speed_tar;
    speed_tar_4 = speed_tar;
}

void car_back()
{
    speed_tar_1 = -speed_tar;
    speed_tar_2 = -speed_tar;
    speed_tar_3 = -speed_tar;
    speed_tar_4 = -speed_tar;
}

void car_lsideWay()
{
    speed_tar_1 = speed_tar;
    speed_tar_2 = -speed_tar;
    speed_tar_3 = -speed_tar;
    speed_tar_4 = speed_tar;
}

void car_rsideWay()
{
    speed_tar_1 = -speed_tar;
    speed_tar_2 = speed_tar;
    speed_tar_3 = speed_tar;
    speed_tar_4 = -speed_tar;
}

void car_diagonal()
{
    speed_tar_1 = speed_tar;
    speed_tar_2 = 0;
    speed_tar_3 = speed_tar;
    speed_tar_4 = 0;
}

void car_turnround()
{
    speed_tar_1 = speed_tar;
    speed_tar_2 = speed_tar;
    speed_tar_3 = -speed_tar;
    speed_tar_4 = -speed_tar;
}

void car_anticlockwise()
{
    speed_tar_1 = -speed_tar;
    speed_tar_2 = -speed_tar;
    speed_tar_3 = speed_tar;
    speed_tar_4 = speed_tar;
}

void car_concerning()
{
    speed_tar_1 = speed_tar;
    speed_tar_2 = 0;
    speed_tar_3 = 0;
    speed_tar_4 = speed_tar;
}

void car_stop()
{
    speed_tar_1 = 0;
    speed_tar_2 = 0;
    speed_tar_3 = 0;
    speed_tar_4 = 0;
}

/*!
 * @brief
 *
 * This function 增量式PID控制电机速度
 *
 * @param 目标编码器值
 * @param 当前编码器值
 * @return 电机占空比
 */

int Incremental_pid1(int Target, int Encoder)
{
    static float Bias1, Pwm1, Last_Bias1;
    Bias1 = (float)(Target - Encoder);
    Pwm1 += Incremental_kp[0] * (Bias1 - Last_Bias1)  + Incremental_ki[0] * Bias1;
    Last_Bias1 = Bias1;
    return (int)Pwm1;
}

int Incremental_pid2(int Target, int Encoder)
{
    static float Bias2, Pwm2, Last_Bias2;
    Bias2 = (float)(Target - Encoder);
    Pwm2 += Incremental_kp[1] * (Bias2 - Last_Bias2 )+ Incremental_ki[1] * Bias2;
    Last_Bias2 = Bias2;
    return (int)Pwm2;
}

int Incremental_pid3(int Target, int Encoder)
{
    static float Bias3, Pwm3,Last_Bias3;
    Bias3 = (float)(Target - Encoder);
    Pwm3 += Incremental_kp[2] * (Bias3 - Last_Bias3 )+ Incremental_ki[2] * Bias3;
    Last_Bias3 = Bias3;
    return (int)Pwm3;
}

int Incremental_pid4(int Target, int Encoder)
{
    static float Bias4, Pwm4, Last_Bias4;
    Bias4 = (float)(Target - Encoder);
    Pwm4 += Incremental_kp[3] * (Bias4 - Last_Bias4) + Incremental_ki[3] * Bias4;
    Last_Bias4 = Bias4;
    return (int)Pwm4;
}

int angel_pid(int NowAngel, int TargetAngel)
{
    if (NowAngel <= 0)
    {
        if (NowAngel - TargetAngel <= -180)
        {
            NowAngel += 180;
            TargetAngel -= 180;
        }
    }
    else if (NowAngel > 0)
    {
        if (NowAngel - TargetAngel >= 180)
        {
            NowAngel -= 180;
            TargetAngel += 180;
        }
    }
    static float current_error, Speed_Z, Integral_current_error, Last_current_error;
    current_error = (float)(TargetAngel - NowAngel);
    Integral_current_error += current_error;
    Speed_Z = Angel_kp * current_error + Angel_ki * Integral_current_error + Angel_kd * (current_error - Last_current_error);
    Last_current_error = current_error;
    //     rt_kprintf("%d\n",(int)(Speed_Z*1000));

    if (Speed_Z >= 200)
        Speed_Z = 200;
    if (Speed_Z <= -200)
        Speed_Z = -200;
    return (int)Speed_Z;
}

int picture_x_pid(int16 now_x, int16 target_x)
{
    static float Bias, Speed_X, Integral_bias, Last_Bias;
    Bias = (float)(target_x - now_x);
    Integral_bias += Bias;
    Speed_X = Position_kp * Bias + Position_ki * Integral_bias + Position_kd * (Bias - Last_Bias);
    Last_Bias = Bias;
    if (Speed_X >= 500)
        Speed_X = 500;
    if (Speed_X <= -500)
        Speed_X = -500;
    return (int)Speed_X;
}

int picture_y_pid(int16 now_y, int16 target_y)
{
    static float Bias, Speed_Y, Integral_bias, Last_Bias;
    Bias = (float)(target_y - now_y);
    Integral_bias += Bias;
    Speed_Y = Position_kp * Bias + Position_ki * Integral_bias + Position_kd * (Bias - Last_Bias);
    Last_Bias = Bias;
    if (Speed_Y >= 500)
        Speed_Y = 500;
    if (Speed_Y <= -500)
        Speed_Y = -500;
    return (int)Speed_Y;
}

int correct_x_pid(int16 now_x, int16 target_x)
{
    static float Bias, Speed_X, Integral_bias, Last_Bias;
    Bias = (float)(target_x - now_x);
    Integral_bias += Bias;
    Speed_X = correct_kp * Bias + correct_ki * Integral_bias + correct_kd * (Bias - Last_Bias);
    Last_Bias = Bias;
    if (Speed_X >= 200)
        Speed_X = 200;
    if (Speed_X <= -200)
        Speed_X = -200;
    return (int)Speed_X;
}

int correct_y_pid(int16 now_y, int16 target_y)
{
    static float Bias, Speed_Y, Integral_bias, Last_Bias;
    Bias = (float)(target_y - now_y);
    Integral_bias += Bias;
    Speed_Y = correct_kp * Bias + correct_ki * Integral_bias + correct_kd * (Bias - Last_Bias);
    Last_Bias = Bias;
    if (Speed_Y >= 200)
        Speed_Y = 200;
    if (Speed_Y <= -200)
        Speed_Y = -200;
    return (int)Speed_Y;
}

void pid_calculate(void)
{

	duty1 = Incremental_pid1(speed_tar_1, RC_encoder1); // 计算得到每个电机输出目标值
	duty2 = Incremental_pid2(speed_tar_2, RC_encoder2);
	duty3 = Incremental_pid3(speed_tar_3, RC_encoder3);
	duty4 = Incremental_pid4(speed_tar_4, RC_encoder4);
	
	
//	duty1 = -5000; // 计算得到每个电机输出目标值
//	duty2 = 0;
//	duty3 = 0;
//	duty4 = 0;
//		rt_kprintf("%d,%d,%d,%d   ", duty1,duty2,duty3,duty4);
		//rt_kprintf("%d,%d,%d,%d\n", RC_encoder1,RC_encoder2,RC_encoder3,RC_encoder4);
//		rt_kprintf("%d,%d,%d,%d\n", (int)speed_tar_1*116,(int)speed_tar_2*116,(int)speed_tar_3*116,(int)speed_tar_4*116);

	duty1 = limit(duty1, PWM_LIMIT);
	duty2 = limit(duty2, PWM_LIMIT);
	duty3 = limit(duty3, PWM_LIMIT);
	duty4 = limit(duty4, PWM_LIMIT);

}

//void pid_calculate(void)
//{
//    static float previous_duty1 = 0.0;
//    static float previous_duty2 = 0.0;
//    static float previous_duty3 = 0.0;
//    static float previous_duty4 = 0.0;
//    float acceleration_factor = 0.1; // 缓慢加速的因子

//    float target_duty1 = Incremental_pid1(speed_tar_1, RC_encoder1);
//    float target_duty2 = Incremental_pid2(speed_tar_2, RC_encoder2);
//    float target_duty3 = Incremental_pid3(speed_tar_3, RC_encoder3);
//    float target_duty4 = Incremental_pid4(speed_tar_4, RC_encoder4);

//    duty1 = previous_duty1 + (target_duty1 - previous_duty1) * acceleration_factor;
//    duty2 = previous_duty2 + (target_duty2 - previous_duty2) * acceleration_factor;
//    duty3 = previous_duty3 + (target_duty3 - previous_duty3) * acceleration_factor;
//    duty4 = previous_duty4 + (target_duty4 - previous_duty4) * acceleration_factor;

//    duty1 = limit(duty1, PWM_LIMIT);
//    duty2 = limit(duty2, PWM_LIMIT);
//    duty3 = limit(duty3, PWM_LIMIT);
//    duty4 = limit(duty4, PWM_LIMIT);

//    previous_duty1 = duty1;
//    previous_duty2 = duty2;
//    previous_duty3 = duty3;
//    previous_duty4 = duty4;
//}


void motor_control(bool run)
{
    if (run)
    {
        pid_calculate();
    }
    else
    {
        duty1 = 0;
        duty2 = 0;
        duty3 = 0;
        duty4 = 0;
    }

    if (duty1 >= 0)
    {
        gpio_set(DIR_1, 0);
        pwm_duty(PWM_1, duty1);
    }
    else
    {
        gpio_set(DIR_1, 1);
        pwm_duty(PWM_1, -duty1);
    }
    if (duty2 >= 0)
    {
        gpio_set(DIR_2, 0);
        pwm_duty(PWM_2, duty2);
    }
    else
    {
        gpio_set(DIR_2, 1);
        pwm_duty(PWM_2, -duty2);
    }

    if (duty3 >= 0)
    {
        gpio_set(DIR_3, 0);
        pwm_duty(PWM_3, duty3);
    }
    else
    {
        gpio_set(DIR_3, 1);
        pwm_duty(PWM_3, -duty3);
    }

    if (duty4 >= 0)
    {
        gpio_set(DIR_4, 0);
        pwm_duty(PWM_4, duty4);
    }
    else
    {
        gpio_set(DIR_4, 1);
        pwm_duty(PWM_4, -duty4);
    }
}