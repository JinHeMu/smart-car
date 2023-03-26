#include "carmove.h"

/**************************************************************************
1.读取坐标，保存在相应的数组中
2.路径规划，每次都以最近的点位，重新排列一个新的点位
3.如何控制小车运动到应该到的点位，同时规划出下一个点位
**************************************************************************/

struct point tar_point[15];//排序好顺序的目标坐标

Pose_car car;//定义car，作为位姿的载体

/**************************************************************************
函数功能：将串口接受的字符坐标转化成数字坐标
入口参数：ART识别到的坐标   
返回值：无
**************************************************************************/
void uart_coordinate_transforming(uint8 ART1_POINT_X[15], uint8 ART1_POINT_Y[15], uint8 num)
{
    for(int i = 0; i < num; i++)
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
void nearest_neighbor_sort(struct point* arr, int size)
 {
    // 找到距离原点最近的点
    int min_dist = INT_MAX;
    int nearest = -1;
    for (int i = 0; i < size; i++) {
        int dist = arr[i].x * arr[i].x + arr[i].y * arr[i].y;
        if (dist < min_dist && dist != 0) {
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

    for (int i = 1; i < size; i++) {
        int min_dist = INT_MAX;
        int nearest = -1;
        for (int j = 0; j < size; j++) {
            if (!visited[j]) {
                int dist = (arr[j].x - sorted[i-1].x) * (arr[j].x - sorted[i-1].x) +
                           (arr[j].y - sorted[i-1].y) * (arr[j].y - sorted[i-1].y);
                if (dist < min_dist) {
                    min_dist = dist;
                    nearest = j;
                }
            }
        }
        visited[nearest] = true;
        sorted[i] = arr[nearest];
    }

    // 将排序后的点复制回原始数组
    for (int i = 0; i < size; i++) {
        arr[i] = sorted[i];
    }
}


/**************************************************************************
函数功能：求两点间的距离
入口参数：当前位置 目标位置  
返回值：两点距离
**************************************************************************/
float distance(float current_x,float current_y,float target_x,float target_y) 
{ 
	static float temp;
	float det_x=(target_x*20-current_x);
	float det_y=(target_y*20-current_y);
	temp = (float)sqrt(det_x*det_x+det_y*det_y);
//	rt_kprintf("%d\n", (int)temp);
	return temp;
}

//获得当前位置，以及距离下一个点位的距离
void get_location(void)
{
//    charge_locate();
//    get_target();
    //用两点式计算角度和距离
    car.target_angle=atan2((car.target_x-car.current_x),(car.target_y-car.current_y))*180/PI;
	//计算两点x，y的距离差值
    car.target_distanceX=20*(car.target_x-car.current_x);
    car.target_distanceY=20*(car.target_y-car.current_y);
    //Car.Distance=sqrt(Car.DistanceX*Car.DistanceX+Car.DistanceY*Car.DistanceY);
}

void traverse_points() 
{

		
    for (int i = 0; i < point_num/2; i++) 
    {   

       
        //获取目标坐标
        car.target_x = tar_point[i].x;
        car.target_y = tar_point[i].y;
				rt_kprintf("TARGET_X%d\n", (int)car.target_x);
				rt_kprintf("TARGET_Y%d\n", (int)car.target_y);				
				
				rt_thread_mdelay(2000);
        while (distance(car.MileageX, car.MileageY, car.target_x, car.target_y) > 10) 
        {
            car.Speed_X=picture_x_pid((int)car.MileageX, (int)car.target_x*20);
            car.Speed_Y=picture_y_pid((int)car.MileageY, (int)car.target_y*20);
        }
        //更新当前坐标
        car.current_x = car.target_x;
        car.current_y = car.target_y;
				rt_mb_send(buzzer_mailbox, 1000);//给buzzer_mailbox发送100
				

				
    }
		car.Speed_X = 0;
		car.Speed_Y = 0;

}


void route_planning_entry(void *param)
{
	rt_sem_take(point_sem, RT_WAITING_FOREVER);
	uart_coordinate_transforming(ART1_POINT_X,ART1_POINT_Y, point_num/2);
	nearest_neighbor_sort(tar_point, point_num/2);
	for(int i = 0; i < point_num/2; i++)
	{
				rt_kprintf("x:%d", tar_point[i].x);
				rt_kprintf("y:%d\n", tar_point[i].y);
  }
	rt_mb_send(buzzer_mailbox, 1000);//给buzzer_mailbox发送100
	traverse_points();
	
	
}

void route_planning_init()
{
	rt_thread_t route_planning_th;
	point_sem = rt_sem_create("point_sem", 0, RT_IPC_FLAG_FIFO);
	route_planning_th = rt_thread_create("route_planning_th", route_planning_entry, RT_NULL, 1024, 21, 1000);
	rt_thread_startup(route_planning_th);
	
}



