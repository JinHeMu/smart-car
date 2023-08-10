import os, tf
from pyb import LED
from machine import UART
import utime
import seekfree
import math
import sensor, image, time


uart = UART(2, baudrate=115200) # 串口

# 3：屏幕显示方向 参数范围0-3
lcd = seekfree.IPS200(2)
lcd.full()  # 将背景颜色显示到整个屏幕

day_brightness = 2000


#qqvga 160 120
camera_center = (160, 120)

x_roi_min =70
x_roi_max =90

y_roi_min =0
y_roi_max =120


roi = (70,0,80,120)

#设置模型路径
face_detect = '/sd/yolo3_iou_smartcar_final_with_post_processing.tflite'
#载入模型
net = tf.load(face_detect)

def openart_init():
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QQVGA)
    sensor.set_brightness(day_brightness)
    sensor.skip_frames(20)
    sensor.set_windowing(roi)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(False, (0, 0, 0))


def object_detect():

    detect_flag = 1

    while detect_flag:
        uart_num = uart.any()  # 获取当前串口数据数量
        img = sensor.snapshot()

        roi = (x_roi_min, y_roi_min, x_roi_max - x_roi_min, y_roi_max - y_roi_min)

        img.draw_rectangle(roi, thickness=1,color = (255, 0, 0))

        #cropped_img = img.copy(roi)

        if uart_num != 0:
            detect_flag = 0
            break
        else:
            for obj in tf.detect(net,img):
                x1,y1,x2,y2,label,scores = obj

                if(scores>0.80):
                    w = x2- x1
                    h = y2 - y1
                    x1 = int((x1-0.1)*img.width())
                    y1 = int(y1*img.height())
                    w = int(w*img.width())
                    h = int(h*img.height())

                    card_dis_x = x1+w//2
                    card_dis_y = y1+h//2

                    if card_dis_x >= x_roi_min and card_dis_x <= x_roi_max and card_dis_y >=y_roi_min:
                        img.draw_rectangle((x1, y1, w, h), color = (0, 255, 0),thickness=2)
                        print(int(120-y1))
                        uart.write("C")
                        uart.write("%c" % int(120-y1))  # 找到卡片发送1
                        uart.write("%c" % 1)  # 找到卡片发送
                        uart.write("D")
                        #lcd.show_image(img, 160, 120, zoom=0)
                    else:
                        img.draw_rectangle((x1, y1, w, h), thickness=2,color = (255, 0, 0))
                        #lcd.show_image(img, 160, 120, zoom=0)

def main():
    openart_init()

    while True:
        img = sensor.snapshot()

        object_detect()
        uart_num = uart.any()  # 鑾峰彇褰撳墠涓插彛鏁版嵁鏁伴噺
        if uart_num:
            uart_str = uart.read(uart_num).strip()  # 璇诲彇涓插彛鏁版嵁
            print(uart_str.decode())
            if(uart_str.decode() == "A"):
                print("A")
                uart_num=0
                object_detect()
            #lcd.show_image(img, 160, 120, zoom=2)

if __name__ == '__main__':
    main()
