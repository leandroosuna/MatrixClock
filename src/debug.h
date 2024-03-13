#ifndef _DEBUG_H
#define _DEBUG_H

#define DEBUG true

#if DEBUG
    #define debug(x) Serial.print(x)
    #define debugln(x) Serial.println(x)
#else
    #define debug(x)
    #define debugln(x)
#endif

#endif