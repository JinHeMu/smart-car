#include "carmove.h"

/**************************************************************************
1.读取坐标，保存在相应的数组中
2.路径规划，每次都以最近的点位，重新排列一个新的点位
3.如何控制小车运动到应该到的点位，同时规划出下一个点位
**************************************************************************/

// 模式0  停止等待
// 模式1  导航 纠正 识别
// 模式2  搬运图片模式
// 模式3  回车库
// #define field_width 35
// #define field_height 25

#define field_width 18
#define field_height 18

uint8 running_mode = 0; // 小车功能模式：0为寻找坐标点模式   1为目标检测模式

point tar_point[40];    // 排序好顺序的目标坐标
int8 visited[40] = {0}; // 初始化访问数组，所有坐标都未访问
int8 nearestIndex = -1;

Pose_car car; // 定义car，作为位姿的载体

rt_sem_t correct_sem;       // 矫正
rt_sem_t arrive_sem;        // 到达
rt_sem_t recognize_sem;     // 识别
rt_sem_t obj_detection_sem; // 目标检测

rt_thread_t route_planning_th; // 路径规划线程
rt_thread_t correct_th;        // 矫正线程
rt_thread_t carry_th;          // 搬运线程
rt_thread_t obj_detection_th;  // 目标检测线程

char taget_Big_category[10] = "";

uint8 pic_dis = 0;
uint8 find_card_x = 0;
uint8 find_card_y = 0;
uint8 count = 0;
uint8 boundry_num = 0;

uint8 unknow_card_loction_x = 60;
uint8 unknow_card_loction_y = 60;

//**初赛**
// 车载 盒4
// 上三 单搬
// 右三 盒1
// 下三 盒2
// 左三 盒3

//**决赛**
// 车载 盒4
// 上三 单搬
// 右三 盒1
// 下三 单搬
// 左三 盒3
// 大类 盒2

struct card apple = {"fruit", "apple", 1, 36, 8};     // 右三类
struct card bannana = {"fruit", "bannana", 2, 8, 26}; // 下三类
struct card durian = {"fruit", "durian", 3, -1, 8};   // 左三类
struct card grape = {"fruit", "grape", 4, 0, 0};      // 车载
struct card orange = {"fruit", "orange", 0, 8, 26};   // 上三类

struct card cabbage = {"vegetable", "cabbage", 0, 12, 26};  // 上三类
struct card cucumber = {"vegetable", "cucumber", 3, -1, 4}; // 左三类
struct card eggplant = {"vegetable", "eggplant", 4, 0, 0};  // 车载
struct card radish = {"vegetable", "radish", 2, 12, 26};    // 下三类
struct card pepper = {"vegetable", "pepper", 1, 36, 4};     // 右三类

struct card corn = {"food", "corn", 4, 0, 0};       // 车载
struct card bean = {"food", "bean", 0, 4, 26};      // 上三类
struct card peanut = {"food", "peanut", 1, 36, 12}; // 右三类
struct card potato = {"food", "potato", 3, -1, 12}; // 左三类
struct card rice = {"food", "rice", 2, 4, 26};      // 下三类

/**************************************************************************
函数功能：求两点间的距离
入口参数：当前位置 目标位置
返回值：两点距离
**************************************************************************/

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

// 获得目标角度
int get_angle(float current_x, float current_y, float target_x, float target_y)
{
    int angle;
    angle = atan2((target_x - current_x), (target_y - current_y)) * 180 / PI;
    return (int)angle;
}

/**************************************************************************
函数功能：整合函数  输入坐标 小车到指定坐标
入口参数：目标坐标
返回值：无
**************************************************************************/

