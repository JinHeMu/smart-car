#include "carmove.h"

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

point tar_point[15]; // �����˳���Ŀ������

Pose_car car; // ����car����Ϊλ�˵�����

rt_sem_t correct_sem;       // ����
rt_sem_t arrive_sem;        // ����
rt_sem_t carry_sem;         // ����
rt_sem_t recognize_sem;     // ʶ��
rt_sem_t obj_detection_sem; // Ŀ����
// rt_sem_t move_sem;

rt_thread_t route_planning_th; // ·���滮�߳�
rt_thread_t correct_th;        // �����߳�
rt_thread_t carry_th;          // �����߳�
rt_thread_t obj_detection_th;  //Ŀ�����߳�

char taget_Big_category[10];
// rt_thread_t move_th;

int pic_dis = 0;

struct card apple = {"fruit", "apple", 0, 18, 8};
struct card bannana = {"fruit", "bannana", 0, 8, 0};
struct card durian = {"fruit", "durian", 0, 0, 8};
struct card grape = {"fruit", "grape", 1, 0, 0}; // ����
struct card orange = {"fruit", "orange", 0, 8, 18};

struct card cabbage = {"vegetable", "cabbage", 0, 12, 8};
struct card cucumber = {"vegetable", "cucumber", 0, 0, 4};
struct card eggplant = {"vegetable", "eggplant", 1, 0, 0}; // ����
struct card radish = {"vegetable", "radish", 0, 12, 0};
struct card pepper = {"vegetable", "pepper", 0, 18, 4};

struct card corn = {"food", "corn", 1, 0, 0}; // ����
struct card bean = {"food", "bean", 0, 4, 18};
struct card peanut = {"food", "peanut", 0, 18, 12};
struct card potato = {"food", "potato", 0, 0, 12};
struct card rice = {"food", "rice", 0, 4, 0};

