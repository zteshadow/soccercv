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
#include <opencv2/stitching.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cvaux.hpp>

#include <fstream>

#include "SSNormalStitcher.hpp"
#include "SSMovWriter.hpp"
#include "SSMeta.hpp"
#include "SSVideoMatcher.hpp"

#define BYTE unsigned char

using namespace std;
using namespace cv;

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

void stitch_detail_test(void)
{
    Mat image1, image2;
    image1 = imread("/Users/majie/Think/repository/soccercv/1.jpg");
    image2 = imread("/Users/majie/Think/repository/soccercv/2.jpg");
    
    //Mat referenceImage1, referenceImage2;
    SSNormalStitcher stitcher = SSNormalStitcher(image1, image2);
    
    Mat result = stitcher.stitch(image1, image2);
    imwrite("/Users/majie/Think/repository/soccercv/my1.jpg", result);
    
    Mat another = stitcher.stitch(image1, image2);
    imwrite("/Users/majie/Think/repository/soccercv/my2.jpg", another);

    //SSNormalStitcher::test();
}

void stitch_test_ext(Mat &image1, Mat &image2)
{
    vector<Mat> list;
    list.push_back(image1);
    list.push_back(image2);
    
    Mat pano;
    Ptr<Stitcher> stitcher = Stitcher::create(Stitcher::PANORAMA, true);
    
    Stitcher::Status status = stitcher->stitch(list, pano);
    if (status != Stitcher::OK)
    {
        cout << "Can't stitch images, error code = " << int(status) << endl;
        return;
    }
    imwrite("/Users/majie/Think/repository/soccercv/output/tmp.jpg", pano);
    return;
}

void stitch_test(void)
{
    Mat image1 = imread("/Users/majie/Think/repository/soccercv/1.jpg");
    Mat image2 = imread("/Users/majie/Think/repository/soccercv/2.jpg");
    
    stitch_test_ext(image1, image2);
    return;
}

int my_stitch_test()
{
    const char *file1 = "/Users/majie/Think/repository/soccercv/data/video/qiyi/1.mov";
    VideoCapture cap1(file1); // open the default camera
    if (!cap1.isOpened())  // check if we succeeded
    {
        return -1;
    }
    
    const char *file2 = "/Users/majie/Think/repository/soccercv/data/video/qiyi/2.mov";
    VideoCapture cap2(file2);
    if (!cap2.isOpened())  // check if we succeeded
    {
        return -1;
    }    

    const char *file3 = "/Users/majie/Think/repository/soccercv/data/video/qiyi/3.mov";
    VideoCapture cap3(file3);
    if (!cap3.isOpened())  // check if we succeeded
    {
        return -1;
    }

    size_t startOfFile1, startOfFile2, startOfFile3, count;
    SSVideoMatcher matcher(file1, file2, file3);
    int value = matcher.match(startOfFile1, startOfFile2, startOfFile3, count);
    if (value >= 0) //match成功
    {
        //用中间帧做参考帧, 生成stitcher
        Mat refFrame1, refFrame2, refFrame3;
        
        size_t refIndex3 = startOfFile3 + count / 2;
        cap3.set(CAP_PROP_POS_FRAMES, refIndex3);//从此时的帧数开始获取帧
        cap3 >> refFrame3;
        imwrite("/Users/majie/Think/repository/soccercv/output/ref3.jpg", refFrame3);
        
        size_t refIndex2 = startOfFile2 + count / 2;
        cap2.set(CAP_PROP_POS_FRAMES, refIndex2);//从此时的帧数开始获取帧
        cap2 >> refFrame2;
        imwrite("/Users/majie/Think/repository/soccercv/output/ref2.jpg", refFrame2);

        size_t refIndex1 = startOfFile1 + count / 2;
        cap1.set(CAP_PROP_POS_FRAMES, refIndex1);//从此时的帧数开始获取帧
        cap1 >> refFrame1;
        imwrite("/Users/majie/Think/repository/soccercv/output/ref1.jpg", refFrame1);

        {
            vector<Mat> list;
            list.push_back(refFrame1);
            list.push_back(refFrame2);
            //list.push_back(refFrame3);

            Mat pano;
            Ptr<Stitcher> stitcher = Stitcher::create(Stitcher::PANORAMA, true);
            
            Stitcher::Status status = stitcher->stitch(list, pano);
            if (status != Stitcher::OK)
            {
                cout << "Can't stitch images, error code = " << int(status) << endl;
                return -1;
            }
            imwrite("/Users/majie/Think/repository/soccercv/output/tmp.jpg", pano);
        }
        //stitch_test_ext(refFrame1, refFrame2);
        
        SSNormalStitcher stitcher = SSNormalStitcher(refFrame1, refFrame2);
        size_t width, height;
        Mat test = stitcher.stitch(refFrame1, refFrame2, width, height);
        imwrite("/Users/majie/Think/repository/soccercv/output/movref.jpg", test);
        
        SSMovWriter writer((Int32)width, (Int32)height);
        writer.open("/Users/majie/Think/repository/soccercv/output/output.mov");
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
