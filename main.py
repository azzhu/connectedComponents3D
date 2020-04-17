#!/GPFS/zhangli_lab_permanent/zhuqingjie/env/py3_tf2/bin/python
'''
@Time    : 03.11 0011 下午 04:16
@Author  : zhuqingjie 
@User    : zhu
@FileName: main.py
@Software: PyCharm
'''
import numpy as np
import ctypes as ct
import time, cv2, os
from PIL import Image
from skimage import io


# 处理一张3d图像，返回2d图像list，16为图像改成了8位
def read3dtif(p, deep8=True):
    tiff = Image.open(p, mode='r')
    print(tiff.size)
    print(tiff.n_frames)
    img_list = []
    for i in range(tiff.n_frames):
        tiff.seek(i)
        img = np.array(tiff)
        if img.dtype == np.uint16 and deep8:
            img = img.astype(np.float)
            img = img / 65535. * 255
            img = img.astype(np.uint8)
        img_list.append(img)
    return np.array(img_list)


def save3dtif(p, imgs):
    io.imsave(p, imgs)


def test_denoise_with_img3D():
    so = ct.cdll.LoadLibrary('denoise.so')
    img = read3dtif('p1_bin_in.tif')
    img = img.astype(np.int32)
    src = img.copy()
    dst = np.zeros_like(src)
    h, w, c = dst.shape[:3]
    p1 = src.ctypes.data_as(ct.POINTER(ct.c_int32))
    p2 = dst.ctypes.data_as(ct.POINTER(ct.c_int32))
    ti = time.time()
    so.connectedComponents3D(p1, p2, h, w, c, 3000)
    print(time.time() - ti)

    dst = dst != 0
    dst = dst.astype(np.uint8) * 255
    save3dtif('p1_bin_out.tif', dst)


if __name__ == '__main__':
    os.system('make')
    time.sleep(2)
    test_denoise_with_img3D()
