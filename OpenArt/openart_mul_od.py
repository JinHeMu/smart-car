import seekfree, pyb
import sensor, image, time, tf, gc

sensor.reset()                      # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565) # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)   # Set frame size to QVGA (320x240)
sensor.skip_frames(time = 2000)     # Wait for settings take effect.
sensor.set_brightness(2000)
clock = time.clock()                # Create a clock object to track the FPS.


## 3：屏幕显示方向 参数范围0-3
lcd = seekfree.IPS200(2)
lcd.full()  # 将背景颜色显示到整个屏幕

# 定义一个变量来保存上一次卡片的中心点位置
last_card_center = None
# 定义一个阈值，当两张卡片之间的距离大于此值时，认为是新的卡片
distance_threshold = 10

#设置模型路径
face_detect = '/sd/yolo3_iou_smartcar_final_with_post_processing.tflite'
#载入模型
net = tf.load(face_detect)

detect_flag = 0

# 定义摄像头中心下方的位置，你可能需要根据具体的摄像头参数进行调整
camera_center = (sensor.width() / 2, sensor.height())


while(True):
    img = sensor.snapshot()
    img.draw_line(140, 0, 140, 240, color=(255, 0, 0))
    img.draw_line(180, 0, 180, 240, color=(255, 0, 0))
    closest_card = None
    closest_distance = float("inf") # Set to infinity initially

    for obj in tf.detect(net,img):
        x1,y1,x2,y2,label,scores = obj

        if(scores>0.70):
            w = x2- x1
            h = y2 - y1
            x1 = int((x1-0.1)*img.width())
            y1 = int(y1*img.height())
            w = int(w*img.width())
            h = int(h*img.height())

            if x1+w//2 >= 140 and x1+w//2 <= 180:
                current_card_center = ((x1 + x2) / 2, (y1 + y2) / 2)
                current_distance = ((current_card_center[0] - camera_center[0]) ** 2 + (current_card_center[1] - camera_center[1]) ** 2) ** 0.5

                # If this card is closer to the camera center than the currently saved card, save it
                if current_distance < closest_distance:
                    closest_card = (x1, y1, w, h)
                    closest_distance = current_distance
            else:
                continue

    # Draw rectangle for the closest card after checking all cards
    if closest_card is not None:
        x1, y1, w, h = closest_card
        img.draw_rectangle((x1, y1, w, h), thickness=2)
                # 如果没有上一张卡片或者当前卡片与上一张卡片的距离大于阈值，那么认为这是一张新的卡片
        if last_card_center is None or ((current_card_center[0] - last_card_center[0]) ** 2 + (current_card_center[1] - last_card_center[1]) ** 2) ** 0.5 > distance_threshold:
            print("New card detected")
            # 在卡片右上角画出"new"
            img.draw_string(int(x1), int(y1), "New", color=(255,0,0), scale=2)
        else:
            # 在卡片右上角画出"old"
            img.draw_string(int(x1), int(y1), "Old", color=(0,255,0), scale=2)

        # 更新上一张卡片的中心点位置
        last_card_center = current_card_center
        lcd.show_image(img, 320, 240, zoom=0)











# while(True):
#     img = sensor.snapshot()


#     closest_card = None
#     closest_distance = float("inf") # Set to infinity initially

#     #使用模型进行识别
#     for obj in tf.detect(net,img):
#         x1,y1,x2,y2,label,scores = obj

#         if(scores>0.70):
#                 w = x2- x1
#                 h = y2 - y1
#                 x1 = int((x1-0.1)*img.width())
#                 y1 = int(y1*img.height())
#                 w = int(w*img.width())
#                 h = int(h*img.height())
#                             # 计算当前卡片的中心点位置
#                 current_card_center = ((x1 + x2) / 2, (y1 + y2) / 2)
#                 current_distance = ((current_card_center[0] - camera_center[0]) ** 2 + (current_card_center[1] - camera_center[1]) ** 2) ** 0.5

#                             # If this card is closer to the camera center than the currently saved card, save it
#                 if current_distance < closest_distance:
#                     closest_card = (x1, y1, w, h)
#                     closest_distance = current_distance

#                 if closest_card is not None:
#         x1, y1, w, h = closest_card
#         img.draw_rectangle((x1, y1, w, h), thickness=2)
#         print("Closest card detected")
#         img.draw_string(int(x1), int(y1), "Card", color=(255,0,0), scale=2)


#                 print(x1, x1+w)
#                 if x1 >= 60 and x1+w <= 260:
#                     current_card_center = ((x1 + x2) / 2, (y1 + y2) / 2)

#                     # 如果没有上一张卡片或者当前卡片与上一张卡片的距离大于阈值，那么认为这是一张新的卡片
#                     if last_card_center is None or ((current_card_center[0] - last_card_center[0]) ** 2 + (current_card_center[1] - last_card_center[1]) ** 2) ** 0.5 > distance_threshold:
#                         print("New card detected")
#                         # 在卡片右上角画出"new"
#                         img.draw_string(int(x1), int(y1), "New", color=(255,0,0), scale=2)

#                     else:
#                         print("Old card detected")
#                         # 在卡片右上角画出"old"
#                         img.draw_string(int(x1), int(y1), "Old", color=(0,255,0), scale=2)

#                     # 更新上一张卡片的中心点位置
#                     last_card_center = current_card_center