void car_move(float tar_x, float tar_y)
{
    rt_kprintf("MOVING!!!\n");
		
		
		int16 angle = get_angle(car.MileageX,car.MileageY,tar_x,tar_y);
	
		if(abs(angle) > 90)//如果是向下移动，就多向下移动一格，防止卡片在车底下
		{
			tar_y -= 10;
			rt_kprintf("tar_angle:\n", angle);
		}
		
    float target_distance = distance(car.MileageX, car.MileageY, tar_x, tar_y);
    float current_distance = target_distance;
    float acceleration = 0.02; // 加速度，可根据实际情况调整
    float max_speed = 1;       // 最大速度，可根据实际情况调整
    float current_speed = 0;

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
    }

    car.Speed_X = 0;
    car.Speed_Y = 0;
    rt_mb_send(buzzer_mailbox, 100); // 给buzzer_mailbox发送100

    rt_kprintf("GO TO X:%d, Y:%d\n", (int)car.MileageX, (int)car.MileageY);
}

void car_moveto_boundry(int8 tar_x, int8 tar_y)
{

    // 定义四条边线
    rt_kprintf("GO TO BOUNDRE!!!\n");

    ART1_mode = 4;

    if (tar_x < 0)
    {
        uart_putchar(USART_4, 0x44); // 发送OPENART1告诉该识别边线了
        rt_thread_mdelay(1000);
        while (ART1_CORRECT_Boundary_Flag == 0)
        {
            if (detect_flag) // 是否接受数据
            {
							rt_kprintf("I FOUND CARD!!!");
                break;
            }
            if (car.MileageX > 20)
            {
                car.Speed_X = -200;
            }
            else
            {
                car.Speed_X = -50;
            }
            rt_thread_mdelay(50);
        }

        if (detect_flag == 0) // 如果不是目标检测中断，就是边线
        {
            car.MileageX = 0;
        }
    }
    else if (tar_y < 0)
    {
        uart_putchar(USART_4, 0x45); // 发送OPENART1告诉该识别边线了
        rt_thread_mdelay(1000);

        while (ART1_CORRECT_Boundary_Flag == 0)
        {
            if (detect_flag)
            {
							rt_kprintf("I FOUND CARD!!!");
                break;
            }
            if (car.MileageY > 20)
            {
                car.Speed_Y = -200;
            }
            else
            {
                car.Speed_Y = -50;
            }
            rt_thread_mdelay(50);
        }

        if (detect_flag == 0)
        {
            car.MileageY = 0;
        }
    }
    else if (tar_x > field_width)
    {
        uart_putchar(USART_4, 0x44); // 发送OPENART1告诉该识别边线了
        rt_thread_mdelay(1000);

        while (ART1_CORRECT_Boundary_Flag == 0)
        {

            if (detect_flag)
            {
							rt_kprintf("I FOUND CARD!!!");
                break;
            }
            if (car.MileageX < (field_width * 20 - 20))
            {
                car.Speed_X = 200;
            }
            else
            {
                car.Speed_X = 50;
            }
            rt_thread_mdelay(50);
        }

        if (detect_flag == 0)
        {
            car.MileageX = field_width * 20;
        }
    }
    else if (tar_y > field_height)
    {
        uart_putchar(USART_4, 0x45); // 发送OPENART1告诉该识别边线了
        rt_thread_mdelay(1000);

        while (ART1_CORRECT_Boundary_Flag == 0)
        {

            if (detect_flag)
            {
                rt_kprintf("I FOUND CARD!!!");
                break;
            }
            if (car.MileageY < (field_height * 20 - 40))
            {
                car.Speed_Y = 150;
            }
            else
            {
                car.Speed_Y = 50;
            }
            rt_thread_mdelay(50);
        }
        if (detect_flag == 0)
        {
            car.MileageY = field_height * 20;
        }
    }

    car.Speed_X = 0;
    car.Speed_Y = 0;
    rt_mb_send(buzzer_mailbox, 100); // 给buzzer_mailbox发送100
    // rt_kprintf("I HAVE ARRIVED BOUNDRE!!!");
    // rt_kprintf("flag:%d\n", detect_flag);
    ART1_CORRECT_Boundary_Flag = 0;
}

