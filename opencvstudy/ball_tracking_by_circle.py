from collections import deque
from imutils.video import VideoStream
import numpy as np
import argparse
import cv2
import imutils
import time

ap = argparse.ArgumentParser()
ap.add_argument('-v', '--video', help = 'path to the(optional) video file')
ap.add_argument('-b', '--buffer', type = int, default = 64, help = 'max buffer size')
args = vars(ap.parse_args())

greenLower = (29, 86, 6)
greenUpper = (64, 255, 255)
pts = deque(maxlen = args['buffer'])

if not args.get('video', False):
    vs = VideoStream(src = 0).start()
else:
    vs = cv2.VideoCapture(args['video'])
time.sleep(2.0)

while True:
    frame = vs.read()
    frame = frame[1] if args.get('video', False) else frame
    if frame is None:
        break

    frame = imutils.resize(frame, width = 600)
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    blur = cv2.medianBlur(gray, 5)
    circles = cv2.HoughCircles(blur, cv2.HOUGH_GRADIENT, 1, 120, param1 = 100, param2 = 30, minRadius = 0, maxRadius = 0)
    circles = np.uint16(np.around(circles))

    for i in circles[0, :]:
        cv2.circle(frame, (i[0], i[1]), i[2], (0, 255, 0), 2)
        cv2.circle(frame, (i[0], i[1]), 2, (0, 0, 255), 3)

    cv2.imshow('frame', frame)

    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break

if not args.get('video', False):
    vs.stop()
else:
    vs.release()

cv2.destroyAllWindows()
