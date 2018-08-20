
import cv2
import numpy as np
from scipy import ndimage

def strokeEdges(src, dst, blurKsize = 7, edgeKsize = 5):
    if blurKsize >= 3:
        blurredSrc = cv2.medianBlur(src, blurKsize)
        graySrc = cv2.cvtColor(blurredSrc, cv2.COLOR_BGR2GRAY)
    else:
        graySrc = cv2.cvtColor(src, cv2.COLOR_BGR2GRAY)

    cv2.Laplacian(graySrc, cv2.CV_8U, graySrc, ksize = edgeKsize)
    normalizedInverseAlpha = (1.0 / 255) * (255 - graySrc)
    channels = cv2.split(src)
    for channel in channels:
        channel[:] = channel * normalizedInverseAlpha
    cv2.merge(channels, dst)


image = cv2.imread('./my.jpg', 1)

dst = image
strokeEdges(image, dst, blurKsize=9)
cv2.imshow('stroke edge', dst)
cv2.imwrite('./detect.jpg', dst)

#graySrc = cv2.cvtColor(blurredSrc, cv2.COLOR_BGR2GRAY)
#cv2.imshow('graySrc', graySrc)

#cv2.imwrite("cany.jpg", cv2.Canny(image, 200, 300))

#cv2.imshow('canny', cv2.Canny(image, 200, 300))

#cv2.imwrite('./3x3.jpg', k3)

#cv2.imwrite('./hpf.jpg', hpf)

cv2.waitKey()
cv2.destroyAllWindows()