void car_boundry_carry(int8 tar_x, int8 tar_y)
{
    if (tar_x < 0)
    {
        car.Speed_X = -200;
        rt_thread_mdelay(1000);
        car.Speed_X = 0;
    }
    else if (tar_x > field_width)
    {
        car.Speed_X = 200;
        rt_thread_mdelay(500);
        car.Speed_X = 0;
    }
    else if (tar_y < 0)
    {
        car.Speed_Y = -200;
        rt_thread_mdelay(500);
        car.Speed_Y = 0;
    }
    else if (tar_y > field_height)
    {
        car.Speed_Y = 200;
        rt_thread_mdelay(500);
        car.Speed_Y = 0;
    }
}

void car_turn(float angle)
{

    if (car.current_angle > 360)
    {
        car.current_angle -= 360;
    }
    else if (car.current_angle < -360)
    {
        car.current_angle += 360;
    }
    else
    {
        car.current_angle += angle;
    }
}

void car_turnto(float angle)
{
    car.current_angle = angle;
}

/**************************************************************************
函数功能：将串口接受的字符坐标转化成数字坐标
入口参数：ART识别到的坐标
返回值：无
**************************************************************************/
void uart_coordinate_transforming(uint8 ART1_POINT_X[40], uint8 ART1_POINT_Y[40], uint8 num)
{
    for (int i = 0; i < num; i++)
    {
        tar_point[i].x = ART1_POINT_X[i];
        tar_point[i].y = ART1_POINT_Y[i];
    }
}

/**************************************************************************
函数功能：静态路径规划
入口参数：ART识别到的坐标
返回值：无
**************************************************************************/
void static_planning(struct point *arr, int size)
{
    // 找到距离原点最近的点
    int min_dist = INT_MAX;
    int nearest = -1;
    for (int i = 0; i < size; i++)
    {
        int dist = arr[i].x * arr[i].x + arr[i].y * arr[i].y;
        if (dist < min_dist && dist != 0)
        {
            min_dist = dist;
            nearest = i;
        }
    }

    // 从最近的点开始遍历并查找最近的邻居
    bool visited[size];
    memset(visited, false, size * sizeof(bool));
    visited[nearest] = true;

    struct point sorted[size];
    sorted[0] = arr[nearest];

    for (int i = 1; i < size; i++)
    {
        int min_dist = INT_MAX;
        int nearest = -1;
        for (int j = 0; j < size; j++)
        {
            if (!visited[j])
            {
                int dist = (arr[j].x - sorted[i - 1].x) * (arr[j].x - sorted[i - 1].x) +
                           (arr[j].y - sorted[i - 1].y) * (arr[j].y - sorted[i - 1].y);
                if (dist < min_dist)
                {
                    min_dist = dist;
                    nearest = j;
                }
            }
        }
        visited[nearest] = true;
        sorted[i] = arr[nearest];
    }
    // 将排序后的点复制回原始数组
    for (int i = 0; i < size; i++)
    {
        arr[i] = sorted[i];
    }
}

int findNearestCoordinate(struct Pose_car car, struct point tar_point[], int size, int8 visited[], int8 *nearestIndex)
{
    double minDist = INT_MAX;

    for (int i = 0; i < size; i++)
    {
        if (visited[i])
        {
            continue; // 跳过已访问过的坐标
        }

        double dist = sqrt(pow(car.MileageX - tar_point[i].x * 20, 2) + pow(car.MileageY - tar_point[i].y * 20, 2));
        if (dist < minDist)
        {
            minDist = dist;
            *nearestIndex = i;
        }
    }

    if (*nearestIndex != -1)
    {
        visited[*nearestIndex] = 1; // 将最近点标记为已访问
    }

    return *nearestIndex;
}

