
import cv2
import filters
from managers import WindowManager, CaptureManager

class Cameo(object):
    def __init__(self):
        self._window = WindowManager('Cameo', self.onKeypress)
        self._capture = CaptureManager(cv2.VideoCapture(0), self._window, True)

    def run(self):
        self._window.createWindow()
        while self._window.isWindowCreated:
            self._capture.enterFrame()

            frame = self._capture.frame
            
            filters.strokeEdges(frame, frame)

            self._window.show(frame)
            self._capture.exitFrame()
            self._window.processEvents()

    def onKeypress(self, keycode):
        """
        space -> take a snapshot
        tab ->start/stop recording a screencast
        escape -> Quit
        """
        if keycode == 32: #space
            self._capture.writeImage('screenshot.jpg')
        elif keycode == 9: #tab
            if not self._capture.isWritingVideo:
                self._capture.startWritingVideo('screenshot.avi')
            else:
                self._capture.stopWritingVideo()
        elif keycode == 27: #esc
            self._window.destroyWindow()

if __name__ == '__main__':
    Cameo().run()


