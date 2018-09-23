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
    blurred = cv2.GaussianBlur(frame, (11, 11), 0)
    hsv = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)

    mask = cv2.inRange(hsv, greenLower, greenUpper)
    mask = cv2.erode(mask, None, iterations = 2)
    mask = cv2.dilate(mask, None, iterations = 2)

    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break

if not args.get('video', False):
    vs.stop()
else:
    vs.release()

cv2.destroyAllWindows()

