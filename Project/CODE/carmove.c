//#include "carmove.h"

///**************************************************************************
//1.��ȡ���꣬��������Ӧ��������
//2.·���滮��ÿ�ζ�������ĵ�λ����������һ���µĵ�λ
//3.��ο���С���˶���Ӧ�õ��ĵ�λ��ͬʱ�滮����һ����λ
//**************************************************************************/

//// ģʽ0  ֹͣ�ȴ�
//// ģʽ1  ���� ���� ʶ��
//// ģʽ2  ����ͼƬģʽ
//// ģʽ3  �س���
//// #define field_width 35
//// #define field_height 25

//#define field_width 35
//#define field_height 25

//uint8 running_mode = 0; // С������ģʽ��0ΪѰ�������ģʽ   1ΪĿ����ģʽ
//uint8 game_mode = 0;    // 0Ϊ����  1Ϊ����
//uint8 find_mode = 0;    // 0Ϊ��Ŀ����  1Ϊ��Ŀ����

//point tar_point[40];    // �����˳���Ŀ������
//int8 visited[40] = {0}; // ��ʼ���������飬�������궼δ����
//int8 nearestIndex = -1;

//Pose_car car; // ����car����Ϊλ�˵�����

//rt_sem_t correct_sem;       // ����
//rt_sem_t arrive_sem;        // ����
//rt_sem_t recognize_sem;     // ʶ��
//rt_sem_t obj_detection_sem; // Ŀ����

//rt_thread_t route_planning_th; // ·���滮�߳�
//rt_thread_t correct_th;        // �����߳�
//rt_thread_t carry_th;          // �����߳�
//rt_thread_t obj_detection_th;  // Ŀ�����߳�

//uint8 pic_dis = 0;
//uint8 find_card_x = 0;
//uint8 find_card_y = 0;
//uint8 count = 0;
//uint8 boundry_num = 0;

//float unknow_card_loction_x = 80;
//float unknow_card_loction_y = 60;

////**����**

///*
//1-��
//2-��
//3-��
//4-��
//5-����
//*/


////**����**

///*
//1-��
//2-��
//3-��
//4-��
//5-����
//6-����
//*/



//struct card cabbage = {"vegetable", "cabbage", 1, 12, 26};  // ������
//struct card bean = {"food", "bean", 1, 4, 26};              // ������
//struct card orange = {"fruit", "orange", 1, 8, 26};         // ������

//struct card apple = {"fruit", "apple", 2, 36, 8};           // ������
//struct card pepper = {"vegetable", "pepper", 2, 36, 4};     // ������
//struct card peanut = {"food", "peanut", 2, 36, 12};         // ������

//struct card bannana = {"fruit", "bannana", 3, 8, -1};       // ������
//struct card radish = {"vegetable", "radish", 3, 12, -1};    // ������
//struct card rice = {"food", "rice", 3, 4, -1};              // ������

//struct card durian = {"fruit", "durian", 4, -1, 8};         // ������
//struct card cucumber = {"vegetable", "cucumber", 4, -1, 4}; // ������
//struct card potato = {"food", "potato", 4, -1, 12};         // ������

//struct card grape = {"fruit", "grape", 5, 0, 0};            // ����
//struct card corn = {"food", "corn", 5, 0, 0};               // ����
//struct card eggplant = {"vegetable", "eggplant", 5, 0, 0};  // ����


////  struct card apple = {"fruit", "apple", 4, 36, 8};     // ������
//// struct card bannana = {"fruit", "bannana", 2, 8, -1}; // ������
//// struct card durian = {"fruit", "durian", 3, -1, 8};   // ������
//// struct card grape = {"fruit", "grape", 1, 0, 0};      // ����
//// struct card orange = {"fruit", "orange", 0, 8, 26};   // ������

//// struct card cabbage = {"vegetable", "cabbage", 0, 12, 26};  // ������
//// struct card cucumber = {"vegetable", "cucumber", 3, -1, 4}; // ������
//// struct card eggplant = {"vegetable", "eggplant", 1, 0, 0};  // ����
//// struct card radish = {"vegetable", "radish", 2, 12, -1};    // ������
//// struct card pepper = {"vegetable", "pepper", 4, 36, 4};     // ������

//// struct card corn = {"food", "corn", 1, 0, 0};       // ����
//// struct card bean = {"food", "bean", 0, 4, 26};      // ������
//// struct card peanut = {"food", "peanut", 4, 36, 12}; // ������
//// struct card potato = {"food", "potato", 3, -1, 12}; // ������
//// struct card rice = {"food", "rice", 2, 4, -1};      // ������

//void select_mode(void)
//{
//    if (running_mode == 0)
//    {
//        rt_sem_release(arrive_sem);
//    }
//    else if (running_mode == 1)
//    {
//        rt_sem_release(obj_detection_sem);
//    }
//}

///**************************************************************************
//�������ܣ��������ľ���
//��ڲ�������ǰλ�� Ŀ��λ��
//����ֵ���������
//**************************************************************************/

//int distance(float current_x, float current_y, float target_x, float target_y)
//{

//    float det_x;
//    float det_y;
//    float distance;

//    det_x = target_x - current_x;
//    det_y = target_y - current_y;
//    distance = sqrt(det_x * det_x + det_y * det_y);

//    return (int)distance;
//}

//// ���Ŀ��Ƕ�
//float get_angle(float current_x, float current_y, float target_x, float target_y)
//{
//    float angle;
//    angle = atan2((target_x - current_x), (target_y - current_y));
//    return angle;
//}

///**************************************************************************
//�������ܣ����Ϻ���  �������� С����ָ������
//��ڲ�����Ŀ������
//����ֵ����
//**************************************************************************/

//void car_speed_x(int16 tar_speed)
//{
//    float acceleration = 0.02; // ���ٶȣ��ɸ���ʵ���������
//    float max_speed = 1;    // ����ٶȣ��ɸ���ʵ���������
//    float current_speed = 0;
//    float progress = 0; // ���ڱ�ʾ���ȣ���Χ�� 0 �� 1

//    // �������ٶ�
//    while (progress < 1)
//    {
//        // ʹ�û������������λ����������������
//        progress = 1 - pow(1 - current_speed / max_speed, 2);

