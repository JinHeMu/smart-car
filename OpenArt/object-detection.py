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
    sensor.set_brightness(500)
    sensor.skip_frames(20)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(True,(0,0,0))

def object_detection(net, face_detect):
    img = sensor.snapshot()

    closest_dis = float('inf')  # Initialize with a very large distance
    closest_obj = None

    # 使用模型进行识别
    for obj in tf.detect(net, img):
        x1, y1, x2, y2, label, scores = obj

        if scores > 0.50:
            w = x2 - x1
            h = y2 - y1
            x1 = int((x1 - 0.1) * img.width())
            y1 = int(y1 * img.height())
            w = int(w * img.width())
            h = int(h * img.height())
            center_x = x1 + w // 2
            center_y = y1 + h // 2

            dis_center_x = center_x - 160
            dis_center_y = center_y - 240

            if dis_center_x < -128:
                dis_center_x = -128
            if dis_center_x > 128:
                dis_center_x = 128

            if dis_center_y < -128:
                dis_center_y = -128
            if dis_center_y > 128:
                dis_center_y = 128

            angle = -int(math.atan((center_x - 160) / (center_y - 240)) * 60)
            dis = int(math.sqrt(((center_x - 160) ** 2) + ((center_y - 240) ** 2)))

            if dis < closest_dis:
                closest_dis = dis
                closest_obj = obj

    if closest_obj is not None:
        x1, y1, x2, y2, label, scores = closest_obj
        w = x2 - x1
        h = y2 - y1
        x1 = int((x1 - 0.1) * img.width())
        y1 = int(y1 * img.height())
        img.draw_cross(x1, y1, size=5, color=(255, 0, 0))
        w = int(w * img.width())
        h = int(h * img.height())
        center_x = x1 + w // 2
        center_y = y1 + h // 2

        dis_center_x = center_x - 160
        dis_center_y = center_y - 240

        if dis_center_x < -128:
            dis_center_x = -128
        if dis_center_x > 128:
            dis_center_x = 128

        if dis_center_y < -128:
            dis_center_y = -128
        if dis_center_y > 128:
            dis_center_y = 128

        img.draw_string(center_x, center_y, "Angle:" + str(angle), color=(255, 0, 0), scale=2, mono_space=False)
        img.draw_string(center_x, center_y - 30, "Dis:" + str(dis), color=(255, 0, 0), scale=2, mono_space=False)
        img.draw_cross(center_x, center_y, size=5, color=(255, 0, 0))
        img.draw_line(160, 240, center_x, center_y, color=(255, 0, 0))
        img.draw_rectangle((x1, y1, w, h), thickness=2)

        uart.write("C")
        uart.write("%c" % dis_center_x)
        uart.write("%c" % -dis_center_y)
        uart.write("D")

        print("dis:%d, angle:%d, center_x:%d, center_y:%d" % (dis, angle, center_x - 160, dis_center_y))

def main():
    openart_init()

    face_detect = '/sd/yolo3_iou_smartcar_final_with_post_processing.tflite'
    net = tf.load(face_detect)

    while True:
        img = sensor.snapshot()
        object_detection(net, face_detect)

if __name__ == '__main__':
    main()
