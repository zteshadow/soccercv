
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

        #estimate the fts by _frameElapsed / time
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
    
    def enterFrame(self):
        """Capture the next frame if any"""
        assert not self.enterFrame, "previous enter frame had no matching exit frame"

        if self._capture is not None:
            self._enteredFrame = self._capture.grab()

    def exitFrame(self):
        """write file, show in window, release frame"""
        if self.frame is None:
            self._enteredFrame = False
            return

        if self._frameElapsed == 0:
            self._startTime = time.time()
        else:
            self._fptEstimate = self._frameElapsed / time.time() - self._startTime
        self._frameElapsed += 1
        
        #show in window
        if self.window is not None:
            if self.mirror:
                mirrorFrame = numpy.fliplr(self._frame).copy()
                self.window.show(mirrorFrame)
            else:
                self.window.show(self._frame)

        #write
        if self.isWritingImage:
            cv2.imwrite(self._imageFileName, self._frame)
            self._imageFileName = None

        self._writeVideoFrame()

        self._frame = None
        self._enteredFrame = False

    def writeImage(self, fname):
        self._imageFileName = fname
        
