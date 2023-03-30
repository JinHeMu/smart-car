import seekfree, pyb
import sensor, image, time, math
import os, tf
from pyb import LED
from machine import UART

uart = UART(1, baudrate=115200)  # 初始化串口 波特率设置为115200 TX是B12 RX是B13

sensor.reset()
sensor.set_pixformat(sensor.RGB565)  # 设置摄像头像素格式
sensor.set_framesize(sensor.QQVGA)   # 设置摄像头分辨率
sensor.set_brightness(3000)          # 设置摄像头亮度
sensor.skip_frames(time = 200)
clock = time.clock()


def uart_correct(img):
    dis_X = 0
    dis_Y = 0
    for r in img.find_rects(threshold=20000):
        img.draw_rectangle(r.rect(), color=(255, 0, 0))  # 绘制红色矩形框
        img_x = (int)(r.rect()[0] + r.rect()[2] / 2)  # 图像中心的x值
        img_y = (int)(r.rect()[1] + r.rect()[3] / 2)  # 图像中心的y值
        img.draw_circle(img_x, img_y, 5, color=(0, 255, 0))  # 给矩形中心绘制一个小圆 便于观察矩形中心是否识别正确
        dis_X = img_x - 88
        dis_Y = img_y - 90
        print("disx:%d , disy:%d" % (dis_X, dis_Y))
        uart.write("C")
        uart.write("%c" % dis_X)
        uart.write("%c" % dis_Y)
        uart.write("D")

while(True):
        clock.tick()
        img = sensor.snapshot()
        uart_correct(img)




