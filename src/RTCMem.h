#pragma once

#include <RTCMemory.h>
#include "common.h"
#include "led.h"

#define SLEEP_DURATION 6e6

typedef struct 
{
    byte r;
    byte g;
    byte b;
}RTCLED;


typedef struct 
{
    byte state;
    byte mode;
    byte hours;
    byte minutes;
    byte seconds;
    byte brightness;
    float execTime;
    char text[LED_COUNT];
    RTCLED leds[LED_COUNT];
}RTCData;



typedef struct 
{
    byte a;
    
}RTCTest;

typedef enum
{
    STATE_ERROR,
    STATE_WIFI_SYNC,
    STATE_WIFI_OFF,
    STATE_APP
}state;

typedef enum
{
    MODE_ERROR,
    MODE_TIX,
    MODE_CLOCK,
    MODE_TEXT,
    MODE_FREE_COLOR
}mode;


void initRTCMem();
void setTime(byte hours, byte minutes, byte seconds);
void setState(byte state);

RTCData* getRTCData();
byte getState();
byte getHours();
byte getMinutes();
byte getSeconds();

void checkStateChange(); 