/**************************************************************************
�������ܣ��������ľ���
��ڲ�������ǰλ�� Ŀ��λ��
����ֵ���������
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

// ���Ŀ��Ƕ�
int get_angle(float current_x, float current_y, float target_x, float target_y)
{
    int angle;
    angle = atan2((target_x - current_x), (target_y - current_y)) * 180 / PI;
    return (int)angle;
}

/**************************************************************************
�������ܣ����Ϻ���  �������� С����ָ������
��ڲ�����Ŀ������
����ֵ����
**************************************************************************/
void car_move(float tar_x, float tar_y)
{
    rt_kprintf("MOVEING !!! \n");
    while (distance(car.MileageX, car.MileageY, tar_x, tar_y) > 10) // �����˶�
    {
        car.Speed_X = picture_x_pid((int)car.MileageX, (int)tar_x); // cm
        car.Speed_Y = picture_y_pid((int)car.MileageY, (int)tar_y);
        rt_thread_mdelay(20);
    }
    car.Speed_X = 0;
    car.Speed_Y = 0;
    rt_mb_send(buzzer_mailbox, 1000); // ��buzzer_mailbox����100
    rt_thread_delay(1000);            // ?? 1 ??
    rt_kprintf("GO TO X:%d, Y:%d\n", (int)car.MileageX, (int)car.MileageY);
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
�������ܣ������ڽ��ܵ��ַ�����ת������������
��ڲ�����ARTʶ�𵽵�����
����ֵ����
**************************************************************************/
void uart_coordinate_transforming(uint8 ART1_POINT_X[15], uint8 ART1_POINT_Y[15], uint8 num)
{
    for (int i = 0; i < num; i++)
    {
        tar_point[i].x = ART1_POINT_X[i];
        tar_point[i].y = ART1_POINT_Y[i];
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

void route_planning_entry(void *param)
{
    rt_sem_take(uart_corrdinate_sem, RT_WAITING_FOREVER);
    uart_coordinate_transforming(ART1_POINT_X, ART1_POINT_Y, point_num / 2);
    static_planning(tar_point, point_num / 2);
    int point = 0;

    for (int i = 0; i < point_num / 2; i++)
    {
        rt_kprintf("x:%d", tar_point[i].x);
        rt_kprintf("y:%d\n", tar_point[i].y);
    }
    rt_mb_send(buzzer_mailbox, 1000); // ��buzzer_mailbox����100

    car_move(0, 60); // ����

    while (1)
    {

        rt_sem_take(arrive_sem, RT_WAITING_FOREVER); // ���ܵ����ź���
        rt_kprintf("arriveing!!!\n");
        rt_mb_send(buzzer_mailbox, 500); // ��buzzer_mailbox����100

        if (point == point_num / 2)
        {
            rt_sem_release(obj_detection_sem);
            rt_thread_delete(route_planning_th);
        }
        else
        {
            car.target_x = tar_point[point].x * 20; // ��ȡĿ�������
            car.target_y = tar_point[point].y * 20;
            rt_kprintf("TARGET_X%d   ", (int)car.target_x);
            rt_kprintf("TARGET_Y%d\n", (int)car.target_y);
            point++; // ͳ�Ƶ������
        }

        car_move(car.target_x, car.target_y);

        ART1_mode = 2;               // art����ģʽ
        uart_putchar(USART_4, 0x42); // �������͡�B��������openart�ý�����
        rt_thread_mdelay(1000);

//        if(ART1_CORRECT_Angle < 5)
//        {
//            car_turn(ART1_CORRECT_Angle);rt_thread_mdelay(100);
//            angle_z = 0;//�����趨������ֵ
//        }
        
        rt_sem_release(correct_sem); // ������ͽ����ź�
//				 rt_sem_release(arrive_sem); // ������ͽ����ź�
    }
}

void correct_entry(void *param)
{
    while (1)
    {
        rt_sem_take(correct_sem, RT_WAITING_FOREVER); // ��ȡ�����ź�

        rt_kprintf("correcting!!!\n");

        while (distance(ART1_CORRECT_X, ART1_CORRECT_Y, 0, 0) > 5 || (ART1_CORRECT_X == 0 && ART1_CORRECT_Y == 0))
        {

            rt_thread_mdelay(5);
            pic_dis = (int)distance(ART1_CORRECT_X, ART1_CORRECT_Y, 0, 0);

            if (pic_dis > 65)
            {
                car.correct_speed = 4;
            }
            else if (pic_dis <= 65 && pic_dis > 45)
            {
                car.correct_speed = 3;
            }
            else if (pic_dis <= 45 && pic_dis > 25)
            {
                car.correct_speed = 2;
            }
            else if (pic_dis <= 25 && pic_dis > 5)
            {
                car.correct_speed = 1;
            }
            else
            {
                car.correct_speed = 0;
            }
						
						if(car.correct_speed * ART1_CORRECT_X > 100)
						{
							car.Speed_X = 100;
						}else if(car.correct_speed * ART1_CORRECT_X < -100)
						{
							car.Speed_X = -100;
						}else
						{
							car.Speed_X = car.correct_speed * ART1_CORRECT_X ;
						}
						
						if(car.correct_speed * ART1_CORRECT_Y > 100)
						{
							car.Speed_Y = -100;
						}else if(car.correct_speed * ART1_CORRECT_Y < -100)
						{
							car.Speed_Y = 100;
						}else
						{
							car.Speed_Y = -car.correct_speed * ART1_CORRECT_Y;
						}
							

            
        
						
						rt_kprintf("%d,%d\n", (int)car.Speed_X, (int)car.Speed_Y);

        }
        car.Speed_X = 0;
        car.Speed_Y = 0;


        car.MileageX = car.target_x; // ���µ�ǰ����
        car.MileageY = car.target_y;
        rt_mb_send(buzzer_mailbox, 1000); // ��buzzer_mailbox����100
				

        ART1_mode = 3;
        uart_putchar(USART_4, 0x43);
        rt_thread_mdelay(1000);
        rt_sem_release(recognize_sem);
//				rt_sem_release(correct_sem);
    }
}

void carry_entry(void *param)
{

    while (1)
    {
        rt_sem_take(recognize_sem, RT_WAITING_FOREVER); // ����ʶ���ź���
        rt_thread_mdelay(100);

        if (strcmp(classified, apple.Small_category) == 0)
        {
            rt_kprintf("This is a apple.\n");
            if (strcmp(taget_Big_category, apple.Big_category) == 0)
            {
                apple.Box_location = 2; // �������2��
                arm_putbox(grape.Box_location);
            }
            else
            {
                // ���˲��ŵ�ָ��λ��
                arm_carry();
                car_move(apple.Target_x * 20, apple.Target_y * 20);
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
            else
            {
                // ���˲��ŵ�ָ��λ��
                arm_carry();
                car_move(bannana.Target_x * 20, bannana.Target_y * 20);
                arm_down();
            }
        }
        else if (strcmp(classified, grape.Small_category) == 0)
        {
            // ����
            rt_kprintf("This is a grape.\n");
            arm_putbox(grape.Box_location); // �������1��
        }
        else if (strcmp(classified, durian.Small_category) == 0)
        {
            rt_kprintf("This is a durian.\n");
            if (strcmp(taget_Big_category, durian.Big_category) == 0)
            {
                durian.Box_location = 2;
                arm_putbox(durian.Box_location);
            }
            else
            {
                // ���˲��ŵ�ָ��λ��
                arm_carry();
                car_move(durian.Target_x * 20, durian.Target_y * 20);
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
                // ���������
            }
            else
            {
                // ���˲��ŵ�ָ��λ��
                arm_carry();
                car_move(orange.Target_x * 20, orange.Target_y * 20);
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
                // ���������
            }
            else
            {
                // ���˲��ŵ�ָ��λ��
                arm_carry();
                car_move(cabbage.Target_x * 20, cabbage.Target_y * 20);
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
                // ���������
            }
            else
            {
                // ���˲��ŵ�ָ��λ��
                arm_carry();
                car_move(cucumber.Target_x * 20, cucumber.Target_y * 20);
                arm_down();
            }
        }
        else if (strcmp(classified, eggplant.Small_category) == 0)
        {
            // ����
            rt_kprintf("This is a eggplant.\n");
            arm_putbox(eggplant.Box_location); // �������1��
        }
        else if (strcmp(classified, radish.Small_category) == 0)
        {
            rt_kprintf("This is a radish.\n");
            if (strcmp(taget_Big_category, radish.Big_category) == 0)
            {
                radish.Box_location = 2;
                arm_putbox(radish.Box_location);
                // ���������
            }
            else
            {
                // ���˲��ŵ�ָ��λ��
                arm_carry();
                car_move(radish.Target_x * 20, radish.Target_y * 20);
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
                // ���������
            }
            else
            {
                // ���˲��ŵ�ָ��λ��
                arm_carry();
                car_move(pepper.Target_x * 20, pepper.Target_y * 20);
                arm_down();
            }
        }
        else if (strcmp(classified, corn.Small_category) == 0)
        {
            rt_kprintf("This is an corn.\n");
            arm_putbox(corn.Box_location); // �������1��
        }
        else if (strcmp(classified, bean.Small_category) == 0)
        {
            rt_kprintf("This is a bean.\n");
            if (strcmp(taget_Big_category, bean.Big_category) == 0)
            {
                bean.Box_location = 2;
                arm_putbox(bean.Box_location);
                // ���������
            }
            else
            {
                // ���˲��ŵ�ָ��λ��
                arm_carry();
                car_move(bean.Target_x * 20, bean.Target_y * 20);
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
                // ���������
            }
            else
            {
                // ���˲��ŵ�ָ��λ��
                arm_carry();
                car_move(peanut.Target_x * 20, peanut.Target_y * 20);
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
                // ���������
            }
            else
            {
                // ���˲��ŵ�ָ��λ��
                arm_carry();
                car_move(potato.Target_x * 20, potato.Target_y * 20);
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
                // ���������
            }
            else
            {
                // ���˲��ŵ�ָ��λ��
                arm_carry();
                car_move(rice.Target_x * 20, rice.Target_y * 20);
                arm_down();
            }
        }

        else
        {
            rt_kprintf("Unknown classification.\n");
            // ֱ��ȥ��һ����λ
        }
        rt_sem_release(arrive_sem);
    }
}

void obj_detection_entry(void *param)
{
    rt_sem_take(obj_detection_sem, RT_WAITING_FOREVER);
    rt_kprintf("DETECT !!!\n");

    while (1)
    {

        rt_kprintf("dis:%d ", ART2_dis);
        rt_kprintf("ang:%d\n", ART2_angle);
        if (ART2_angle > 40)
        {
            car_turn(-1);
        }
        if (ART2_angle < -40)
        {
            car_turn(1);
        }
        if (ART2_dis > 60)
        {
            car.Speed_Y = 0.5;
        }
        else
        {
            car.Speed_Y = 0;
        }
        rt_thread_mdelay(200);
    }
}

void car_start_init(void)
{

    arrive_sem = rt_sem_create("arrive_sem", 1, RT_IPC_FLAG_FIFO);                   // �����ź��������ܾͿ�ʼ�ܵ�
    uart_corrdinate_sem = rt_sem_create("uart_corrdinate_sem", 0, RT_IPC_FLAG_FIFO); // ���������ź���
    correct_sem = rt_sem_create("correct_sem", 0, RT_IPC_FLAG_FIFO);                 // �����ź��������ܾͿ�ʼ����
    recognize_sem = rt_sem_create("recognize_sem", 0, RT_IPC_FLAG_FIFO);             // ʶ���ź��������ߵ�Ƭ���Ѿ�ʶ�𣬽��ܾͿ�ʼ����
    carry_sem = rt_sem_create("carry_sem", 0, RT_IPC_FLAG_FIFO);                     // �����ź��������ܼ��Ѿ����˵���Ӧ��λ
                                        
    route_planning_th = rt_thread_create("route_planning_th", route_planning_entry, RT_NULL, 1024, 28, 10);

    correct_th = rt_thread_create("correct_th", correct_entry, RT_NULL, 1024, 28, 10);

    carry_th = rt_thread_create("carry_th", carry_entry, RT_NULL, 1024, 28, 10);

    obj_detection_th = rt_thread_create("obj_detection_th", obj_detection_entry, RT_NULL, 1024, 28, 10);


   
    rt_thread_startup(route_planning_th);
    rt_thread_startup(correct_th);
    rt_thread_startup(carry_th);
    // rt_thread_startup(obj_detection_th); 
    uart_putchar(USART_4, 0x41); // ����OPENART1���߸�ʶ��A4ֽ��
}