//        current_speed += acceleration;

//        if (current_speed > max_speed)current_speed = max_speed;

//        car.Speed_X = current_speed * tar_speed;
//        rt_thread_mdelay(20);
//    }
//}


//void car_speed_y(int16 tar_speed)
//{
//    float acceleration = 0.02; // ���ٶȣ��ɸ���ʵ���������
//    float max_speed = 1;    // ����ٶȣ��ɸ���ʵ���������
//    float current_speed = 0;
//    float progress = 0; // ���ڱ�ʾ���ȣ���Χ�� 0 �� 1

//    // �������ٶ�
//    while (progress < 1)
//    {
//        // ʹ�û������������λ����������������
//        progress = 1 - pow(1 - current_speed / max_speed, 2);

//        current_speed += acceleration;
//        
//        if (current_speed > max_speed)current_speed = max_speed;

//        car.Speed_Y = current_speed * tar_speed;
//        rt_thread_mdelay(20);
//    }
//}


//void car_move(float tar_x, float tar_y)
//{

//    float target_distance = distance(car.MileageX, car.MileageY, tar_x, tar_y);
//    float current_distance = target_distance;
//    float acceleration = 0.02; // ���ٶȣ��ɸ���ʵ���������
//    float max_speed = 0.5;    // ����ٶȣ��ɸ���ʵ���������
//    float current_speed = 0;
//    float angle = get_angle(car.MileageX, car.MileageY, tar_x, tar_y);

//    // rt_kprintf("TARGRT TO X:%d, Y:%d\n", (int)tar_x, (int)tar_y);


//    if (running_mode == 0)
//    {
//			
//			 if (target_distance >= 450)
//			{

//        acceleration = 0.01;
//        max_speed = 0.5;
//				
//				
//				tar_x += sin(angle) * target_distance / 5;
//                tar_y += cos(angle) * target_distance / 5 - 40;
//			}else
//			{
//				tar_x += sin(angle) * target_distance / 7;
//                tar_y += cos(angle) * target_distance / 7 - 25;
//				
//			}
//        
//    }
////    else if (running_mode == 1)

////    {
////        tar_y -= 10;
////    }

//		rt_kprintf("dis:%d\n", (int)target_distance);
//    rt_kprintf("MOVEING TO X:%d, Y:%d\n", (int)tar_x, (int)tar_y);

//    while (current_distance > 5) // �����˶�
//    {
//        // �������ٶ�
//        if (current_speed < max_speed)
//            current_speed += acceleration;

//        // ���Ƶ���ٶ�
//        car.Speed_X = current_speed * picture_x_pid((int)car.MileageX, (int)tar_x); // cm
//        car.Speed_Y = current_speed * picture_y_pid((int)car.MileageY, (int)tar_y);

//        rt_thread_mdelay(20);

//        // ���µ�ǰ����
//        current_distance = distance(car.MileageX, car.MileageY, tar_x, tar_y);
//        //rt_kprintf("dis:%d\n", (int)current_distance);
//    }

//    car.Speed_X = 0;
//    car.Speed_Y = 0;

//    rt_mb_send(buzzer_mailbox, 100); // ��buzzer_mailbox����100

//    rt_kprintf("I HAVE ARRIVE X:%d, Y:%d\n", (int)car.MileageX, (int)car.MileageY);
//}

//void back(uint8 boundry_num)
//{
//    if (boundry_num == 5)
//    {
//			
//        detect_flag = 0;
//        running_mode = 0;
//        ART2_mode = 0;
//        car_moveto_boundry(field_width + 1, 1);
//        car_boundry_carry(field_width + 1, 1);
//		car_boundry_carry(field_width + 1, 1);
//        arm_openbox(3); // ����

//        car.Speed_Y = -200;
//        rt_thread_mdelay(1000);
//        car.Speed_Y = 0;

//        car.Speed_X = -200;
//        rt_thread_mdelay(2000);
//        car.Speed_X = 0;

//        if (game_mode == 1)
//        {
//            car_move(70, 500);
//            arm_openbox(2); // ����

//        }else
//        {
//            car_moveto_boundry(1, field_height + 1);
//            car_boundry_carry(1, field_height + 1);
//						car_boundry_carry(1, field_height + 1);
//            arm_openbox(2); // ����
//        }

//						car.Speed_Y = -200;
//						rt_thread_mdelay(1000);
//						car.Speed_Y = 0;


//        car_moveto_boundry(-1, 1);
//        car_boundry_carry(-1, 1);
//        arm_openbox(1); // ����

//				
//				
//				car.Speed_X = 200;
//        rt_thread_mdelay(750);
//        car.Speed_X = 0;
//				
//				

//				

//        car_moveto_boundry(1, -1);
//        rt_thread_mdelay(100000);

//        rt_thread_delete(obj_detection_th);
//    }
//}
//void car_moveto_boundry(int8 tar_x, int8 tar_y)
//{

//    // ������������
//    rt_kprintf("GO TO BOUNDRE!!!\n");

//    ART1_mode = 4;
//    ART1_CORRECT_Boundary_Flag = 0;

//    if (tar_x < 0)
//    {
//        uart_putchar(USART_4, 0x44); // ����OPENART1���߸�ʶ�������
//        rt_thread_mdelay(500);
//        while (ART1_CORRECT_Boundary_Flag == 0)
//        {
//            if (detect_flag)
//            {
//                rt_kprintf("OUT1 !!!\n");
//                break;
//            }

//            if (car.MileageX > 40)
//            {
//							if(running_mode == 1)
//							{
//								car.Speed_X = -200;
//							}else
//							{
//								car.Speed_X = -300;
//							}
//               
//            }
//            else
//            {
//                car.Speed_X = -100;
//            }
//            rt_thread_mdelay(50);
//        }

//        if (detect_flag == 0) // �������Ŀ�����жϣ����Ǳ���
//        {
//            car.MileageX = 0;
//        }
//    }
//    else if (tar_y < 0)
//    {
//        uart_putchar(USART_4, 0x45); // ����OPENART1���߸�ʶ�������
//        rt_thread_mdelay(500);

