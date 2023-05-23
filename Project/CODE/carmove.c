#include "carmove.h"

/**************************************************************************

 *      �϶���5�� 27��        ���ӣ�15�� 27��        �ײˣ�25�� 27��

����-2��25��                                                  ������37�� 25��


������-2�� 17��                                                 ƻ����37�� 17��


�ƹϣ�-2�� 9��                                                  ������37�� 9��

        ˮ����5�� -2��        �㽶��15�� -2��        �ܲ���25�� -2��

���أ����ף����ѣ�����
**************************************************************************/

/**************************************************************************
1.��ȡ���꣬��������Ӧ��������
2.·���滮��ÿ�ζ�������ĵ�λ����������һ���µĵ�λ
3.��ο���С���˶���Ӧ�õ��ĵ�λ��ͬʱ�滮����һ����λ
**************************************************************************/

// ģʽ0  ֹͣ�ȴ�
// ģʽ1  ���� ���� ʶ��
// ģʽ2  ����ͼƬģʽ
// ģʽ3  �س���
uint8 running_mode = 0; // С������ģʽ

struct point tar_point[15]; // �����˳���Ŀ������

Pose_car car; // ����car����Ϊλ�˵�����

rt_sem_t correct_sem;   // ����
rt_sem_t arrive_sem;    // ����
rt_sem_t carry_sem;     // ����
rt_sem_t recognize_sem; // ʶ��

rt_thread_t route_planning_th; // ·���滮�߳�
// rt_thread_t arrive_point_th;//�����λ�߳�
rt_thread_t correct_th;          // �����߳�
rt_thread_t carry_th;            // �����߳�
rt_thread_t dynamic_planning_th; // ��̬�滮�߳�


int pic_dis = 0 ;

/**************************************************************************
�������ܣ��������ľ���
��ڲ�������ǰλ�� Ŀ��λ��
����ֵ���������
**************************************************************************/

float distance(float current_x, float current_y, float target_x, float target_y) {

    float det_x = target_x * 20 - current_x;
    float det_y = target_y * 20 - current_y;
    float current_distance = sqrt(det_x * det_x + det_y * det_y);

    return current_distance;
}





// ���Ŀ��Ƕ�
float get_angle(void)
{
    static float pic_dis;
    pic_dis = car.target_angle = atan2((car.target_x - car.current_x), (car.target_y - car.current_y)) * 180 / PI;
    return pic_dis;
}

/**************************************************************************
�������ܣ����Ϻ���  �������� С����ָ������ ��λ ���굥λ �Ƕ�
��ڲ�����Ŀ������
����ֵ����
**************************************************************************/
void car_move(float tar_x, float tar_y, float tar_angle, uint8 flag)
{
    // ��̬����
    if (flag == 0)
    {
        car.target_angle = tar_angle;
        rt_thread_mdelay(1000);

        while (distance(car.MileageX, car.MileageY, tar_x, tar_y) > 10) // �����˶�
        {
            car.target_angle = tar_angle;
            car.Speed_X = picture_x_pid((int)car.MileageX, (int)car.target_x * 20);
            car.Speed_Y = picture_y_pid((int)car.MileageY, (int)car.target_y * 20);
            //            rt_kprintf("Speed_X:%d, Speed_Y:%d\n", (int)car.Speed_X, (int)car.Speed_Y);
        }
    }
    else
    {
        // ȫ���ƶ�
        while (distance(car.MileageX, car.MileageY, tar_x, tar_y) > 10) // �����˶�
        {
            car.target_angle = 0;
            car.Speed_X = picture_x_pid((int)car.MileageX, (int)car.target_x * 20);
            car.Speed_Y = picture_y_pid((int)car.MileageY, (int)car.target_y * 20);
            //            rt_kprintf("Speed_X:%d, Speed_Y:%d\n", (int)car.Speed_X, (int)car.Speed_Y);
        }
    }
}

/**************************************************************************
�������ܣ������ڽ��ܵ��ַ�����ת������������
��ڲ�����ARTʶ�𵽵�����
����ֵ����
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
�������ܣ���̬·���滮
��ڲ�����ARTʶ�𵽵�����
����ֵ����
**************************************************************************/
void static_planning(struct point *arr, int size)
{
    // �ҵ�����ԭ������ĵ�
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

    // ������ĵ㿪ʼ����������������ھ�
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
    // �������ĵ㸴�ƻ�ԭʼ����
    for (int i = 0; i < size; i++)
    {
        arr[i] = sorted[i];
    }
}

/**************************************************************************
�������ܣ���̬·���滮
��ڲ�������ǰ��
����ֵ��Ŀ���
**************************************************************************/
void dynamic_planning(float current_x, float current_y)
{
}

