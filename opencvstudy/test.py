
"""
import numpy
import cv2
import os


click = False
def onMouse(event, x, y, flag, parm):
    global click
    if event == cv2.EVENT_LBUTTONDOWN:
        click = True

window = cv2.namedWindow('MyWindow')
cv2.setMouseCallback('MyWindow', onMouse)

camera = cv2.VideoCapture(0)
sucess, frame = camera.read()
while sucess and cv2.waitKey(1) == -1 and not click:
    cv2.imshow('MyWindow', frame)
    sucess, frame = camera.read()

cv2.destroyWindow('MyWindow')
camera.release

"""

import cv2
import numpy as np
from scipy import ndimage

kernel_3x3 = np.array([[-1, -1, -1], [-1, 8, -1], [-1, -1, -1]])
kernel_5x5 = np.array([[-1, -1, -1, -1, -1], [-1, 1, 2, 1, -1], [-1, 2, 4, 2, -1], [-1, 1, 2, 1, -1], [-1, -1, -1, -1, -1]])

image = cv2.imread('./my.jpg', 0)

k3 = ndimage.convolve(image, kernel_3x3)
k5 = ndimage.convolve(image, kernel_5x5)
blur = cv2.GaussianBlur(image, (11, 11), 0)
hpf = image - blur

cv2.imshow('3x3', k3)
cv2.imshow('5x5', k5)
cv2.imshow('hpf', hpf)

#cv2.imwrite('./3x3.jpg', k3)
#cv2.imwrite('./5x5.jpg', k5)
#cv2.imwrite('./hpf.jpg', hpf)

cv2.waitKey()
cv2.destroyAllWindows()