void route_planning_entry(void *param)
{
    rt_sem_take(uart_corrdinate_sem, RT_WAITING_FOREVER);
    uart_coordinate_transforming(ART1_POINT_X, ART1_POINT_Y, coordinate_num);
    static_planning(tar_point, coordinate_num);

    static uint8 count = 0;

    for (int i = 0; i < coordinate_num; i++)
    {
        rt_kprintf("x:%d", tar_point[i].x);
        rt_kprintf("y:%d\n", tar_point[i].y);
    }
    rt_mb_send(buzzer_mailbox, 100); // 给buzzer_mailbox发送100

    car_move(0, 40); // 出库

    while (1)
    {

        rt_sem_take(arrive_sem, RT_WAITING_FOREVER); // 接受到达信号量
        rt_kprintf("arriving!!!\n");
        rt_mb_send(buzzer_mailbox, 100); // 给buzzer_mailbox发送100

        findNearestCoordinate(car, tar_point, coordinate_num, visited, &nearestIndex);

        if (nearestIndex != -1)
        {
            rt_kprintf("The nearest point(%d,%d)\n", tar_point[nearestIndex].x * 20, tar_point[nearestIndex].y * 20);
            visited[nearestIndex] = 1; // 将本次访问过的点标记为已访问
        }

        if (count == coordinate_num)
        {
            //决赛分类
            // car_move(0, 320);
            // arm_openbox(2);

            //初赛分类

//            car_moveto_boundry(field_width + 1, 1);
//            car_boundry_carry(field_width + 1, 1);
//            arm_openbox(1);//右三


//            car.Speed_X = -200;
//            rt_thread_mdelay(2000);
//            car.Speed_X = 0;

//            car_moveto_boundry(1, field_height + 1);
//            car_boundry_carry(1, field_height + 1);
//            arm_openbox(2);//下三（需要改动）



//            car.Speed_Y = -200;
//            rt_thread_mdelay(1000);
//            car.Speed_Y = 0;

//            car_moveto_boundry(-1, 1);
//            car_boundry_carry(-1, 1);
//            arm_openbox(3);//左三


//            car.Speed_X = 200;
//            rt_thread_mdelay(1000);
//            car.Speed_X = 0;

//            car_moveto_boundry(1, -1);
//            car_boundry_carry(1, -1);//测试回库


            rt_sem_release(obj_detection_sem);
						rt_thread_mdelay(2000);
						rt_thread_delete(route_planning_th);
						
        }
        else
        {
            car.target_x = tar_point[nearestIndex].x * 20; // 获取目标坐标点
            car.target_y = tar_point[nearestIndex].y * 20;
            rt_kprintf("TARGET_X%d   ", (int)car.target_x);
            rt_kprintf("TARGET_Y%d\n", (int)car.target_y);
            count++;
        }

        car_move(car.target_x, car.target_y);

        ART1_mode = 2;               // art矫正模式
        uart_putchar(USART_4, 0x42); // 持续发送“B”来告诉openart该矫正了
        rt_thread_mdelay(500);

        rt_sem_release(correct_sem); // 到达后发送矫正信号
    }
}

