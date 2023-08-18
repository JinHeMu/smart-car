#include "carmove2.h"

#define field_width 35
#define field_height 25

#define A 0x41
#define B 0x42
#define C 0x43
#define COLUMN 0x44
#define ROW 0x45

#define UP 1
#define RIGHT 2
#define LOW 3
#define LEFT 4
#define MAIN_CATEGORY 6

#define boundry_speed 300
#define back_speed 300

uint8 game_mode = 0; // 0为初赛  1为决赛

Pose_car car; // 定义car，作为位姿的载体

rt_sem_t back_sem;
rt_sem_t arrive_sem;        // 到达
rt_sem_t boundry_sem;       // 边线
rt_sem_t correct_sem;       // 矫正
rt_sem_t recognize_sem;     // 识别
rt_sem_t obj_detection_sem; // 目标检测

rt_thread_t back_th;
rt_thread_t arrive_th;        // 到达位置点线程
rt_thread_t boundry_th;       // 边线
rt_thread_t correct_th;       // 矫正线程
rt_thread_t recognize_th;     // 识别并放入盒中搬运线程
rt_thread_t obj_detection_th; // 目标检测线程

uint16 pic_dis = 0; // 卡片距离
uint16 det_dis = 0;

uint8 boundry_num = 0;      // 识别边界个数
uint8 boundry_mode = RIGHT; // 1向前 2向右 3向下 4向左
uint8 card_current_num = 0; // 识别到的卡片数量
uint8 card_sum_num = 0;     // 识别到的总卡片数量
float card_y_average = 0;
float card_y_max = 0;

unknowcard detectedCards[24];



//**初赛**

/*
1-上
2-右
3-下
4-左
5-车载
*/

//**决赛**

/*
1-上
2-右
3-下
4-左
5-车载
6-大类
*/

struct card cabbage = {"vegetable", "cabbage", 1}; // 上三类
struct card bean = {"food", "bean", 1};            // 上三类
struct card orange = {"fruit", "orange", 1};       // 上三类

struct card apple = {"fruit", "apple", 2};       // 右三类
struct card pepper = {"vegetable", "pepper", 2}; // 右三类
struct card peanut = {"food", "peanut", 2};      // 右三类

struct card bannana = {"fruit", "bannana", 4};   // 下三类
struct card radish = {"vegetable", "radish", 4}; // 下三类
struct card rice = {"food", "rice", 4};          // 下三类

struct card durian = {"fruit", "durian", 5};         // 左三类
struct card cucumber = {"vegetable", "cucumber", 5}; // 左三类
struct card potato = {"food", "potato", 5};          // 左三类

struct card grape = {"fruit", "grape", 3};           // 车载
struct card corn = {"food", "corn", 3};              // 车载
struct card eggplant = {"vegetable", "eggplant", 3}; // 车载

void car_turnto(float angle)
{
    car.current_angle = angle;
}

int distance(float current_x, float current_y, float target_x, float target_y)
{

    float det_x;
    float det_y;
    float distance;

    det_x = target_x - current_x;
    det_y = target_y - current_y;
    distance = sqrt(det_x * det_x + det_y * det_y);

    return (int)distance;
}

float get_angle(float current_x, float current_y, float target_x, float target_y)
{
    float angle;
    angle = atan2((target_x - current_x), (target_y - current_y));
    return angle;
}

void car_speed_x(int16 tar_speed)
{
    float acceleration = 0.02; // 加速度，可根据实际情况调整
    float max_speed = 1;       // 最大速度，可根据实际情况调整
    float current_speed = 0;
    float progress = 0; // 用于表示进度，范围从 0 到 1

    // 逐渐增加速度
    while (progress < 1)
    {
        // 使用缓动函数（二次缓动函数）计算进度
        progress = 1 - pow(1 - current_speed / max_speed, 2);

        current_speed += acceleration;

        if (current_speed > max_speed)
            current_speed = max_speed;

        car.Speed_X = current_speed * tar_speed;
        rt_thread_mdelay(20);
    }
}

