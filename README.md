# soccercv



## 1. 在mac上面搭建open cv的开发环境

**1. 现有的环境**

python --version: Python 2.7.10
which python: /usr/bin/python


python3 --version: Python 3.5.0
which python3: /usr/local/bin/python3

**2. open cv的示例需要哪个版本?**

没看出来哪个版本(结果参考下面的文档可同时安装两个版本)

**3. 全过程参考**

http://www.pyimagesearch.com/2016/12/19/install-opencv-3-on-macos-with-homebrew-the-easy-way/

**4. 错误处理**

最后还是不行, 报错: "No module named 'cv2'""

echo $PATH发现我的python3有两个路径, 把其中的/Library/Frameworks/Python.framework/Versions/3.5/lib/python3.5路径去掉就好了(注意: 开始我没发现, 又pip3 install opencv-python了一下, 结果运行报告"highgui/src/window.cpp error: (-2) The function is not implemented"错误, 后来找到路径错误, 运行pip3 uninstall opencv-python才可以工作了)

