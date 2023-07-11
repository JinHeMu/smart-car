import os
from PIL import Image

def resize_images(directory, new_size):
    for root, dirs, files in os.walk(directory):
        for filename in files:
            if filename.endswith(".jpg") or filename.endswith(".png") or filename.endswith(".jpeg"):
                filepath = os.path.join(root, filename)
                try:
                    img = Image.open(filepath)
                    img = img.resize((new_size, new_size), Image.ANTIALIAS)
                    img.save(filepath)
                    print(f"Resized {filename}")
                except Exception as e:
                    print(f"Error resizing {filename}: {str(e)}")

# 指定文件夹路径和新尺寸
directory = r"D:\eiq\eiQ_Project"
new_size = 128

# 调用函数进行图片尺寸修改
resize_images(directory, new_size)
