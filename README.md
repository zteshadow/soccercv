
1. 在mac上面搭建open cv的开发环境
a. 现有的环境
python --version: Python 2.7.10
which python: /usr/bin/python

python3 --version: Python 3.5.0
which python3: /usr/local/bin/python3

b.open cv的示例需要哪个版本?
没看出来哪个版本(结果参考下面的文档可同时安装两个版本)

c. 全过程参考:
http://www.pyimagesearch.com/2016/12/19/install-opencv-3-on-macos-with-homebrew-the-easy-way/

d. 错误处理
最后还是不行, 报错: "No module named 'cv2'""
pip3 install opencv-python
搞定收工
