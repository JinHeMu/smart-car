//#include "carmove.h"

///**************************************************************************
//1.读取坐标，保存在相应的数组中
//2.路径规划，每次都以最近的点位，重新排列一个新的点位
//3.如何控制小车运动到应该到的点位，同时规划出下一个点位
//**************************************************************************/

//// 模式0  停止等待
//// 模式1  导航 纠正 识别
//// 模式2  搬运图片模式
//// 模式3  回车库
//// #define field_width 35
//// #define field_height 25

//#define field_width 35
//#define field_height 25

//uint8 running_mode = 0; // 小车功能模式：0为寻找坐标点模式   1为目标检测模式
//uint8 game_mode = 0;    // 0为初赛  1为决赛
//uint8 find_mode = 0;    // 0为无目标检测  1为有目标检测

//point tar_point[40];    // 排序好顺序的目标坐标
//int8 visited[40] = {0}; // 初始化访问数组，所有坐标都未访问
//int8 nearestIndex = -1;

//Pose_car car; // 定义car，作为位姿的载体

//rt_sem_t correct_sem;       // 矫正
//rt_sem_t arrive_sem;        // 到达
//rt_sem_t recognize_sem;     // 识别
//rt_sem_t obj_detection_sem; // 目标检测

//rt_thread_t route_planning_th; // 路径规划线程
//rt_thread_t correct_th;        // 矫正线程
//rt_thread_t carry_th;          // 搬运线程
//rt_thread_t obj_detection_th;  // 目标检测线程

//uint8 pic_dis = 0;
//uint8 find_card_x = 0;
//uint8 find_card_y = 0;
//uint8 count = 0;
//uint8 boundry_num = 0;

//float unknow_card_loction_x = 80;
//float unknow_card_loction_y = 60;

////**初赛**

///*
//1-上
//2-右
//3-下
//4-左
//5-车载
//*/


////**决赛**

///*
//1-上
//2-右
//3-下
//4-左
//5-车载
//6-大类
//*/



//struct card cabbage = {"vegetable", "cabbage", 1, 12, 26};  // 上三类
//struct card bean = {"food", "bean", 1, 4, 26};              // 上三类
//struct card orange = {"fruit", "orange", 1, 8, 26};         // 上三类

//struct card apple = {"fruit", "apple", 2, 36, 8};           // 右三类
//struct card pepper = {"vegetable", "pepper", 2, 36, 4};     // 右三类
//struct card peanut = {"food", "peanut", 2, 36, 12};         // 右三类

//struct card bannana = {"fruit", "bannana", 3, 8, -1};       // 下三类
//struct card radish = {"vegetable", "radish", 3, 12, -1};    // 下三类
//struct card rice = {"food", "rice", 3, 4, -1};              // 下三类

//struct card durian = {"fruit", "durian", 4, -1, 8};         // 左三类
//struct card cucumber = {"vegetable", "cucumber", 4, -1, 4}; // 左三类
//struct card potato = {"food", "potato", 4, -1, 12};         // 左三类

//struct card grape = {"fruit", "grape", 5, 0, 0};            // 车载
//struct card corn = {"food", "corn", 5, 0, 0};               // 车载
//struct card eggplant = {"vegetable", "eggplant", 5, 0, 0};  // 车载


////  struct card apple = {"fruit", "apple", 4, 36, 8};     // 右三类
//// struct card bannana = {"fruit", "bannana", 2, 8, -1}; // 下三类
//// struct card durian = {"fruit", "durian", 3, -1, 8};   // 左三类
//// struct card grape = {"fruit", "grape", 1, 0, 0};      // 车载
//// struct card orange = {"fruit", "orange", 0, 8, 26};   // 上三类

//// struct card cabbage = {"vegetable", "cabbage", 0, 12, 26};  // 上三类
//// struct card cucumber = {"vegetable", "cucumber", 3, -1, 4}; // 左三类
//// struct card eggplant = {"vegetable", "eggplant", 1, 0, 0};  // 车载
//// struct card radish = {"vegetable", "radish", 2, 12, -1};    // 下三类
//// struct card pepper = {"vegetable", "pepper", 4, 36, 4};     // 右三类

//// struct card corn = {"food", "corn", 1, 0, 0};       // 车载
//// struct card bean = {"food", "bean", 0, 4, 26};      // 上三类
//// struct card peanut = {"food", "peanut", 4, 36, 12}; // 右三类
//// struct card potato = {"food", "potato", 3, -1, 12}; // 左三类
//// struct card rice = {"food", "rice", 2, 4, -1};      // 下三类

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
//函数功能：求两点间的距离
//入口参数：当前位置 目标位置
//返回值：两点距离
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

