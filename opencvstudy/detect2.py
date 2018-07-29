
import cv2
import numpy as np

camera = cv2.VideoCapture('/Users/samuel/Think/source/video/all.MOV')
#camera = cv2.VideoCapture(0)
mog = cv2.createBackgroundSubtractorMOG2()

while(True):
    ret, frame = camera.read()
    fgmask = mog.apply(frame)
    cv2.imshow('frame', fgmask)
    if cv2.waitKey(0) & 0xFF == ord('q'):
        break

cv2.destroyAllWindows()
camera.release()
