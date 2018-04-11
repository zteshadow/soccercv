
import cv2
import numpy
import time

class CaptureManager(object):
    def __init__(self, capture, window = None, mirror = False):
        self.window = window
        self.mirror = mirror

        self._capture = capture
        self._channel = 0
        self._enteredFrame = False

        self._frame = None
        self._imageFileName = None
        self._videoFileName = None
        self._videoEncoding = None
        self._videoWriter = None

        self._startTime = None
        self._frameElapsed = long(0)
        self._fptEstimate = None

    @property
    def channel(sellf):
        return self._channel

    @channel.setter
    def channle(self, value):
        if self._channel != value:
            self._channel = value
            self._frame = None

    @property
    def frame(self):
        if self._enteredFrame and self._frame is None:
            _, self._frame = self._capture.retrieve()
        return self._frame

    @property
    def isWritingImage(self):
        return self._imageFileName is not None

    @property
    def isWritingVideo(self):
        return self._videoFileName is not None
    
    