//// 获得目标角度
//float get_angle(float current_x, float current_y, float target_x, float target_y)
//{
//    float angle;
//    angle = atan2((target_x - current_x), (target_y - current_y));
//    return angle;
//}

///**************************************************************************
//函数功能：整合函数  输入坐标 小车到指定坐标
//入口参数：目标坐标
//返回值：无
//**************************************************************************/

//void car_speed_x(int16 tar_speed)
//{
//    float acceleration = 0.02; // 加速度，可根据实际情况调整
//    float max_speed = 1;    // 最大速度，可根据实际情况调整
//    float current_speed = 0;
//    float progress = 0; // 用于表示进度，范围从 0 到 1

//    // 逐渐增加速度
//    while (progress < 1)
//    {
//        // 使用缓动函数（二次缓动函数）计算进度
//        progress = 1 - pow(1 - current_speed / max_speed, 2);

//        current_speed += acceleration;

//        if (current_speed > max_speed)current_speed = max_speed;

//        car.Speed_X = current_speed * tar_speed;
//        rt_thread_mdelay(20);
//    }
//}


//void car_speed_y(int16 tar_speed)
//{
//    float acceleration = 0.02; // 加速度，可根据实际情况调整
//    float max_speed = 1;    // 最大速度，可根据实际情况调整
//    float current_speed = 0;
//    float progress = 0; // 用于表示进度，范围从 0 到 1

//    // 逐渐增加速度
//    while (progress < 1)
//    {
//        // 使用缓动函数（二次缓动函数）计算进度
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
//    float acceleration = 0.02; // 加速度，可根据实际情况调整
//    float max_speed = 0.5;    // 最大速度，可根据实际情况调整
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

//    while (current_distance > 5) // 持续运动
//    {
//        // 逐渐增加速度
//        if (current_speed < max_speed)
//            current_speed += acceleration;

//        // 控制电机速度
//        car.Speed_X = current_speed * picture_x_pid((int)car.MileageX, (int)tar_x); // cm
//        car.Speed_Y = current_speed * picture_y_pid((int)car.MileageY, (int)tar_y);

//        rt_thread_mdelay(20);

//        // 更新当前距离
//        current_distance = distance(car.MileageX, car.MileageY, tar_x, tar_y);
//        //rt_kprintf("dis:%d\n", (int)current_distance);
//    }

//    car.Speed_X = 0;
//    car.Speed_Y = 0;

//    rt_mb_send(buzzer_mailbox, 100); // 给buzzer_mailbox发送100

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
//        arm_openbox(3); // 右三

//        car.Speed_Y = -200;
//        rt_thread_mdelay(1000);
//        car.Speed_Y = 0;

//        car.Speed_X = -200;
//        rt_thread_mdelay(2000);
//        car.Speed_X = 0;

//        if (game_mode == 1)
//        {
//            car_move(70, 500);
//            arm_openbox(2); // 大类

//        }else
//        {
//            car_moveto_boundry(1, field_height + 1);
//            car_boundry_carry(1, field_height + 1);
//						car_boundry_carry(1, field_height + 1);
//            arm_openbox(2); // 上三
//        }

//						car.Speed_Y = -200;
//						rt_thread_mdelay(1000);
//						car.Speed_Y = 0;


//        car_moveto_boundry(-1, 1);
//        car_boundry_carry(-1, 1);
//        arm_openbox(1); // 左三

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

//    // 定义四条边线
//    rt_kprintf("GO TO BOUNDRE!!!\n");

//    ART1_mode = 4;
//    ART1_CORRECT_Boundary_Flag = 0;

//    if (tar_x < 0)
//    {
//        uart_putchar(USART_4, 0x44); // 发送OPENART1告诉该识别边线了
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

//        if (detect_flag == 0) // 如果不是目标检测中断，就是边线
//        {
//            car.MileageX = 0;
//        }
//    }
//    else if (tar_y < 0)
//    {
//        uart_putchar(USART_4, 0x45); // 发送OPENART1告诉该识别边线了
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
//        uart_putchar(USART_4, 0x44); // 发送OPENART1告诉该识别边线了
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
//        uart_putchar(USART_4, 0x45); // 发送OPENART1告诉该识别边线了
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
//    rt_mb_send(buzzer_mailbox, 100); // 给buzzer_mailbox发送100
//    // rt_kprintf("I HAVE ARRIVED BOUNDRE!!!");
//    rt_kprintf("flag:%d\n", detect_flag);
//    ART1_CORRECT_Boundary_Flag = 0;
//}


