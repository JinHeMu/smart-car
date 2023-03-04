#include "motor.h"




#define PWM_LIMIT 10000



#define DIR_1 D0
#define DIR_2 D1
#define PWM_1 PWM2_MODULE3_CHA_D2
#define PWM_2 PWM2_MODULE3_CHB_D3

#define DIR_3 D14
#define DIR_4 D15
#define PWM_3 PWM1_MODULE0_CHA_D12
#define PWM_4 PWM1_MODULE0_CHB_D13

int32 duty1=0,duty2=0,duty3=0,duty4=0;//电机PWM值
float kp=0.9, ki=2.5, kd=0.1;//增量式PID
float Angel_KP=0.15 , Angel_KI=0 , Angel_KD=1.5 ;


//float pictureP=0.15f,pictureI=0,pictureD=0.03f;


// duty 10000 - encoder 2800
// when duty > 3000 , motor is running
//(encoder+653.33)*3 = duty


//3000 500
//4000 900
//5000 1200
//6000 1500
//7000 1900
//8000 2200
//9000 2500


//积分法计算位移参数

//电机目标速度
int speed_tar_1 = 0;
int speed_tar_2 = 0;
int speed_tar_3 = 0;
int speed_tar_4 = 0;

double speed_tar = 5;//目标速度 0.0086*encoder encoder = speed * 116.279 cm/s


void motor_init(void)
{
			gpio_init(DIR_1, GPO, 0, GPIO_PIN_CONFIG); 		//单片机端口D0 初始化DIR_1			GPIO
			gpio_init(DIR_2, GPO, 0, GPIO_PIN_CONFIG); 		//单片机端口D1 初始化DIR_2			GPIO
			pwm_init(PWM_1, 17000, 0);      				//单片机端口D2 初始化PWM_1周期10K 占空比0
			pwm_init(PWM_2, 17000, 0);     					//单片机端口D3 初始化PWM_2周期10K 占空比0
    
			gpio_init(DIR_3, GPO, 0, GPIO_PIN_CONFIG);      //单片机端口D0 初始化DIR_1          GPIO
			gpio_init(DIR_4, GPO, 0, GPIO_PIN_CONFIG);      //单片机端口D1 初始化DIR_2          GPIO
			pwm_init(PWM_3, 17000, 0);                      //单片机端口D2 初始化PWM_1周期10K 占空比0
			pwm_init(PWM_4, 17000, 0);                      //单片机端口D3 初始化PWM_2周期10K 占空比0
}


void car_omni(float x, float y, float z)//全向移动逆运算
{
//    speed_tar_1= y + x + z;
//    speed_tar_2= y - x - z;
//    speed_tar_3= y - x + z;
//    speed_tar_4= y + x - z;
	
	  speed_tar_1= x - y + z;
    speed_tar_2= x + y - z;
    speed_tar_3= x + y + z;
    speed_tar_4= x - y - z;
	
	
	//
}



void car_ahead(){
    speed_tar_1 = speed_tar;
    speed_tar_2 = speed_tar;
    speed_tar_3 = speed_tar;
    speed_tar_4 = speed_tar;
}

void car_back(){
    speed_tar_1 = -speed_tar;
    speed_tar_2 = -speed_tar;
    speed_tar_3 = -speed_tar;
    speed_tar_4 = -speed_tar;
}

void car_lsideWay(){
    speed_tar_1 = speed_tar;
    speed_tar_2 = -speed_tar;
    speed_tar_3 = -speed_tar;
    speed_tar_4 = speed_tar;
}

void car_rsideWay(){
    speed_tar_1 = -speed_tar;
    speed_tar_2 = speed_tar;
    speed_tar_3 = speed_tar;
    speed_tar_4 = -speed_tar;
}

void car_diagonal(){
    speed_tar_1 = speed_tar;
    speed_tar_2 = 0;
    speed_tar_3 = speed_tar;
    speed_tar_4 = 0;
}

void car_turnround(){
    speed_tar_1 = speed_tar;
    speed_tar_2 = speed_tar;
    speed_tar_3 = -speed_tar;
    speed_tar_4 = -speed_tar;
}



void car_anticlockwise() {
    speed_tar_1 = -speed_tar;
    speed_tar_2 = -speed_tar;
    speed_tar_3 = speed_tar;
    speed_tar_4 = speed_tar;
}

void car_concerning(){
    speed_tar_1 = speed_tar;
    speed_tar_2 = 0;
    speed_tar_3 = 0;
    speed_tar_4 = speed_tar;
}

