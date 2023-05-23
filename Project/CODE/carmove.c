#include "carmove.h"

/**************************************************************************

 *      蚕豆（5， 27）        橙子（15， 27）        白菜（25， 27）

番薯（-2，25）                                                  花生（37， 25）


榴莲（-2， 17）                                                 苹果（37， 17）


黄瓜（-2， 9）                                                  辣椒（37， 9）

        水稻（5， -2）        香蕉（15， -2）        萝卜（25， -2）

车载：玉米，葡萄，茄子
**************************************************************************/

/**************************************************************************
1.读取坐标，保存在相应的数组中
2.路径规划，每次都以最近的点位，重新排列一个新的点位
3.如何控制小车运动到应该到的点位，同时规划出下一个点位
**************************************************************************/

// 模式0  停止等待
// 模式1  导航 纠正 识别
// 模式2  搬运图片模式
// 模式3  回车库
uint8 running_mode = 0; // 小车功能模式

struct point tar_point[15]; // 排序好顺序的目标坐标

Pose_car car; // 定义car，作为位姿的载体

rt_sem_t correct_sem;   // 矫正
rt_sem_t arrive_sem;    // 到达
rt_sem_t carry_sem;     // 搬运
rt_sem_t recognize_sem; // 识别

rt_thread_t route_planning_th; // 路径规划线程
// rt_thread_t arrive_point_th;//到达点位线程
rt_thread_t correct_th;          // 矫正线程
rt_thread_t carry_th;            // 搬运线程
rt_thread_t dynamic_planning_th; // 动态规划线程


int pic_dis = 0 ;

/**************************************************************************
函数功能：求两点间的距离
入口参数：当前位置 目标位置
返回值：两点距离
**************************************************************************/

float distance(float current_x, float current_y, float target_x, float target_y) {

    float det_x = target_x * 20 - current_x;
    float det_y = target_y * 20 - current_y;
    float current_distance = sqrt(det_x * det_x + det_y * det_y);

    return current_distance;
}





// 获得目标角度
float get_angle(void)
{
    static float pic_dis;
    pic_dis = car.target_angle = atan2((car.target_x - car.current_x), (car.target_y - car.current_y)) * 180 / PI;
    return pic_dis;
}

/**************************************************************************
函数功能：整合函数  输入坐标 小车到指定坐标 单位 坐标单位 角度
入口参数：目标坐标
返回值：无
**************************************************************************/
void car_move(float tar_x, float tar_y, float tar_angle, uint8 flag)
{
    // 姿态控制
    if (flag == 0)
    {
        car.target_angle = tar_angle;
        rt_thread_mdelay(1000);

        while (distance(car.MileageX, car.MileageY, tar_x, tar_y) > 10) // 持续运动
        {
            car.target_angle = tar_angle;
            car.Speed_X = picture_x_pid((int)car.MileageX, (int)car.target_x * 20);
            car.Speed_Y = picture_y_pid((int)car.MileageY, (int)car.target_y * 20);
            //            rt_kprintf("Speed_X:%d, Speed_Y:%d\n", (int)car.Speed_X, (int)car.Speed_Y);
        }
    }
    else
    {
        // 全向移动
        while (distance(car.MileageX, car.MileageY, tar_x, tar_y) > 10) // 持续运动
        {
            car.target_angle = 0;
            car.Speed_X = picture_x_pid((int)car.MileageX, (int)car.target_x * 20);
            car.Speed_Y = picture_y_pid((int)car.MileageY, (int)car.target_y * 20);
            //            rt_kprintf("Speed_X:%d, Speed_Y:%d\n", (int)car.Speed_X, (int)car.Speed_Y);
        }
    }
}

