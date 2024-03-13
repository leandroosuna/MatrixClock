#ifndef _WIFI_H
#define _WIFI_H

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "debug.h"
#include "led.h"

void initServer();
void notFound(AsyncWebServerRequest *request);
void saveFileToSPIFFS();
#endif