
# 导入需要的库
import sensor, image, time, math
import os, tf
from pyb import LED
from machine import UART
import utime
#import SEEKFREE_IPS114_SPI as ips114

global pos_perx,uart_flag
pos_perx = []
uart_flag = 0

uart = UART(1, baudrate=115200)  # 初始化串口 波特率设置为115200 TX是B12 RX是B13



# 初始化屏幕
def openart_init():

    # 初始化摄像头
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_brightness(2000)
    sensor.skip_frames(20)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(True,(0,0,0))
    # 打开LED灯

    #ips114.ips_init()


def uart_coordinates(pos_resultx, pos_resulty):

    global uart_flag
    if (uart_flag == 0):#发送标志位
        uart.write("A")  # 发送包头 coordinates

        for i in range(len(pos_resultx)):
            uart.write("%c" % (pos_resultx[i]))
            utime.sleep_ms(5)

        for i in range(len(pos_resulty)):
            uart.write("%c" % (pos_resulty[i]))
            utime.sleep_ms(5)

        uart.write("Y")#发送包尾

        uart_flag = 1 #结束发送-


def find_coordinates(img):
    global pos_perx
    global uart_flag
    pos_nowx = [] #x坐标现在的值
    pos_y = [] #y坐标

    for r in img.find_rects(threshold=3000):
        # 判断矩形是否满足要求
        if r.w() > 120 and r.w() < 180 and r.h() > 70 and r.h() < 130:
            # 绘制矩形外框，便于在IDE上查看识别到的矩形位置
            img.draw_rectangle(r.rect(), color=(255, 0, 0))
            # 将矩形属性打印出来
            # jiangly 不是dd
            print("w:" + str(r.w()) + "  h:" + str(r.h()))
            # 在矩形中搜索圆形
            for c in img.find_circles(roi=r.rect(), threshold=2800, x_margin=5, y_margin=5, r_margin=5, r_min=1,
                                      r_max=5, r_step=1):
                # 绘制圆形外框
                img.draw_circle(c.x(), c.y(), c.r(), color=(255, 0, 0))
                # 计算实际X坐标点
                posx = (float(c.x()) - float(r.x())) / float(r.w()) * 35 + 1
                # 计算实际Y坐标点
                posy = 25 - ((float(c.y()) - float(r.y())) / float(r.h()) * 25) + 1

                pos_nowx.append(int(posx)) #将识别到的坐标点加入列表中
                pos_y.append(int(posy))

                img.draw_string(c.x() - 5, c.y() - 10, "(" + str(int(posx)) + "," + str(int(posy)) + ")",
                                color=(0, 0, 255), scale=1, mono_space=False)

        result_x = sorted(pos_nowx) #将x坐标点进行排序
        if(result_x == pos_perx and len(result_x) > 10): #如果这一次识别和上一次相同
            uart_coordinates(pos_nowx, pos_y)
            print(pos_nowx)
            print(pos_y)
        else:
            pos_perx = result_x #将上一次排序好的坐标保存



def main():
    openart_init()
    while(1):
        img = sensor.snapshot()
        find_coordinates(img)
        #ips114.ips_display(img, 160, 120)

if __name__ == '__main__':
    main()