void car_speed_y(int16 tar_speed)
{
    float acceleration = 0.02; // 加速度，可根据实际情况调整
    float max_speed = 1;       // 最大速度，可根据实际情况调整
    float current_speed = 0;
    float progress = 0; // 用于表示进度，范围从 0 到 1

    // 逐渐增加速度
    while (progress < 1)
    {
        // 使用缓动函数（二次缓动函数）计算进度
        progress = 1 - pow(1 - current_speed / max_speed, 2);

        current_speed += acceleration;

        if (current_speed > max_speed)
            current_speed = max_speed;

        car.Speed_Y = current_speed * tar_speed;
        rt_thread_mdelay(20);
    }
}

void car_move(float tar_x, float tar_y)
{

    float target_distance = distance(car.MileageX, car.MileageY, tar_x, tar_y);
    float current_distance = target_distance;
    float acceleration = 0.01; // 加速度，可根据实际情况调整
    float max_speed = 1;     // 最大速度，可根据实际情况调整
    float current_speed = 0;
    float angle = get_angle(car.MileageX, car.MileageY, tar_x, tar_y);

	
	
	
    // rt_kprintf("TARGRT TO X:%d, Y:%d\n", (int)tar_x, (int)tar_y);

//    if (target_distance >= 450)
//    {

//        acceleration = 0.01;
//        max_speed = 0.5;

//        tar_x += sin(angle) * target_distance / 5;
//        tar_y += cos(angle) * target_distance / 5 - 40;
//    }
//    else
//    {
            tar_x -= 30;
						tar_y -= 30;
//			 tar_y += cos(angle) * target_distance / 7;
//       tar_y += cos(angle) * target_distance / 7;


//        tar_y -= 20;

//    }

    // rt_kprintf("dis:%d\n", (int)target_distance);
    // rt_kprintf("MOVEING TO X:%d, Y:%d\n", (int)tar_x, (int)tar_y);

    while (current_distance > 5) // 持续运动
    {
        // 逐渐增加速度
        if (current_speed < max_speed)
            current_speed += acceleration;

        // 控制电机速度
        car.Speed_X = current_speed * picture_x_pid((int)car.MileageX, (int)tar_x); // cm
        car.Speed_Y = current_speed * picture_y_pid((int)car.MileageY, (int)tar_y);

        rt_thread_mdelay(20);

        // 更新当前距离
        current_distance = distance(car.MileageX, car.MileageY, tar_x, tar_y);
        // rt_kprintf("dis:%d\n", (int)current_distance);
    }

    car.Speed_X = 0;
    car.Speed_Y = 0;

    rt_mb_send(buzzer_mailbox, 100); // 给buzzer_mailbox发送100

    // rt_kprintf("I HAVE ARRIVE X:%d, Y:%d\n", (int)car.MileageX, (int)car.MileageY);
}

