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
#include <opencv2/stitching/detail/camera.hpp>

using namespace cv;
using namespace cv::detail;
using namespace std;

class SSNormalStitcher
{
public:
    static void test(void);
    
    SSNormalStitcher(Mat &refFrame1, Mat &refFrame2);
    Mat stitch(Mat &frame1, Mat &frame2);
    Mat stitch(Mat &frame1, Mat &frame2, size_t &dst_width ,size_t &dst_height);
    
private:
    bool is_work_scale_set, is_seam_scale_set, is_compose_scale_set;
    double work_scale, seam_scale, compose_scale;
    
    float warped_image_scale;
    double seam_work_aspect;
    
    //
    vector<CameraParams> cameras;
    
    int matchMov(const char *file1, int &index1, const char *file2, int &index2);
};

#endif /* SSNormalStitcher_hpp */
