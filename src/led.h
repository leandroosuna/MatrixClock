#ifndef _LED_H
#define _LED_H

#include <Adafruit_NeoPixel.h>
#include "debug.h"
#include <ArduinoJson.h>
#include "RTCMem.h"

#define NEO_PIN   D3

void initLED();
void cfgLED();
void setLED(int l, byte r, byte g, byte b, bool show);
void setStrip(byte r, byte g, byte b, bool show);
void showLED();
void setBrightness(byte val);

void rainbowTick();
void colorWheel(byte led, byte wheelPos);

void drawChar(char c, byte r, byte g, byte b, byte offset);
void drawText(String message);
void drawText(String message, byte r, byte g, byte b, byte offset);
void drawClock();
void drawTextFromMem();
JsonDocument getLEDupdates(bool force);

struct LED
{
    
    byte r;
    byte g;
    byte b;
    byte type;
    bool changed;
};
#endif