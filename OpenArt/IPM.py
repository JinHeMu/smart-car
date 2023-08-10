import sensor, image
import seekfree
# 初始化摄像头
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time=2000)

lcd = seekfree.LCD180(2)#显示屏
lcd.full()

perspective_matrix = [[ 2.15789474e+00  ,8.43874392e-01 ,-1.00421053e+02],
 [ 1.22219659e-16,  2.15789474e+00 ,-2.02876495e-14],
 [ 8.80827798e-19 , 9.73020787e-03  ,1.00000000e+00]]



def inverse_perspective_transform(img, matrix):
    width = img.width()
    height = img.height()
    new_img = image.Image(width, height, sensor.RGB565)

    for x in range(width):
        for y in range(height):
            denominator = matrix[2][0]*x + matrix[2][1]*y + matrix[2][2]
            new_x = (matrix[0][0]*x + matrix[0][1]*y + matrix[0][2]) / denominator
            new_y = (matrix[1][0]*x + matrix[1][1]*y + matrix[1][2]) / denominator

            # Clamping values to ensure they are within the bounds of the image.
            new_x = min(max(int(new_x), 0), width-1)
            new_y = min(max(int(new_y), 0), height-1)

            new_img.set_pixel(x, y, img.get_pixel(new_x, new_y))

    return new_img

# 主循环
while True:
    img = sensor.snapshot()  # 捕获图像
    img = inverse_perspective_transform(img, perspective_matrix)
    lcd.show_image(img, 160, 120, zoom=2)
