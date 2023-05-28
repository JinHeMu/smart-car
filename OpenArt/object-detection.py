import os, tf
from pyb import LED
from machine import UART
import utime
import seekfree
import math
import sensor, image, time

uart = UART(1, baudrate=115200) #串口



def openart_init():

    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_brightness(1500)
    sensor.skip_frames(20)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(True,(0,0,0))

def object_detection(net, face_detect):
    img = sensor.snapshot()
    #使用模型进行识别
    for obj in tf.detect(net,img):
        x1,y1,x2,y2,label,scores = obj

        if(scores>0.50):
            w = x2- x1
            h = y2 - y1
            x1 = int((x1-0.1)*img.width())
            y1 = int(y1*img.height())
            w = int(w*img.width())
            h = int(h*img.height())

            angle = -int(math.atan((x1+w//2- 160) / (y1+h//2 - 240)) * 60)
            dis = int(math.sqrt(((x1+w//2 - 160) ** 2) + ((y1+h//2 - 240)** 2)))

            img.draw_string(x1+w//2, y1+h//2, "Angle:" + str(angle), color = (255, 0, 0), scale = 2, mono_space = False)
            img.draw_string(x1+w//2, y1+h//2 - 30, "Dis:" + str(dis), color = (255, 0, 0), scale = 2, mono_space = False)
            img.draw_cross(x1+w//2, y1+h//2, size=5, color=(255, 0, 0))
            img.draw_line(160, 240, x1+w//2, y1+h//2, color=(255, 0, 0))
            img.draw_rectangle((x1,y1,w,h),thickness=2)

            uart.write("C")
            uart.write("%c" % dis)
            uart.write("%c" % angle)
            uart.write("D")

            print("dis:%d, angle:%d" % (dis, angle))

def main():
    openart_init()

    face_detect = '/sd/yolo3_iou_smartcar_final_with_post_processing.tflite'
    net = tf.load(face_detect)

    while(True):
        img = sensor.snapshot()
        object_detection(net, face_detect)

if __name__ == '__main__':
    main()

