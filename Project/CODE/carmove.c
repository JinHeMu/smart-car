#include "carmove.h"

// 模式0  停止等待
// 模式1  导航 纠正 识别
// 模式2  搬运图片模式
// 模式3  回车库
uint8 running_mode = 0; // 小车运行模式

/**************************************************************************
1.读取坐标，保存在相应的数组中
2.路径规划，每次都以最近的点位，重新排列一个新的点位
3.如何控制小车运动到应该到的点位，同时规划出下一个点位
**************************************************************************/

struct point tar_point[15]; // 排序好顺序的目标坐标

Pose_car car; // 定义car，作为位姿的载体

rt_sem_t correct_sem;   // 矫正
rt_sem_t arrive_sem;    // 到达
rt_sem_t carry_sem;     // 搬运
rt_sem_t recognize_sem; // 识别

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
函数功能：路径规划
入口参数：ART识别到的坐标
返回值：无
**************************************************************************/
void nearest_neighbor_sort(struct point *arr, int size)
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
函数功能：求两点间的距离
入口参数：当前位置 目标位置
返回值：两点距离
**************************************************************************/
float distance(float current_x, float current_y, float target_x, float target_y)
{
    static float temp;
    float det_x = (target_x * 20 - current_x);
    float det_y = (target_y * 20 - current_y);
    temp = (float)sqrt(det_x * det_x + det_y * det_y);
    //	rt_kprintf("%d\n", (int)temp);
    return temp;
}

// 获得当前位置，以及距离下一个点位的距离
void get_location(void)
{
    //    charge_locate();
    //    get_target();
    // 用两点式计算角度和距离
    car.target_angle = atan2((car.target_x - car.current_x), (car.target_y - car.current_y)) * 180 / PI;
    // 计算两点x，y的距离差值
    car.target_distanceX = 20 * (car.target_x - car.current_x);
    car.target_distanceY = 20 * (car.target_y - car.current_y);
    // Car.Distance=sqrt(Car.DistanceX*Car.DistanceX+Car.DistanceY*Car.DistanceY);
}

void traverse_points()
{

    while (1)
    {

        static int point = 0;

        rt_sem_take(arrive_sem, RT_WAITING_FOREVER);//接受到达信号量
        rt_mb_send(buzzer_mailbox, 1000); // 给buzzer_mailbox发送100

        car.target_x = tar_point[point].x;//获取目标坐标点
        car.target_y = tar_point[point].y;

        rt_kprintf("TARGET_X%d   ", (int)car.target_x);
        rt_kprintf("TARGET_Y%d\n", (int)car.target_y);

        // rt_mb_send(buzzer_mailbox, 500);//给buzzer_mailbox发送100

        while (distance(car.MileageX, car.MileageY, car.target_x, car.target_y) > 10)//持续运动
        {
            car.Speed_X = picture_x_pid((int)car.MileageX, (int)car.target_x * 20);
            car.Speed_Y = picture_y_pid((int)car.MileageY, (int)car.target_y * 20);
            //            rt_kprintf("Speed_X:%d, Speed_Y:%d\n", (int)car.Speed_X, (int)car.Speed_Y);
        }

        rt_thread_mdelay(2000);

        point++;//统计到达点数
        if (point == point_num / 2)
        {
            car.Speed_X = 0;
            car.Speed_Y = 0;
        }

        rt_sem_release(correct_sem);//到达后发送矫正信号
    }
}



void correct_entry(void *param)
{
    while (1)
    {
        rt_sem_take(correct_sem, RT_WAITING_FOREVER);//获取矫正信号
        ART1_mode = 2;//art矫正模式

        rt_thread_mdelay(5);

        while (distance(ART1_CORRECT_X, ART1_CORRECT_Y, 0, 0) > 10 || (ART1_CORRECT_X == 0 && ART1_CORRECT_Y == 0))
        {
            // uart_putchar(USART_4, 0x42);//持续发送“B”来告诉openart该矫正了
            rt_thread_mdelay(5);
            int max_speed = 2;
            car.Speed_X = -picture_x_pid((int)ART1_CORRECT_X, 0);
            car.Speed_Y = picture_y_pid((int)ART1_CORRECT_Y, 0);
            if (car.Speed_X > max_speed)
            {
                car.Speed_X = max_speed;
            }
            else if (car.Speed_X < -max_speed)
            {
                car.Speed_X = -max_speed;
            }

            if (car.Speed_Y > max_speed)
            {
                car.Speed_Y = max_speed;
            }
            else if (car.Speed_Y < -max_speed)
            {
                car.Speed_Y = -max_speed;
            }
            //             rt_kprintf("Speed_X:%d, Speed_Y:%d\n", (int)car.Speed_X, (int)car.Speed_Y);
        }
        rt_mb_send(buzzer_mailbox, 1000); // 给buzzer_mailbox发送100

        car.current_x = car.target_x;//更新当前坐标
        car.current_y = car.target_y;

        ART1_mode = 3;//告诉openart该识别图片了
        
        
    }
}

