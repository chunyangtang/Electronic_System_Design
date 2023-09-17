import requests
import numpy as np
import cv2
import os

def image_converter():
    # 从网站读图并保存
    url = 'http://192.168.1.1:8080/?action=snapshot'
    res = requests.get(url)
    if res.status_code != 200:
        print('Error: ', res.status_code)
        return None
    with open('camera_get_.jpg', 'wb') as f:
        f.write(res.content)

    # 读取图片
    img = cv2.imread('camera_get_.jpg')
    # 删除图片
    os.remove('camera_get_.jpg')

    # 模式L为灰色图像，它的每个像素用8个bit表示，0表示黑，255表示白，其他数字表示不同的灰度。
    img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # 裁剪 240*240
    img = img[:, 40:280]


    # 根据整张图片选择灰度界限，大于这个值为白色，小于这个值为黑色
    img = cv2.adaptiveThreshold(img, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 25, 10)
    # # 按像素主体所在位置裁剪为 240*240
    # img_center = np.sum(img, axis=0).argmax()
    # img_center = img_center if img_center > 120 else 120
    # img_center = img_center if img_center < 200 else 200
    # img = img[:, img_center - 120: img_center + 120]


    # 调整大小为 28*28
    img = cv2.resize(img, (28, 28), interpolation=cv2.INTER_CUBIC)

    # 二值化与颜色反转
    # img = np.where((img > 254), 0, 255)
    
    # 颜色反转
    img = cv2.bitwise_not(img)

    return img


if __name__ == '__main__':
    image = image_converter()
    cv2.imshow('image', image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()