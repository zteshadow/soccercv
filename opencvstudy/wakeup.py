
import cv2

#cap = cv2.VideoCapture("rtsp://admin:admin@192.168.2.64:554//Streaming/Channels/1")
cap = cv2.VideoCapture(0)
print(cap.isOpened())

while cap.isOpened():
    ret,frame = cap.read()
    cv2.imshow("frame",frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
