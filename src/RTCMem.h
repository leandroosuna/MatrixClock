#include <RTCMemory.h>
#include "debug.h"

typedef struct 
{
    byte r;
    byte g;
    byte b;
}RTCLED;


typedef struct 
{
    byte state;
    byte hour;
    byte minutes;
    byte seconds;
    RTCLED leds[85];
}RTCData;



typedef struct 
{
    byte a;
    
}RTCTest;



void initRTCMem();