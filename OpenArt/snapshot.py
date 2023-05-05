import os, tf
from pyb import LED
from machine import UART
import utime
import seekfree
import math
import sensor, image, time


LED(4).on()#照明


threshold = [(31, 100, -128, 127, -128, 127)]

def openart_init():

    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_brightness(800)
    sensor.skip_frames(20)
    sensor.set_auto_gain(False)
    sensor.set_auto_whitebal(True,(0,0,0))


#for i in range(5):
    #img = sensor.snapshot()
    #lcd.show_image(img, 160, 120, zoom=2)
    #filename = "/picture/example%d.jpg" % i
    #img.save(filename)
    #time.sleep(1)


def snapshot(count):
    img = sensor.snapshot()
    for b in img.find_blobs(threshold,pixels_threshold=400,area_threshold=400,margin= 1,merge=True,invert=0):
        #corners = b.corners()

        x, y, w, h = b.rect()  # 设置剪裁区域的左上角坐标和宽度、高度

        img_copy = img.copy()
        #img_copy.draw_rectangle((x,y,w,h), color=(255, 0, 0), thickness=3)
        img_cropped = img_copy.copy(roi=b.rect())

        # 保存裁剪后的图像
        img_cropped.save("/picture/cropped%d.jpg" % count)


        # img1 = img.rotation_corr(0,0,0,0,0,1,60,corners).replace(vflip=True,  hmirror=False,  transpose=False)
        #img.draw_rectangle(b.rect(), color = (255, 0, 0), scale = 1, thickness = 2)

def main():
    openart_init()
    count = 1
    while(True):
        if(count > 101) :
            break
        else:
            img = sensor.snapshot()
            snapshot(count)
            count += 1
            time.sleep(1)



if __name__ == '__main__':
    main()

