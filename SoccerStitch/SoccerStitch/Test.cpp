//
//  Test.cpp
//  SoccerStitch
//
//  Created by samuel on 23/02/2018.
//  Copyright © 2018 baidu. All rights reserved.
//

#include "Test.hpp"

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cvaux.hpp>
#include <fstream>

#include "SSNormalStitcher.hpp"
#include "SSMovWriter.hpp"

using namespace std;
using namespace cv;

#define BYTE unsigned char

int matchFrame(const char *file1, const char *file2, size_t &file1Start, size_t &file2Start, size_t &count)
{
    return 0;
}

int test(void)
{
    const char *file1 = "/Users/majie/Think/repository/soccercv/data/baidu.mov";
    VideoCapture cap(file1); // open the default camera
    if (!cap.isOpened())  // check if we succeeded
    {
        return -1;
    }
    
    Mat edges;
    namedWindow("edges",1);
    cout<<"width: "<<cap.get(CV_CAP_PROP_FRAME_WIDTH)<<endl;
    cout<<"height: "<<cap.get(CV_CAP_PROP_FRAME_HEIGHT)<<endl;
    cout<<"frame rate: "<<cap.get(CV_CAP_PROP_FPS)<<endl;
    cout<<"total frame: "<<cap.get(CV_CAP_PROP_FRAME_COUNT)<<endl;
    
#if 0
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        cout<<cap.get(CV_CAP_PROP_POS_MSEC)<<endl;
        
        cvtColor(frame, edges, COLOR_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        Canny(edges, edges, 0, 30, 3);
        imshow("edges", edges);
        if(waitKey(30) >= 0) break;
    }
#else
    Mat frame;
    cap >> frame; // get a new frame from camera
    
    cvtColor(frame, edges, COLOR_BGR2GRAY);
    GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
    Canny(edges, edges, 0, 30, 3);
    imshow("edges", edges);
#endif

    waitKey(0);
    
    return 0;
}

int stitch_test()
{
    const char *file1 = "/Users/majie/Think/repository/soccercv/data/baidu.mov";
    VideoCapture cap1(file1); // open the default camera
    if (!cap1.isOpened())  // check if we succeeded
    {
        return -1;
    }
    
    const char *file2 = "/Users/majie/Think/repository/soccercv/data/baidu.mov";
    VideoCapture cap2(file2);
    if (!cap2.isOpened())  // check if we succeeded
    {
        return -1;
    }
    
    size_t startOfFile1, startOfFile2, count;
    int value = matchFrame(file1, file2, startOfFile1, startOfFile2, count);
    if (value >= 0) //match成功
    {
        //用中间帧做参考帧, 生成stitcher
        Mat refFrame1, refFrame2;
        
        size_t refIndex1 = startOfFile1 + count / 2;
        cap1.set(CAP_PROP_POS_FRAMES, refIndex1);//从此时的帧数开始获取帧
        cap1 >> refFrame1;
        
        size_t refIndex2 = startOfFile2 + count / 2;
        cap2.set(CAP_PROP_POS_FRAMES, refIndex2);//从此时的帧数开始获取帧
        cap2 >> refFrame2;
        
        SSNormalStitcher stitcher = SSNormalStitcher(refFrame1, refFrame2);
        int width, height;
        stitcher.getOutputSize(width, height);
        
        SSMovWriter writer(width, height);
        writer.open("/Users/majie/Think/repository/soccercv/data/output.mov");
        cap1.set(CAP_PROP_POS_FRAMES, startOfFile1);//从此时的帧数开始获取帧
        cap2.set(CAP_PROP_POS_FRAMES, startOfFile2);//从此时的帧数开始获取帧
        for (size_t i = 0; i < count; i++) //逐帧读取
        {
            Mat frame1, frame2;
            
            cap1 >> frame1;
            cap2 >> frame2;
            
            Mat output = stitcher.stitch(frame1, frame2);
            
            writer.write(output);
        }
        writer.close();
    }

    return 0;
}