//void car_moveto_boundry_fast(int8 tar_x, int8 tar_y)
//{

//    // 定义四条边线
//    rt_kprintf("GO TO BOUNDRE!!!\n");

//    ART1_mode = 4;
//    ART1_CORRECT_Boundary_Flag = 0;

//    if (tar_x < 0)
//    {
//        uart_putchar(USART_4, 0x44); // 发送OPENART1告诉该识别边线了
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

//        if (detect_flag == 0) // 如果不是目标检测中断，就是边线
//        {
//            car.MileageX = 0;
//        }
//    }
//    else if (tar_y < 0)
//    {
//        uart_putchar(USART_4, 0x45); // 发送OPENART1告诉该识别边线了
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
//        uart_putchar(USART_4, 0x44); // 发送OPENART1告诉该识别边线了
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
//        uart_putchar(USART_4, 0x45); // 发送OPENART1告诉该识别边线了
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
//    rt_mb_send(buzzer_mailbox, 100); // 给buzzer_mailbox发送100
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
//        arm_openbox(3); // 右三

//        car.Speed_Y = -400;
//        rt_thread_mdelay(500);
//        car.Speed_Y = 0;

//        car.Speed_X = -400;
//        rt_thread_mdelay(1000);
//        car.Speed_X = 0;

//        if (game_mode == 1)
//        {
//            car_move(50, 500);
//            arm_openbox(2); // 大类
//        }else
//        {
//            car_moveto_boundry_fast(1, field_height + 1);
//            car_boundry_carry(1, field_height + 1);
//						car_boundry_carry(1, field_height + 1);
//            arm_openbox(2); // 上三
//        }



//        car.Speed_Y = -400;
//        rt_thread_mdelay(500);
//        car.Speed_Y = 0;

//        car_moveto_boundry_fast(-1, 1);
//        car_boundry_carry(-1, 1);
//        arm_openbox(1); // 左三

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
//函数功能：将串口接受的字符坐标转化成数字坐标
//入口参数：ART识别到的坐标
//返回值：无
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
//函数功能：静态路径规划
//入口参数：ART识别到的坐标
//返回值：无
//**************************************************************************/
//void static_planning(struct point *arr, int size)
//{
//    // 找到距离原点最近的点
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

//    // 从最近的点开始遍历并查找最近的邻居
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
//    // 将排序后的点复制回原始数组
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
//            continue; // 跳过已访问过的坐标
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
//        visited[*nearestIndex] = 1; // 将最近点标记为已访问
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
//    rt_mb_send(buzzer_mailbox, 100); // 给buzzer_mailbox发送100

//    car_move(10, 40); // 出库

//    while (1)
//    {

//        rt_sem_take(arrive_sem, RT_WAITING_FOREVER); // 接受到达信号量
//        rt_kprintf("arriving!!!\n");
//        rt_mb_send(buzzer_mailbox, 100); // 给buzzer_mailbox发送100

//        findNearestCoordinate(car, tar_point, coordinate_num, visited, &nearestIndex);

//        if (nearestIndex != -1)
//        {
//            rt_kprintf("The nearest point(%d,%d)\n", tar_point[nearestIndex].x * 20, tar_point[nearestIndex].y * 20);
//            visited[nearestIndex] = 1; // 将本次访问过的点标记为已访问
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
//            car.target_x = tar_point[nearestIndex].x * 20; // 获取目标坐标点
//            car.target_y = tar_point[nearestIndex].y * 20;
//            rt_kprintf("TARGET_X%d   ", (int)car.target_x);
//            rt_kprintf("TARGET_Y%d\n", (int)car.target_y);
//            count++;
//        }

//        car_move(car.target_x, car.target_y);

//        rt_sem_release(correct_sem); // 到达后发送矫正信号
//    }
//}

//void correct_entry(void *param)
//{
//    while (1)
//    {
//        rt_sem_take(correct_sem, RT_WAITING_FOREVER); // 获取矫正信号

//        ART2_CORRECT_Flag = 0;

//        ART2_mode = 2;               // art矫正模式
//        uart_putchar(USART_1, 0x42); // 持续发送“B”来告诉openart该矫正了

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

//        if (running_mode == 0) // 目标检测模式就不更新，普通模式更新
//        {
//            car.MileageX = car.target_x; // 更新当前坐标
//            car.MileageY = car.target_y;
//        }

