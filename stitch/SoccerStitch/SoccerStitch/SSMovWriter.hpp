//
//  SSMovWriter.hpp
//  SoccerStitch
//
//  Created by samuel on 28/02/2018.
//  Copyright © 2018 baidu. All rights reserved.
//

#ifndef SSMovWriter_hpp
#define SSMovWriter_hpp

#include <opencv2/opencv.hpp>
#include "SSTypeDef.h"

using namespace cv;

class SSMovWriter
{
public:
    SSMovWriter(Int32 width, Int32 height);
    int open(const char *fname);
    int write(Mat &frame);
    void close(void);
};

#endif /* SSMovWriter_hpp */
