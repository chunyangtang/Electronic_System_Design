
import torch
import torch.nn as nn
import torch.nn.functional as F
import time
import requests
import numpy as np
import cv2
import os

PREDICTION_CMP_TIME = 8
MAX_PREDICTION_TIME = 300


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
    # os.remove('camera_get_.jpg')

    # 模式L为灰色图像，它的每个像素用8个bit表示，0表示黑，255表示白，其他数字表示不同的灰度。
    img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

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
    
    cv2.imwrite("camera_get.jpg", img)

    return img

class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        self.conv1 = nn.Conv2d(1, 32, 3, 1)
        self.conv2 = nn.Conv2d(32, 64, 3, 1)
        self.dropout1 = nn.Dropout(0.25)
        self.dropout2 = nn.Dropout(0.5)
        self.fc1 = nn.Linear(9216, 128)
        self.fc2 = nn.Linear(128, 10)

    def forward(self, x):
        x = self.conv1(x)
        x = F.relu(x)
        x = self.conv2(x)
        x = F.relu(x)
        x = F.max_pool2d(x, 2)
        x = self.dropout1(x)
        x = torch.flatten(x, 1)
        x = self.fc1(x)
        x = F.relu(x)
        x = self.dropout2(x)
        x = self.fc2(x)
        output = F.log_softmax(x, dim=1)
        return output

def model_prediction():
    device = torch.device("cuda") if torch.cuda.is_available() else torch.device("cpu")
    model = Net().to(device)
    model.load_state_dict(torch.load('mnist_cnn.pt', map_location=device))
    
    prediction_times = 0
    prediction_list = []
    while True:
        image = image_converter()
        if image is not None:
            image = torch.tensor(image, dtype=torch.float, device=device)
            image = image.reshape(1, 1, 28, 28)
            output = model(image)
            prediction_times += 1
            time.sleep(0.1)
            prediction_list.append(output.argmax(dim=1, keepdim=True).item())
            if len(prediction_list) == PREDICTION_CMP_TIME:
                prediction_list = list(set(prediction_list))
                if len(prediction_list) == 1:
                    return prediction_list[0]
                else:
                    print("Character difficult to recognize, pls keep the camera stable.")
                    prediction_list.clear()
                    continue
            if prediction_times == MAX_PREDICTION_TIME:
                return prediction_list[-1]
        else:
            print('Error: image is None')
            time.sleep(0.1)
            continue

if __name__ == '__main__':
    print(model_prediction())