
# 导入需要的库
import sensor, image, time, math
import os, tf
from pyb import LED
from machine import UART
import utime
#import SEEKFREE_IPS114_SPI as ips114

global pos_perx,uart_flag
pos_perx = []
uart_flag = 0

uart = UART(1, baudrate=115200)  # 初始化串口 波特率设置为115200 TX是B12 RX是B13




# 初始化屏幕
def openart_init():

    # 初始化摄像头
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_brightness(600)
    sensor.skip_frames(20)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(True,(0,0,0))
    # 打开LED灯

    #ips114.ips_init()





def find_and_send_coordinates():
    global pos_perx
    global uart_flag
    pos_nowx = [] #x坐标现在的值
    pos_y = [] #y坐标

    img = sensor.snapshot()

    for r in img.find_rects(threshold=3000):
        # 判断矩形是否满足要求
        if r.w() > 120 and r.w() < 180 and r.h() > 70 and r.h() < 130:
            # 绘制矩形外框，便于在IDE上查看识别到的矩形位置
            img.draw_rectangle(r.rect(), color=(255, 0, 0))
            # 将矩形属性打印出来
            # jiangly 不是dd
            print("w:" + str(r.w()) + "  h:" + str(r.h()))
            # 在矩形中搜索圆形
            for c in img.find_circles(roi=r.rect(), threshold=2800, x_margin=5, y_margin=5, r_margin=5, r_min=1,
                                      r_max=5, r_step=1):
                # 绘制圆形外框
                img.draw_circle(c.x(), c.y(), c.r(), color=(255, 0, 0))
                # 计算实际X坐标点
                posx = (float(c.x()) - float(r.x())) / float(r.w()) * 35 + 1
                # 计算实际Y坐标点
                posy = 25 - ((float(c.y()) - float(r.y())) / float(r.h()) * 25) + 1

                pos_nowx.append(int(posx)) #将识别到的坐标点加入列表中
                pos_y.append(int(posy))

                img.draw_string(c.x() - 5, c.y() - 10, "(" + str(int(posx)) + "," + str(int(posy)) + ")",
                                color=(0, 0, 255), scale=1, mono_space=False)

        result_x = sorted(pos_nowx) #将x坐标点进行排序
        if(result_x == pos_perx and len(result_x) > 10): #如果这一次识别和上一次相同
            if (uart_flag == 0):#发送标志位
                uart.write("A")  # 发送包头 coordinates

                for i in range(len(pos_nowx)):
                    uart.write("%c" % (pos_nowx[i]))
                    time.sleep_ms(10)

                for i in range(len(pos_y)):
                    uart.write("%c" % (pos_y[i]))
                    time.sleep_ms(10)

                uart.write("Y")  # 发送包尾
                uart_flag = 1  # 结束发送-
                print(pos_nowx)
                print(pos_y)
                print("OK")
        else:
            pos_perx = result_x

def uart_correct(img):
    dis_X = 0
    dis_Y = 0
    distance = 0
    sensor.set_framesize(sensor.QQVGA)
    sensor.set_brightness(600)          # 设置摄像头亮度
    while True:
        img = sensor.snapshot()
        for r in img.find_rects(threshold=20000):
            img.draw_rectangle(r.rect(), color=(255, 0, 0))  # 绘制红色矩形框
            img_x = (int)(r.rect()[0] + r.rect()[2] / 2)  # 图像中心的x值
            img_y = (int)(r.rect()[1] + r.rect()[3] / 2)  # 图像中心的y值
            img.draw_circle(img_x, img_y, 5, color=(0, 255, 0))  # 给矩形中心绘制一个小圆 便于观察矩形中心是否识别正确
            dis_X = img_x - 88
            dis_Y = img_y - 60
            print("disx:%d , disy:%d" % (dis_X, dis_Y))
            uart.write("C")
            uart.write("%c" % dis_X)
            uart.write("%c" % dis_Y)
            uart.write("D")
            distance = math.sqrt((dis_X ** 2) + (dis_Y ** 2))
            if distance < 10:
                break
            img = sensor.snapshot()

def recognize_pic(labels, net):

    while True:
        img = sensor.snapshot()
        for r in img.find_rects(threshold=20000):  # 在图像中搜索矩形
            img.draw_rectangle(r.rect(), color=(255, 0, 0))  # 绘制矩形外框，便于在IDE上查看识别到的矩形位置
            img1 = img.copy(1, 1, r.rect())  # 拷贝矩形框内的图像

            # 将矩形框内的图像使用训练好的模型进行分类
            # tf.classify()将在图像的roi上运行网络(如果没有指定roi，则在整个图像上运行)
            # 将为每个位置生成一个分类得分输出向量。
            # 在每个比例下，检测窗口都以x_overlap（0-1）和y_overlap（0-1）为指导在ROI中移动。
            # 如果将重叠设置为0.5，那么每个检测窗口将与前一个窗口重叠50%。
            # 请注意，重叠越多，计算工作量就越大。因为每搜索/滑动一次都会运行一下模型。
            # 最后，对于在网络沿x/y方向滑动后的多尺度匹配，检测窗口将由scale_mul（0-1）缩小到min_scale（0-1）。
            # 下降到min_scale(0-1)。例如，如果scale_mul为0.5，则检测窗口将缩小50%。
            # 请注意，如果x_overlap和y_overlap较小，则在较小的比例下可以搜索更多区域...

            # 默认设置只是进行一次检测...更改它们以搜索图像...
            for obj in tf.classify(net, img1, min_scale=1.0, scale_mul=0.5, x_overlap=0.0, y_overlap=0.0):
                print("**********\nTop 1 Detections at [x=%d,y=%d,w=%d,h=%d]" % obj.rect())
                sorted_list = sorted(zip(labels, obj.output()), key=lambda x: x[1], reverse=True)
                # 打印准确率最高的结果
                for i in range(1):
                    print("%s = %f" % (sorted_list[i][0], sorted_list[i][1]))
                    uart.write("I")
                    uart.write(sorted_list[i][0])
                    uart.write("J")
                    if sorted_list[i][1] != 0:
                        break
            img = sensor.snapshot()




def main():
    openart_init()
    net_path = "model.tflite"                                  # 定义模型的路径
    labels = [line.rstrip() for line in open("/sd/labels.txt")]   # 加载标签
    net = tf.load(net_path, load_to_fb=True)                                  # 加载模型


    while(True):
        img = sensor.snapshot()
        uart_num = uart.any()  # 获取当前串口数据数量
        if (uart_num):
            uart_str = uart.read(uart_num).strip()  # 读取串口数据
            if(uart_str.decode() == "A"):
                print("A")
                while(uart_flag != 1) :
                    find_and_send_coordinates()
            elif(uart_str.decode() == "B"):
                uart_correct(img)
                print("B")
            elif(uart_str.decode() == "C"):
                recognize_pic(labels, net)
                print("C")
        else:
            img = sensor.snapshot()
        # img = sensor.snapshot()
        # find_and_send_coordinates(img)
        #ips114.ips_display(img, 160, 120)


if __name__ == '__main__':
    main()
