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

using namespace std;
using namespace cv;

#define BYTE unsigned char

int test(void)
{
    VideoCapture cap("/Users/majie/Think/repository/soccercv/data/baidu.mov"); // open the default camera
    
    if (!cap.isOpened())  // check if we succeeded
    {
        return -1;
    }
    
    Mat edges;
    namedWindow("edges",1);
    cout<<"width: "<<cap.get(CV_CAP_PROP_FRAME_WIDTH)<<endl;
    cout<<"height: "<<cap.get(CV_CAP_PROP_FRAME_HEIGHT)<<endl;
    cout<<"frame rate: "<<cap.get(CV_CAP_PROP_FPS)<<endl;
    
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
