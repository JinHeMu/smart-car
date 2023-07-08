import os, tf
from pyb import LED
from machine import UART
import utime
import seekfree
import math
import sensor, image, time

global pos_perx
line = None
pos_perx = []
find_coordinates_flag = 1
correct_flag = 1
recognize_flag = 1
uart_num = 0





#白天阈值
card_threshold = [(47, 100, -4, 68, -17, 29)]#色块检测阈值
boundary_threshold = [(45, 71, -20, 15, 23, 127)]#边线检测阈值
boundary_column_threshold = [(45, 71, -20, 15, 23, 127)]#边线检测阈值
boundary_row_threshold = [(45, 71, -20, 15, 23, 127)]#边线检测阈值
day_brightness = 300
binary_threshold = (183, 255)
#LED(4).on()#照明

#  #晚上阈值
# card_threshold = [(48, 87, -33, 33, -2, 80)]#色块检测阈值
# boundary_threshold = [(46, 71, -28, 9, 47, 84)]#边线检测阈值
# boundary_column_threshold = [(46, 71, -28, 9, 47, 84)]#边线检测阈值
# boundary_row_threshold = [(46, 71, -28, 9, 47, 84)]#边线检测阈值
# day_brightness = 1000
# LED(4).on()#照明
# binary_threshold = (154, 255)

uart = UART(2, baudrate=115200) #串口

lcd = seekfree.LCD180(2)#显示屏
lcd.full()







#openart初始化
def openart_init():

    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_brightness(day_brightness)
    sensor.skip_frames(20)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(False,(0,0,0))

#寻找A4纸坐标，并发送A4纸坐标
def find_coordinates():
    global pos_perx

    sensor.reset()
    sensor.set_pixformat(sensor.GRAYSCALE)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_brightness(day_brightness)
    sensor.skip_frames(20)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(False,(0,0,0))


    find_coordinates_flag = 1
    global uart_num

    while find_coordinates_flag and (uart_num==0):
        pos_nowx = []
        pos_y = []
        uart_num = uart.any()  # 鑾峰彇褰撳墠涓插彛鏁版嵁鏁伴噺
        img = sensor.snapshot()
        img.binary([binary_threshold])
        img.erode(2)
        img.dilate(2)
        for r in img.find_rects(threshold=3000):
            #寻找矩形
            if r.w() > 120 and r.w() < 180 and r.h() > 70 and r.h() < 130:
                #获取角点，并进行变换
                corners = r.corners()
                img1 = img.rotation_corr(0,0,0,0,0,1,60,corners).replace(vflip=True,  hmirror=False,  transpose=False)
                #寻找圆点
                for c in img1.find_circles((5,5,300,220),x_stride = 2, y_stride = 1,threshold=3000, x_margin=10, y_margin=10, r_margin=5, r_min=1,
                                        r_max=5, r_step=1):
                    img1.draw_circle(c.x(), c.y(), c.r(), color=(255, 0, 0))
                    #计算坐标点
                    posx = (float(c.x())) / 315 * 35 + 1
                    posy = 25 - ((float(c.y())-1) / 235 * 25) + 1

                    pos_nowx.append(int(posx)) #将坐标点添加至数组
                    pos_y.append(int(posy))

                    img1.draw_string(c.x() - 5, c.y() - 10, "(" + str(int(posx)) + "," + str(int(posy)) + ")",
                                    color=(0, 0, 255), scale=1, mono_space=False)
                    lcd.show_image(img1, 320, 240, zoom=2)

                result_x = sorted(pos_nowx) #重新排列当前数组
                if(result_x == pos_perx ): #如果当前数组等于上一次，则发送数据
                    uart.write("A")  # 发送包头
                    for i in range(len(pos_nowx)):
                        uart.write("%c" % (pos_nowx[i]))
                        time.sleep_ms(10)

                    for i in range(len(pos_y)):
                        uart.write("%c" % (pos_y[i]))
                        time.sleep_ms(10)

                    uart.write("Y")  # 发送包尾
                        # 串口发送成功标志位
                    print(pos_nowx)
                    print(pos_y)
                    print("OK")
                    find_coordinates_flag = 0
                    break
                else:
                    pos_perx = result_x
                    lcd.show_image(img, 320, 240, zoom=2)



