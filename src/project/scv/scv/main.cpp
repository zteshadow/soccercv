//
//  main.cpp
//  scv
//
//  Created by samuelsong on 2018/11/25.
//  Copyright © 2018 samuelsong. All rights reserved.
//

#include <iostream>
#include "tld.hpp"

using namespace scv;

int main(int argc, const char * argv[]) {
    // insert code here...
    
    TLD tld = TLD();
    Frame frame;
    Rect rect;
    
    tld.buildGrids(frame, rect);
    
    std::cout << "Hello, World!\n";
    return 0;
}
