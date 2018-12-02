//
//  tld.hpp
//  scv
//
//  Created by samuelsong on 2018/11/25.
//  Copyright © 2018 samuelsong. All rights reserved.
//

#ifndef tld_hpp
#define tld_hpp

#include <vector>
#include <stdio.h>
#include "scvdef.h"
#include "frame.hpp"
#include "types.hpp"

using namespace scv;
using namespace std;

class SCV_EXPORTS TLD
{
public:
    /*multiple-scale, sliding window, with scale = 1.2 and step = 0.1*/
    void buildGrids(const Frame &frame, const Rect &box);
    
private:
    vector<Size> scale_list; //multiple scale from 1.2^-10 ~ 1.2^10[1/6 ~ 6]
    vector<Rect> grid_list; //all sliding window
};

#endif /* tld_hpp */
