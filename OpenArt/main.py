
import os, tf
from pyb import LED
from machine import UART
import utime
import seekfree
import math
import sensor, image, time

global pos_perx,uart_flag
line = None
pos_perx = []
uart_flag = 0
threshold = [(70, 100, -128, 24, -25, 125)]#色块检测阈值

uart = UART(2, baudrate=115200) #串口

lcd = seekfree.LCD180(2)#显示屏
lcd.full()

LED(4).off()#照明





#openart初始化
def openart_init():

    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_brightness(800)
    sensor.skip_frames(20)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(True,(0,0,0))

#寻找A4纸坐标，并发送A4纸坐标
def find_coordinates():
    global pos_perx
    global uart_flag
    pos_nowx = []
    pos_y = []

    img = sensor.snapshot()
    for r in img.find_rects(threshold=3000):
        #寻找矩形
        if r.w() > 120 and r.w() < 180 and r.h() > 70 and r.h() < 130:
            #获取角点，并进行变换
            corners = r.corners()
            img1 = img.rotation_corr(0,0,0,0,0,1,60,corners).replace(vflip=True,  hmirror=False,  transpose=False)
            #寻找圆点
            for c in img1.find_circles((5,5,300,220),x_stride = 2, y_stride = 1,threshold=1500, x_margin=10, y_margin=10, r_margin=5, r_min=1,
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
                uart_flag = 1    # 串口发送成功标志位
                print(pos_nowx)
                print(pos_y)
                print("OK")
                break
            else:
                pos_perx = result_x


#图片矫正函数
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

    q_list = [0] * 5  # the length of the list is set to 10

    while True:
        img = sensor.snapshot()


        for b in img.find_blobs(threshold, pixels_threshold=400, area_threshold=400, margin=1, merge=True, invert=0):
            #寻找角点
            corners = b.min_corners()
            point_corners = tuple(sorted(corners))
            for corner in corners:
                img.draw_circle(corner[0], corner[1], 5, color=(0, 255, 0))
            x0, y0 = point_corners[3]
            x1, y1 = point_corners[2]
            x2, y2 = point_corners[1]
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
            dis_X = b.cx() - 80
            dis_Y = b.cy() - 63
            print("disx:%d , disy:%d, angle:%d" % (dis_X, dis_Y, q))

            #发送数据
            uart.write("C")
            uart.write("%c" % dis_X)
            uart.write("%c" % dis_Y)
            uart.write("%c" % q)
            uart.write("D")
            lcd.show_image(img, 160, 120, zoom=0)
            distance = math.sqrt((dis_X ** 2) + (dis_Y ** 2))
            if distance < 10:
                break
            img = sensor.snapshot()

#边线矫正函数
def boundary_correct():

    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QQVGA)
    sensor.set_brightness(1000)
    sensor.skip_frames(20)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(True,(0,0,0))

    img = sensor.snapshot()

    while True:
        img = sensor.snapshot()

        line = img.get_regression(threshold)

    # 如果找到了线段
        if line:
            img.draw_line(line.line(), color = (255, 0, 0)) # 在图像上标出线段
            angle = line.theta() # 计算线段的角度
            if(angle > 90):
                angle -= 180
            uart.write("%c" % angle)
            print("Line Angle: ", angle)



#识别卡片
def recognize_pic(labels, net):

    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QQVGA)
    sensor.set_brightness(1000)
    sensor.skip_frames(20)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(True,(0,0,0))

    img = sensor.snapshot()

    while True:
        img = sensor.snapshot()

        for b in img.find_blobs(threshold,pixels_threshold=400,area_threshold=400,margin= 1,merge=True,invert=0):

            img.draw_rectangle(b.rect(), color = (255, 0, 0), scale = 1, thickness = 2)

            img1 = img.copy()

            for obj in tf.classify(net, img1, min_scale=1.0, scale_mul=0.5, x_overlap=0.0, y_overlap=0.0):
                print("**********\nTop 1 Detections at [x=%d,y=%d,w=%d,h=%d]" % obj.rect())
                sorted_list = sorted(zip(labels, obj.output()), key=lambda x: x[1], reverse=True)
                # 鎵撳嵃鍑嗙‘鐜囨渶楂樼殑缁撴灉
                for i in range(1):
                    print("%s = %f" % (sorted_list[i][0], sorted_list[i][1]))
                    uart.write("I")
                    uart.write(sorted_list[i][0])
                    uart.write("J")
                    lcd.show_image(img, 160, 120, zoom=0)
                    if sorted_list[i][1] != 0:
                        break
            img = sensor.snapshot()





def main():
    openart_init()
    #net_path = "mobilenet_v2-2023-06-03T04-14-30.049Z_in-int8_out-int8.tflite"                                  # 瀹氫箟妯″瀷鐨勮矾寰
    #labels = [line.rstrip() for line in open("/sd/labels.txt")]   # 鍔犺浇鏍囩
    #net = tf.load(net_path, load_to_fb=True)                                  # 鍔犺浇妯″瀷


    while(True):
        img = sensor.snapshot()
        # recognize_pic(labels, net)
        # # boundary_correct()
        # # picture_correct()


        uart_num = uart.any()  # 鑾峰彇褰撳墠涓插彛鏁版嵁鏁伴噺
        if (uart_num):
            uart_str = uart.read(uart_num).strip()  # 璇诲彇涓插彛鏁版嵁
            if(uart_str.decode() == "A"):
                print("A")
                while(uart_flag != 1):
                    find_coordinates()

            elif(uart_str.decode() == "B"):
                print("B")
                picture_correct()

            elif(uart_str.decode() == "C"):
                print("C")
                recognize_pic(labels, net)

        else:
            img = sensor.snapshot()
            lcd.show_image(img, 320, 240, zoom=2)




if __name__ == '__main__':
    main()