void recognize_entry(void *param)
{
    while (1)
    {
        rt_sem_take(recognize_sem, RT_WAITING_FOREVER);//接受识别信号量


        if (strcmp(classified, "bean") == 0) {
            rt_kprintf("This is a bean.\n");
        } else if (strcmp(classified, "corn") == 0) {
            rt_kprintf("This is a corn.\n");
        } else if (strcmp(classified, "peanut") == 0) {
            rt_kprintf("This is a peanut.\n");
        } else if (strcmp(classified, "potato") == 0) {
            rt_kprintf("This is a potato.\n");
        } else if (strcmp(classified, "rice") == 0) {
            rt_kprintf("This is a rice.\n");
        } else if (strcmp(classified, "apple") == 0) {
            rt_kprintf("This is an apple.\n");
        } else if (strcmp(classified, "banana") == 0) {
            rt_kprintf("This is a banana.\n");
        } else if (strcmp(classified, "grape") == 0) {
            rt_kprintf("This is a grape.\n");
        } else if (strcmp(classified, "durian") == 0) {
            rt_kprintf("This is a durian.\n");
        } else if (strcmp(classified, "orange") == 0) {
            rt_kprintf("This is an orange.\n");
        } else if (strcmp(classified, "cucumber") == 0) {
            rt_kprintf("This is a cucumber.\n");
        } else if (strcmp(classified, "eggplant") == 0) {
            rt_kprintf("This is an eggplant.\n");
        } else if (strcmp(classified, "pepper") == 0) {
            rt_kprintf("This is a pepper.\n");
        } else if (strcmp(classified, "cabbage") == 0) {
            rt_kprintf("This is a cabbage.\n");
        } else if (strcmp(classified, "radish") == 0) {
            rt_kprintf("This is a radish.\n");
        } else {
            rt_kprintf("Unknown classification.\n");
        }
    }
}

void carry_entry(void *param)
{
    while (1)
    {
        rt_sem_take(carry_sem, RT_WAITING_FOREVER);
    }
}


void route_planning_entry(void *param)
{
    rt_sem_take(point_sem, RT_WAITING_FOREVER);
    uart_coordinate_transforming(ART1_POINT_X, ART1_POINT_Y, point_num / 2);
    nearest_neighbor_sort(tar_point, point_num / 2);
    for (int i = 0; i < point_num / 2; i++)
    {
        rt_kprintf("x:%d", tar_point[i].x);
        rt_kprintf("y:%d\n", tar_point[i].y);
    }
    rt_mb_send(buzzer_mailbox, 1000); // 给buzzer_mailbox发送100
    traverse_points();
}

void route_planning_init()
{
    rt_thread_t route_planning_th;
    rt_thread_t correct_th;
    rt_thread_t recognize_th;
    rt_thread_t carry_th;

    arrive_sem = rt_sem_create("arrive_sem", 1, RT_IPC_FLAG_FIFO);
    point_sem = rt_sem_create("point_sem", 0, RT_IPC_FLAG_FIFO);
    correct_sem = rt_sem_create("correct_sem", 0, RT_IPC_FLAG_FIFO);
    recognize_sem = rt_sem_create("recognize_sem", 0, RT_IPC_FLAG_FIFO);
    carry_sem = rt_sem_create("carry_sem", 0, RT_IPC_FLAG_FIFO);

    route_planning_th = rt_thread_create("route_planning_th", route_planning_entry, RT_NULL, 1024, 28, 10);
    correct_th = rt_thread_create("correct_th", correct_entry, RT_NULL, 1024, 28, 10);
    recognize_th = rt_thread_create("recognize_th", recognize_entry, RT_NULL, 1024, 28, 10);
    carry_th = rt_thread_create("carry_th", carry_entry, RT_NULL, 1024, 28, 10);

    // uart_putchar(USART_4, 0x41);
    rt_thread_startup(route_planning_th);
    rt_thread_startup(correct_th);
	rt_thread_startup(recognize_th);
}