//        while (ART1_CORRECT_Boundary_Flag == 0)
//        {
//            if (car.MileageY > 40)
//            {
//                car.Speed_Y = -300;
//            }
//            else
//            {
//                car.Speed_Y = -100;
//            }
//            rt_thread_mdelay(50);
//        }

//        if (detect_flag == 0)
//        {
//            car.MileageY = 16;
//        }
//    }
//    else if (tar_x > field_width)
//    {
//        uart_putchar(USART_4, 0x44); // ����OPENART1���߸�ʶ�������
//        rt_thread_mdelay(500);

//        while (ART1_CORRECT_Boundary_Flag == 0)
//        {

//            if (detect_flag)
//            {
//                rt_kprintf("OUT2 !!!\n");
//                break;
//            }
//            if (car.MileageX < (field_width * 20) - 40)
//            {
//							if(running_mode == 1)
//							{
//								
//								car.Speed_X = 200;
//							}else
//							{
//								car.Speed_X = 300;
//							}
//            }
//            else
//            {
//                car.Speed_X = 100;
//            }
//            rt_thread_mdelay(50);
//        }

//        if (detect_flag == 0)
//        {
//            car.MileageX = field_width * 20;
//        }
//    }
//    else if (tar_y > field_height)
//    {
//        uart_putchar(USART_4, 0x45); // ����OPENART1���߸�ʶ�������
//        rt_thread_mdelay(500);

//        while (ART1_CORRECT_Boundary_Flag == 0)
//        {

//            if (car.MileageY < (field_height * 20 - 40))
//            {
//                car.Speed_Y = 300;
//            }
//            else
//            {
//                car.Speed_Y = 100;
//            }
//            rt_thread_mdelay(50);
//        }
//        if (detect_flag == 0)
//        {
//            car.MileageY = field_height * 20 - 20;
//        }
//    }

//    car.Speed_X = 0;
//    car.Speed_Y = 0;
//    rt_mb_send(buzzer_mailbox, 100); // ��buzzer_mailbox����100
//    // rt_kprintf("I HAVE ARRIVED BOUNDRE!!!");
//    rt_kprintf("flag:%d\n", detect_flag);
//    ART1_CORRECT_Boundary_Flag = 0;
//}


//void car_moveto_boundry_fast(int8 tar_x, int8 tar_y)
//{

//    // ������������
//    rt_kprintf("GO TO BOUNDRE!!!\n");

//    ART1_mode = 4;
//    ART1_CORRECT_Boundary_Flag = 0;

//    if (tar_x < 0)
//    {
//        uart_putchar(USART_4, 0x44); // ����OPENART1���߸�ʶ�������
//        rt_thread_mdelay(500);
//        while (ART1_CORRECT_Boundary_Flag == 0)
//        {
//            if (detect_flag)
//            {
//                rt_kprintf("OUT1 !!!\n");
//                break;
//            }

//            if (car.MileageX > 40)
//            {
//							if(running_mode == 1)
//							{
//								car.Speed_X = -200;
//							}else
//							{
//								car.Speed_X = -500;
//							}
//               
//            }
//            else
//            {
//                car.Speed_X = -300;
//            }
//            rt_thread_mdelay(50);
//        }

//        if (detect_flag == 0) // �������Ŀ�����жϣ����Ǳ���
//        {
//            car.MileageX = 0;
//        }
//    }
//    else if (tar_y < 0)
//    {
//        uart_putchar(USART_4, 0x45); // ����OPENART1���߸�ʶ�������
//        rt_thread_mdelay(500);

//        while (ART1_CORRECT_Boundary_Flag == 0)
//        {
//            if (car.MileageY > 40)
//            {
//                car.Speed_Y = -500;
//            }
//            else
//            {
//                car.Speed_Y = -300;
//            }
//            rt_thread_mdelay(50);
//        }

//        if (detect_flag == 0)
//        {
//            car.MileageY = 0;
//        }
//    }
//    else if (tar_x > field_width)
//    {
//        uart_putchar(USART_4, 0x44); // ����OPENART1���߸�ʶ�������
//        rt_thread_mdelay(500);

//        while (ART1_CORRECT_Boundary_Flag == 0)
//        {

//            if (detect_flag)
//            {
//                rt_kprintf("OUT2 !!!\n");
//                break;
//            }
//            if (car.MileageX < (field_width * 20) - 40)
//            {
//                if(running_mode == 1)
//                {
//                    
//                    car.Speed_X = 500;
//                }else
//                {
//                    car.Speed_X = 400;
//                }
//            }
//            else
//            {
//                car.Speed_X = 300;
//            }
//            rt_thread_mdelay(50);
//        }

//        if (detect_flag == 0)
//        {
//            car.MileageX = field_width * 20;
//        }
//    }
//    else if (tar_y > field_height)
//    {
//        uart_putchar(USART_4, 0x45); // ����OPENART1���߸�ʶ�������
//        rt_thread_mdelay(500);

//        while (ART1_CORRECT_Boundary_Flag == 0)
//        {

//            if (car.MileageY < (field_height * 20 - 40))
//            {
//                car.Speed_Y = 500;
//            }
//            else
//            {
//                car.Speed_Y = 300;
//            }
//            rt_thread_mdelay(50);
//        }
//        if (detect_flag == 0)
//        {
//            car.MileageY = field_height * 20 - 20;
//        }
//    }

//    car.Speed_X = 0;
//    car.Speed_Y = 0;
//    rt_mb_send(buzzer_mailbox, 100); // ��buzzer_mailbox����100
//    // rt_kprintf("I HAVE ARRIVED BOUNDRE!!!");
//    rt_kprintf("flag:%d\n", detect_flag);
//    ART1_CORRECT_Boundary_Flag = 0;
//}


//void back_fast(uint8 boundry_num)
//{
//    if (boundry_num == 5)
//    {
//			
//        detect_flag = 0;
//        running_mode = 0;
//        ART2_mode = 0;
//        car_moveto_boundry_fast(field_width + 1, 1);
//        car_boundry_carry(field_width + 1, 1);
//				car_boundry_carry(field_width + 1, 1);
//        arm_openbox(3); // ����

//        car.Speed_Y = -400;
//        rt_thread_mdelay(500);
//        car.Speed_Y = 0;

