import cv2
import numpy as np

image = np.zeros((200, 200), dtype = np.uint8)
image[50:150, 50:150] = 255

cv2.imshow('image', image)

ret, thresh = cv2.threshold(image, 127, 255, 0)
cv2.imshow('thresh', thresh)

contourImage, contour, hierarchy = cv2.findContours(image.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
color = cv2.cvtColor(image, cv2.COLOR_GRAY2BGR)
imageWithContour = cv2.drawContours(color, contour, -1, (0, 255, 0), 2)
cv2.imshow('contour', imageWithContour)

cv2.waitKey()
cv2.destroyAllWindows()
