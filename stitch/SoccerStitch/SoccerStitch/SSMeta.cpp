//
//  SSMeta.cpp
//  SoccerStitch
//
//  Created by samuel on 24/02/2018.
//  Copyright © 2018 baidu. All rights reserved.
//

#include "SSMeta.hpp"
#include <exiv2/exiv2.hpp>
#include <exiv2/quicktimevideo.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace Exiv2;
using namespace std;

long SSMeta::getCreateTime(const char *movFile)
{
    long value = -1;
    
    BasicIo::AutoPtr data = ImageFactory::createIo(movFile);
    if(data.get() == 0)
    {
        cout<<"read file error."<<endl;
        return -1;
    }
    
    QuickTimeVideo *video = new QuickTimeVideo(data);
    if (video)
    {
        video->readMetadata();
        
        XmpData xmp = video->xmpData();
        if (xmp.empty())
        {
            cout<<"xmp empty error"<<endl;
        }
        else
        {
            Xmpdatum item = xmp["Xmp.video.DateUTC"];
            value = item.toLong();
        }
    }

    return value;
}

int SSMeta::getImageMeta(const char *fname)
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

int SSMeta::getVideoMeta(const char *fname)
{
    BasicIo::AutoPtr data = ImageFactory::createIo(fname);
    if(data.get() == 0)
    {
        cout<<"read file error."<<endl;
        return -1;
    }
 
    QuickTimeVideo *video = new QuickTimeVideo(data);
    cout<<"open ok: "<<video->good()<<endl;
    
    video->readMetadata();
    
    ExifData exif = video->exifData();
    if(exif.empty())
    {
        cout<<"exif empty error!"<<endl;
    }
    else
    {
        cout<<exif["Exif.Image.GPSTag"]<<endl;
        cout<<exif["Exif.GPSInfo.GPSLatitudeRef"]<<endl;
        cout<<exif["Exif.GPSInfo.GPSLatitude"]<<endl;
        cout<<exif["Exif.GPSInfo.GPSDateStamp"]<<endl;
        cout<<exif["Exif.Image.DateTime"]<<endl;
    }
    
    IptcData iptc = video->iptcData();
    if (iptc.empty())
    {
        cout<<"iptc empty error!"<<endl;
    }
    else
    {
    #if 0
        cout<<iptc["Exif.Image.GPSTag"]<<endl;
        cout<<iptc["Exif.GPSInfo.GPSLatitudeRef"]<<endl;
        cout<<iptc["Exif.GPSInfo.GPSLatitude"]<<endl;
        cout<<iptc["Exif.GPSInfo.GPSDateStamp"]<<endl;
        cout<<iptc["Exif.Image.DateTime"]<<endl;
    #endif
    }

    XmpData xmp = video->xmpData();
    if (xmp.empty())
    {
        cout<<"xmp empty error"<<endl;
    }
    else
    {
        cout<<"data count: "<<xmp.count()<<endl;
        for (Exiv2::XmpData::const_iterator md = xmp.begin(); md != xmp.end(); ++md)
        {
            std::cout << std::setfill(' ') << std::left
            << std::setw(44)
            << md->key() << " "
            << std::setw(9) << std::setfill(' ') << std::left
            << md->typeName() << " "
            << std::dec << std::setw(3)
            << std::setfill(' ') << std::right
            << md->count() << "  "
            << std::dec << md->value()
            << std::endl;
        }
    }
    
    return 0;
}