//        rt_mb_send(buzzer_mailbox, 100); // 给buzzer_mailbox发送100

//        rt_sem_release(recognize_sem);
//        // rt_sem_release(correct_sem);
//        //				rt_sem_release(arrive_sem);
//    }
//}

//void carry_entry(void *param)
//{

//    while (1)
//    {
//        rt_sem_take(recognize_sem, RT_WAITING_FOREVER); // 接受识别信号量

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
//                apple.Box_location = 6; // 放入盒子2中
//                arm_putbox(apple.Box_location);
//            }
//            else if (apple.Box_location)
//            {
//                arm_putbox(apple.Box_location);
//            }
//            else
//            {
//                // 搬运并放到指定位置
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
//                // 搬运并放到指定位置
//                arm_carry();
//                car_moveto_boundry(bannana.Target_x, bannana.Target_y);
//                car_boundry_carry(bannana.Target_x, bannana.Target_y);
//                arm_down();
//                select_mode();
//            }
//        }
//        else if (strcmp(classified, grape.Small_category) == 0)
//        {
//            // 车载
//            rt_kprintf("This is a grape.\n");
//            if (strcmp(taget_Big_category, grape.Big_category) == 0)
//            {
//                rt_kprintf("This is  fruit.\n");
//                grape.Box_location = 6; // 放入盒子6中
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
//                // 搬运并放到指定位置
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
//                orange.Box_location = 0; // 如果模式为决赛，则单搬
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
//                // 放入盒子中
//            }
//            else if (orange.Box_location)
//            {
//                arm_putbox(orange.Box_location);
//            }
//            else
//            {
//                // 搬运并放到指定位置
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
//                cabbage.Box_location = 0; // 如果模式为决赛，则单搬
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
//                // 放入盒子中
//            }
//            else if (cabbage.Box_location)
//            {
//                arm_putbox(cabbage.Box_location);
//            }
//            else
//            {
//                // 搬运并放到指定位置
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
//                // 放入盒子中
//            }
//            else if (cucumber.Box_location)
//            {
//                arm_putbox(cucumber.Box_location);
//            }
//            else
//            {
//                // 搬运并放到指定位置
//                arm_carry();
//                car_moveto_boundry(cucumber.Target_x, cucumber.Target_y);
//                car_boundry_carry(cucumber.Target_x, cucumber.Target_y);
//                arm_down();
//            }
//        }
//        else if (strcmp(classified, eggplant.Small_category) == 0)
//        {
//            // 车载
//            rt_kprintf("This is a eggplant.\n");
//            if (strcmp(taget_Big_category, eggplant.Big_category) == 0)
//            {
//                rt_kprintf("This is  vegetable.\n");
//                eggplant.Box_location = 6;
//                arm_putbox(eggplant.Box_location);
//                // 放入盒子中
//            }
//            else
//            {
//                arm_putbox(eggplant.Box_location); // 放入盒子1中
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
//                // 放入盒子中
//            }
//            else if (radish.Box_location)
//            {
//                arm_putbox(radish.Box_location);
//            }
//            else
//            {
//                // 搬运并放到指定位置
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
//                // 放入盒子中
//            }
//            else if (pepper.Box_location)
//            {
//                arm_putbox(pepper.Box_location);
//            }
//            else
//            {
//                // 搬运并放到指定位置
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
//                // 放入盒子中
//            }
//            else
//            {
//                arm_putbox(corn.Box_location); // 放入盒子1中
//            }
//        }
//        else if (strcmp(classified, bean.Small_category) == 0)
//        {
//            if (game_mode == 1)
//            {
//                bean.Box_location = 0; // 如果模式为决赛，则单搬
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
//                // 放入盒子中
//            }
//            else if (bean.Box_location)
//            {
//                arm_putbox(bean.Box_location);
//            }
//            else
//            {
//                // 搬运并放到指定位置
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
//                // 放入盒子中
//            }
//            else if (peanut.Box_location)
//            {
//                arm_putbox(peanut.Box_location);
//            }
//            else
//            {
//                // 搬运并放到指定位置
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
//                // 放入盒子中
//            }
//            else if (potato.Box_location)
//            {
//                arm_putbox(potato.Box_location);
//            }
//            else
//            {
//                // 搬运并放到指定位置
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
//                // 放入盒子中
//            }
//            else if (rice.Box_location)
//            {
//                arm_putbox(rice.Box_location);
//            }
//            else
//            {
//                // 搬运并放到指定位置
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
//            // 直接去下一个点位
//        }
//    }
//}



