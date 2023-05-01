# 导入需要的库
import sensor, image, time, math
import os, tf
from pyb import LED
from machine import UART
import utime
import seekfree

global pos_perx,uart_flag
pos_perx = []
uart_flag = 0
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
    dis_X = 0
    dis_Y = 0
    distance = 0



    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QQVGA)
    sensor.set_brightness(1000)
    sensor.skip_frames(20)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(True,(0,0,0))
    while True:
        img = sensor.snapshot()

        for b in img.find_blobs(threshold,pixels_threshold=400,area_threshold=400,margin= 1,merge=True,invert=0):

            corners = b.corners()

            #img1 = img.rotation_corr(0,0,0,0,0,1,60,corners).replace(vflip=True,  hmirror=False,  transpose=False)
            img.draw_circle(corners[0][0],corners[0][1], 5, color=(0, 255, 0))
            img.draw_circle(corners[1][0],corners[1][1], 5, color=(0, 255, 0))
            img.draw_circle(corners[2][0],corners[2][1], 5, color=(0, 255, 0))
            img.draw_circle(corners[3][0],corners[3][1], 5, color=(0, 255, 0))

            x1, y1 = corners[0]
            x2, y2 = corners[1]
            x3, y3 = corners[2]
            x4, y4 = corners[3]

            dis_x_1 = x1-x3

            dis_x_4 = x4-x2

            dis_y_1 = y1-y3

            dis_y_4 = y4-y2

            if(dis_x_1 > 0 ):
                a1 = math.atan2(dis_y_1,dis_x_1)
            else:
                a1 = math.atan2(dis_x_1,dis_y_1)

            if(dis_x_4 > 0):
                a4 = math.atan2(dis_x_4,dis_y_4)
                angle4 = a4/math.pi*180 -45
            else:
                a4 =math.atan2(dis_y_4,dis_x_4)
                angle4 = (90 - (a4/math.pi*180 -45))



            angle1 = a1/math.pi*180 - 180


            #averger  = (angle1 + angle2 + angle3 + angle4)/4 - 180
            print("angle1 : %d  angle4 : %d\n" % (angle1, angle4))
            #print(averger)


            # 将夹角转换为旋转角度





            img.draw_rectangle(b.rect(), color = (255, 0, 0), scale = 1, thickness = 2)
            img_x = (int)(b.rect()[0] + b.rect()[2] / 2)  # 图像中心的x值b
            img_y = (int)(b.rect()[1] + b.rect()[3] / 2)  # 图像中心的y值
            img.draw_circle(img_x, img_y, 5, color=(0, 255, 0))  # 给矩形中心绘制一个小圆 便于观察矩形中心是否识别正确
            lcd.show_image(img, 160, 120, zoom=0)
            img = sensor.snapshot()







def main():
    openart_init()
    while(True):
        img = sensor.snapshot()
        uart_correct(img)






if __name__ == '__main__':
    main()
