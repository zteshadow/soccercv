#
# usage:python extract_image 'video_file' 'output_dir'
# used to extract all frames from a video file, 
# and save to dir with name, '000001.jpg', '000002.jpg', ...
# 提取视频文件中的所有视频帧, 保存到目录中, 并以'000001.jpg', '000002.jpg', 命名,
#
# ==============================================================================
import cv2
import sys

def show_usage():
    print('python extract_image \'file\', \'output dir\')

def extract(video_file, output_dir):
    capture = cv2.VideoCapture(video_file)
    sucess, frame = capture.read()
    index = 1
    while sucess:
        name = '%6d' % index
        cv2.imwrite(name, frame)
        index += 1
        sucess, frame = camera.read()

if __name__=='__main__':
    if sys.argc != 2:
        show_usage()
    else:
        file = sys.argv[0]
        dir = sys.argv[1]
        extract(file, dir)

