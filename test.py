import imutils
import cv2
import numpy as np

# load image
image = cv2.imread('./1.jpg', 0)
image = imutils.resize(image, width = 800)

# Initiate SIFT detector
detector = cv2.xfeatures2d.SIFT_create()

# find the keypoints and descriptors with SIFT
kps, descriptor = detector.detectAndCompute(image, None)

# filter key points
(h, w) = image.shape[:2]
half_height = h / 2;
new_kps = []
for point in kps:
    if (point.pt[1] > half_height):
        new_kps.append(point)
        #print(point.pt[1])

# draw key points into feature_image
feature_image = cv2.drawKeypoints(image, new_kps, None, color = (0, 0, 255), flags = cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

# resize image for show if needed
if feature_image.shape[1] > 800:
  feature_image = imutils.resize(feature_image, width = 800)

# show feature image
cv2.imshow("feature image", feature_image)

cv2.waitKey(0)
