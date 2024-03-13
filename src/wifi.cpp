#include "wifi.h"

IPAddress local_IP(192, 168, 1, 200);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(1, 1, 1, 1);
IPAddress dns2(1, 0, 0, 0);


String eSSID; 
String ePass; 
String bssid;
AsyncWebServer server(80);
const char* PARAM_MESSAGE = "message";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");




void initServer()
{
    // eSSID = std::getenv("ESP_WIFI_SSID");
    // ePass = std::getenv("ESP_WIFI_PASSWORD");
    eSSID = WIFI_SSID;
    ePass = WIFI_PASSWORD;
    
    // bssid = std::getenv("BSSID");

    debug("SSID");
    debugln(eSSID);
    debug("pass");
    debugln(ePass);
    
    if(getState() == STATE_WIFI_OFF)
    {
        WiFi.mode(WIFI_OFF);
        return;
    }

    if(!SPIFFS.begin()){
        debugln("An Error has occurred while mounting SPIFFS");
        return;
    }
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(eSSID.c_str(), ePass.c_str());
    if (!WiFi.config(local_IP, gateway, subnet, dns, dns2)) {
        debugln("STA Failed to configure");
    }
    int c =0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(10);
        
        if(c%500 == 0){
            debugln("Connecting to WiFi...");
        }
        c++;
    }
    syncTime();
    
    //we dont need 
    if(getState() == STATE_WIFI_SYNC)
        return;

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", "text/html");
    });
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/style.css", "text/css");
    });
    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/script.js", "text/javascript");
    });

    // Send a GET request to <IP>/data?message=<message> [template]
    server.on("/data", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String message;
        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "ok" + message);
    });

    // //upload file? err index file uploads but doesnt get replaced
    // server.onFileUpload([](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final){
    //     if(!index)
    //     Serial.printf("UploadStart: %s\n", filename.c_str());
    //     // Serial.printf("%s", (const char*)data);
    //     if(final)
    //         Serial.printf("UploadEnd: %s (%u)\n", filename.c_str(), index+len);
    //     request->send(200, "text/plain", "ok");
    // });
    
    //update mx on webpage
    server.on("/mx-update", HTTP_GET, [] (AsyncWebServerRequest *request) {
        JsonDocument updates = getLEDupdates(false);
        AsyncResponseStream *response = request->beginResponseStream("aplication/json");
        serializeJson(updates, *response);
        request->send(response);
    });
    server.on("/mx-update-force", HTTP_GET, [] (AsyncWebServerRequest *request) {
        debugln("client forced update");
        JsonDocument updates = getLEDupdates(true);
        AsyncResponseStream *response = request->beginResponseStream("aplication/json");
        serializeJson(updates, *response);
        request->send(response);
    });


    server.on("/set", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String message;
        if (request->hasParam("led") && request->hasParam("r") && 
            request->hasParam("g") && request->hasParam("b")) 
        {
            message = request->getParam("led")->value();
            byte led =  message.toInt();

            message = request->getParam("r")->value();
            byte red =  message.toInt();

            message = request->getParam("g")->value();
            byte green =  message.toInt();

            message = request->getParam("b")->value();
            byte blue =  message.toInt();

            setLED(led, red, green, blue, 1);
        }
        else if(request->hasParam("text"))
        {
            message = request->getParam("text")->value();
            drawText(message);
        }
        else if(request->hasParam("brightness"))
        {
            message = request->getParam("brightness")->value();
            int val = message.toInt();
            if(val>=0 && val<=255)
                setBrightness(val);
        }
        else
        {
            message = "param error";
        }

        request->send(200, "text/plain", "ok");
    });

    // Send a POST request to <IP>/post with a form field message set to <message> 
    server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
        String message;
        if (request->hasParam(PARAM_MESSAGE, true)) {
            message = request->getParam(PARAM_MESSAGE, true)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, POST: " + message);
    });
    
    server.onNotFound(notFound);
    server.begin();
    debugln("Server started");
    
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void syncTime()
{
    timeClient.begin();
    timeClient.setTimeOffset(-10800); //GMT -3
    int c = 0;
    while(!timeClient.update())
    {
        delay(10);
        
        if(c%500 == 0)
            debugln("Time sync failed, Retrying...");
        c++;
    }

    int currentHour = timeClient.getHours();
    debug("Time: ");
    debug(currentHour);
    debug(":");
    //String t = "Time: "+currentHour;  

    int currentMinute = timeClient.getMinutes();
    debug(currentMinute);
    debug(":");
    
    int currentSecond = timeClient.getSeconds();
    debugln(currentSecond);

    setTime(currentHour, currentMinute, currentSecond);
}