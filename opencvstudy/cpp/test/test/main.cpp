//
//  main.cpp
//  test
//
//  Created by samuelsong on 2018/9/8.
//  Copyright © 2018 samuelsong. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>

extern int object_detect_main(int argc, char** argv);
extern void hog_test(void);

using namespace cv;
using namespace std;

#define SSTR( x ) static_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()

int tracker_test(void)
{
    string tracker_list[] ={"BOOSTING", "MIL", "KCF", "TLD", "MEDIANFLOW", "GOTURN", "MOSSE", "CSRT"};
    string tracker_type = tracker_list[2];
    Ptr<Tracker> tracker;
    
    if (tracker_type == "BOOSTING")
    {
        tracker = TrackerBoosting::create();
    }
    else if (tracker_type == "MIL")
    {
        tracker = TrackerMIL::create();
    }
    else if (tracker_type == "KCF")
        tracker = TrackerKCF::create();
    else if (tracker_type == "TLD")
        tracker = TrackerTLD::create();
    else if (tracker_type == "MEDIANFLOW")
        tracker = TrackerMedianFlow::create();
    else if (tracker_type == "GOTURN")
        tracker = TrackerGOTURN::create();
    else if (tracker_type == "MOSSE")
        tracker = TrackerMOSSE::create();
    else if (tracker_type == "CSRT")
    {
        tracker = TrackerCSRT::create();
    }
    
    VideoCapture video("/Users/samuel/Think/source/video/1.mp4");
    if (!video.isOpened())
    {
        cout << "can't open file" <<endl;
        return -1;
    }
    
    Mat frame;
    bool ok = video.read(frame);
    
    //resize
    
    //save
    
    Rect2d bbox = selectROI(frame, false);
    rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
    
    imshow("Tradking", frame);
    tracker->init(frame, bbox);
    
    while(true)
    {
        Mat frame;
        video.read(frame);
        if (frame.empty())
        {
            cout << "No frame" << endl;
            break;
        }
        
        double start = getTickCount();
        ok = tracker->update(frame, bbox);
        float fps = getTickFrequency() / (getCPUTickCount() - start);
        
        if (ok)
        {
            rectangle(frame, bbox, Scalar(0, 0, 255), 2, 1);
        }
        else
        {
            putText(frame, "Tracking failure detected", Point(100, 80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
        }
        
        putText(frame, tracker_type + " Tracker", Point(100,20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50),2);
        
        // Display FPS on frame
        char fpsMessage[100] = {0};
        sprintf(fpsMessage, "FPS: %d", (int)fps);
        putText(frame, fpsMessage, Point(100,50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50), 2);
        
        // Display frame.
        imshow("Tracking", frame);
        
#if 1
        int key = cv::waitKey(1);
        if ((key & 0xFF) == 'q')
        {
            break;
        }
#endif
    }
    
    return 0;
}

int main(int argc, const char * argv[])
{
    tracker_test();
    //object_detect_main(argc, (char **)argv);
    //hog_test();
    
    return 0;
}
