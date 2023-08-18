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





##白天阈值
#card_threshold = [(62, 100, -42, 23, -19, 73)]#色块检测阈值
#boundary_threshold = [(71, 100, -29, 32, 51, 119)]#边线检测阈值
#boundary_column_threshold = [(71, 100, -29, 32, 51, 119)]#边线检测阈值
#boundary_row_threshold = [(71, 100, -29, 32, 51, 119)]#边线检测阈值
#day_brightness = 300
#binary_threshold = (183, 255)


#晚上阈值（需要调试）
card_threshold = [(54, 100, -128, 127, -128, 46)]#色块检测阈值
boundary_threshold = [(58, 100, -128, 127, 15, 127)]#边线检测阈值
boundary_column_threshold = [(58, 100, -128, 127, 15, 127)]#边线检测阈值
boundary_row_threshold = [(58, 100, -128, 127, 15, 127)]#边线检测阈值
day_brightness = 600
binary_threshold = (175, 255)   #需要调节
LED(4).on()#照明

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
        lcd.show_image(img, 320, 240, zoom=2)
        img.binary([binary_threshold])
        img.erode(2)
        img.dilate(2)
        for r in img.find_rects(threshold=3000):
            #寻找矩形
            if r.w() > 120 and r.w() < 250 and r.h() > 70 and r.h() < 180:
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

                result_x = sorted(pos_nowx) #重新排列当前数组
                if(result_x == pos_perx and len(result_x) > 10): #如果当前数组等于上一次，则发送数据
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


                    sensor.reset()
                    sensor.set_pixformat(sensor.RGB565)
                    sensor.set_framesize(sensor.QVGA)
                    sensor.set_brightness(day_brightness)
                    sensor.skip_frames(20)
                    sensor.set_auto_gain(False)
                    sensor.set_auto_whitebal(False,(0,0,0))
                    break
                else:
                    pos_perx = result_x



#边线矫正函数
def boundary_correct(mode):

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
        lcd.show_image(img, 320, 240, zoom=2)
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
            if center > 5:
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
                            lcd.show_image(img, 320, 240, zoom=2)
                            break
                        else:
                            break
                    else:
                        # uart.write("%c" % 0)
                        # print("Line Angle: ", angle)
                        img.draw_string(10,10,"boundary", (255,0,0))
                        lcd.show_image(img, 320, 240, zoom=2)




def recognize_pic(labels, net):
    global uart_num
    recognize_flag = 1
    class_per = []

    recognized_result = None  # 记录识别结果
    same_result_count = 0  # 相同结果的计数器
    nearest_blob = None  # 最近的色块
    nearest_distance = float('inf')  # 最近色块与图像中心点的距离

    while recognize_flag:
        img = sensor.snapshot()
        uart_num = uart.any()  # 获取当前串口数据数量
        lcd.show_image(img, 320, 240, zoom=2)
        if uart_num != 0:
            recognize_flag = 0
            break
        else:
            for b in img.find_blobs(card_threshold, pixels_threshold=1000, area_threshold=1000, margin=1, merge=True,
                                    invert=0):
                blob_width = b.w()  # 获取blob的宽度
                blob_height = b.h()  # 获取blob的高度
                if blob_width > 40 and blob_height > 40:
                    img.draw_rectangle(b.rect(), color=(255, 0, 0), scale=1, thickness=2)
                    x, y, w, h = b.rect()
                    # 缩小矩形框的宽度和高度
                    new_w = w
                    new_h = h
                    # 计算新的矩形框左上角坐标
                    new_x = x + (w - new_w) // 2
                    new_y = y + (h - new_h) // 2

                    img.draw_rectangle((new_x, new_y, new_w, new_h), color=(255, 0, 0), scale=1, thickness=2)

                    img1 = img.copy(1, 1, (new_x, new_y, new_w, new_h))

                    # 计算色块中心点坐标
                    blob_center_x = new_x + new_w // 2
                    blob_center_y = new_y + new_h // 2

                    # 计算色块中心点与图像中心点的距离
                    distance_to_center = (blob_center_x - img.width() // 2) ** 2 + (blob_center_y - img.height() // 2) ** 2

                    # 更新最近色块的信息
                    if distance_to_center < nearest_distance:
                        nearest_blob = b
                        nearest_distance = distance_to_center

                if nearest_blob is not None:
                    img.draw_rectangle(nearest_blob.rect(), color=(0, 255, 0), scale=1, thickness=2)

                    # 在最近的色块上进行识别
                    x, y, w, h = nearest_blob.rect()
                    img1 = img.copy(1, 1, (x, y, w, h))

                    for obj in tf.classify(net, img1, min_scale=1.0, scale_mul=0.5, x_overlap=0.0, y_overlap=0.0):
                        sorted_list = sorted(zip(labels, obj.output()), key=lambda x: x[1], reverse=True)
                        # 打印排名第一的结果
                        for i in range(1):
                            class_per = sorted_list[i][0]
                            if class_per == recognized_result:
                                same_result_count += 1
                            else:
                                recognized_result = class_per
                                same_result_count = 1

                            if same_result_count >= 3:
                                print("识别结果为：%s" % recognized_result)
                                uart.write("I")
                                uart.write(recognized_result)
                                uart.write("%c" % 1)
                                uart.write("J")
                                img.draw_string(10, 10, "%s:%d" % (recognized_result, sorted_list[i][1] * 100),
                                                (255, 0, 0), 2)
                                lcd.show_image(img, 320, 240, zoom=2)
                                recognize_flag = 0
                                break
                            else:
                                img.draw_string(10, 10, "继续识别", (255, 0, 0))
                                lcd.show_image(img, 320, 240, zoom=2)

                if recognize_flag == 0:
                    break







def main():
    openart_init()
    net_path = "7-13-epoch600-99.68.tflite"                                  # 瀹氫箟妯″瀷鐨勮矾寰
    labels = [line.rstrip() for line in open("/sd/labels.txt")]   # 鍔犺浇鏍囩
    net = tf.load(net_path, load_to_fb=True)                                  # 鍔犺浇妯″瀷


    while(True):
        img = sensor.snapshot()
        #find_coordinates()
        #recognize_pic(labels, net)
        #boundary_correct('row')
        uart_num = uart.any()  # 鑾峰彇褰撳墠涓插彛鏁版嵁鏁伴噺
        if (uart_num):
            uart_str = uart.read(uart_num).strip()  # 璇诲彇涓插彛鏁版嵁
            #print(uart_str.decode())
            if(uart_str.decode() == "A"):
                print("A")
                uart_num=0
                find_coordinates()

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
