# 导入所需的库
import sensor, image, lcd
import KPU as kpu

# 设置摄像头分辨率
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)



# 加载tflite模型
task = kpu.load('/sd/model_cifar10.tflite')

# 加载标签文件
labels = []
with open('/sd/labels.txt','r') as f:
    labels = f.readlines()

# 设置分类阈值
threshold = 0.5

while(True):
    # 捕获图像
    img = sensor.snapshot()

    # 转换为RGB颜色空间
    img = img.to_rgb()

    # 调整图像尺寸
    img = img.resize(32, 32)

    # 图像归一化
    img = img.normalize()

    # 图像预处理
    img = img.invert()

    # 将图像转换为numpy数组
    img_array = img.to_bytes()

    # 运行图像分类器
    pred = kpu.run(task, img_array)

    # 获取最大预测值的索引
    max_index = int(pred[0].argmax())

    # 获取预测的置信度得分
    confidence = float(pred[0][max_index])

    # 显示分类结果
    lcd.clear()
    lcd.draw_string(0, 0, 'Prediction: ' + labels[max_index])
    lcd.draw_string(0, 20, 'Confidence: {:.2f}'.format(confidence))

    # 如果置信度低于阈值，将分类结果标记为未知
    if confidence < threshold:
        lcd.draw_string(0, 40, 'Unknown')
    else:
        lcd.draw_string(0, 40, 'Known')

    # 暂停一段时间，以使结果显示在屏幕上
    time.sleep(1)

# 释放资源
kpu.deinit(task)
