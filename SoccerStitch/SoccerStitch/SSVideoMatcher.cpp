//
//  SSVideoMatcher.cpp
//  SoccerStitch
//
//  Created by samuel on 06/03/2018.
//  Copyright © 2018 baidu. All rights reserved.
//

#include "SSVideoMatcher.hpp"
#include "SSMeta.hpp"

SSVideoMatcher::SSVideoMatcher(const char *file1, const char *file2)
{
    _file1 = file1;
    _file2 = file2;
}

int SSVideoMatcher::match(size_t &startFrame1, size_t &startFrame2, size_t &count)
{
    int value = -1;
    
    VideoCapture cap1(_file1); // open the default camera
    if (!cap1.isOpened())  // check if we succeeded
    {
        return -1;
    }
    
    VideoCapture cap2(_file2);
    if (!cap2.isOpened())  // check if we succeeded
    {
        return -1;
    }

    long create_time1 = SSMeta::getCreateTime(_file1);
    long create_time2 = SSMeta::getCreateTime(_file2);
    if (create_time1 > 0 && create_time2 > 0)
    {
        long startTime = MAX(create_time1, create_time2);
        
        size_t fps1 = (size_t)(round(cap1.get(CV_CAP_PROP_FPS)));
        size_t start1 = (size_t)(fps1 * (startTime - create_time1));
        size_t left1 = cap1.get(CV_CAP_PROP_FRAME_COUNT) - start1;
        if (left1 <= 0)
        {
            return -1;
        }

        size_t fps2 = (size_t)(round(cap2.get(CV_CAP_PROP_FPS)));
        size_t start2 = (size_t)(fps2 * (startTime - create_time2));
        size_t left2 = cap2.get(CV_CAP_PROP_FRAME_COUNT) - start2;
        if (left2 <= 0)
        {
            return -1;
        }
        
        startFrame1 = start1;
        startFrame2 = start2;
        count = MIN(left1, left2);
        value = 0;
    }
    
    return value;
}