void car_stop(){
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
    static float current_error,result, last_result, prev_error, last_error;
    prev_error = last_error;
    last_error = current_error;
    
    current_error=(float)(Target - Encoder);//当前误差
    result += kp*(current_error - last_error) + kd*(current_error - 2*last_error + prev_error) + ki*current_error;

    return (int)result;
	
}

int Incremental_pid2(int Target, int Encoder)
{
    static float current_error,result, last_result, prev_error, last_error;
    prev_error = last_error;
    last_error = current_error;
    
    current_error=(float)(Target - Encoder);//当前误差
    result += kp*(current_error - last_error) + kd*(current_error - 2*last_error + prev_error) + ki*current_error;

    return (int)result;
}
	

int Incremental_pid3(int Target, int Encoder)
{
    static float current_error,result, last_result, prev_error, last_error;
    prev_error = last_error;
    last_error = current_error;
    
    current_error=(float)(Target - Encoder);//当前误差
    result += kp*(current_error - last_error) + kd*(current_error - 2*last_error + prev_error) + ki*current_error;

    return (int)result;
}
	

int Incremental_pid4(int Target, int Encoder)
{
    static float current_error,result, last_result, prev_error, last_error;
    prev_error = last_error;
    last_error = current_error;
    
    current_error=(float)(Target - Encoder);//当前误差
    result += kp*(current_error - last_error) + kd*(current_error - 2*last_error + prev_error) + ki*current_error;
    

    return (int)result;
	
}

int angel_pid(int NowAngel,int TargetAngel){
    if (NowAngel<=0){
        if(NowAngel-TargetAngel<=-180){
            NowAngel+=180; 
            TargetAngel-=180;
        }
    }else if(NowAngel>0){
        if(NowAngel-TargetAngel>=180){
            NowAngel-=180;
            TargetAngel+=180;
        }
    }
    static float current_error,Speed_Z,Integral_current_error,Last_current_error;
    current_error=(float)(TargetAngel - NowAngel);
    Integral_current_error+=current_error;
    Speed_Z=Angel_KP*current_error+Angel_KI*Integral_current_error+Angel_KD*(current_error-Last_current_error);
    Last_current_error=current_error;

    if(Speed_Z>=10)
        Speed_Z=10;
    if(Speed_Z<=-10)
        Speed_Z=-10;
    return (int)Speed_Z;
}






void pid_calculate(void){
    duty1 = Incremental_pid1(speed_tar_1*116,RC_encoder1);
    duty2 = Incremental_pid2(speed_tar_2*116,RC_encoder2);
    duty3 = Incremental_pid3(speed_tar_3*116,RC_encoder3);
    duty4 = Incremental_pid4(speed_tar_4*116,RC_encoder4);
//		rt_kprintf("speed_tar_2:%d\n", speed_tar_2);
//		rt_kprintf("duty2:%d\n",duty2);
//		rt_kprintf("encoder2:%d\n", RC_encoder2);

//		duty1 = 0;
//    duty2 = 4000;
//    duty3 = 6000;
//    duty4 = 8000;
	
//	duty1 = speed_tar;
//	duty2 = speed_tar;
//	duty3 = speed_tar;
//	duty4 = speed_tar;
	
	
	
	
	
    duty1 = limit(duty1, PWM_LIMIT);
    duty2 = limit(duty2, PWM_LIMIT);
    duty3 = limit(duty3, PWM_LIMIT);
    duty4 = limit(duty4, PWM_LIMIT);
}


void motor_control(bool run)
{
    if(run) {
        pid_calculate();
    }else{
        duty1 = 0;
        duty2 = 0;
        duty3 = 0;
        duty4 = 0;
    }

    if(duty1>=0){
        gpio_set(DIR_1,0);
        pwm_duty(PWM_1,duty1);
    } else {
        gpio_set(DIR_1,1);
        pwm_duty(PWM_1,-duty1);
    }
    if(duty2>=0){
        gpio_set(DIR_2,0);
        pwm_duty(PWM_2,duty2);
    } else {
        gpio_set(DIR_2,1);
        pwm_duty(PWM_2,-duty2);
    }

    if(duty3>=0){
        gpio_set(DIR_3,0);
        pwm_duty(PWM_3,duty3);
    } else {
        gpio_set(DIR_3,1);
        pwm_duty(PWM_3,-duty3);
    }

    if(duty4>=0){
        gpio_set(DIR_4,0);
        pwm_duty(PWM_4,duty4);
    } else {
        gpio_set(DIR_4,1);
        pwm_duty(PWM_4,-duty4);
    }
}