//        car.Speed_X = -400;
//        rt_thread_mdelay(1000);
//        car.Speed_X = 0;

//        if (game_mode == 1)
//        {
//            car_move(50, 500);
//            arm_openbox(2); // ����
//        }else
//        {
//            car_moveto_boundry_fast(1, field_height + 1);
//            car_boundry_carry(1, field_height + 1);
//						car_boundry_carry(1, field_height + 1);
//            arm_openbox(2); // ����
//        }



//        car.Speed_Y = -400;
//        rt_thread_mdelay(500);
//        car.Speed_Y = 0;

//        car_moveto_boundry_fast(-1, 1);
//        car_boundry_carry(-1, 1);
//        arm_openbox(1); // ����

//        car.Speed_X = 200;
//        rt_thread_mdelay(600);
//        car.Speed_X = 0;

//        car_moveto_boundry_fast(1, -1);
//        rt_thread_mdelay(100000);

//        rt_thread_delete(obj_detection_th);
//    }
//}








//void car_boundry_carry(int8 tar_x, int8 tar_y)
//{
//    if (tar_x < 0)
//    {
//        car.Speed_X = -200;
//        rt_thread_mdelay(400);
//        car.Speed_X = 0;
//    }
//    else if (tar_x > field_width)
//    {
//        car.Speed_X = 200;
//        rt_thread_mdelay(400);
//        car.Speed_X = 0;
//    }
//    else if (tar_y < 0)
//    {
//        car.Speed_Y = -200;
//        rt_thread_mdelay(400);
//        car.Speed_Y = 0;
//    }
//    else if (tar_y > field_height)
//    {
//        car.Speed_Y = 200;
//        rt_thread_mdelay(400);
//        car.Speed_Y = 0;
//    }
//}

//void car_turn(float angle)
//{

//    if (car.current_angle > 360)
//    {
//        car.current_angle -= 360;
//    }
//    else if (car.current_angle < -360)
//    {
//        car.current_angle += 360;
//    }
//    else
//    {
//        car.current_angle += angle;
//    }
//}

//void car_turnto(float angle)
//{
//    car.current_angle = angle;
//}

///**************************************************************************
//�������ܣ������ڽ��ܵ��ַ�����ת������������
//��ڲ�����ARTʶ�𵽵�����
//����ֵ����
//**************************************************************************/
//void uart_coordinate_transforming(uint8 ART1_POINT_X[40], uint8 ART1_POINT_Y[40], uint8 num)
//{
//    for (int i = 0; i < num; i++)
//    {
//        tar_point[i].x = ART1_POINT_X[i];
//        tar_point[i].y = ART1_POINT_Y[i];
//    }
//}

///**************************************************************************
//�������ܣ���̬·���滮
//��ڲ�����ARTʶ�𵽵�����
//����ֵ����
//**************************************************************************/
//void static_planning(struct point *arr, int size)
//{
//    // �ҵ�����ԭ������ĵ�
//    int min_dist = INT_MAX;
//    int nearest = -1;
//    for (int i = 0; i < size; i++)
//    {
//        int dist = arr[i].x * arr[i].x + arr[i].y * arr[i].y;
//        if (dist < min_dist && dist != 0)
//        {
//            min_dist = dist;
//            nearest = i;
//        }
//    }

//    // ������ĵ㿪ʼ����������������ھ�
//    bool visited[size];
//    memset(visited, false, size * sizeof(bool));
//    visited[nearest] = true;

//    struct point sorted[size];
//    sorted[0] = arr[nearest];

//    for (int i = 1; i < size; i++)
//    {
//        int min_dist = INT_MAX;
//        int nearest = -1;
//        for (int j = 0; j < size; j++)
//        {
//            if (!visited[j])
//            {
//                int dist = (arr[j].x - sorted[i - 1].x) * (arr[j].x - sorted[i - 1].x) +
//                           (arr[j].y - sorted[i - 1].y) * (arr[j].y - sorted[i - 1].y);
//                if (dist < min_dist)
//                {
//                    min_dist = dist;
//                    nearest = j;
//                }
//            }
//        }
//        visited[nearest] = true;
//        sorted[i] = arr[nearest];
//    }
//    // �������ĵ㸴�ƻ�ԭʼ����
//    for (int i = 0; i < size; i++)
//    {
//        arr[i] = sorted[i];
//    }
//}

//int findNearestCoordinate(struct Pose_car car, struct point tar_point[], int size, int8 visited[], int8 *nearestIndex)
//{
//    double minDist = INT_MAX;

//    for (int i = 0; i < size; i++)
//    {
//        if (visited[i])
//        {
//            continue; // �����ѷ��ʹ�������
//        }

//        double dist = sqrt(pow(car.MileageX - tar_point[i].x * 20, 2) + pow(car.MileageY - tar_point[i].y * 20, 2));
//        if (dist < minDist)
//        {
//            minDist = dist;
//            *nearestIndex = i;
//        }
//    }

//    if (*nearestIndex != -1)
//    {
//        visited[*nearestIndex] = 1; // ���������Ϊ�ѷ���
//    }

//    return *nearestIndex;
//}

//void route_planning_entry(void *param)
//{
//    rt_sem_take(uart_corrdinate_sem, RT_WAITING_FOREVER);
//    uart_coordinate_transforming(ART1_POINT_X, ART1_POINT_Y, coordinate_num);
//    static_planning(tar_point, coordinate_num);

//    static uint8 count = 0;

//    for (int i = 0; i < coordinate_num; i++)
//    {
//        rt_kprintf("x:%d", tar_point[i].x);
//        rt_kprintf("y:%d\n", tar_point[i].y);
//    }
//    rt_mb_send(buzzer_mailbox, 100); // ��buzzer_mailbox����100

//    car_move(10, 40); // ����

//    while (1)
//    {

//        rt_sem_take(arrive_sem, RT_WAITING_FOREVER); // ���ܵ����ź���
//        rt_kprintf("arriving!!!\n");
//        rt_mb_send(buzzer_mailbox, 100); // ��buzzer_mailbox����100

//        findNearestCoordinate(car, tar_point, coordinate_num, visited, &nearestIndex);

