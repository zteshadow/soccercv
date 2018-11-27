//
//  tld.cpp
//  scv
//
//  Created by samuelsong on 2018/11/25.
//  Copyright © 2018 samuelsong. All rights reserved.
//

#include <math.h>
#include "tld.hpp"
#include "utils.hpp"

//定义最小尺寸, x, y都为15
static const int s_min_size = 15;

/*构建多尺寸滑动窗口(multiple-scale, sliding window)*/
void TLD::buildGrids(const Frame &frame, const Rect &box)
{
    //10个放大的, 10个缩小的, 1个原尺寸, 共21个缩放尺寸
    //1/1.2^10, 1/1.2^9, ..., 1, 1.2, 1.2^2, ..., 1.2^10
    const float scales[] = {0.16151,0.19381,0.23257,0.27908,0.33490,0.40188,0.48225,
        0.57870,0.69444,0.83333,1,1.20000,1.44000,1.72800,
        2.07360,2.48832,2.98598,3.58318,4.29982,5.15978,6.19174};

    //x, y方向的步长为width, height的0.1
    float x_step_scale = 0.1;
    float y_step_scale = 0.1;
    
    const Size frame_size = frame.size();
    
    //遍历所有的图像金字塔
    int count = sizeof(scales)/sizeof(scales[0]);
    for (int i = 0; i < count; i++)
    {
        const float scale = scales[i];
        
        int width = round(frame_size.width * scale);
        int height = round(frame_size.height * scale);
        
        //越界检查
        if (is_invalid(width, height, frame_size, s_min_size))
        {
            continue;
        }
    }
}
