
import cv2
import numpy as np

camera = cv2.VideoCapture(0)
es = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (9, 4))
kernel = np.ones((5, 5), np.uint8)
background = None

#drop unstable frames at very first
for i in range(1):
    ret, frame = camera.read()

#get background frame
ret, frame = camera.read()    
background = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
background = cv2.GaussianBlur(background, (21, 21), 0)

#debug first frame and background frame
'''
cv2.imshow('first', frame)
cv2.imshow('background', background)
while (True):
    if cv2.waitKey((int)(1000 / 12)) & 0xFF == ord('q'):
        exit(0)
'''

while (True):
    ret, frame = camera.read()
    gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    gray_frame = cv2.GaussianBlur(gray_frame, (21, 21), 0)

    diff = cv2.absdiff(background, gray_frame)
    diff = cv2.threshold(diff, 25, 255, cv2.THRESH_BINARY)[1]
    diff = cv2.dilate(diff, es, iterations = 2)
    image, cnts, hierachy = cv2.findContours(diff.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    for c in cnts:
        if cv2.contourArea(c) < 1500:
            continue
        (x, y, w, h) = cv2.boundingRect(c)
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

    cv2.imshow("contours", frame)
    cv2.imshow("diff", diff)
    if cv2.waitKey((int)(1000 / 12)) & 0xFF == ord('q'):
        break

cv2.destroyAllWindows()
camera.release()

