//
//  utils.hpp
//  scv
//
//  Created by samuelsong on 2018/11/27.
//  Copyright © 2018 samuelsong. All rights reserved.
//

#ifndef utils_hpp
#define utils_hpp

#include <stdio.h>
#include "types.hpp"

//smaller than min or large than max is invalid
__inline bool is_invalid(int w, int h, Size max_size, int min)
{
    if (w < min || h < min || w > max_size.width || h > max_size.height)
    {
        return true;
    }
    else
    {
        return false;
    }
}

#endif /* utils_hpp */
