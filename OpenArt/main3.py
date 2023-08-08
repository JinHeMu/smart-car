import os, tf
from pyb import LED
from machine import UART
import utime
import seekfree
import math
import sensor, image, time


uart = UART(2, baudrate=115200) # 串口

## 3：屏幕显示方向 参数范围0-3
lcd = seekfree.IPS200(2)
lcd.full()  # 将背景颜色显示到整个屏幕

day_brightness = 2000

camera_center = (160, 240)

# 定义一个变量来保存上一次卡片的中心点位置
last_card_center = None
# 定义一个阈值，当两张卡片之间的距离大于此值时，认为是新的卡片
distance_threshold = 20

x_roi =120
y_roi =0
x1_roi =200
y1_roi =240

#设置模型路径
face_detect = '/sd/yolo3_iou_smartcar_final_with_post_processing.tflite'
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
        img.draw_line(120, 0, 120, 240, color=(255, 0, 0))
        img.draw_line(200, 0, 200, 240, color=(255, 0, 0))
        closest_card = None
        closest_distance = float("inf") # Set to infinity initially

        if uart_num != 0:
            detect_flag = 0
            break
        else:
            for obj in tf.detect(net,img):
                x1,y1,x2,y2,label,scores = obj

                if(scores>0.70):
                    w = x2- x1
                    h = y2 - y1
                    x1 = int((x1-0.1)*img.width())
                    y1 = int(y1*img.height())
                    w = int(w*img.width())
                    h = int(h*img.height())

                    if x1+w//2 >= 120 and x1+w//2 <= 200:
                        current_card_center = ((x1 + x2) / 2, (y1 + y2) / 2)
                        current_distance = ((current_card_center[0] - camera_center[0]) ** 2 + (current_card_center[1] - camera_center[1]) ** 2) ** 0.5

                        # If this card is closer to the camera center than the currently saved card, save it
                        if current_distance < closest_distance:
                            closest_card = (x1, y1, w, h)
                            closest_distance = current_distance
                    else:
                        continue

            # Draw rectangle for the closest card after checking all cards
            if closest_card is not None:
                x1, y1, w, h = closest_card
                img.draw_rectangle((x1, y1, w, h), thickness=2)
                        # 如果没有上一张卡片或者当前卡片与上一张卡片的距离大于阈值，那么认为这是一张新的卡片
                if last_card_center is None or ((current_card_center[0] - last_card_center[0]) ** 2 + (current_card_center[1] - last_card_center[1]) ** 2) ** 0.5 > distance_threshold:
                    print("New card detected")
                    # 在卡片右上角画出"new"
                    img.draw_string(int(x1), int(y1), "New", color=(255,0,0), scale=2)
                    img.draw_string(int(x1+60), int(y1), str(int(240-y1)), color=(0,0,255), scale=2)
                    uart.write("C")
                    uart.write("%c" % int(240-y1))  # 找到卡片发送1
                    uart.write("%c" % 1)  # 找到卡片发送
                    uart.write("D")
                else:
                    # 在卡片右上角画出"old"
                    img.draw_string(int(x1), int(y1), "Old", color=(0,255,0), scale=2)
                    img.draw_string(int(x1+60), int(y1), str(int(240-y1)), color=(0,0,255), scale=2)
                # 更新上一张卡片的中心点位置
                last_card_center = current_card_center
                lcd.show_image(img, 320, 240, zoom=0)












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
        else:
            lcd.show_image(img, 320, 240, zoom=2)

if __name__ == '__main__':
    main()