void car_moveto_boundry(uint8 mode, uint16 speed)
{

    // 1.向上 2.向右  3.向下  4.向左

    // 定义四条边线
    // rt_kprintf("GO TO BOUNDRE!!!\n");

    ART1_mode = 4;
    ART1_CORRECT_Boundary_Flag = 0;

    switch (mode)
    {
    case UP:
        uart_putchar(USART_4, ROW); // 发送OPENART1告诉该识别边线了
        //rt_thread_mdelay(500);
        car_speed_y(speed);
        while (ART1_CORRECT_Boundary_Flag == 0)
        {
            rt_thread_mdelay(100);
        }
        break;

    case RIGHT:
        uart_putchar(USART_4, COLUMN); // 发送OPENART1告诉该识别边线了
        //rt_thread_mdelay(500);
        car_speed_x(speed);
        while (ART1_CORRECT_Boundary_Flag == 0)
        {

            rt_thread_mdelay(100);
        }
        break;

    case LOW:
        uart_putchar(USART_4, ROW); // 发送OPENART1告诉该识别边线了
        //rt_thread_mdelay(500);
        car_speed_y(-speed);
        while (ART1_CORRECT_Boundary_Flag == 0)
        {

            rt_thread_mdelay(100);
        }
        break;

    case LEFT:
        uart_putchar(USART_4, COLUMN); // 发送OPENART1告诉该识别边线了
        //rt_thread_mdelay(500);
        car_speed_x(-speed);
        while (ART1_CORRECT_Boundary_Flag == 0)
        {

            rt_thread_mdelay(100);
        }
        break;
    }
    car.Speed_X = 0;
    car.Speed_Y = 0;
    rt_mb_send(buzzer_mailbox, 100);
    // rt_kprintf("I HAVE ARRIVED BOUNDRE!!!");
}


void boundry_correct_angle(int8 angle)
{
    if(abs(angle) < 10)
    {
        angle_z = -angle;
    }

}



// void car_moveto_boundry(

// )

void unload(uint8 mode)
{
    switch (mode)
    {
    case UP:
        car.Speed_Y = 150;
        rt_thread_mdelay(2000);
        car.Speed_Y = 0;
        arm_openbox(1, 1);


        //需要添加部分函数防止压到舱门

        car.Speed_X = 150;
        car.Speed_Y = -300;
        rt_thread_mdelay(2000);
        car.Speed_X = 0;
        car.Speed_Y = 0;

        break;


    case RIGHT:

        car.Speed_X = 150;
        rt_thread_mdelay(1000);
        car.Speed_X = 0;
        arm_openbox(2, 2);


        car.Speed_X = -150;
        car.Speed_Y = -150;
        rt_thread_mdelay(2000);
        car.Speed_X = 0;
        car.Speed_Y = 0;
        break;

    case LOW:

//        car.Speed_Y = -150;
//        rt_thread_mdelay(1000);
//        car.Speed_Y = 0;
		
        arm_openbox(4, 4);

        car.Speed_X = -150;
        car.Speed_Y = 150;
        rt_thread_mdelay(2000);
        car.Speed_X = 0;
        car.Speed_Y = 0;

        break;
        
    case LEFT:

        car.Speed_X = -150;
        rt_thread_mdelay(1000);
        car.Speed_X = 0;

        arm_openbox(5,5);


        car.Speed_X = 150;
        // car.Speed_Y = 150;
        rt_thread_mdelay(2000);
        car.Speed_X = 0;
        car.Speed_Y = 0;

        break;

    case MAIN_CATEGORY:

        arm_openbox(6,6);

        break;
    }
}

void back_entry(void *param)
{
    while (1)
    {
        rt_sem_take(back_sem, RT_WAITING_FOREVER); // 获取回库信号

        // 默认此时位于左侧边线
        car_moveto_boundry(LEFT, back_speed);unload(LEFT);              //左类

        car_moveto_boundry(UP, boundry_speed);
			
			
        if(game_mode)
        {
            
            unload(MAIN_CATEGORY);       //大类

        }
        unload(UP);                                         //上类

        car_moveto_boundry(RIGHT, back_speed);unload(RIGHT);            //右类

        car_moveto_boundry(LOW, back_speed);unload(LOW);                //下类

        car_moveto_boundry(LEFT, back_speed);
			
			
		car.Speed_X = 150;
        rt_thread_mdelay(750);
        car.Speed_X = 0;
			
		car_moveto_boundry(LOW, back_speed);   //回库
			

				

        rt_thread_mdelay(10000);

    }
}

