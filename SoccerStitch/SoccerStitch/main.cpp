//
//  main.cpp
//  SoccerStitch
//
//  Created by samuel on 23/02/2018.
//  Copyright © 2018 baidu. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cvaux.hpp>
#include <fstream>

using namespace std;

#define BYTE unsigned char

int main(int argc, const char * argv[])

{
#if 1
    //get the image from the directed path
    IplImage* img = cvLoadImage("/Users/majie/Think/repository/soccercv/1.jpg", 1);
    //NSLog(img);
    //create a window to display the image
    cvNamedWindow("picture", 1);
    //show the image in the window
    cvShowImage("picture", img);
    //wait for the user to hit a key
    cvWaitKey(0);
    //delete the image and window
    cvReleaseImage(&img);
    cvDestroyWindow("picture");
#endif

    return 0;
}
