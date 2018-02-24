//
//  SSMeta.cpp
//  SoccerStitch
//
//  Created by samuel on 24/02/2018.
//  Copyright © 2018 baidu. All rights reserved.
//

#include "SSMeta.hpp"
#include <exiv2/exiv2.hpp>
#include <iostream>

using namespace Exiv2;
using namespace std;

int SSMeta::getImageTime(const char *fname)
{
    Image::AutoPtr image = ImageFactory::open(fname);
    if(image.get() == 0)
    {
        cout<<"read file error."<<endl;
        return -1;
    }
    image->readMetadata();
    ExifData ed = image->exifData();
    
    if(ed.empty())
    {
        cout<<"ed empty error!"<<endl;
        return -2;
    }
    
    //some tag location
    cout<<ed["Exif.Image.GPSTag"]<<endl;
    cout<<ed["Exif.GPSInfo.GPSLatitudeRef"]<<endl;
    cout<<ed["Exif.GPSInfo.GPSLatitude"]<<endl;
    cout<<ed["Exif.GPSInfo.GPSLongitudeRef"]<<endl;
    cout<<ed["Exif.GPSInfo.GPSLongitude"]<<endl;
    cout<<ed["Exif.GPSInfo.GPSAltitudeRef"]<<endl;
    cout<<ed["Exif.GPSInfo.GPSAltitude"]<<endl;
    cout<<ed["Exif.GPSInfo.GPSDateStamp"]<<endl;
    cout<<ed["Exif.Image.DateTime"]<<endl;
    
    return 0;
}

int SSMeta::getVideoTime(const char *fname)
{
    Image::AutoPtr image = ImageFactory::open(fname);
    if(image.get() == 0)
    {
        cout<<"read file error."<<endl;
        return -1;
    }
    image->readMetadata();
    ExifData ed = image->exifData();
    
    if(ed.empty())
    {
        cout<<"ed empty error!"<<endl;
        return -2;
    }
    
    //some tag location
    cout<<ed["Exif.Image.GPSTag"]<<endl;
    cout<<ed["Exif.GPSInfo.GPSLatitudeRef"]<<endl;
    cout<<ed["Exif.GPSInfo.GPSLatitude"]<<endl;
    cout<<ed["Exif.GPSInfo.GPSLongitudeRef"]<<endl;
    cout<<ed["Exif.GPSInfo.GPSLongitude"]<<endl;
    cout<<ed["Exif.GPSInfo.GPSAltitudeRef"]<<endl;
    cout<<ed["Exif.GPSInfo.GPSAltitude"]<<endl;
    cout<<ed["Exif.GPSInfo.GPSDateStamp"]<<endl;
    cout<<ed["Exif.Image.DateTime"]<<endl;
    
    return 0;
}

