import imutils
import cv2
import numpy as np

image = cv2.imread('./1.jpg', 0)

# Initiate SIFT detector
detector = cv2.xfeatures2d.SIFT_create()

# find the keypoints and descriptors with SIFT
kps, descriptor = detector.detectAndCompute(image, None)

# draw key points into feature_image
feature_image = cv2.drawKeypoints(image, kps, None, flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

# show feature image
cv2.imshow("feature image", feature_image)

cv2.waitKey(0)
