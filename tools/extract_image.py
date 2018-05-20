#
# usage:python extract_image 'video_file' 'output_dir'
# used to extract all frames from a video file, 
# and save to dir with name, '000001.jpg', '000002.jpg', ...
# 提取视频文件中的所有视频帧, 保存到目录中, 并以'000001.jpg', '000002.jpg', 命名,
#
# ==============================================================================
import cv2
import sys, os

def show_usage():
    print('usage:\n\tpython extract_image \'file\' \'output dir\'')

def get_file_count(dir):
    count = 0
    for fn in os.listdir(dir):
        count += 1
    print('%d total files in dir: %s' % (count, dir))
    return count

def extract(video_file, output_dir, start_index):
    capture = cv2.VideoCapture(video_file)
    sucess, frame = capture.read()
    index = start_index
    while sucess:
        print(output_dir + '/%06d.jpg' % index)
        cv2.imwrite(output_dir + '/%06d.jpg' % index, frame)
        index += 1
        sucess, frame = capture.read()
    print('total %d images etracted to dir: %s' % (index - start_index, output_dir))

if __name__=='__main__':
    len = len(sys.argv)
    if len != 3:
        show_usage()
    else:
        file = sys.argv[1]
        dir = sys.argv[2]
        start_index = 1
        if os.path.isdir(dir):
            start_index = get_file_count(dir) + 1
        else:
            os.mkdir(dir)
        extract(file, dir, start_index)

