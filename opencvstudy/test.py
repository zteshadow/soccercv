
import cv2
import numpy as np
from scipy import ndimage

kernel_3x3 = np.array([[-1, -1, -1], [-1, 8, -1], [-1, -1, -1]])
kernel_5x5 = np.array([[-1, -1, -1, -1, -1], [-1, 1, 2, 1, -1], [-1, 2, 4, 2, -1], [-1, 1, 2, 1, -1], [-1, -1, -1, -1, -1]])

image = cv2.imread('./my1.jpg', 0)
#cv2.imwrite("cany.jpg", cv2.Canny(image, 200, 300))
#cv2.imshow('canny', cv2.imread('cany.jpg'))
#cv2.imshow('canny', cv2.Canny(image, 200, 300))

k3 = ndimage.convolve(image, kernel_3x3)
k5 = ndimage.convolve(image, kernel_5x5)
blur = cv2.GaussianBlur(image, (11, 11), 0)
hpf = image - blur

cv2.imshow('original', image)
cv2.imshow('3x3', k3)
cv2.imshow('5x5', k5)
cv2.imshow('hpf', hpf)

#cv2.imwrite('./3x3.jpg', k3)
cv2.imwrite('./5x5.jpg', k5)
#cv2.imwrite('./hpf.jpg', hpf)

cv2.waitKey()
cv2.destroyAllWindows()