//        if (nearestIndex != -1)
//        {
//            rt_kprintf("The nearest point(%d,%d)\n", tar_point[nearestIndex].x * 20, tar_point[nearestIndex].y * 20);
//            visited[nearestIndex] = 1; // �����η��ʹ��ĵ���Ϊ�ѷ���
//        }

//        if (count == coordinate_num)
//        {
//					
//						if(find_mode)
//						{
//							rt_sem_release(obj_detection_sem);
//						}else
//						{
//							car_move(500,400);
//							back(5);
//						}
//						
//				   
//            break;
//        }
//        else
//        {
//            car.target_x = tar_point[nearestIndex].x * 20; // ��ȡĿ�������
//            car.target_y = tar_point[nearestIndex].y * 20;
//            rt_kprintf("TARGET_X%d   ", (int)car.target_x);
//            rt_kprintf("TARGET_Y%d\n", (int)car.target_y);
//            count++;
//        }

//        car_move(car.target_x, car.target_y);

//        rt_sem_release(correct_sem); // ������ͽ����ź�
//    }
//}

//void correct_entry(void *param)
//{
//    while (1)
//    {
//        rt_sem_take(correct_sem, RT_WAITING_FOREVER); // ��ȡ�����ź�

//        ART2_CORRECT_Flag = 0;

//        ART2_mode = 2;               // art����ģʽ
//        uart_putchar(USART_1, 0x42); // �������͡�B��������openart�ý�����

//        rt_kprintf("correcting!!!\n");

//        while (ART2_CORRECT_Flag == 0)
//        {
//            pic_dis = (int)distance(ART2_CORRECT_X, ART2_CORRECT_Y, 0, 0);

//            if (pic_dis > 65)
//            {
//                car.correct_speed = 1;
//            }
//            else if (pic_dis <= 65 && pic_dis > 45)
//            {
//                car.correct_speed = 0.75;
//            }
//            else if (pic_dis <= 45 && pic_dis > 25)
//            {
//                car.correct_speed = 0.5;
//            }
//            else if (pic_dis <= 25 && pic_dis > 15)
//            {
//                car.correct_speed = 1;
//            }
//						else if (pic_dis < 15)
//            {
//                car.correct_speed = 2;
//            }
//            else
//            {
//                car.correct_speed = 0;
//            }

//            car.Speed_X = car.correct_speed * ART2_CORRECT_X;
//            car.Speed_Y = car.correct_speed * ART2_CORRECT_Y;
//						rt_thread_mdelay(5);
//        }

//        car.Speed_X = 0;
//        car.Speed_Y = 0;

//        if (running_mode == 0) // Ŀ����ģʽ�Ͳ����£���ͨģʽ����
//        {
//            car.MileageX = car.target_x; // ���µ�ǰ����
//            car.MileageY = car.target_y;
//        }

//        rt_mb_send(buzzer_mailbox, 100); // ��buzzer_mailbox����100

//        rt_sem_release(recognize_sem);
//        // rt_sem_release(correct_sem);
//        //				rt_sem_release(arrive_sem);
//    }
//}

//void carry_entry(void *param)
//{

//    while (1)
//    {
//        rt_sem_take(recognize_sem, RT_WAITING_FOREVER); // ����ʶ���ź���

//        ART1_mode = 3;
//        uart_putchar(USART_4, 0x43);
//			
//				while(ART1_CLASS_Flag==0)
//				{
//					rt_thread_mdelay(100);
//					
//				};

//				ART1_CLASS_Flag = 0;
//        if (strcmp(classified, apple.Small_category) == 0)
//        {
//            rt_kprintf("This is a apple.\n");
//            if (strcmp(taget_Big_category, apple.Big_category) == 0)
//            {
//                rt_kprintf("This is  fruit.\n");
//                apple.Box_location = 6; // �������2��
//                arm_putbox(apple.Box_location);
//            }
//            else if (apple.Box_location)
//            {
//                arm_putbox(apple.Box_location);
//            }
//            else
//            {
//                // ���˲��ŵ�ָ��λ��
//                arm_carry();
//                car_moveto_boundry(apple.Target_x, apple.Target_y);
//                car_boundry_carry(apple.Target_x, apple.Target_y);
//                arm_down();
//                select_mode();
//            }
//        }
//        else if (strcmp(classified, bannana.Small_category) == 0)
//        {
//            rt_kprintf("This is a bannana.\n");
//            if (strcmp(taget_Big_category, bannana.Big_category) == 0)
//            {
//                rt_kprintf("This is  fruit.\n");
//                bannana.Box_location = 6;
//                arm_putbox(bannana.Box_location);
//            }
//            else if (bannana.Box_location)
//            {
//                arm_putbox(bannana.Box_location);
//            }
//            else
//            {
//                // ���˲��ŵ�ָ��λ��
//                arm_carry();
//                car_moveto_boundry(bannana.Target_x, bannana.Target_y);
//                car_boundry_carry(bannana.Target_x, bannana.Target_y);
//                arm_down();
//                select_mode();
//            }
//        }
//        else if (strcmp(classified, grape.Small_category) == 0)
//        {
//            // ����
//            rt_kprintf("This is a grape.\n");
//            if (strcmp(taget_Big_category, grape.Big_category) == 0)
//            {
//                rt_kprintf("This is  fruit.\n");
//                grape.Box_location = 6; // �������6��
//                arm_putbox(grape.Box_location);
//            }
//            else
//            {
//                arm_putbox(grape.Box_location); 
//            }
//        }
//        else if (strcmp(classified, durian.Small_category) == 0)
//        {
//            rt_kprintf("This is a durian.\n");
//            if (strcmp(taget_Big_category, durian.Big_category) == 0)
//            {
//                rt_kprintf("This is  fruit.\n");
//                durian.Box_location = 6;
//                arm_putbox(durian.Box_location);
//            }
//            else if (durian.Box_location)
//            {
//                arm_putbox(durian.Box_location);
//            }
//            else
//            {
//                // ���˲��ŵ�ָ��λ��
//                arm_carry();
//                car_moveto_boundry(durian.Target_x, durian.Target_y);
//                car_boundry_carry(durian.Target_x, durian.Target_y);
//                arm_down();
//                select_mode();
//            }
//        }
//        else if (strcmp(classified, orange.Small_category) == 0)
//        {
//            if (game_mode == 1)
//            {
//                orange.Box_location = 0; // ���ģʽΪ�������򵥰�
//                rt_kprintf("final game\n");
//            }
//            else
//            {
//                rt_kprintf("ori game\n");
//            }
//            rt_kprintf("This is a orange.\n");