#图片矫正函数
def picture_correct():

    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QQVGA)
    sensor.set_brightness(day_brightness)
    sensor.skip_frames(20)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(False,(0,0,0))

    dis_X = 0
    dis_Y = 0
    distance = 0
    correct_flag = 1

    global uart_num

    while correct_flag:

        uart_num = uart.any()  # 鑾峰彇褰撳墠涓插彛鏁版嵁鏁伴噺
        img = sensor.snapshot()

        if(uart_num != 0):
            correct_flag = 0
            break
        else:
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

                q = int(q * 60)


                img.draw_circle(b.cx(), b.cy(), 5, color=(0, 255, 0))
                dis_X = b.cx() - 91
                dis_Y = b.cy() - 70
                distance = math.sqrt((dis_X ** 2) + (dis_Y ** 2))
                print("disx:%d , disy:%d, angle:%d" % (dis_X, dis_Y, q))

                #发送数据
                uart.write("C")
                uart.write("%c" % dis_X)
                uart.write("%c" % dis_Y)
                if distance < 5:
                    uart.write("%c" % 1)
                else:
                    uart.write("%c" % 0)
                uart.write("D")
                img.draw_string(10,10,"correct", (255,0,0))
                lcd.show_image(img, 160, 120, zoom=0)

                if distance < 5:
                    correct_flag = 0



#边线矫正函数
def boundary_correct(mode):

    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_brightness(day_brightness)
    sensor.skip_frames(20)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(False,(0,0,0))

    boundary_correct_flag = 1
    boundary_uart_flag = 0
    global uart_num
    angle_per = 0

    if mode == 'row':
        num = [0,45,90,185,230,275]
    if mode == 'column':
        num = [0,35,70,105,140,175]

    while boundary_correct_flag :
        uart_num = uart.any()  # 鑾峰彇褰撳墠涓插彛鏁版嵁鏁伴噺
        img = sensor.snapshot()
        blobs = []
        center = 0

        if(uart_num!=0):
            boundary_correct_flag=0
            break
        else:
            for x in num:
                if mode == 'row':
                    result = img.find_blobs(boundary_row_threshold, roi = [x,0,45,240] ,pixels_threshold=400, area_threshold=400, margin=1, merge=True, invert=0)
                if mode == 'column':
                    result = img.find_blobs(boundary_column_threshold, roi = [0,x,320,35] ,pixels_threshold=400, area_threshold=400, margin=1, merge=True, invert=0)
                if result:
                    result = min(result, key= lambda b: abs(b.area() - 1250))
                    blobs.append(result)
                    center += 1
                    img.draw_rectangle(result.rect(), color = (255, 0, 0), scale = 1, thickness = 2)
                else:
                    break
            if center > 3:
                    l = img.get_regression(boundary_threshold)
                    if l:
                        img.draw_line(l.line(), color = (255, 0, 0)) # 在图像上标出线段
                        print(l.theta())
                        print(l.line())
                        angle = l.theta()
                        if(mode == 'row'):
                            angle = -(angle - 90)
                        if(mode == 'column'):
                            if l.x2() < 160:
                                if(angle < 140):
                                    angle = -angle

                                elif (angle > 140):
                                    angle = 180 - angle

                            elif l.x2() > 160:
                                if(angle > 90):
                                    angle =180 - angle

                                elif (angle < 90):
                                    angle = - angle

                        boundary_uart_flag = 1
                        angle_per = angle
                        if(angle_per == angle):
                            uart.write("B")
                            uart.write("%c" % angle)
                            uart.write("%c" % boundary_uart_flag)
                            uart.write("Y")
                            print("now:angle:%d",angle)
                            img.draw_string(10,10,"boundary", (255,0,0))
                            boundary_correct_flag = 0
                            break
                        else:
                            break
                    else:
                        # uart.write("%c" % 0)
                        # print("Line Angle: ", angle)
                        img.draw_string(10,10,"boundary", (255,0,0))
                        lcd.show_image(img, 320, 240, zoom=1)



