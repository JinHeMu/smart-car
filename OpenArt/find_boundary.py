# ������Ҫ�Ŀ�
import sensor, image, time, math
import os, tf
from pyb import LED
from machine import UART
import utime
import seekfree


line = None

threshold = [(55, 100, -128, 127, -30, 127)]

uart = UART(2, baudrate=115200)  # ��ʼ������ ����������Ϊ115200 TX��B12 RX��B13
lcd = seekfree.LCD180(2)
lcd.full()

LED(4).on()





# ��ʼ����Ļ
def openart_init():

    # ��ʼ������ͷ
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_brightness(800)
    sensor.skip_frames(20)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(True,(0,0,0))
    # ��LED��

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

    # ����ҵ����߶�
        if line:
            img.draw_line(line.line(), color = (255, 0, 0)) # ��ͼ���ϱ���߶�
            angle = line.theta() # �����߶εĽǶ�
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