//            if (strcmp(taget_Big_category, orange.Big_category) == 0)
//            {
//                rt_kprintf("This is  fruit.\n");
//                orange.Box_location = 6;
//                arm_putbox(orange.Box_location);
//                // ���������
//            }
//            else if (orange.Box_location)
//            {
//                arm_putbox(orange.Box_location);
//            }
//            else
//            {
//                // ���˲��ŵ�ָ��λ��
//                arm_carry();
//                car_moveto_boundry(orange.Target_x, orange.Target_y);
//                car_boundry_carry(orange.Target_x, orange.Target_y);
//                arm_down();
//                select_mode();
//            }
//        }
//        else if (strcmp(classified, cabbage.Small_category) == 0)
//        {
//            if (game_mode == 1)
//            {
//                cabbage.Box_location = 0; // ���ģʽΪ�������򵥰�
//                rt_kprintf("final game\n");
//            }
//            else
//            {
//                rt_kprintf("ori game\n");
//            }
//            rt_kprintf("This is a cabbage.\n");
//            if (strcmp(taget_Big_category, cabbage.Big_category) == 0)
//            {
//                rt_kprintf("This is  vegetable.\n");
//                cabbage.Box_location = 6;
//                arm_putbox(cabbage.Box_location);
//                // ���������
//            }
//            else if (cabbage.Box_location)
//            {
//                arm_putbox(cabbage.Box_location);
//            }
//            else
//            {
//                // ���˲��ŵ�ָ��λ��
//                arm_carry();
//                car_moveto_boundry(cabbage.Target_x, cabbage.Target_y);
//                car_boundry_carry(cabbage.Target_x, cabbage.Target_y);
//                arm_down();
//                select_mode();
//            }
//        }
//        else if (strcmp(classified, cucumber.Small_category) == 0)
//        {
//            rt_kprintf("This is an cucumber.\n");
//            if (strcmp(taget_Big_category, cucumber.Big_category) == 0)
//            {
//                rt_kprintf("This is  vegetable.\n");
//                cucumber.Box_location = 6;
//                arm_putbox(cucumber.Box_location);
//                // ���������
//            }
//            else if (cucumber.Box_location)
//            {
//                arm_putbox(cucumber.Box_location);
//            }
//            else
//            {
//                // ���˲��ŵ�ָ��λ��
//                arm_carry();
//                car_moveto_boundry(cucumber.Target_x, cucumber.Target_y);
//                car_boundry_carry(cucumber.Target_x, cucumber.Target_y);
//                arm_down();
//            }
//        }
//        else if (strcmp(classified, eggplant.Small_category) == 0)
//        {
//            // ����
//            rt_kprintf("This is a eggplant.\n");
//            if (strcmp(taget_Big_category, eggplant.Big_category) == 0)
//            {
//                rt_kprintf("This is  vegetable.\n");
//                eggplant.Box_location = 6;
//                arm_putbox(eggplant.Box_location);
//                // ���������
//            }
//            else
//            {
//                arm_putbox(eggplant.Box_location); // �������1��
//            }
//        }
//        else if (strcmp(classified, radish.Small_category) == 0)
//        {
//            rt_kprintf("This is a radish.\n");
//            if (strcmp(taget_Big_category, radish.Big_category) == 0)
//            {
//                rt_kprintf("This is  vegetable.\n");
//                radish.Box_location = 6;
//                arm_putbox(radish.Box_location);
//                // ���������
//            }
//            else if (radish.Box_location)
//            {
//                arm_putbox(radish.Box_location);
//            }
//            else
//            {
//                // ���˲��ŵ�ָ��λ��
//                arm_carry();
//                car_moveto_boundry(radish.Target_x, radish.Target_y);
//                car_boundry_carry(radish.Target_x, radish.Target_y);
//                arm_down();
//                select_mode();
//            }
//        }
//        else if (strcmp(classified, pepper.Small_category) == 0)
//        {
//            rt_kprintf("This is a pepper.\n");
//            if (strcmp(taget_Big_category, pepper.Big_category) == 0)
//            {
//                rt_kprintf("This is  vegetable.\n");
//                pepper.Box_location = 6;
//                arm_putbox(pepper.Box_location);
//                // ���������
//            }
//            else if (pepper.Box_location)
//            {
//                arm_putbox(pepper.Box_location);
//            }
//            else
//            {
//                // ���˲��ŵ�ָ��λ��
//                arm_carry();
//                car_moveto_boundry(pepper.Target_x, pepper.Target_y);
//                car_boundry_carry(pepper.Target_x, pepper.Target_y);
//                arm_down();
//                select_mode();
//            }
//        }
//        else if (strcmp(classified, corn.Small_category) == 0)
//        {
//            rt_kprintf("This is an corn.\n");
//            if (strcmp(taget_Big_category, corn.Big_category) == 0)
//            {
//                rt_kprintf("This is  food.\n");
//                corn.Box_location = 6;
//                arm_putbox(corn.Box_location);
//                // ���������
//            }
//            else
//            {
//                arm_putbox(corn.Box_location); // �������1��
//            }
//        }
//        else if (strcmp(classified, bean.Small_category) == 0)
//        {
//            if (game_mode == 1)
//            {
//                bean.Box_location = 0; // ���ģʽΪ�������򵥰�
//                rt_kprintf("final game\n");
//            }
//            else
//            {
//                rt_kprintf("ori game\n");
//            }
//            rt_kprintf("This is a bean.\n");
//            if (strcmp(taget_Big_category, bean.Big_category) == 0)
//            {
//                rt_kprintf("This is  food.\n");
//                bean.Box_location = 6;
//                arm_putbox(bean.Box_location);
//                // ���������
//            }
//            else if (bean.Box_location)
//            {
//                arm_putbox(bean.Box_location);
//            }
//            else
//            {
//                // ���˲��ŵ�ָ��λ��
//                arm_carry();
//                car_moveto_boundry(bean.Target_x, bean.Target_y);
//                car_boundry_carry(bean.Target_x, bean.Target_y);
//                arm_down();
//                select_mode();
//            }
//        }
//        else if (strcmp(classified, peanut.Small_category) == 0)
//        {
//            rt_kprintf("This is an peanut.\n");
//            if (strcmp(taget_Big_category, peanut.Big_category) == 0)
//            {
//                rt_kprintf("This is  food.\n");
//                peanut.Box_location = 6;
//                arm_putbox(peanut.Box_location);
//                // ���������
//            }
//            else if (peanut.Box_location)
//            {
//                arm_putbox(peanut.Box_location);
//            }
//            else
//            {
//                // ���˲��ŵ�ָ��λ��
//                arm_carry();
//                car_moveto_boundry(peanut.Target_x, peanut.Target_y);
//                car_boundry_carry(peanut.Target_x, peanut.Target_y);
//                arm_down();
//                select_mode();
//            }
//        }
//        else if (strcmp(classified, potato.Small_category) == 0)
//        {
//            rt_kprintf("This is a potato.\n");
//            if (strcmp(taget_Big_category, potato.Big_category) == 0)
//            {
//                rt_kprintf("This is  food.\n");
//                potato.Box_location = 6;
//                arm_putbox(potato.Box_location);
//                // ���������
//            }
//            else if (potato.Box_location)
//            {
//                arm_putbox(potato.Box_location);
//            }
//            else
//            {
//                // ���˲��ŵ�ָ��λ��
//                arm_carry();
//                car_moveto_boundry(potato.Target_x, potato.Target_y);
//                car_boundry_carry(potato.Target_x, potato.Target_y);
//                arm_down();
//                select_mode();
//            }
//        }
//        else if (strcmp(classified, rice.Small_category) == 0)
//        {
//            rt_kprintf("This is a rice.\n");
//            if (strcmp(taget_Big_category, rice.Big_category) == 0)
//            {
//                rt_kprintf("This is  food.\n");
//                rice.Box_location = 6;
//                arm_putbox(rice.Box_location);
//                // ���������
//            }
//            else if (rice.Box_location)
//            {
//                arm_putbox(rice.Box_location);
//            }
//            else
//            {
//                // ���˲��ŵ�ָ��λ��
//                arm_carry();
//                car_moveto_boundry(rice.Target_x, rice.Target_y);
//                car_boundry_carry(rice.Target_x, rice.Target_y);
//                arm_down();
//                select_mode();
//            }
//        }

