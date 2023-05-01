# 导入需要的库
import sensor, image, time, math
import os, tf
from pyb import LED
from machine import UART
import utime
import seekfree


line = None

threshold = [(55, 100, -128, 127, -30, 127)]

uart = UART(2, baudrate=115200)  # 初始化串口 波特率设置为115200 TX是B12 RX是B13
lcd = seekfree.LCD180(2)
lcd.full()

LED(4).on()





# 初始化屏幕
def openart_init():

    # 初始化摄像头
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_brightness(800)
    sensor.skip_frames(20)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(True,(0,0,0))
    # 打开LED灯

    #ips114.ips_init()


def uart_correct(img):

    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QQVGA)
    sensor.set_brightness(1000)
    sensor.skip_frames(20)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(True,(0,0,0))
    while True:
        img = sensor.snapshot()

        line = img.get_regression(threshold)

    # 如果找到了线段
        if line:
            img.draw_line(line.line(), color = (255, 0, 0)) # 在图像上标出线段
            angle = line.theta() # 计算线段的角度
            if(angle > 90):
                angle -= 180
            print("Line Angle: ", angle)







def main():
    openart_init()
    while(True):
        img = sensor.snapshot()
        uart_correct(img)






if __name__ == '__main__':
    main()
