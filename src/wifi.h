#pragma once

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "common.h"
#include "led.h"
#include "RTCMem.h"
#include "credentials.h"
void initServer();
void syncTime();
void notFound(AsyncWebServerRequest *request);
