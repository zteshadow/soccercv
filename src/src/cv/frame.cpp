//
//  frame.cpp
//  scv
//
//  Created by samuelsong on 2018/11/25.
//  Copyright © 2018 samuelsong. All rights reserved.
//

#include "frame.hpp"

using namespace scv;

Size Frame::size() const
{
    Size size = Size(1024, 768);
    return size;
}