//// void obj_detection_entry(void *param)
//// {
////     while (1)
////     {
////         rt_sem_take(obj_detection_sem, RT_WAITING_FOREVER); // 接受识别信号量

////         running_mode = 1;
////         detect_flag = 0;

////         rt_kprintf("DETECT !!!\n");

////         car_move(unknow_card_loction_x, unknow_card_loction_y); // 首先到达上一次位置
////         rt_thread_mdelay(2000);
////         ART2_mode = 1;
////         uart_putchar(USART_1, 0x46);

////         while (detect_flag == 0) // 是否接受数据
////         {

////             if (boundry_num % 2 == 0)
////             {
////                 car_moveto_boundry(field_width + 1, 0); // 向右移动找边线

////                 if (detect_flag == 0) // 如果没有找到边线
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
////                 else if (detect_flag == 1) // 如果有数据
////                 {
////                     if (car.MileageX > 80 && car.MileageX < 670) // 如果在范围内
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
////                 car_moveto_boundry(-1, 0); // 向左移动寻找边线

////                 if (detect_flag == 0) // 如果没有卡片
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
////                     if (car.MileageX > 80 && car.MileageX < 670) // 找到卡片
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
////         unknow_card_loction_y = car.MileageY; // 记录当前坐标
////         detect_flag = 0;

////         rt_sem_release(correct_sem);

////         // 记录当前坐标

////         // rt_sem_release(obj_detection_sem);
////     }
//// }


////每次识别到卡片记录当前里程计位置，到达边线之后，从右往左逐渐遍历每一个卡片
////是否需要遍历的时候也识别卡片？ 

//void obj_detection_entry(void *param)
//{
//    while (1)
//    {
//        rt_sem_take(obj_detection_sem, RT_WAITING_FOREVER); // 接受识别信号量

//        running_mode = 1;
//        detect_flag = 0;

//        rt_kprintf("DETECT !!!\n");

//        car_move(unknow_card_loction_x, unknow_card_loction_y); // 首先到达上一次位置
//        rt_thread_mdelay(2000);
//        ART2_mode = 1;
//        uart_putchar(USART_1, 0x46);

//        while (detect_flag == 0) // 是否接受数据
//        {

//            if (boundry_num % 2 == 0)
//            {
//                car_moveto_boundry(field_width + 1, 0); // 向右移动找边线

//                if (detect_flag == 0) // 如果没有找到边线
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
//                else if (detect_flag == 1) // 如果有数据
//                {
//                    if (car.MileageX > 80 && car.MileageX < 670) // 如果在范围内
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
//                car_moveto_boundry(-1, 0); // 向左移动寻找边线

//                if (detect_flag == 0) // 如果没有卡片
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
//                    if (car.MileageX > 80 && car.MileageX < 670) // 找到卡片
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
//        unknow_card_loction_y = car.MileageY; // 记录当前坐标
//        detect_flag = 0;

//        rt_sem_release(correct_sem);

//        // 记录当前坐标

//        // rt_sem_release(obj_detection_sem);
//    }
//}



//void car_start_init(void)
//{

//    arrive_sem = rt_sem_create("arrive_sem", 1, RT_IPC_FLAG_FIFO);                   // 到达信号量，接受就开始跑点
//    uart_corrdinate_sem = rt_sem_create("uart_corrdinate_sem", 0, RT_IPC_FLAG_FIFO); // 接收坐标信号量
//    correct_sem = rt_sem_create("correct_sem", 0, RT_IPC_FLAG_FIFO);                 // 矫正信号量，接受就开始矫正
//    recognize_sem = rt_sem_create("recognize_sem", 0, RT_IPC_FLAG_FIFO);             // 识别信号量，告诉单片机已经识别，接受就开始搬运
//    obj_detection_sem = rt_sem_create("obj_detection_sem", 0, RT_IPC_FLAG_FIFO);     // 目标检测信号量

//    route_planning_th = rt_thread_create("route_planning_th", route_planning_entry, RT_NULL, 1024, 28, 10);
//    correct_th = rt_thread_create("correct_th", correct_entry, RT_NULL, 1024, 28, 10);
//    carry_th = rt_thread_create("carry_th", carry_entry, RT_NULL, 1024, 28, 10);
//    obj_detection_th = rt_thread_create("obj_detection_th", obj_detection_entry, RT_NULL, 1024, 28, 10);

//    rt_thread_startup(route_planning_th);
//    rt_thread_startup(correct_th);
//    rt_thread_startup(carry_th);
//    rt_thread_startup(obj_detection_th);
//}
