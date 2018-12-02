//
//  tld.cpp
//  scv
//
//  Created by samuelsong on 2018/11/25.
//  Copyright © 2018 samuelsong. All rights reserved.
//

#include <math.h>
#include <algorithm>
#include "tld.hpp"
#include "utils.hpp"

using namespace std;

//min for x, y
static const int s_min_size = 15;

/* multiple-scale, sliding window */
void TLD::buildGrids(const Frame &frame, const Rect &box)
{
    //multiple scale
    //10 for enlarge, 10 for shrink and one original, 21 in total
    //scale is 1.2, so value is :1/1.2^10, 1/1.2^9, ..., 1, 1.2, 1.2^2, ..., 1.2^10
    const float scales[] = {0.16151, 0.19381, 0.23257, 0.27908, 0.33490, 0.40188, 0.48225,
        0.57870, 0.69444, 0.83333, 1, 1.20000, 1.44000, 1.72800,
        2.07360, 2.48832, 2.98598, 3.58318, 4.29982, 5.15978, 6.19174};
    
    const Size frame_size = frame.size();
    
    //sliding window for every scale
    int count = sizeof(scales)/sizeof(scales[0]);
    for (int i = 0; i < count; i++)
    {
        //scale
        const float scale = scales[i];
        
        //box size for current scale
        int width = round(frame_size.width * scale);
        int height = round(frame_size.height * scale);
        
        //valid check
        if (is_invalid(width, height, frame_size, s_min_size))
        {
            continue;
        }
        
        //save new bbox
        Size scaled_bbox = Size(width, height);
        scale_list.push_back(scaled_bbox);
        
        //step for current scale
        int step = round(min(width, height) * 0.1);
        
        //slide bbox on the frame, drop the bbox if smaller than 'step'
        for (int y = 0; y + step < frame_size.height; y += step)
        {
            for (int x = 0; x + step < frame_size.width; x += step)
            {
                grid_list.push_back(Rect(x, y, width, height));
            }
        }
    }
}
