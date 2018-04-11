
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