// void arrive_point_entry(void *param)
// {
//     while (1)
//     {
//         rt_sem_take(arrive_sem, RT_WAITING_FOREVER);//���ܵ����ź���
//         rt_mb_send(buzzer_mailbox, 1000); // ��buzzer_mailbox����1000
//         car_move(car.target_x, car.target_y, car.target_angle, 1);
//     }

// }

void traverse_points()
{

    while (1)
    {

        static int point = 0;

        rt_sem_take(arrive_sem, RT_WAITING_FOREVER); // ���ܵ����ź���
        rt_kprintf("arriveing!!!\n");
        rt_mb_send(buzzer_mailbox, 500); // ��buzzer_mailbox����100

        if (point == point_num / 2)
        {
            car_move(5, 5, 0, 1);
            rt_thread_delete(route_planning_th);
            
        }
        else
        {
            car.target_x = tar_point[point].x; // ��ȡĿ�������
            car.target_y = tar_point[point].y;
            rt_kprintf("TARGET_X%d   ", (int)car.target_x);
            rt_kprintf("TARGET_Y%d\n", (int)car.target_y);
            point++; // ͳ�Ƶ������
        }

        car_move(car.target_x, car.target_y, 0, 1);

        rt_thread_mdelay(2000);
//        rt_sem_release(arrive_sem);
        rt_sem_release(correct_sem); // ������ͽ����ź�
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
    rt_mb_send(buzzer_mailbox, 1000); // ��buzzer_mailbox����100
    traverse_points();
}

void correct_entry(void *param)
{
    while (1)
    {
        rt_sem_take(correct_sem, RT_WAITING_FOREVER); // ��ȡ�����ź�
        rt_kprintf("correcting!!!\n");
        ART1_mode = 2;               // art����ģʽ
        uart_putchar(USART_4, 0x42); // �������͡�B��������openart�ý�����
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
        rt_mb_send(buzzer_mailbox, 500); // ��buzzer_mailbox����100

        car.current_x = car.target_x; // ���µ�ǰ����
        car.current_y = car.target_y;

        // rt_kprintf("current_x: %d, current_y : %d\n", car.current_x, car.current_y);

        car.MileageX = car.target_x; // ���µ�ǰ����
        car.MileageY = car.target_y;

        
        // arm_carry();
        // arm_down();
        // ART1_mode = 3;//����openart��ʶ��ͼƬ��
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
        rt_sem_take(recognize_sem, RT_WAITING_FOREVER); // ����ʶ���ź���
        uart_putchar(USART_4, 0x43);
        rt_thread_mdelay(5);

        if (strcmp(classified, "bean") == 0)
        {
            rt_kprintf("This is a bean.\n");
            arm_carry();
            // ����ָ��λ�ã�Ѱ����һ���㣬������һ����
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
            // ֱ��ȥ��һ����λ
        }
    }
}

void dynamic_planning_entry(void *param)
{
}

void route_planning_init()
{


    arrive_sem = rt_sem_create("arrive_sem", 1, RT_IPC_FLAG_FIFO);         // �����ź��������ܾͿ�ʼ�ܵ�
    uart_point_sem = rt_sem_create("uart_point_sem", 0, RT_IPC_FLAG_FIFO); // ���������ź���
    correct_sem = rt_sem_create("correct_sem", 0, RT_IPC_FLAG_FIFO);       // �����ź��������ܾͿ�ʼ����
    recognize_sem = rt_sem_create("recognize_sem", 0, RT_IPC_FLAG_FIFO);   // ʶ���ź��������ߵ�Ƭ���Ѿ�ʶ�𣬽��ܾͿ�ʼ����
    carry_sem = rt_sem_create("carry_sem", 0, RT_IPC_FLAG_FIFO);           // �����ź��������ܼ��Ѿ����˵���Ӧ��λ

    dynamic_planning_th = rt_thread_create("dynamic_planning_th", dynamic_planning_entry, RT_NULL, 1024, 28, 10);
    // arrive_point_th = rt_thread_create("arrive_point_th", arrive_point_entry, RT_NULL, 1024, 28, 10);
    route_planning_th = rt_thread_create("route_planning_th", route_planning_entry, RT_NULL, 1024, 28, 10);
    correct_th = rt_thread_create("correct_th", correct_entry, RT_NULL, 1024, 28, 10);
    carry_th = rt_thread_create("carry_th", carry_entry, RT_NULL, 1024, 28, 10);

    uart_putchar(USART_4, 0x41); // ����A���߸�ʶ��A4ֽ��
    rt_thread_startup(route_planning_th);
    rt_thread_startup(correct_th);
    rt_thread_startup(carry_th);
}
