import os, tf
from pyb import LED
from machine import UART
import utime
import seekfree
import math
import sensor, image, time


uart = UART(2, baudrate=115200) # 串口

## 3：屏幕显示方向 参数范围0-3
lcd = seekfree.LCD180(2)#显示屏
lcd.full()

day_brightness = 1500

camera_center = (160, 240)

# 定义一个变量来保存上一次卡片的中心点位置
last_card_center = None
# 定义一个阈值，当两张卡片之间的距离大于此值时，认为是新的卡片+
distance_threshold = 20



#128 21
#195 21
#274 240
#60 240
x_roi_min =100
x_roi_max =220

y_roi_min =20
y_roi_max =240


points = [(128, 21), (195, 21), (274, 240), (60, 240)]
roi = (x_roi_min, y_roi_min, x_roi_max-x_roi_min,y_roi_max-y_roi_min)

#设置模型路径
face_detect = '/sd/2yolo3_iou_smartcar_final_with_post_processing.tflite'
#载入模型
net = tf.load(face_detect)

def openart_init():
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_brightness(day_brightness)
    sensor.skip_frames(20)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(False, (0, 0, 0))


def object_detect():
    global last_card_center
    detect_flag = 1

    while detect_flag:
        uart_num = uart.any()  # 获取当前串口数据数量
        img = sensor.snapshot()
        #roi = (x_roi_min, y_roi_min, x_roi_max - x_roi_min, y_roi_max - y_roi_min)
        #img.draw_rectangle(roi, thickness=1,color = (0, 0, 255))

        for i in range(len(points)):
            start_point = points[i]
            end_point = points[(i + 1) % len(points)]  # 获取下一个点，确保连接最后一个点到第一个点
            img.draw_line(start_point[0], start_point[1], end_point[0], end_point[1], color=(0, 0, 0), thickness=2)

        #img.draw_line(points[i], points[i + 1], color=(0, 0, 0), thickness=2)

        if uart_num != 0:
            detect_flag = 0
            break
        else:
            for obj in tf.detect(net,img):


                x1,y1,x2,y2,label,scores = obj

                print(obj)

                if(scores>0.2):
                    w = x2 - x1
                    h = y2 - y1
                    x1 = int((x1-0.1)*img.width())
                    y1 = int(y1*img.height())
                    w = int(w*img.width())
                    h = int(h*img.height())

                    card_dis_x = x1
                    card_dis_y = y1 + h // 2

                    img.draw_rectangle((x1, y1, w, h), color = (255, 0, 0),thickness=2)


                    if card_dis_y >= 165:
                        if card_dis_x >=84 and card_dis_x <= 248:
                            uart_dis(x1, y1, w, h,img,card_dis_y)

                    elif card_dis_y >= 114 and card_dis_y < 165:
                        if card_dis_x >=98 and card_dis_x <= 228:
                            uart_dis(x1, y1, w, h,img,card_dis_y)

                    elif card_dis_y >= 87 and card_dis_y < 114:
                        if card_dis_x >=108 and card_dis_x <= 219:
                            uart_dis(x1, y1, w, h,img,card_dis_y)

                    elif card_dis_y >= 69 and card_dis_y < 87:
                        if card_dis_x >=114 and card_dis_x <= 212:
                            uart_dis(x1, y1, w, h,img,card_dis_y)

                    elif card_dis_y >= 51 and card_dis_y < 69:
                        if card_dis_x >=119 and card_dis_x <= 206:
                            uart_dis(x1, y1, w, h,img,card_dis_y)
                    elif card_dis_y <= 51:
                        if card_dis_x >=124 and card_dis_x <= 201:
                            uart_dis(x1, y1, w, h,img,card_dis_y)

                    # if card_dis_x >= x_roi_min and card_dis_x <= x_roi_max and card_dis_y >=y_roi_min:

                    #     # detect_flag = 0
                else:
                    lcd.show_image(img, 320, 240, zoom=2)


def uart_dis(x1, y1, w, h,img,card_dis_y):
    img.draw_rectangle((x1, y1, w, h), color = (0, 255, 0),thickness=2)
    lcd.show_image(img, 320, 240, zoom=2)
    #print(int(240-y1))
    uart.write("C")
    uart.write("%c" % int(240-card_dis_y))  # 找到卡片发送1
    uart.write("%c" % 1)  # 找到卡片发送
    uart.write("D")

    print(240-card_dis_y)

    img.draw_rectangle((x1, y1, w, h), thickness=2,color = (0, 255, 0))
    lcd.show_image(img, 320, 240, zoom=2)

    utime.sleep_ms(100)






def main():
    openart_init()
    #object_detect()



    while True:
        img = sensor.snapshot()

        #object_detect()
        uart_num = uart.any()  # 鑾峰彇褰撳墠涓插彛鏁版嵁鏁伴噺
        if uart_num:
            uart_str = uart.read(uart_num).strip()  # 璇诲彇涓插彛鏁版嵁
            print(uart_str.decode())
            if(uart_str.decode() == "A"):
                print("A")
                uart_num=0
                object_detect()
        else:
            lcd.show_image(img, 320, 240, zoom=2)


if __name__ == '__main__':
    main()