//        else
//        {
//            rt_kprintf("Unknown classification.\n");
//            select_mode();
//            // ֱ��ȥ��һ����λ
//        }
//    }
//}



//// void obj_detection_entry(void *param)
//// {
////     while (1)
////     {
////         rt_sem_take(obj_detection_sem, RT_WAITING_FOREVER); // ����ʶ���ź���

////         running_mode = 1;
////         detect_flag = 0;

////         rt_kprintf("DETECT !!!\n");

////         car_move(unknow_card_loction_x, unknow_card_loction_y); // ���ȵ�����һ��λ��
////         rt_thread_mdelay(2000);
////         ART2_mode = 1;
////         uart_putchar(USART_1, 0x46);

////         while (detect_flag == 0) // �Ƿ��������
////         {

////             if (boundry_num % 2 == 0)
////             {
////                 car_moveto_boundry(field_width + 1, 0); // �����ƶ��ұ���

////                 if (detect_flag == 0) // ���û���ҵ�����
////                 {
////                     boundry_num++;
////                     back(boundry_num);
////                     car.Speed_Y = 200;
////                     rt_thread_mdelay(1000);
////                     car.Speed_Y = 0;

////                     car.Speed_X = -200;
////                     rt_thread_mdelay(1500);
////                     car.Speed_X = 0;

////                     rt_thread_mdelay(1000);
////                     detect_flag = 0;

////                     rt_kprintf("boundry_num:%d\n", boundry_num);
////                 }
////                 else if (detect_flag == 1) // ���������
////                 {
////                     if (car.MileageX > 80 && car.MileageX < 670) // ����ڷ�Χ��
////                     {
////                         break;
////                     }
////                     else
////                     {

////                         boundry_num++;
////                         back(boundry_num);
////                         car.Speed_Y = 200;
////                         rt_thread_mdelay(1000);
////                         car.Speed_Y = 0;

////                         car.Speed_X = -200;
////                         rt_thread_mdelay(1500);
////                         car.Speed_X = 0;
////                         rt_thread_mdelay(1000);
////                         rt_kprintf("boundry_num:%d\n", boundry_num);
////                         detect_flag = 0;
////                         uart_putchar(USART_1, 0x46);
////                         continue;
////                     }
////                 }
////             }
////             else if (boundry_num % 2 == 1)
////             {
////                 car_moveto_boundry(-1, 0); // �����ƶ�Ѱ�ұ���

////                 if (detect_flag == 0) // ���û�п�Ƭ
////                 {

////                     boundry_num++;
////                     back(boundry_num);
////                     car.Speed_Y = 200;
////                     rt_thread_mdelay(1000);
////                     car.Speed_Y = 0;

////                     car.Speed_X = 200;
////                     rt_thread_mdelay(1500);
////                     car.Speed_X = 0;
////                     rt_thread_mdelay(1000);
////                     detect_flag = 0;
////                     rt_kprintf("boundry_num:%d\n", boundry_num);
////                 }
////                 else
////                 {
////                     if (car.MileageX > 80 && car.MileageX < 670) // �ҵ���Ƭ
////                     {
////                         break;
////                     }
////                     else
////                     {

////                         boundry_num++;
////                         back(boundry_num);
////                         car.Speed_Y = 200;
////                         rt_thread_mdelay(1000);
////                         car.Speed_Y = 0;
////                         car.Speed_X = 200;
////                         rt_thread_mdelay(1500);
////                         car.Speed_X = 0;
////                         rt_kprintf("boundry_num:%d\n", boundry_num);
////                         detect_flag = 0;
////                         rt_thread_mdelay(1000);
////                         uart_putchar(USART_1, 0x46);

