import pyb
import sensor, image, time, math
import os, tf
import SEEKFREE_18TFT as tft18

tft18.tft_init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA) # we run out of memory if the resolution is much bigger...
sensor.set_brightness(2000)
sensor.skip_frames(time = 20)
sensor.set_auto_gain(False)  # must turn this off to prevent image washout...
sensor.set_auto_whitebal(False,(0,0x80,0))  # must turn this off to prevent image washout...
clock = time.clock()

net_path = "model.tflite"                                  # 定义模型的路径
labels = [line.rstrip() for line in open("/sd/labels.txt")]   # 加载标签
net = tf.load(net_path, load_to_fb=True)                                  # 加载模型

# 这个示例演示如何加载tflite模型并运行
# 这个示例演示如何加载tflite模型并运行
# 这个示例演示如何加载tflite模型并运行
while(True):
    img = sensor.snapshot()

    for r in img.find_rects(threshold = 25000):             # 在图像中搜索矩形
        img.draw_rectangle(r.rect(), color = (255, 0, 0))   # 绘制矩形外框，便于在IDE上查看识别到的矩形位置
        img1 = img.copy(r.rect())                           # 拷贝矩形框内的图像

        # 将矩形框内的图像使用训练好的模型进行分类
        # net.classify()将在图像的roi上运行网络(如果没有指定roi，则在整个图像上运行)
        # 将为每个位置生成一个分类得分输出向量。
        # 在每个比例下，检测窗口都以x_overlap（0-1）和y_overlap（0-1）为指导在ROI中移动。
        # 如果将重叠设置为0.5，那么每个检测窗口将与前一个窗口重叠50%。
        # 请注意，重叠越多，计算工作量就越大。因为每搜索/滑动一次都会运行一下模型。
        # 最后，对于在网络沿x/y方向滑动后的多尺度匹配，检测窗口将由scale_mul（0-1）缩小到min_scale（0-1）。
        # 下降到min_scale(0-1)。例如，如果scale_mul为0.5，则检测窗口将缩小50%。
        # 请注意，如果x_overlap和y_overlap较小，则在较小的比例下可以搜索更多区域...

        # 默认设置只是进行一次检测...更改它们以搜索图像...
        for obj in tf.classify(net , img1, min_scale=1.0, scale_mul=0.5, x_overlap=0.0, y_overlap=0.0):
            print("**********\nTop 1 Detections at [x=%d,y=%d,w=%d,h=%d]" % obj.rect())

            sorted_list = sorted(zip(labels, obj.output()), key = lambda x: x[1], reverse = True)
            # 打印准确率最高的结果
            for i in range(1):
                print("%s = %f" % (sorted_list[i][0], sorted_list[i][1]))
                tft18.tft_display_string_in_image(img,r.x(),r.y(),str(sorted_list[i][0]),255,0,0)
                tft18.tft_display_string_in_image(img,r.x(),r.y()+20, str(sorted_list[i][1])[:4],255,0,0)
                tft18.tft_display(img,160,120)
