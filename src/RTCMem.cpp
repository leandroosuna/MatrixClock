#include "RTCMem.h"

RTCMemory<RTCData> rtcMemory;

RTCData *data;
void initRTCMem()
{
    if(rtcMemory.begin())
    {
        debugln("RTC data found");
        data = rtcMemory.getData();
        
        if(data->state == STATE_ERROR)
        {
            data->state = STATE_WIFI_SYNC;
        }
        if(data->mode == MODE_ERROR)
        {
            data->mode= MODE_CLOCK;
        }
        
        
        rtcMemory.save();

        
    }
    else
    {
        debugln("no RTC data found, init");

        data = rtcMemory.getData();
        data->state = STATE_WIFI_SYNC;
        data->mode= MODE_CLOCK;
        data->execTime = 0;
        rtcMemory.save();
    }

    debug("[State ");
    debug(data->state);
    debug("]\n");
}
byte getState(){return data->state;}
byte getHours(){return data->hours;}
byte getMinutes(){return data->minutes;}
byte getSeconds(){return data->seconds;}
RTCLED* getLEDdata(){return data->leds;}
RTCData* getRTCData(){return data;}
void setTime(byte hours, byte minutes, byte seconds)
{
    data->hours = hours;
    data->minutes = minutes;
    data->seconds = seconds;
    rtcMemory.save();
}
void setState(byte state)
{
    data->state = state;
    rtcMemory.save();
}

void checkStateChange()
{
    data->execTime += millis()/1000; 
    data->execTime += SLEEP_DURATION/1e6;
    debug("exec ");
    debugln(data->execTime);

    if(data->state != STATE_WIFI_SYNC)
    {
        //if accumulated exec time exceeds seconds left to minute 
        //(out of sync condition)
        if(data->execTime >= (60 - data->seconds))
        {
            setState(STATE_WIFI_SYNC);
            debugln("restarting to sync");
            data->execTime = 0;
            rtcMemory.save();
            ESP.deepSleep(SLEEP_DURATION, RF_DEFAULT);
        }
    }
    //otherwise, keep wifi off state
    setState(STATE_WIFI_OFF);
    ESP.deepSleep(SLEEP_DURATION, RF_DISABLED);
     
}