void boundry_entry(void *param)
{
    while (1)
    {
        rt_sem_take(boundry_sem, RT_WAITING_FOREVER);
			
			
				
				ART1_mode = 4;
        ART1_CORRECT_Boundary_Flag = 0;

        // if(boundry_num % 2 == 0)
        // {
        //     boundry_mode = LEFT;
        // }else if (boundry_num % 2 == 1)
        // {
        //     boundry_mode = RIGHT;
        // }

        switch (boundry_mode)
        {
        case UP:
            uart_putchar(USART_4, ROW); // 发送OPENART1告诉该识别边线了
            //rt_thread_mdelay(500);
            car_speed_y(boundry_speed);
            while (ART1_CORRECT_Boundary_Flag == 0)
            {
                rt_thread_mdelay(10);
            }
            break;

        case RIGHT:
            uart_putchar(USART_4, COLUMN); // 发送OPENART1告诉该识别边线了
            //rt_thread_mdelay(500);
            car_speed_x(boundry_speed);
            while (ART1_CORRECT_Boundary_Flag == 0)
            {
                rt_thread_mdelay(10);
            }
            break;

        case LOW:
            uart_putchar(USART_4, ROW); // 发送OPENART1告诉该识别边线了
						//rt_thread_mdelay(500);
            car_speed_y(-boundry_speed);
            while (ART1_CORRECT_Boundary_Flag == 0)
            {

                rt_thread_mdelay(10);
            }
            break;

        case LEFT:
            uart_putchar(USART_4, COLUMN); // 发送OPENART1告诉该识别边线了
            //rt_thread_mdelay(500);
            car_speed_x(-boundry_speed);
            while (ART1_CORRECT_Boundary_Flag == 0)
            {
                rt_thread_mdelay(10);
            }
            //boundry_correct_angle(ART1_CORRECT_Boundary_Angle);
            break;
        }
				
				ART3_mode = 0;
        car.Speed_X = 0;
        car.Speed_Y = 0;
        boundry_num++;
        rt_mb_send(buzzer_mailbox, 100);

        rt_sem_release(arrive_sem); // 获取到达信号
    }
}

void arrive_entry(void *param)
{
    while (1)
    {
        rt_sem_take(arrive_sem, RT_WAITING_FOREVER); // 获取到达信号

        ART3_mode = 0;

        if (card_current_num == 0)
        {


            //先移动到目标检测到的最远距离
            uint16 car_current_x = car.MileageX;
            car_move(car_current_x, card_y_max);
            card_sum_num = 0;
            // card_y_average = card_y_average / card_sum_num + 40;
            // car_move(car_current_x, card_y_average);
            // card_sum_num = 0;

            //如果第三次识别到边线
            if(boundry_num == 3)
            {
                //back
                rt_sem_release(back_sem);
            }else
            {

//                //向前移动
//                car.Speed_Y = 150;
//                rt_thread_mdelay(500);
//                car.Speed_Y = 0;

                car_moveto_boundry(LEFT, boundry_speed);//一直向左移动至边线


                //向右移动
                car.Speed_X = 150;
                rt_thread_mdelay(1500);
                car.Speed_X = 0;

                //继续向右平移寻找边线
                boundry_mode = RIGHT;
                ART3_mode = 1;
                rt_sem_release(boundry_sem); // 遍历完所有检测到的点,进行下一次遍历

            }
        }
        else
        {
            car_move(detectedCards[card_current_num].Current_x, detectedCards[card_current_num].Current_y); // 根据摄像头的距离进行相应的修改
            rt_sem_release(correct_sem); // 没有遍历完就矫正卡片
        }
    }
}





