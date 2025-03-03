from PIL import Image

import matplotlib.pyplot as plt
import numpy as np

# img = np.array(Image.open("/home/andrei/Pictures/Webcam/calibrate.jpg"))

# plt.figure()
# plt.imshow(img)
# plt.show()

# box dimensions in pixels
boxPXWidth = 935 - 456
boxPXHeight = 708 - 374

# measurements in meters
boxDistance = 0.29
boxRealWidth = 0.14
boxRealHeight = 0.095

fx = (boxPXWidth * boxDistance) / boxRealWidth
fy = (boxPXHeight * boxDistance) / boxRealHeight

cx = 1280 / 2
cy = 720/2

K = np.array([[fx, 0, cx], [0, fy, cy], [0, 0, 1]])
print(K)