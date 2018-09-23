//
//  hog_test.cpp
//  test
//
//  Created by samuelsong on 2018/9/13.
//  Copyright © 2018 samuelsong. All rights reserved.
//

#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void hog_test(void)
{
    Mat image = imread("/Users/samuel/Think/source/video/play.png");
    
    if (image.data)
    {
        HOGDescriptor *hog = new HOGDescriptor(cvSize(64, 48), cvSize(32, 32), cvSize(8, 8), cvSize(16, 16), 9);
        if (hog)
        {
            vector<float> descriptor;
            hog->compute(image, descriptor, Size(2, 2), Size(0, 0));
            cout << "descriptor size: " << descriptor.size() << endl;
        }
    }
}