void correct_entry(void *param)
{
    while (1)
    {
        rt_sem_take(correct_sem, RT_WAITING_FOREVER); // 获取矫正信号

        ART2_CORRECT_Flag = 0;

        ART2_mode = 2;            // art矫正模式
        uart_putchar(USART_1, B); // 持续发送“B”来告诉openart该矫正了

        // rt_kprintf("correcting!!!\n");

        while (ART2_CORRECT_Flag == 0)
        {
            pic_dis = (int)distance(ART2_CORRECT_X, ART2_CORRECT_Y, 0, 0);

            if (pic_dis > 65)
            {
                car.correct_speed = 1.5;
            }
            else if (pic_dis <= 65 && pic_dis > 45)
            {
                car.correct_speed = 1;
            }
            else if (pic_dis <= 45 && pic_dis > 25)
            {
                car.correct_speed = 1.5;
            }
            else if (pic_dis <= 25 && pic_dis > 15)
            {
                car.correct_speed = 2.5;
            }
            else if (pic_dis < 15)
            {
                car.correct_speed = 3;
            }
            else
            {
                car.correct_speed = 0;
            }

            car.Speed_X = car.correct_speed * ART2_CORRECT_X;
            car.Speed_Y = car.correct_speed * ART2_CORRECT_Y;
            rt_thread_mdelay(5);
        }
        car.Speed_X = 0;
        car.Speed_Y = 0;
        rt_mb_send(buzzer_mailbox, 100);
				
//				//矫正当前x坐标
//				car.MileageX = detectedCards[card_current_num].Current_x;
				
	      // 更新最大的 Current_y 值
				// card_y_average += car.MileageY;
				
        if(card_y_max < car.MileageY)
        {
            card_y_max = car.MileageY;
        }


        //rt_sem_release(recognize_sem);
        //rt_sem_release(correct_sem);
        card_current_num--;
        //rt_sem_release(arrive_sem);
				rt_sem_release(recognize_sem);
    }
}

