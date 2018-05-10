
import cv2
import filters
from managers import WindowManager, CaptureManager

class Cameo(object):
    def __init__(self):
        self._window = WindowManager('Cameo', self.onKeypress)
        #self._capture = CaptureManager(cv2.VideoCapture(0), self._window, True)
        self._capture = CaptureManager(cv2.VideoCapture('./all.MOV') , None, False)

    def run(self):
        self._window.createWindow()
        index = 1
        while self._window.isWindowCreated:
            self._capture.enterFrame()

            frame = self._capture.frame
            if index % 300 == 0:
                name = './output/' + str(index) + '.jpg'
                cv2.imwrite(name, frame)
                print(name)

            index += 1

            #filters.strokeEdges(frame, frame)
            #frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            #frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            #frame = cv2.Canny(frame, 200, 300)

            #self._window.show(frame)
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


