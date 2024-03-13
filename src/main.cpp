#include "main.h"


void setup() {
    if(DEBUG)
        Serial.begin(115200);
    debugln("\nESP started");
    
    initLED();
    
    initRTCMem();
    
    initServer();

    cfgLED();

    checkStateChange();    
    
}
void loop() {

    rainbowTick();
    delay(200);
}
