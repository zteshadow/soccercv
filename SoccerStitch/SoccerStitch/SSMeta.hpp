//
//  SSMeta.hpp
//  SoccerStitch
//
//  Created by samuel on 24/02/2018.
//  Copyright © 2018 baidu. All rights reserved.
//

#ifndef SSMeta_hpp
#define SSMeta_hpp

#include <sys/types.h>

class SSMeta
{
public:
    //返回mov文件的创建时间, <= 0 error
    //[注意]这个时间是从***1904年1月1日***开始的秒数
    static long getCreateTime(const char *movFile);

    static int getImageMeta(const char *fname);
    static int getVideoMeta(const char *fname);
};

#endif /* SSMeta_hpp */