void recognize_entry(void *param)
{

    while (1)
    {
        rt_sem_take(recognize_sem, RT_WAITING_FOREVER); // 接受识别信号量

        ART1_mode = 3;
        uart_putchar(USART_4, C);

        while (ART1_CLASS_Flag == 0)
        {
            rt_thread_mdelay(100);
        };

        ART1_CLASS_Flag = 0;

        if (strcmp(classified, apple.Small_category) == 0)
        {
            rt_kprintf("This is a apple.\n");
            if (strcmp(taget_Big_category, apple.Big_category) == 0)
            {
                rt_kprintf("This is  fruit.\n");
                apple.Box_location = 6;
                arm_putbox(apple.Box_location);
            }
            else if (apple.Box_location)
            {
                arm_putbox(apple.Box_location);
            }
        }
        else if (strcmp(classified, bannana.Small_category) == 0)
        {
            rt_kprintf("This is a bannana.\n");
            if (strcmp(taget_Big_category, bannana.Big_category) == 0)
            {
                rt_kprintf("This is  fruit.\n");
                bannana.Box_location = 6;
                arm_putbox(bannana.Box_location);
            }
            else if (bannana.Box_location)
            {
                arm_putbox(bannana.Box_location);
            }
        }
        else if (strcmp(classified, grape.Small_category) == 0)
        {
            // 车载
            rt_kprintf("This is a grape.\n");
            if (strcmp(taget_Big_category, grape.Big_category) == 0)
            {
                rt_kprintf("This is  fruit.\n");
                grape.Box_location = 6; // 放入盒子6中
                arm_putbox(grape.Box_location);
            }
            else
            {
                arm_putbox(grape.Box_location);
            }
        }
        else if (strcmp(classified, durian.Small_category) == 0)
        {
            rt_kprintf("This is a durian.\n");
            if (strcmp(taget_Big_category, durian.Big_category) == 0)
            {
                rt_kprintf("This is  fruit.\n");
                durian.Box_location = 6;
                arm_putbox(durian.Box_location);
            }
            else if (durian.Box_location)
            {
                arm_putbox(durian.Box_location);
            }
        }
        else if (strcmp(classified, orange.Small_category) == 0)
        {
            rt_kprintf("This is a orange.\n");

            if (strcmp(taget_Big_category, orange.Big_category) == 0)
            {
                rt_kprintf("This is  fruit.\n");
                orange.Box_location = 6;
                arm_putbox(orange.Box_location);
                // 放入盒子中
            }
            else if (orange.Box_location)
            {
                arm_putbox(orange.Box_location);
            }
        }
        else if (strcmp(classified, cabbage.Small_category) == 0)
        {
            rt_kprintf("This is a cabbage.\n");
            if (strcmp(taget_Big_category, cabbage.Big_category) == 0)
            {
                rt_kprintf("This is  vegetable.\n");
                cabbage.Box_location = 6;
                arm_putbox(cabbage.Box_location);
                // 放入盒子中
            }
            else if (cabbage.Box_location)
            {
                arm_putbox(cabbage.Box_location);
            }
        }
        else if (strcmp(classified, cucumber.Small_category) == 0)
        {
            rt_kprintf("This is an cucumber.\n");
            if (strcmp(taget_Big_category, cucumber.Big_category) == 0)
            {
                rt_kprintf("This is  vegetable.\n");
                cucumber.Box_location = 6;
                arm_putbox(cucumber.Box_location);
                // 放入盒子中
            }
            else if (cucumber.Box_location)
            {
                arm_putbox(cucumber.Box_location);
            }
        }
        else if (strcmp(classified, eggplant.Small_category) == 0)
        {
            // 车载
            rt_kprintf("This is a eggplant.\n");
            if (strcmp(taget_Big_category, eggplant.Big_category) == 0)
            {
                rt_kprintf("This is  vegetable.\n");
                eggplant.Box_location = 6;
                arm_putbox(eggplant.Box_location);
                // 放入盒子中
            }
            else
            {
                arm_putbox(eggplant.Box_location); // 放入盒子1中
            }
        }
        else if (strcmp(classified, radish.Small_category) == 0)
        {
            rt_kprintf("This is a radish.\n");
            if (strcmp(taget_Big_category, radish.Big_category) == 0)
            {
                rt_kprintf("This is  vegetable.\n");
                radish.Box_location = 6;
                arm_putbox(radish.Box_location);
                // 放入盒子中
            }
            else if (radish.Box_location)
            {
                arm_putbox(radish.Box_location);
            }
        }
        else if (strcmp(classified, pepper.Small_category) == 0)
        {
            rt_kprintf("This is a pepper.\n");
            if (strcmp(taget_Big_category, pepper.Big_category) == 0)
            {
                rt_kprintf("This is  vegetable.\n");
                pepper.Box_location = 6;
                arm_putbox(pepper.Box_location);
                // 放入盒子中
            }
            else if (pepper.Box_location)
            {
                arm_putbox(pepper.Box_location);
            }
        }
        else if (strcmp(classified, corn.Small_category) == 0)
        {
            rt_kprintf("This is an corn.\n");
            if (strcmp(taget_Big_category, corn.Big_category) == 0)
            {
                rt_kprintf("This is  food.\n");
                corn.Box_location = 6;
                arm_putbox(corn.Box_location);
                // 放入盒子中
            }
            else
            {
                arm_putbox(corn.Box_location); // 放入盒子1中
            }
        }
        else if (strcmp(classified, bean.Small_category) == 0)
        {
            rt_kprintf("This is a bean.\n");
            if (strcmp(taget_Big_category, bean.Big_category) == 0)
            {
                rt_kprintf("This is  food.\n");
                bean.Box_location = 6;
                arm_putbox(bean.Box_location);
                // 放入盒子中
            }
            else if (bean.Box_location)
            {
                arm_putbox(bean.Box_location);
            }
        }
        else if (strcmp(classified, peanut.Small_category) == 0)
        {
            rt_kprintf("This is an peanut.\n");
            if (strcmp(taget_Big_category, peanut.Big_category) == 0)
            {
                rt_kprintf("This is  food.\n");
                peanut.Box_location = 6;
                arm_putbox(peanut.Box_location);
                // 放入盒子中
            }
            else if (peanut.Box_location)
            {
                arm_putbox(peanut.Box_location);
            }
        }
        else if (strcmp(classified, potato.Small_category) == 0)
        {
            rt_kprintf("This is a potato.\n");
            if (strcmp(taget_Big_category, potato.Big_category) == 0)
            {
                rt_kprintf("This is  food.\n");
                potato.Box_location = 6;
                arm_putbox(potato.Box_location);
                // 放入盒子中
            }
            else if (potato.Box_location)
            {
                arm_putbox(potato.Box_location);
            }
        }
        else if (strcmp(classified, rice.Small_category) == 0)
        {
            rt_kprintf("This is a rice.\n");

            if (strcmp(taget_Big_category, rice.Big_category) == 0)
            {
                rt_kprintf("This is  food.\n");
                rice.Box_location = 6;
                arm_putbox(rice.Box_location);
                // 放入盒子中
            }
            else if (rice.Box_location)
            {
                arm_putbox(rice.Box_location);
            }
        }
        rt_mb_send(buzzer_mailbox, 100);
    }
}