void correct_entry(void *param)
{
    while (1)
    {
        rt_sem_take(correct_sem, RT_WAITING_FOREVER); // 获取矫正信号

        rt_kprintf("correcting!!!\n");

        while (distance(ART1_CORRECT_X, ART1_CORRECT_Y, 0, 0) > 5 || (ART1_CORRECT_X == 0 && ART1_CORRECT_Y == 0))
        {

            rt_thread_mdelay(10);
            pic_dis = (int)distance(ART1_CORRECT_X, ART1_CORRECT_Y, 0, 0);

            if (pic_dis > 65)
            {
                car.correct_speed = 1;
            }
            else if (pic_dis <= 65 && pic_dis > 45)
            {
                car.correct_speed = 1;
            }
            else if (pic_dis <= 45 && pic_dis > 25)
            {
                car.correct_speed = 2;
            }
            else if (pic_dis <= 25 && pic_dis > 5)
            {
                car.correct_speed = 3;
            }
            else
            {
                car.correct_speed = 0;
            }

            if (car.correct_speed * ART1_CORRECT_X > 100)
            {
                car.Speed_X = 100;
            }
            else if (car.correct_speed * ART1_CORRECT_X < -100)
            {
                car.Speed_X = -100;
            }
            else
            {
                car.Speed_X = car.correct_speed * ART1_CORRECT_X;
            }

            if (car.correct_speed * ART1_CORRECT_Y > 100)
            {
                car.Speed_Y = -100;
            }
            else if (car.correct_speed * ART1_CORRECT_Y < -100)
            {
                car.Speed_Y = 100;
            }
            else
            {
                car.Speed_Y = -car.correct_speed * ART1_CORRECT_Y;
            }
        }

        car.Speed_X = 0;
        car.Speed_Y = 0;

        if (running_mode == 0)//目标检测模式就不更新，普通模式更新
        {
            car.MileageX = car.target_x; // 更新当前坐标
            car.MileageY = car.target_y;
        }

        rt_mb_send(buzzer_mailbox, 100); // 给buzzer_mailbox发送100

        ART1_mode = 3;
        uart_putchar(USART_4, 0x43);
        rt_thread_mdelay(1000);

        rt_sem_release(recognize_sem);
    }
}

