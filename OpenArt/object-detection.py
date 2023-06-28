import os, tf
from pyb import LED
from machine import UART
import utime
import seekfree
import math
import sensor, image, time

uart = UART(1, baudrate=115200) #串口

card_threshold = [(69, 92, -30, 24, -15, 77)]#色块检测阈值

def openart_init():

    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_brightness(1500)
    sensor.skip_frames(20)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(True,(0,0,0))


def picture_correct():

    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QQVGA)
    sensor.set_brightness(1000)
    sensor.skip_frames(20)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(True,(0,0,0))

    dis_X = 0
    dis_Y = 0
    distance = 0
    correct_flag = 1

    while correct_flag:
        img = sensor.snapshot()

        for b in img.find_blobs(card_threshold, pixels_threshold=400, area_threshold=400, margin=1, merge=True, invert=0):
            #寻找角点
            corners = b.min_corners()
            point_corners = tuple(sorted(corners))
            for corner in corners:
                img.draw_circle(corner[0], corner[1], 5, color=(0, 255, 0))
            x0, y0 = point_corners[3]
            x1, y1 = point_corners[2]
            x3, y3 = point_corners[0]

            len1 = (x0 - x1) ** 2 + (y0 - y1) ** 2
            len2 = (x1 - x3) ** 2 + (y1 - y3) ** 2

            if x1 == x0:
                q = 0
            else:
                q = math.atan((x1 - x0) / (y1 - y0))
                #print(x1 - x0, y1 - y0)

            # if len1 > len2:
            #     if x1 == x0:
            #         q = 0.5 * math.pi
            #     else:
            #         q = math.atan((y1 - y0) / (x1 - x0))
            # else:
            #     if x1 == x3:
            #         q = 0.5 * math.pi
            #     else:
            #         q = math.atan((y3 - y1) / (x3 - x1))

            q = int(q * 60)
            #print("angle:%d" % q)


            img.draw_circle(b.cx(), b.cy(), 5, color=(0, 255, 0))
            dis_X = b.cx() - 91
            dis_Y = b.cy() - 70
            print("disx:%d , disy:%d, angle:%d" % (dis_X, dis_Y, q))

            #发送数据
            uart.write("C")
            uart.write("%c" % dis_X)
            uart.write("%c" % dis_Y)
            uart.write("%c" % q)
            uart.write("D")
            distance = math.sqrt((dis_X ** 2) + (dis_Y ** 2))
            if distance < 5:
                correct_flag = 0


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
            img.draw_cross(x1 ,y1, size=5, color=(255, 0, 0))
            w = int(w*img.width())
            h = int(h*img.height())
            center_x = x1+w//2
            center_y = y1+h//2

            dis_center_x = center_x - 160
            dis_center_y = center_y - 240

            if(dis_center_x < -128):dis_center_x=-128
            if(dis_center_x > 128):dis_center_x=128

            if(dis_center_y < -128):dis_center_y=-128
            if(dis_center_y > 128):dis_center_y=128

            angle =-int(math.atan((center_x - 160) / (center_y - 240)) * 60)
            dis = int(math.sqrt(((center_x - 160) ** 2) + ((center_y - 240)** 2)))

            img.draw_string(center_x, center_y, "Angle:" + str(angle), color = (255, 0, 0), scale = 2, mono_space = False)
            img.draw_string(center_x, center_y - 30, "Dis:" + str(dis), color = (255, 0, 0), scale = 2, mono_space = False)
            img.draw_cross(center_x, center_y, size=5, color=(255, 0, 0))
            img.draw_line(160, 240, center_x, center_y, color=(255, 0, 0))
            img.draw_rectangle((x1,y1,w,h),thickness=2)

            uart.write("C")
            #uart.write("%c" % dis_center_x)
            #uart.write("%c" % dis_center_y)
            uart.write("%c" % dis_center_x)
            uart.write("%c" % -dis_center_y)
            uart.write("D")

            print("dis:%d, angle:%d, center_x:%d, center_y:%d" % (dis, angle, center_x-160, dis_center_y))

def main():
    openart_init()

    face_detect = '/sd/yolo3_iou_smartcar_final_with_post_processing.tflite'
    net = tf.load(face_detect)

    while(True):
        img = sensor.snapshot()
        #picture_correct()
        object_detection(net, face_detect)

if __name__ == '__main__':
    main()

