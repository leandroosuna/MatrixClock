#include "RTCMem.h"

//RTCMemory<RTCData> rtcMemory;
RTCMemory<RTCTest> rtcMemory;

void initRTCMem()
{
    if(rtcMemory.begin())
    {
        debugln("RTC data found");
        RTCTest *data = rtcMemory.getData();
        data->a++;
        debugln(data->a);
        rtcMemory.save();
    }
    else
    {
        debugln("no RTC data found");

        RTCTest *data = rtcMemory.getData();
        data->a=1;
        rtcMemory.save();
    }

    
}