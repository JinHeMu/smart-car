from PIL import Image
import os
import glob


def mkdir(path):
    path = path.strip()
    path = path.rstrip("\\")
    isExists = os.path.exists(path)
    # 判断结果
    if not isExists:
        # 如果不存在则创建目录
        # 创建目录操作函数
        os.makedirs(path)
        print(path + ' 创建成功')
        return True
    else:
        # 如果目录存在则不创建，并提示目录已存在
        print(path + ' 目录已存在')
        return False


def convertjpg(jpgfile, outdir, width=96, height=96):
    img = Image.open(jpgfile)
    try:
        new_img = img.resize((width, height), Image.BILINEAR)
        new_img.save(os.path.join(outdir, os.path.basename(jpgfile)))
    except Exception as e:
        print(e)


outer_path = os.getcwd()
print(outer_path)
folderlist = os.listdir(outer_path)

for folder in folderlist:
    mkdir(outer_path + "\\修改\\" + folder)
    for jpgfile in glob.glob(outer_path + '\\' + folder + "\\*.png"):
        convertjpg(jpgfile, outer_path + "\\修改\\" + folder)
        print(jpgfile + "修改成功")