void carry_entry(void *param)
{

    while (1)
    {
        rt_sem_take(recognize_sem, RT_WAITING_FOREVER); // 接受识别信号量

        if (strcmp(classified, apple.Small_category) == 0)
        {
            rt_kprintf("This is a apple.\n");
            if (strcmp(taget_Big_category, apple.Big_category) == 0)
            {
                apple.Box_location = 2; // 放入盒子2中
                arm_putbox(apple.Box_location);
            }
            else if (apple.Box_location)
            {
                arm_putbox(apple.Box_location);
            }
            else
            {
                // 搬运并放到指定位置
                arm_carry();
                car_moveto_boundry(apple.Target_x, apple.Target_y);
                car_boundry_carry(apple.Target_x, apple.Target_y);
                arm_down();
            }
        }
        else if (strcmp(classified, bannana.Small_category) == 0)
        {
            rt_kprintf("This is a bannana.\n");
            if (strcmp(taget_Big_category, bannana.Big_category) == 0)
            {
                bannana.Box_location = 2;
                arm_putbox(bannana.Box_location);
            }
            else if (bannana.Box_location)
            {
                arm_putbox(bannana.Box_location);
            }
            else
            {
                // 搬运并放到指定位置
                arm_carry();
                car_moveto_boundry(bannana.Target_x, bannana.Target_y);
                car_boundry_carry(bannana.Target_x, bannana.Target_y);
                arm_down();
            }
        }
        else if (strcmp(classified, grape.Small_category) == 0)
        {
            // 车载
            rt_kprintf("This is a grape.\n");
            if (strcmp(taget_Big_category, grape.Big_category) == 0)
            {
                grape.Box_location = 2; // 放入盒子2中
                arm_putbox(grape.Box_location);
            }else
            {
                arm_putbox(grape.Box_location); // 放入盒子1中

            }
            
        }
        else if (strcmp(classified, durian.Small_category) == 0)
        {
            rt_kprintf("This is a durian.\n");
            if (strcmp(taget_Big_category, durian.Big_category) == 0)
            {
                durian.Box_location = 2;
                arm_putbox(durian.Box_location);
            }
            else if (durian.Box_location)
            {
                arm_putbox(durian.Box_location);
            }
            else
            {
                // 搬运并放到指定位置
                arm_carry();
                car_moveto_boundry(durian.Target_x, durian.Target_y);
                car_boundry_carry(durian.Target_x, durian.Target_y);
                arm_down();
            }
        }
        else if (strcmp(classified, orange.Small_category) == 0)
        {
            rt_kprintf("This is a orange.\n");
            if (strcmp(taget_Big_category, orange.Big_category) == 0)
            {
                orange.Box_location = 2;
                arm_putbox(orange.Box_location);
                // 放入盒子中
            }
            else if (orange.Box_location)
            {
                arm_putbox(orange.Box_location);
            }
            else
            {
                // 搬运并放到指定位置
                arm_carry();
                car_moveto_boundry(orange.Target_x, orange.Target_y);
                car_boundry_carry(orange.Target_x, orange.Target_y);
                arm_down();
            }
        }
        else if (strcmp(classified, cabbage.Small_category) == 0)
        {
            rt_kprintf("This is a cabbage.\n");
            if (strcmp(taget_Big_category, cabbage.Big_category) == 0)
            {
                cabbage.Box_location = 2;
                arm_putbox(cabbage.Box_location);
                // 放入盒子中
            }
            else if (cabbage.Box_location)
            {
                arm_putbox(cabbage.Box_location);
            }
            else
            {
                // 搬运并放到指定位置
                arm_carry();
                car_moveto_boundry(cabbage.Target_x, cabbage.Target_y);
                car_boundry_carry(cabbage.Target_x, cabbage.Target_y);
                arm_down();
            }
        }
        else if (strcmp(classified, cucumber.Small_category) == 0)
        {
            rt_kprintf("This is an cucumber.\n");
            if (strcmp(taget_Big_category, cucumber.Big_category) == 0)
            {
                cucumber.Box_location = 2;
                arm_putbox(cucumber.Box_location);
                // 放入盒子中
            }
            else if (cucumber.Box_location)
            {
                arm_putbox(cucumber.Box_location);
            }
            else
            {
                // 搬运并放到指定位置
                arm_carry();
                car_moveto_boundry(cucumber.Target_x, cucumber.Target_y);
                car_boundry_carry(cucumber.Target_x, cucumber.Target_y);
                arm_down();
            }
        }
        else if (strcmp(classified, eggplant.Small_category) == 0)
        {
            // 车载
            rt_kprintf("This is a eggplant.\n");
            if (strcmp(taget_Big_category, eggplant.Big_category) == 0)
            {
                eggplant.Box_location = 2;
                arm_putbox(eggplant.Box_location);
                // 放入盒子中
            }else
            {
                arm_putbox(eggplant.Box_location); // 放入盒子1中
            }
            
        }
        else if (strcmp(classified, radish.Small_category) == 0)
        {
            rt_kprintf("This is a radish.\n");
            if (strcmp(taget_Big_category, radish.Big_category) == 0)
            {
                radish.Box_location = 2;
                arm_putbox(radish.Box_location);
                // 放入盒子中
            }
            else if (radish.Box_location)
            {
                arm_putbox(radish.Box_location);
            }
            else
            {
                // 搬运并放到指定位置
                arm_carry();
                car_moveto_boundry(radish.Target_x, radish.Target_y);
                car_boundry_carry(radish.Target_x, radish.Target_y);
                arm_down();
            }
        }
        else if (strcmp(classified, pepper.Small_category) == 0)
        {
            rt_kprintf("This is a pepper.\n");
            if (strcmp(taget_Big_category, pepper.Big_category) == 0)
            {
                pepper.Box_location = 2;
                arm_putbox(pepper.Box_location);
                // 放入盒子中
            }
            else if (pepper.Box_location)
            {
                arm_putbox(pepper.Box_location);
            }
            else
            {
                // 搬运并放到指定位置
                arm_carry();
                car_moveto_boundry(pepper.Target_x, pepper.Target_y);
                car_boundry_carry(pepper.Target_x, pepper.Target_y);
                arm_down();
            }
        }
        else if (strcmp(classified, corn.Small_category) == 0)
        {
            rt_kprintf("This is an corn.\n");
            if (strcmp(taget_Big_category, corn.Big_category) == 0)
            {
                corn.Box_location = 2;
                arm_putbox(corn.Box_location);
                // 放入盒子中
            }else
            {
                arm_putbox(corn.Box_location); // 放入盒子1中
            }         
            
            
        }
        else if (strcmp(classified, bean.Small_category) == 0)
        {
            rt_kprintf("This is a bean.\n");
            if (strcmp(taget_Big_category, bean.Big_category) == 0)
            {
                bean.Box_location = 2;
                arm_putbox(bean.Box_location);
                // 放入盒子中
            }
            else if (bean.Box_location)
            {
                arm_putbox(bean.Box_location);
            }
            else
            {
                // 搬运并放到指定位置
                arm_carry();
                car_moveto_boundry(bean.Target_x, bean.Target_y);
                car_boundry_carry(bean.Target_x, bean.Target_y);
                arm_down();
            }
        }
        else if (strcmp(classified, peanut.Small_category) == 0)
        {
            rt_kprintf("This is an peanut.\n");
            if (strcmp(taget_Big_category, peanut.Big_category) == 0)
            {
                peanut.Box_location = 2;
                arm_putbox(peanut.Box_location);
                // 放入盒子中
            }
            else if (peanut.Box_location)
            {
                arm_putbox(peanut.Box_location);
            }
            else
            {
                // 搬运并放到指定位置
                arm_carry();
                car_moveto_boundry(peanut.Target_x, peanut.Target_y);
                car_boundry_carry(peanut.Target_x, peanut.Target_y);
                arm_down();
            }
        }
        else if (strcmp(classified, potato.Small_category) == 0)
        {
            rt_kprintf("This is a potato.\n");
            if (strcmp(taget_Big_category, potato.Big_category) == 0)
            {
                potato.Box_location = 2;
                arm_putbox(potato.Box_location);
                // 放入盒子中
            }
            else if (potato.Box_location)
            {
                arm_putbox(potato.Box_location);
            }
            else
            {
                // 搬运并放到指定位置
                arm_carry();
                car_moveto_boundry(potato.Target_x, potato.Target_y);
                car_boundry_carry(potato.Target_x, potato.Target_y);
                arm_down();
            }
        }
        else if (strcmp(classified, rice.Small_category) == 0)
        {
            rt_kprintf("This is a rice.\n");
            if (strcmp(taget_Big_category, rice.Big_category) == 0)
            {
                rice.Box_location = 2;
                arm_putbox(rice.Box_location);
                // 放入盒子中
            }
            else if (rice.Box_location)
            {
                arm_putbox(rice.Box_location);
            }
            else
            {
                // 搬运并放到指定位置
                arm_carry();
                car_moveto_boundry(rice.Target_x, rice.Target_y);
                car_boundry_carry(rice.Target_x, rice.Target_y);
                arm_down();
            }
        }

        else
        {
            rt_kprintf("Unknown classification.\n");
            // 直接去下一个点位
        }

        if (running_mode == 0)
        {
            rt_sem_release(arrive_sem);
        }
        else if (running_mode == 1)
        {
            rt_sem_release(obj_detection_sem);
        }
    }
}

