# 导入需要的库
import sensor, image, time, math
import os, tf
from pyb import LED
from machine import UART
import utime
import seekfree


line = None

boundary_threshold = [(70, 100, -128, 127, 10, 127)]#边线检测阈值

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


#先找色块再进行直线矫正判断
#def boundary_correct():

    #sensor.reset()
    #sensor.set_pixformat(sensor.RGB565)
    #sensor.set_framesize(sensor.QQVGA)
    #sensor.set_brightness(1000)
    #sensor.skip_frames(20)
    #sensor.set_auto_gain(False)
    #sensor.set_auto_whitebal(True,(0,0,0))
    #img = sensor.snapshot()

    #boundary_correct_flag = 1

    #while boundary_correct_flag:

        #img = sensor.snapshot()

        #for b in img.find_blobs(boundary_threshold, pixels_threshold=400, area_threshold=400, margin=1, merge=True, invert=0):
            ##l = img.get_regression(boundary_threshold)
            #img.draw_rectangle(b.rect(), color = (255, 0, 0), scale = 1, thickness = 2)
        ##     if l:
        ##         img.draw_line(l.line(), color = (255, 0, 0)) # 在图像上标出线段
        ##         if (l.theta()>150):
        ##             img.draw_line(l.line(), color = (255, 0, 0)) # 在图像上标出线段
        ##             angle = l.theta() # 计算线段的角度
        ##             if(angle > 150):
        ##                 angle = -(180 - angle)
        ##             uart.write("%c" % angle)
        ##             print("Line Angle: ", angle)
        ##             boundary_correct_flag = 0
        ##         elif(l.theta() > 0 and l.theta() < 45) :
        ##             angle = l.theta() # 计算线段的角度
        ##             uart.write("%c" % angle)
        ##             print("Line Angle: ", angle)
        ##             boundary_correct_flag = 0

        ##         elif(l.theta() > 45 and l.theta() < 135) :
        ##             angle = l.theta() # 计算线段的角度
        ##             angle = -(90 - angle)
        ##             uart.write("%c" % angle)
        ##             print("Line Angle: ", angle)
        ##             boundary_correct_flag = 0



        ## else:
        ##     # uart.write("%c" % 0)
        ##     # print("Line Angle: ", angle)
        ##     lcd.show_image(img, 320, 240, zoom=2)


def boundary_correct(mode):
    blobs = []
    center = 0

    img = sensor.snapshot()

    if mode == 'row':
        num = [0,45,90,185,230,275]
    if mode == 'column':
        num = [0,35,70,105,140,175]
    for x in num:
        if mode == 'row':
            result = img.find_blobs(boundary_threshold, roi = [x,0,45,240] ,pixels_threshold=400, area_threshold=400, margin=1, merge=True, invert=0)
        if mode == 'column':
            result = img.find_blobs(boundary_threshold, roi = [0,x,320,35] ,pixels_threshold=400, area_threshold=400, margin=1, merge=True, invert=0)
        if result:
            result = min(result, key= lambda b: abs(b.area() - 1250))
            blobs.append(result)
            center += 1
            img.draw_rectangle(result.rect(), color = (255, 0, 0), scale = 1, thickness = 2)
        else:
            break
    if center >= 4:
            l = img.get_regression(boundary_threshold)
            if l:
                img.draw_line(l.line(), color = (255, 0, 0)) # 在图像上标出线段
                if (l.theta()>150):
                    img.draw_line(l.line(), color = (255, 0, 0)) # 在图像上标出线段
                    angle = l.theta() # 计算线段的角度
                    if(angle > 150):
                        angle = -(180 - angle)
                    uart.write("%c" % angle)
                    print("Line Angle: ", angle)
                    boundary_correct_flag = 0
                elif(l.theta() > 0 and l.theta() < 45) :
                    angle = l.theta() # 计算线段的角度
                    uart.write("%c" % angle)
                    print("Line Angle: ", angle)
                    boundary_correct_flag = 0

                elif(l.theta() > 45 and l.theta() < 135) :
                    angle = l.theta() # 计算线段的角度
                    angle = -(90 - angle)
                    uart.write("%c" % angle)
                    print("Line Angle: ", angle)
                    boundary_correct_flag = 0

            else:
                # uart.write("%c" % 0)
                # print("Line Angle: ", angle)
                lcd.show_image(img, 320, 240, zoom=2)

    print(center)






def main():
    openart_init()
    while(True):
        img = sensor.snapshot()
        #boundary_correct('column')
        boundary_correct('row')






if __name__ == '__main__':
    main()



