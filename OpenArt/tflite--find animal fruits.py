import pyb
import sensor, image, time, math
import os, tf
from pyb import LED #ĺŻźĺĽLED


sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA) # we run out of memory if the resolution is much bigger...
sensor.set_brightness(1000)
sensor.skip_frames(time = 20)
#sensor.set_auto_gain(False)  # must turn this off to prevent image washout...
#sensor.set_auto_whitebal(False,(0,0x80,0))  # must turn this off to prevent image washout...
clock = time.clock()

net_path = "7-7-epoch225.tflite"                                  # ĺŽäšć¨ĄĺçčˇŻĺž?
labels = [line.rstrip() for line in open("/sd/labels.txt")]   # ĺ č˝˝ć ç­ž
net = tf.load(net_path, load_to_fb=True)                                  # ĺ č˝˝ć¨Ąĺ



while(True):
    img = sensor.snapshot()
    threshold = [(52, 100, -128, 127, -30, 127)]

    for b in img.find_blobs(threshold,pixels_threshold=400,area_threshold=400,margin= 1,merge=True,invert=0):

        img.draw_rectangle(b.rect(), color = (255, 0, 0), scale = 1, thickness = 2)

        img1 = img.copy(1,1,b.rect())


        for obj in tf.classify(net , img1, min_scale=1.0, scale_mul=0.5, x_overlap=0.0, y_overlap=0.0):
            print("**********\nTop 1 Detections at [x=%d,y=%d,w=%d,h=%d]" % obj.rect())
            sorted_list = sorted(zip(labels, obj.output()), key = lambda x: x[1], reverse = True)
            for i in range(1):
                print("%s = %f" % (sorted_list[i][0], sorted_list[i][1]))