#识别卡片
def recognize_pic(labels, net):

    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_brightness(day_brightness)
    sensor.skip_frames(20)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(False,(0,0,0))

    global uart_num
    recognize_flag = 1
    class_per = []

    while recognize_flag:
        img = sensor.snapshot()
        uart_num = uart.any()  # 鑾峰彇褰撳墠涓插彛鏁版嵁鏁伴噺
        if(uart_num!=0):
            recognize_flag=0
            break
        else:
            for b in img.find_blobs(card_threshold,pixels_threshold=400,area_threshold=400,margin= 1,merge=True,invert=0):
                img.draw_rectangle(b.rect(), color = (255, 0, 0), scale = 1, thickness = 2)
                x, y, w, h = b.rect()
                # 缩小矩形框的宽度和高度
                new_w = w
                new_h = h
                # 计算新的矩形框左上角坐标
                new_x = x + (w - new_w) // 2
                new_y = y + (h - new_h) // 2

                img.draw_rectangle((new_x, new_y, new_w, new_h), color=(255, 0, 0), scale=1, thickness=2)

                img1 = img.copy(1, 1, (new_x, new_y, new_w, new_h))

                for obj in tf.classify(net, img1, min_scale=1.0, scale_mul=0.5, x_overlap=0.0, y_overlap=0.0):
                    #print("**********\nTop 1 Detections at [x=%d,y=%d,w=%d,h=%d]" % obj.rect())
                    sorted_list = sorted(zip(labels, obj.output()), key=lambda x: x[1], reverse=True)
                    # 鎵撳嵃鍑嗙‘鐜囨渶楂樼殑缁撴灉
                    for i in range(1):
                        class_per = sorted_list[i][0]
                        if(class_per == sorted_list[i][0]):
                            print("new:%s = %f" % (sorted_list[i][0], sorted_list[i][1]))
                            uart.write("I")
                            uart.write(sorted_list[i][0])
                            uart.write("J")
                            img.draw_string(10,10,"%s:%d" % (sorted_list[i][0],sorted_list[i][1]*100) , (255,0,0), 2)
                            lcd.show_image(img, 320, 240, zoom=2)
                            recognize_flag = 0
                        else:
                            img.draw_string(10,10,"recognize", (255,0,0))
                            lcd.show_image(img, 320, 240, zoom=2)







def main():
    openart_init()
    net_path = "7-8-epoch400.tflite"                                  # 瀹氫箟妯″瀷鐨勮矾寰
    labels = [line.rstrip() for line in open("/sd/labels.txt")]   # 鍔犺浇鏍囩
    net = tf.load(net_path, load_to_fb=True)                                  # 鍔犺浇妯″瀷


    while(True):
        img = sensor.snapshot()
        #find_coordinates()
        #recognize_pic(labels, net)
          ##boundary_correct('column')
        #picture_correct()
        uart_num = uart.any()  # 鑾峰彇褰撳墠涓插彛鏁版嵁鏁伴噺
        if (uart_num):
            uart_str = uart.read(uart_num).strip()  # 璇诲彇涓插彛鏁版嵁
            #print(uart_str.decode())
            if(uart_str.decode() == "A"):
                print("A")
                uart_num=0
                find_coordinates()

            elif(uart_str.decode() == "B"):
                print("B")
                uart_num=0
                picture_correct()

            elif(uart_str.decode() == "C"):
                print("C")
                uart_num=0
                recognize_pic(labels, net)

            elif(uart_str.decode() == "D"):
                print("D")
                uart_num=0
                boundary_correct('column')

            elif(uart_str.decode() == "E"):
                print("E")
                uart_num=0
                boundary_correct('row')

        else:
            lcd.show_image(img, 320, 240, zoom=2)





if __name__ == '__main__':
    main()

