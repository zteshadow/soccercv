//
//  frame.hpp
//  scv
//
//  Created by samuelsong on 2018/11/25.
//  Copyright © 2018 samuelsong. All rights reserved.
//

#ifndef frame_hpp
#define frame_hpp

#include <stdio.h>
#include "scvdef.h"
#include "types.hpp"

namespace scv
{
    
class SCV_EXPORTS Frame
{
public:
    Size size(void) const;
};

}

#endif /* frame_hpp */