void obj_detection_entry(void *param)
{
    while (1)
    {
        rt_sem_take(obj_detection_sem, RT_WAITING_FOREVER); // 接受识别信号量
			
				ARM_LOW_angle(100);
				ARM_UP_angle(180);
				rt_thread_mdelay(300);

        ART2_center_x = 0;
        ART2_center_y = 0;
        running_mode = 1;
        detect_flag = 0;
        detect_arrive_flag = 0;

        rt_kprintf("DETECT !!!\n");
        rt_kprintf("%d\n", detect_flag);
        rt_kprintf("card_x:%d,card_y:%d\n", unknow_card_loction_x, unknow_card_loction_y);

        car_move(unknow_card_loction_x, unknow_card_loction_y); // 首先到达上一次位置
			
				ART2_mode = 1;
        uart_putchar(USART_1, 0x46);
				rt_kprintf("WAITING3 !!!\n");
				rt_thread_mdelay(1000);
			

        while (detect_flag == 0) // 是否接受数据
        {

            if (detect_flag == 1)
            {
								rt_kprintf("OUT1 !!!\n");
                detect_flag = 0;
                break;
            }

            if (boundry_num % 2 == 0)
            {
                car_moveto_boundry(field_width+1, 0);//向右移动找边线

                if (detect_flag == 1)
                {
										rt_kprintf("OUT2 !!!\n");
                    detect_flag = 0;
                    break;
                }
                else if (detect_flag == 0)
                {
                    car.Speed_Y = 100;
                    rt_thread_mdelay(1000);
                    car.Speed_Y = 0;

                    car.Speed_X = -100;
                    rt_thread_mdelay(1000);
                    car.Speed_X = 0;
                    boundry_num++;
                }
            }
            else if (boundry_num % 2 == 1)
            {
                car_moveto_boundry(-1, 0);//向左移动寻找边线

                if (detect_flag == 1)
                {
										rt_kprintf("OUT3 !!!\n");
                    detect_flag = 0;
                    break;
                }
                else if (detect_flag == 0)
                {
                    car.Speed_Y = 100;
                    rt_thread_mdelay(1000);
                    car.Speed_Y = 0;

                    car.Speed_X = 100;
                    rt_thread_mdelay(1000);
                    car.Speed_X = 0;
                    boundry_num++;
                }
            }
            rt_kprintf("boundry_num:%d\n", boundry_num);
            rt_thread_mdelay(50);
        }

				rt_thread_mdelay(2000);
        unknow_card_loction_x = (int)car.MileageX;
        unknow_card_loction_y = (int)car.MileageY; // 记录当前坐标
				rt_kprintf("now card_x:%d,card_y:%d\n", unknow_card_loction_x, unknow_card_loction_y);

        while (detect_arrive_flag == 0) // 如果找到卡片但是没有到达，则就运动到距离小于60
        {

            car.Speed_X = ART2_center_x/2;
            car.Speed_Y = ART2_center_y/2;
            rt_kprintf("x:%d, y:%d,flag:%d\n", ART2_center_x, ART2_center_y, detect_arrive_flag);
            rt_thread_mdelay(100);
        }

					
      detect_flag = 0;
			ART1_mode = 2;               // art矫正模式
			uart_putchar(USART_4, 0x42); // 持续发送“B”来告诉openart该矫正了
			rt_thread_mdelay(1000);
				

        // 记录当前坐标
       rt_sem_release(correct_sem);

				//rt_sem_release(obj_detection_sem);
    }
}

