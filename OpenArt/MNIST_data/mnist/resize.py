from PIL import Image
import os

# 定义输入文件夹路径和输出文件夹路径
input_folder = 'orig_pictures/vehicle/飞机'
output_folder = 'resize_pictures/vehicle/vehicle_plane'

# 定义目标分辨率
target_size = (96, 96)

# 遍历输入文件夹中的所有文件
for filename in os.listdir(input_folder):
    # 仅处理图片文件
    if filename.endswith('.jpg') or filename.endswith('.png'):
        # 打开图片文件
        image = Image.open(os.path.join(input_folder, filename))

        # 修改图片分辨率
        resized_image = image.resize(target_size)

        # 保存修改后的图片到输出文件夹中
        resized_image.save(os.path.join(output_folder, filename))
