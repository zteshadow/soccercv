import cv2
import numpy as np

image = cv2.imread('1.png', cv2.IMREAD_COLOR)
#cv2.imshow('image', image)

pyrDownImage = cv2.pyrDown(image)
#cv2.imshow('pyr down', pyrDownImage)

print(image.shape)
print(pyrDownImage.shape)

ret, thresh = cv2.threshold(cv2.cvtColor(pyrDownImage.copy(), cv2.COLOR_BGR2GRAY), 127, 255, cv2.THRESH_BINARY)
#cv2.imshow('thresh', thresh)

'''
ret, thresh = cv2.threshold(image, 127, 255, 0)
cv2.imshow('thresh', thresh)

contourImage, contour, hierarchy = cv2.findContours(image.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
color = cv2.cvtColor(image, cv2.COLOR_GRAY2BGR)
imageWithContour = cv2.drawContours(color, contour, -1, (0, 255, 0), 2)
cv2.imshow('contour', imageWithContour)
'''

cv2.waitKey()
cv2.destroyAllWindows()