void car_start_init(void)
{

    arrive_sem = rt_sem_create("arrive_sem", 1, RT_IPC_FLAG_FIFO);                   // 到达信号量，接受就开始跑点
    uart_corrdinate_sem = rt_sem_create("uart_corrdinate_sem", 0, RT_IPC_FLAG_FIFO); // 接收坐标信号量
    correct_sem = rt_sem_create("correct_sem", 0, RT_IPC_FLAG_FIFO);                 // 矫正信号量，接受就开始矫正
    recognize_sem = rt_sem_create("recognize_sem", 0, RT_IPC_FLAG_FIFO);             // 识别信号量，告诉单片机已经识别，接受就开始搬运
    obj_detection_sem = rt_sem_create("obj_detection_sem", 0, RT_IPC_FLAG_FIFO);     // 目标检测信号量


    route_planning_th = rt_thread_create("route_planning_th", route_planning_entry, RT_NULL, 1024, 28, 10);
    correct_th = rt_thread_create("correct_th", correct_entry, RT_NULL, 1024, 28, 10);
    carry_th = rt_thread_create("carry_th", carry_entry, RT_NULL, 1024, 28, 10);
    obj_detection_th = rt_thread_create("obj_detection_th", obj_detection_entry, RT_NULL, 1024, 28, 10);


//    rt_thread_startup(route_planning_th);
    rt_thread_startup(correct_th);
    rt_thread_startup(carry_th);
    rt_thread_startup(obj_detection_th);
}