////                         continue;
////                     }
////                 }
////             }
////         }

////         car.Speed_X = 0;
////         car.Speed_Y = 0;
////         unknow_card_loction_x = car.MileageX;
////         unknow_card_loction_y = car.MileageY; // ��¼��ǰ����
////         detect_flag = 0;

////         rt_sem_release(correct_sem);

////         // ��¼��ǰ����

////         // rt_sem_release(obj_detection_sem);
////     }
//// }


////ÿ��ʶ�𵽿�Ƭ��¼��ǰ��̼�λ�ã��������֮�󣬴��������𽥱���ÿһ����Ƭ
////�Ƿ���Ҫ������ʱ��Ҳʶ��Ƭ�� 

//void obj_detection_entry(void *param)
//{
//    while (1)
//    {
//        rt_sem_take(obj_detection_sem, RT_WAITING_FOREVER); // ����ʶ���ź���

//        running_mode = 1;
//        detect_flag = 0;

//        rt_kprintf("DETECT !!!\n");

//        car_move(unknow_card_loction_x, unknow_card_loction_y); // ���ȵ�����һ��λ��
//        rt_thread_mdelay(2000);
//        ART2_mode = 1;
//        uart_putchar(USART_1, 0x46);

//        while (detect_flag == 0) // �Ƿ��������
//        {

//            if (boundry_num % 2 == 0)
//            {
//                car_moveto_boundry(field_width + 1, 0); // �����ƶ��ұ���

//                if (detect_flag == 0) // ���û���ҵ�����
//                {
//                    boundry_num++;
//                    back(boundry_num);
//                    car.Speed_Y = 200;
//                    rt_thread_mdelay(1000);
//                    car.Speed_Y = 0;

//                    car.Speed_X = -200;
//                    rt_thread_mdelay(1500);
//                    car.Speed_X = 0;

//                    rt_thread_mdelay(1000);
//                    detect_flag = 0;

//                    rt_kprintf("boundry_num:%d\n", boundry_num);
//                }
//                else if (detect_flag == 1) // ���������
//                {
//                    if (car.MileageX > 80 && car.MileageX < 670) // ����ڷ�Χ��
//                    {
//                        break;
//                    }
//                    else
//                    {

//                        boundry_num++;
//                        back(boundry_num);
//                        car.Speed_Y = 200;
//                        rt_thread_mdelay(1000);
//                        car.Speed_Y = 0;

//                        car.Speed_X = -200;
//                        rt_thread_mdelay(1500);
//                        car.Speed_X = 0;
//                        rt_thread_mdelay(1000);
//                        rt_kprintf("boundry_num:%d\n", boundry_num);
//                        detect_flag = 0;
//                        uart_putchar(USART_1, 0x46);
//                        continue;
//                    }
//                }
//            }
//            else if (boundry_num % 2 == 1)
//            {
//                car_moveto_boundry(-1, 0); // �����ƶ�Ѱ�ұ���

//                if (detect_flag == 0) // ���û�п�Ƭ
//                {

//                    boundry_num++;
//                    back(boundry_num);
//                    car.Speed_Y = 200;
//                    rt_thread_mdelay(1000);
//                    car.Speed_Y = 0;

//                    car.Speed_X = 200;
//                    rt_thread_mdelay(1500);
//                    car.Speed_X = 0;
//                    rt_thread_mdelay(1000);
//                    detect_flag = 0;
//                    rt_kprintf("boundry_num:%d\n", boundry_num);
//                }
//                else
//                {
//                    if (car.MileageX > 80 && car.MileageX < 670) // �ҵ���Ƭ
//                    {
//                        break;
//                    }
//                    else
//                    {

//                        boundry_num++;
//                        back(boundry_num);
//                        car.Speed_Y = 200;
//                        rt_thread_mdelay(1000);
//                        car.Speed_Y = 0;
//                        car.Speed_X = 200;
//                        rt_thread_mdelay(1500);
//                        car.Speed_X = 0;
//                        rt_kprintf("boundry_num:%d\n", boundry_num);
//                        detect_flag = 0;
//                        rt_thread_mdelay(1000);
//                        uart_putchar(USART_1, 0x46);

//                        continue;
//                    }
//                }
//            }
//        }

//        car.Speed_X = 0;
//        car.Speed_Y = 0;
//        unknow_card_loction_x = car.MileageX;
//        unknow_card_loction_y = car.MileageY; // ��¼��ǰ����
//        detect_flag = 0;

//        rt_sem_release(correct_sem);

//        // ��¼��ǰ����

//        // rt_sem_release(obj_detection_sem);
//    }
//}



//void car_start_init(void)
//{

//    arrive_sem = rt_sem_create("arrive_sem", 1, RT_IPC_FLAG_FIFO);                   // �����ź��������ܾͿ�ʼ�ܵ�
//    uart_corrdinate_sem = rt_sem_create("uart_corrdinate_sem", 0, RT_IPC_FLAG_FIFO); // ���������ź���
//    correct_sem = rt_sem_create("correct_sem", 0, RT_IPC_FLAG_FIFO);                 // �����ź��������ܾͿ�ʼ����
//    recognize_sem = rt_sem_create("recognize_sem", 0, RT_IPC_FLAG_FIFO);             // ʶ���ź��������ߵ�Ƭ���Ѿ�ʶ�𣬽��ܾͿ�ʼ����
//    obj_detection_sem = rt_sem_create("obj_detection_sem", 0, RT_IPC_FLAG_FIFO);     // Ŀ�����ź���

//    route_planning_th = rt_thread_create("route_planning_th", route_planning_entry, RT_NULL, 1024, 28, 10);
//    correct_th = rt_thread_create("correct_th", correct_entry, RT_NULL, 1024, 28, 10);
//    carry_th = rt_thread_create("carry_th", carry_entry, RT_NULL, 1024, 28, 10);
//    obj_detection_th = rt_thread_create("obj_detection_th", obj_detection_entry, RT_NULL, 1024, 28, 10);

//    rt_thread_startup(route_planning_th);
//    rt_thread_startup(correct_th);
//    rt_thread_startup(carry_th);
//    rt_thread_startup(obj_detection_th);
//}
