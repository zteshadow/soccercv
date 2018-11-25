//
//  tld.hpp
//  scv
//
//  Created by samuelsong on 2018/11/25.
//  Copyright © 2018 samuelsong. All rights reserved.
//

#ifndef tld_hpp
#define tld_hpp

#include <stdio.h>
#include "scvdef.h"
#include "frame.hpp"
#include "types.hpp"

using namespace scv;

class SCV_EXPORTS TLD
{
public:
    void buildGrids(const Frame &frame, const Rect &box);
};

#endif /* tld_hpp */
