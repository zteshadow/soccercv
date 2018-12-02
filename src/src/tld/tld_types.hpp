//
//  tld_types.hpp
//  scv
//
//  Created by samuelsong on 2018/11/25.
//  Copyright © 2018 samuelsong. All rights reserved.
//

#ifndef tld_types_hpp
#define tld_types_hpp

#include "types.hpp"

using namespace scv;

//Bounding Boxes
class BBox : public Rect
{
public:
    BBox(const Rect &r);

public:
    float overlap;        //Overlap with current Bounding Box
    int sidx;             //scale index
};

inline BBox::BBox(const Rect &r) : Rect(r) {}

#endif /* tld_types_hpp */
