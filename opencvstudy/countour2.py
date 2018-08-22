import cv2
import numpy as np

image = cv2.imread('1.png', cv2.IMREAD_COLOR)
#cv2.imshow('image', image)

pyrDownImage = cv2.pyrDown(image)
#cv2.imshow('pyr down', pyrDownImage)

print(image.shape)
print(pyrDownImage.shape)

ret, thresh = cv2.threshold(cv2.cvtColor(pyrDownImage.copy(), cv2.COLOR_BGR2GRAY), 127, 255, cv2.THRESH_BINARY)
cv2.imshow('thresh', thresh)

contourImage, contour, hier = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
for c in contour:
    #bounding box
    x, y, w, h = cv2.boundingRect(c)
    cv2.rectangle(pyrDownImage, (x, y), (x + w, y + h), (0, 255, 0), 1)

    #minimum area
    rect = cv2.minAreaRect(c)
    box = cv2.boxPoints(rect)
    box = np.int0(box)
    cv2.drawContours(pyrDownImage, [box], 0, (0, 0, 255), 3)

    #minimum enclosing circle
    (x, y), radius = cv2.minEnclosingCircle(c)
    center = (int(x), int(y))
    radius = int(radius)
    pyrDownImage = cv2.circle(pyrDownImage, center, radius, (255, 0, 0), 1)


cv2.drawContours(pyrDownImage, contour, -1, (255, 0, 0), 1)
cv2.imshow('pyrdown image', pyrDownImage)

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
