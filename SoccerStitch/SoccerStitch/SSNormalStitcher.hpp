//
//  SSNormalStitcher.hpp
//  SoccerStitch
//
//  Created by samuel on 26/02/2018.
//  Copyright © 2018 baidu. All rights reserved.
//

#ifndef SSNormalStitcher_hpp
#define SSNormalStitcher_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class SSNormalStitcher
{
public:
    static void test(void);
    
    SSNormalStitcher(Mat &refFrame1, Mat &refFrame2);
    Mat stitch(Mat &frame1, Mat frame2);
    void getOutputSize(int &width, int &height);
    
private:
    double work_scale, seam_scale, compose_scale;
    float warped_image_scale;
    //vector<detail::CameraParams> cameras;
    
    int matchMov(const char *file1, int &index1, const char *file2, int &index2);
};

#endif /* SSNormalStitcher_hpp */
