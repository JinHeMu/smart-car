## 导入需要的库
#import sensor, image, time, math
#import os, tf
#from pyb import LED
#from machine import UART
#import utime
#import seekfree


#line = None

#threshold = [(18, 100, -34, 125, 27, 127)]

#uart = UART(2, baudrate=115200)  # 初始化串口 波特率设置为115200 TX是B12 RX是B13
#lcd = seekfree.LCD180(2)
#lcd.full()

#LED(4).on()





## 初始化屏幕
#def openart_init():

    ## 初始化摄像头
    #sensor.reset()
    #sensor.set_pixformat(sensor.RGB565)
    #sensor.set_framesize(sensor.QVGA)
    #sensor.set_brightness(800)
    #sensor.skip_frames(20)
    #sensor.set_auto_gain(False)
    #sensor.set_auto_whitebal(True,(0,0,0))
    ## 打开LED灯

    ##ips114.ips_init()


#def uart_correct(img):

    #sensor.reset()
    #sensor.set_pixformat(sensor.RGB565)
    #sensor.set_framesize(sensor.QQVGA)
    #sensor.set_brightness(1000)
    #sensor.skip_frames(20)
    #sensor.set_auto_gain(False)
    #sensor.set_auto_whitebal(True,(0,0,0))
    #while True:
        #img = sensor.snapshot()

        #line = img.get_regression(threshold)

    ## 如果找到了线段
        #if line:
            #img.draw_line(line.line(), color = (255, 0, 0)) # 在图像上标出线段
            #angle = line.theta() # 计算线段的角度
            #if(angle > 90):
                #angle -= 180
            #print("Line Angle: ", angle)
            #uart.write("%c" % angle)







#def main():
    #openart_init()
    #while(True):
        #img = sensor.snapshot()
        #uart_correct(img)






#if __name__ == '__main__':
    #main()


# 识别直线例程
#
# 这个例子展示了如何在图像中查找线条。对于在图像中找到的每个线对象，
# 都会返回一个包含线条旋转的线对象。

# 注意：线条检测是通过使用霍夫变换完成的：
# http://en.wikipedia.org/wiki/Hough_transform
# 请阅读以上关于“theta”和“rho”的更多信息。

# find_lines（）找到无限长度的线。使用find_line_segments（）
# 来查找非无限线。

enable_lens_corr = False # turn on for straighter lines...打开以获得更直的线条…
import sensor, image, time

sensor.reset()
sensor.set_pixformat(sensor.RGB565) #灰度更快
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 2000)
clock = time.clock()

# 所有的线对象都有一个`theta（）`方法来获取它们的旋转角度。
# 您可以根据旋转角度来过滤线条。

min_degree = 0
max_degree = 179

# 所有线段都有 `x1()`, `y1()`, `x2()`, and `y2()` 方法来获得他们的终点
# 一个 `line()` 方法来获得所有上述的四个元组值，可用于 `draw_line()`.

while(True):
    clock.tick()
    img = sensor.snapshot()
    if enable_lens_corr: img.lens_corr(1.8) # for 2.8mm lens...

    # `threshold` controls how many lines in the image are found. Only lines with
    # edge difference magnitude sums greater than `threshold` are detected...

    # `threshold`控制从霍夫变换中监测到的直线。只返回大于或等于阈值的
    # 直线。应用程序的阈值正确值取决于图像。注意：一条直线的大小是组成
    # 直线所有索贝尔滤波像素大小的总和。

    # `theta_margin`和`rho_margin`控件合并相似的直线。如果两直线的
    # theta和ρ值差异小于边际，则它们合并。


            img.draw_line(l.line(), color = (255, 0, 0))
            print(l)

    #print("FPS %f" % clock.fps())

# About negative rho values:
# 关于负rho值:
#
# A [theta+0:-rho] tuple is the same as [theta+180:+rho].
# A [theta+0:-rho]元组与[theta+180:+rho]相同。