// 每次识别到卡片记录当前里程计位置，到达边线之后，从右往左逐渐遍历每一个卡片
// 是否需要遍历的时候也识别卡片？

void obj_detection_entry(void *param)
{
    while (1)
    {
        rt_sem_take(obj_detection_sem, RT_WAITING_FOREVER); // 接受识别信号量

        card_current_num++;
        card_sum_num++;
			
			
			
		float real_distance = (float)ART3_DETECT_DISTANCE * (float)ART3_DETECT_DISTANCE * 0.0039 - (float)ART3_DETECT_DISTANCE * 0.2716 + 47.0712;

        detectedCards[card_current_num].Current_x = car.MileageX;
        detectedCards[card_current_num].Current_y = car.MileageY + real_distance;

			//0.00425722687x ^2−0.247807950x+46.5513049

        ips114_showint16(50, card_current_num, (int)detectedCards[card_current_num].Current_x);

        rt_mb_send(buzzer_mailbox, 100);
    }
}

void car_start_init(void)
{

    back_sem = rt_sem_create("back_sem", 0, RT_IPC_FLAG_FIFO);
    arrive_sem = rt_sem_create("arrive_sem", 0, RT_IPC_FLAG_FIFO);
    boundry_sem = rt_sem_create("boundry_sem", 0, RT_IPC_FLAG_FIFO);
    correct_sem = rt_sem_create("correct_sem", 0, RT_IPC_FLAG_FIFO);             // 矫正信号量，接受就开始矫正
    recognize_sem = rt_sem_create("recognize_sem", 0, RT_IPC_FLAG_FIFO);         // 识别信号量，告诉单片机已经识别，接受就开始搬运
    obj_detection_sem = rt_sem_create("obj_detection_sem", 0, RT_IPC_FLAG_FIFO); // 目标检测信号量

    back_th = rt_thread_create("back_th", back_entry, RT_NULL, 1024, 28, 10);
    arrive_th = rt_thread_create("arrive_th", arrive_entry, RT_NULL, 1024, 28, 10);
    boundry_th = rt_thread_create("boundry_th", boundry_entry, RT_NULL, 1024, 28, 10);
    correct_th = rt_thread_create("correct_th", correct_entry, RT_NULL, 1024, 28, 10);
    recognize_th = rt_thread_create("recognize_th", recognize_entry, RT_NULL, 1024, 28, 28);
    obj_detection_th = rt_thread_create("obj_detection_th", obj_detection_entry, RT_NULL, 1024, 10, 10);
		
		rt_thread_startup(back_th);
    rt_thread_startup(arrive_th);
    rt_thread_startup(correct_th);
    rt_thread_startup(recognize_th);
    rt_thread_startup(obj_detection_th);
    rt_thread_startup(boundry_th);
}
