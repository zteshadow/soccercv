//
//  SSVideoMatcher.hpp
//  SoccerStitch
//
//  Created by samuel on 06/03/2018.
//  Copyright © 2018 baidu. All rights reserved.
//

#ifndef SSVideoMatcher_hpp
#define SSVideoMatcher_hpp

#include <opencv2/opencv.hpp>

using namespace cv;

class SSVideoMatcher
{
public:
    SSVideoMatcher(const char *file1, const char *file2);
    SSVideoMatcher(const char *file1, const char *file2, const char *file3);
    int match(size_t &startFrame1, size_t &startFrame2, size_t &count);
    int match(size_t &startFrame1, size_t &startFrame2, size_t &startFrame3, size_t &count);
    
private:
    const char *_file1;
    const char *_file2;
    const char *_file3;
    
};

#endif /* SSVideoMatcher_hpp */
