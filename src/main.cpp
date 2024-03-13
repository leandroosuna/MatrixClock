#include "main.h"


void setup() {
    if(DEBUG)
        Serial.begin(115200);
    
    debugln("\nESP started");

    initLED();
    debugln("LED started");

    initServer();
    debugln("Server started");
}

void loop() {

    rainbowTick();
    delay(200);
}
