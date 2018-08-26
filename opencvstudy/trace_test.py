
import cv2

cap = cv2.VideoCapture("../../video/all.MOV")
#cap = cv2.VideoCapture(0)
print(cap.isOpened())
width = cap.get(cv2.CAP_PROP_FRAME_WIDTH)
print('width:' + str(width))

height = cap.get(cv2.CAP_PROP_FRAME_HEIGHT)
print('height:' + str(height))

scale = 800 / width
height = int(scale * height)
width = 800

mog = cv2.createBackgroundSubtractorMOG2()

while cap.isOpened():
    ret,frame = cap.read()
    frame = cv2.resize(frame, (width, height), interpolation=cv2.INTER_CUBIC)

    fgmask = mog.apply(frame)
    cv2.imshow("frame",fgmask)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
