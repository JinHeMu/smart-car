import sensor, image, math

def gaussian_elimination(A, B):
    n = len(A)

    # Forward elimination
    for i in range(n):
        max_row_index = max(range(i, n), key=lambda r: abs(A[r][i]))
        A[i], A[max_row_index] = A[max_row_index], A[i]
        B[i], B[max_row_index] = B[max_row_index], B[i]

        for j in range(i+1, n):
            factor = A[j][i] / A[i][i]
            for k in range(i, n):
                A[j][k] -= factor * A[i][k]
            B[j] -= factor * B[i]

    # Backward substitution
    params = [0] * n
    for i in range(n-1, -1, -1):
        params[i] = B[i] / A[i][i]
        for j in range(i):
            B[j] -= A[j][i] * params[i]

    return params

# 初始化摄像头
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 2000)

# 定义原始图像中的关键点和目标位置的像素坐标
src_points = [(70, 56), (103, 56), (103, 22), (70, 22)]
dst_points = [(0, 0), (160, 0), (160, 120), (0, 120)]

# 构建矩阵A和B
A = []
B = []

for i in range(4):
    src_x, src_y = src_points[i]
    dst_x, dst_y = dst_points[i]

    A.append([src_x, src_y, 1, 0, 0, 0, -src_x*dst_x, -src_y*dst_x])
    A.append([0, 0, 0, src_x, src_y, 1, -src_x*dst_y, -src_y*dst_y])

    B.append(dst_x)
    B.append(dst_y)

params = gaussian_elimination(A, B)

# 这就是我们的透视变换矩阵
perspective_matrix = [[params[0], params[1], params[2]],
                      [params[3], params[4], params[5]],
                      [params[6], params[7], 1]]

# 应用逆透视变换
def inverse_perspective_transform(img, matrix):
    width = img.width()
    height = img.height()

    new_img = image.Image(width, height, sensor.RGB565)
    for x in range(width):
        for y in range(height):
            # 这里的变换公式根据实际的透视变换矩阵进行计算
            new_x = (matrix[0][0]*x + matrix[0][1]*y + matrix[0][2]) / (matrix[2][0]*x + matrix[2][1]*y + matrix[2][2])
            new_y = (matrix[1][0]*x + matrix[1][1]*y + matrix[1][2]) / (matrix[2][0]*x + matrix[2][1]*y + matrix[2][2])

            new_x = min(max(int(new_x), 0), width-1)
            new_y = min(max(int(new_y), 0), height-1)

            new_img.set_pixel(x, y, img.get_pixel(new_x, new_y))

    return new_img

# 主循环
while True:
    img = sensor.snapshot()  # 捕获图像
    img = inverse_perspective_transform(img, perspective_matrix)

