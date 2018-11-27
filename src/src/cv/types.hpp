//
//  types.hpp
//  scv
//
//  Created by samuelsong on 2018/11/25.
//  Copyright © 2018 samuelsong. All rights reserved.
//

#ifndef types_hpp
#define types_hpp

#include <stdio.h>

namespace scv
{
template<typename _Tp> class Point_
{
    
};
typedef Point_<int> Point;

template<typename _Tp> class Rect_
{
    
};
typedef Point_<int> Rect;
}

template<typename _Tp> class Size_
{
public:
    Size_(void);
    Size_(_Tp _width, _Tp _height);
    
public:
    _Tp width; //!< the width
    _Tp height; //!< the height
};
typedef Size_<int> Size;

//////////////////////////////////// implementation //////////
template<typename _Tp> inline
Size_<_Tp>::Size_()
: width(0), height(0) {}

template<typename _Tp> inline
Size_<_Tp>::Size_(_Tp _width, _Tp _height)
: width(_width), height(_height) {}

#endif /* types_hpp */
