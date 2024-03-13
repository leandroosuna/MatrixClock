#include "led.h"
#include "chars.h"

Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_RGB + NEO_KHZ800);

LED leds[LED_COUNT];
RTCData* rtcData;

void initLED()
{
    strip.begin();
    strip.show();
    strip.setBrightness(5);
    debugln("LED started");

    // for(int i = 0; i<LED_COUNT; i++)
    // {
    //     leds[i].r = 0;
    //     leds[i].g = 0;
    //     leds[i].b = 0;   
    // }
    //type
    //0: RGB, 1: GRB
    for(int i = 0; i<LED_COUNT; i++)
    {
        leds[i].type = 1;
    }

    
}
void LEDdataFromMem()
{
    for(int i = 0; i < LED_COUNT; i++)
    {
        leds[i].r = rtcData->leds[i].r;
        leds[i].g = rtcData->leds[i].g;
        leds[i].b = rtcData->leds[i].b;
    }
}
void LEDdataToMem()
{
    for(int i = 0; i<LED_COUNT; i++)
    {
        rtcData->leds[i].r = leds[i].r;
        rtcData->leds[i].g = leds[i].g;
        rtcData->leds[i].b = leds[i].b;
    }
}
void cfgLED()
{
    rtcData = getRTCData();
    strip.setBrightness(rtcData->brightness);
    
    if(rtcData->mode == MODE_TIX)
    {
        debugln("[TIX mode]");
        LEDdataFromMem();
    }
    else if(rtcData->mode == MODE_CLOCK)
    {
        debugln("[Clock mode]");
        drawClock();
    }
    else if(rtcData->mode == MODE_TEXT)
    {
        debugln("[Text mode]");
        drawTextFromMem();
    }
    else if(rtcData->mode == MODE_FREE_COLOR)
    {
        LEDdataFromMem();
    }
}
void showLED()
{
    strip.show();
}

void setStrip(byte r, byte g, byte b, bool show)
{
    for(int i = 0; i<LED_COUNT; i++)
    {
        setLED(i, r,g,b, 0);
    }
    if(show)
        showLED();
}
void setLED(int l, byte r, byte g, byte b, bool show)
{
    if(leds[l].r != r || leds[l].g != g || leds[l].b != b)
    {
        leds[l].r = r;
        leds[l].g = g;
        leds[l].b = b;
        leds[l].changed = true;
    }
    switch(leds[l].type)
    {
        case 0:strip.setPixelColor(l,strip.Color(leds[l].r, leds[l].g, leds[l].b));break;
        case 1:strip.setPixelColor(l,strip.Color(leds[l].g, leds[l].r, leds[l].b));break;
    }
    if(show)
    {
        strip.show();
    }
}

std::tuple<uint8_t,uint8_t,uint8_t> extractRGB(uint32_t color)
{
    uint8_t r = (uint8_t)(color >> 16);
    uint8_t g = (uint8_t)(color >> 8);
    uint8_t b = (uint8_t)color;

    return std::make_tuple(r,g,b);
}
void setLED(int l, uint32_t color, bool show)
{
    std::tuple<byte,byte,byte> extracted = extractRGB(color);
    byte r = std::get<0>(extracted);
    byte g = std::get<1>(extracted);
    byte b = std::get<2>(extracted);

    if(leds[l].r != r || leds[l].g != g || leds[l].b != b)
    {
        leds[l].r = r;
        leds[l].g = g;
        leds[l].b = b;
        leds[l].changed = true;
    }
    switch(leds[l].type)
    {
        case 0:strip.setPixelColor(l,strip.Color(leds[l].r, leds[l].g, leds[l].b));break;
        case 1:strip.setPixelColor(l,strip.Color(leds[l].g, leds[l].r, leds[l].b));break;
    }
    if(show)
    {
        strip.show();
    }
}

void drawText(String message)
{
    drawText(message, 255,255,255, 0);
}
void drawText(String message, byte r, byte g, byte b, byte offset)
{
    message.toLowerCase();

    byte charOffset = 0;
    for(byte ch = 0; ch<message.length();ch++)
    {
        char c = message.charAt(ch);
        drawChar(c,r,g,b,charOffset); 
        charOffset+=4;
    }
    
}

 
void drawChar(char c, byte r, byte g, byte b, byte offset)
{
    byte indexCol[5]; 
    byte x;    
    byte red[5];
    byte green[5];
    byte blue[5];
    for(byte i = 0; i < 3; i++)
    {
        x = offset + i;
        //correct for matrix order inversion in rows 1 and 3
        indexCol[0] = x;
        indexCol[1] = 33 - x;
        indexCol[2] = x + 34;
        indexCol[3] = 67 - x;
        indexCol[4] = x + 68;

        for(byte row= 0; row<5; row++)
        {
            //get each row of the char to either set the correct color or black depending on the mx
            bool* charRow = rowOfChar(row, c);
            // debug("CH ");debug(row);debug(" ");debug(i);debug(" ");debug(charRow[i]);debug("\n");
            red[row] = charRow[i] * r;
            green[row] = charRow[i] * g;
            blue[row] = charRow[i] * b;
            // debug("rgb ");debug(red[row]);debug(",");debug(green[row]);debug(",");debug(blue[row]);debug("\n");
        }

        for(byte col = 0; col < 5; col++)
        {
            setLED(indexCol[col],red[col],green[col],blue[col],0);
        }
        
    }
    showLED();
}
uint16_t rainbowHue = 0;


void drawClock()
{
    debug("time ");
    debug(rtcData->hours);
    debug(":");
    debug(rtcData->minutes);
    debug(":");
    debugln(rtcData->seconds);
    
}

void drawTextFromMem()
{
    //save RGB for each char in leds[0],leds[1],leds[2]...
    //"test"
    //drawChar('t', led[0].rgb)
    //drawChar('e', led[1].rgb)
    //drawChar('s', led[2].rgb)
    //drawChar('t', led[3].rgb)
    // upto LED_COUNT chars
}

void rainbowTick() {
    byte r,g,b;
    std::tuple<uint8_t,uint8_t,uint8_t> extracted;

    for(int i = 0; i < LED_COUNT; i++)
    {
        //use hue in hsv to get easy rainbow effect, convert back to rgb values
        extracted = extractRGB(strip.ColorHSV(rainbowHue, 255,255));
        r = std::get<0>(extracted);
        g = std::get<1>(extracted);
        b = std::get<2>(extracted);

        setLED(i, r,g,b,false);
        rainbowHue+=50; //byte, will overflow back to 0 
    }
    showLED();

}
void setBrightness(byte val)
{
    strip.setBrightness(val);
}

JsonDocument getLEDupdates(bool force)
{
    
    JsonDocument json;
    JsonDocument update;    
    for(int i = 0; i < LED_COUNT; i++)
    {   
        if(leds[i].changed || force)
        {
            
            update["led"] = i;
            update["red"] = leds[i].r;
            update["green"] = leds[i].g;
            update["blue"] = leds[i].b;

            leds[i].changed = false;
            json.add(update);
        }
    }
   
    return json;
}
