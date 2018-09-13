//
//  main.cpp
//  test
//
//  Created by samuelsong on 2018/9/8.
//  Copyright © 2018 samuelsong. All rights reserved.
//

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

int video_test(void)
{
#if 1
    VideoCapture cap(0);
    namedWindow("frame", cv::WINDOW_NORMAL);
    
    while(true)
    {
        Mat frame;
        cap.read(frame);
        if(frame.empty())
        {
            cout << "No frame" << endl;
        }
        imshow("frame", frame);
        
        int key = cv::waitKey(1);
        if ((key & 0xFF) == 'q')
        {
            break;
        }
    }
#endif
    
    return 0;
}