/**************************************************************************
函数功能：将串口接受的字符坐标转化成数字坐标
入口参数：ART识别到的坐标
返回值：无
**************************************************************************/
void uart_coordinate_transforming(uint8 ART1_POINT_X[15], uint8 ART1_POINT_Y[15], uint8 num)
{
    for (int i = 0; i < num; i++)
    {
        tar_point[i].x = ART1_POINT_X[i];
        //				rt_kprintf("x:%d", tar_point[i].x);
        tar_point[i].y = ART1_POINT_Y[i];
        //				rt_kprintf("y:%d\n", tar_point[i].y);
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

/**************************************************************************
函数功能：动态路径规划
入口参数：当前点
返回值：目标点
**************************************************************************/
void dynamic_planning(float current_x, float current_y)
{
}

// void arrive_point_entry(void *param)
// {
//     while (1)
//     {
//         rt_sem_take(arrive_sem, RT_WAITING_FOREVER);//接受到达信号量
//         rt_mb_send(buzzer_mailbox, 1000); // 给buzzer_mailbox发送1000
//         car_move(car.target_x, car.target_y, car.target_angle, 1);
//     }

// }

void traverse_points()
{

    while (1)
    {

        static int point = 0;

        rt_sem_take(arrive_sem, RT_WAITING_FOREVER); // 接受到达信号量
        rt_kprintf("arriveing!!!\n");
        rt_mb_send(buzzer_mailbox, 500); // 给buzzer_mailbox发送100

        if (point == point_num / 2)
        {
            car_move(5, 5, 0, 1);
            rt_thread_delete(route_planning_th);
            
        }
        else
        {
            car.target_x = tar_point[point].x; // 获取目标坐标点
            car.target_y = tar_point[point].y;
            rt_kprintf("TARGET_X%d   ", (int)car.target_x);
            rt_kprintf("TARGET_Y%d\n", (int)car.target_y);
            point++; // 统计到达点数
        }

        car_move(car.target_x, car.target_y, 0, 1);

        rt_thread_mdelay(2000);
//        rt_sem_release(arrive_sem);
        rt_sem_release(correct_sem); // 到达后发送矫正信号
    }
}

void route_planning_entry(void *param)
{
    rt_sem_take(uart_point_sem, RT_WAITING_FOREVER);
    uart_coordinate_transforming(ART1_POINT_X, ART1_POINT_Y, point_num / 2);
    static_planning(tar_point, point_num / 2);
    for (int i = 0; i < point_num / 2; i++)
    {
        rt_kprintf("x:%d", tar_point[i].x);
        rt_kprintf("y:%d\n", tar_point[i].y);
    }
    rt_mb_send(buzzer_mailbox, 1000); // 给buzzer_mailbox发送100
    traverse_points();
}

void correct_entry(void *param)
{
    while (1)
    {
        rt_sem_take(correct_sem, RT_WAITING_FOREVER); // 获取矫正信号
        rt_kprintf("correcting!!!\n");
        ART1_mode = 2;               // art矫正模式
        uart_putchar(USART_4, 0x42); // 持续发送“B”来告诉openart该矫正了
        rt_thread_mdelay(20);
				


        

        while (distance(ART1_CORRECT_X, ART1_CORRECT_Y, 0, 0) > 10 ||  (ART1_CORRECT_X == 0 &&ART1_CORRECT_Y == 0)  ||  abs(ART1_CORRECT_Angle) > 5)
        {

            rt_thread_mdelay(30);
            pic_dis = (int)distance(ART1_CORRECT_X, ART1_CORRECT_Y, 0, 0);
//						rt_kprintf("%d\n",pic_dis);
//						car.target_angle = ART1_CORRECT_Angle;
//            car.Speed_X = -correct_x_pid((int)ART1_CORRECT_X, 0);
//            car.Speed_Y = correct_y_pid((int)ART1_CORRECT_Y, 0);

            if(pic_dis>65)
            {
                car.correct_speed = 6;
            }
            else if(pic_dis<=65 && pic_dis>45)
            {
                car.correct_speed = 5.5;
            }
            else if(pic_dis<=45 && pic_dis>25)
            {
                car.correct_speed = 5;
            }
            else if (pic_dis<=25 && pic_dis>10)
            {
                car.correct_speed=6;
            }
						else
						{
							car.correct_speed = 0;
						}
            car.Speed_X = car.correct_speed * ART1_CORRECT_X / 100;
            car.Speed_Y = -car.correct_speed * ART1_CORRECT_Y / 100;
//            car.target_angle = ART1_CORRECT_Angle;
//            rt_kprintf("Speed_X:%d, Speed_Y:%d\n", (int)car.Speed_X, (int)car.Speed_Y);
        }
        rt_mb_send(buzzer_mailbox, 500); // 给buzzer_mailbox发送100

        car.current_x = car.target_x; // 更新当前坐标
        car.current_y = car.target_y;

        // rt_kprintf("current_x: %d, current_y : %d\n", car.current_x, car.current_y);

        car.MileageX = car.target_x; // 更新当前坐标
        car.MileageY = car.target_y;

        
        // arm_carry();
        // arm_down();
        // ART1_mode = 3;//告诉openart该识别图片了
        // rt_sem_release(recognize_sem);
    
        rt_thread_mdelay(1000);
//				rt_sem_release(correct_sem);
       rt_sem_release(arrive_sem);
    }
}

void carry_entry(void *param)
{
    while (1)
    {
        // rt_sem_take(carry_sem, RT_WAITING_FOREVER);
        rt_sem_take(recognize_sem, RT_WAITING_FOREVER); // 接受识别信号量
        uart_putchar(USART_4, 0x43);
        rt_thread_mdelay(5);

        if (strcmp(classified, "bean") == 0)
        {
            rt_kprintf("This is a bean.\n");
            arm_carry();
            // 到达指定位置，寻找下一个点，到达下一个点
            car.target_x = 5;
            car.target_y = 27;
            rt_sem_release(arrive_sem);
            rt_sem_release(carry_sem);
        }
        else if (strcmp(classified, "corn") == 0)
        {
            rt_kprintf("This is a corn.\n");
            arm_box();
        }
        else if (strcmp(classified, "peanut") == 0)
        {
            rt_kprintf("This is a peanut.\n");
            arm_carry();
        }
        else if (strcmp(classified, "potato") == 0)
        {
            rt_kprintf("This is a potato.\n");
            arm_carry();
        }
        else if (strcmp(classified, "rice") == 0)
        {
            rt_kprintf("This is a rice.\n");
            arm_carry();
        }
        else if (strcmp(classified, "apple") == 0)
        {
            rt_kprintf("This is an apple.\n");
            arm_carry();
        }
        else if (strcmp(classified, "banana") == 0)
        {
            rt_kprintf("This is a banana.\n");
            arm_carry();
        }
        else if (strcmp(classified, "grape") == 0)
        {
            rt_kprintf("This is a grape.\n");
            arm_carry();
        }
        else if (strcmp(classified, "durian") == 0)
        {
            rt_kprintf("This is a durian.\n");
            arm_box();
        }
        else if (strcmp(classified, "orange") == 0)
        {
            rt_kprintf("This is an orange.\n");
            arm_carry();
        }
        else if (strcmp(classified, "cucumber") == 0)
        {
            rt_kprintf("This is a cucumber.\n");
            arm_carry();
        }
        else if (strcmp(classified, "eggplant") == 0)
        {
            rt_kprintf("This is an eggplant.\n");
            arm_box();
        }
        else if (strcmp(classified, "pepper") == 0)
        {
            rt_kprintf("This is a pepper.\n");
            arm_carry();
        }
        else if (strcmp(classified, "cabbage") == 0)
        {
            rt_kprintf("This is a cabbage.\n");
            arm_carry();
        }
        else if (strcmp(classified, "radish") == 0)
        {
            rt_kprintf("This is a radish.\n");
            arm_carry();
        }
        else
        {
            rt_kprintf("Unknown classification.\n");
            // 直接去下一个点位
        }
    }
}

void dynamic_planning_entry(void *param)
{
}

void route_planning_init()
{


    arrive_sem = rt_sem_create("arrive_sem", 1, RT_IPC_FLAG_FIFO);         // 到达信号量，接受就开始跑点
    uart_point_sem = rt_sem_create("uart_point_sem", 0, RT_IPC_FLAG_FIFO); // 接收坐标信号量
    correct_sem = rt_sem_create("correct_sem", 0, RT_IPC_FLAG_FIFO);       // 矫正信号量，接受就开始矫正
    recognize_sem = rt_sem_create("recognize_sem", 0, RT_IPC_FLAG_FIFO);   // 识别信号量，告诉单片机已经识别，接受就开始搬运
    carry_sem = rt_sem_create("carry_sem", 0, RT_IPC_FLAG_FIFO);           // 搬运信号量，接受即已经搬运到相应点位

    dynamic_planning_th = rt_thread_create("dynamic_planning_th", dynamic_planning_entry, RT_NULL, 1024, 28, 10);
    // arrive_point_th = rt_thread_create("arrive_point_th", arrive_point_entry, RT_NULL, 1024, 28, 10);
    route_planning_th = rt_thread_create("route_planning_th", route_planning_entry, RT_NULL, 1024, 28, 10);
    correct_th = rt_thread_create("correct_th", correct_entry, RT_NULL, 1024, 28, 10);
    carry_th = rt_thread_create("carry_th", carry_entry, RT_NULL, 1024, 28, 10);

    uart_putchar(USART_4, 0x41); // 发送A告诉该识别A4纸了
    rt_thread_startup(route_planning_th);
    rt_thread_startup(correct_th);
    rt_thread_startup(carry_th);
}
