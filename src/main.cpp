// https://www.esp8266.com/wiki/doku.php?id=ota-over-the-air-esp8266
// 2021-04-26


#include <Arduino.h>
#include "build_defs.h"
#include "ADXL.h"

#include <WiFi.h>
#include <WiFiMulti.h>

#include <HTTPClient.h>
#include <HTTPUpdate.h>

WiFiMulti WiFiMulti1;

void handleOTAProgress (size_t percent, size_t a100) {
    static int savedPercent = -1;

    int prc = (float(percent)*100.0)/a100;
    //if (prc > 98) prc = 100;
    
    if (savedPercent != prc) {
        savedPercent = prc;
        
        if (prc%4==0) {
            Serial.println (prc);
            fastledBlink ();
        }
    }
}

void setup () {
    Serial.begin (115200);
    Serial.println (TimestampedVersion);
    //Serial.setDebugOutput(true);

    Serial.println();

    // https://github.com/espressif/arduino-esp32/blob/a070884441308c5bfa6805d6f515801769ea0b1b/cores/esp32/Esp.h#L80
    // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/system.html
    Serial.print ("ESP32 SDK Ver:");
    Serial.println (esp_get_idf_version ());

    setupADXL ();

    for (uint8_t t = 3; t > 0; t--) {
        Serial.printf ("[OLD SETUP] WAIT %d...\n", t);
        Serial.flush ();
        delay (500);
    }

    WiFi.mode (WIFI_STA);
    WiFiMulti1.addAP ("TKT11", "13031968");
    WiFiMulti1.addAP ("CONSULTANT 2890", "48#24Qa2");
    // WiFiMulti1.addAP (WIFI_SSID, WIFI_PASS);

    //  do not reboot automatically when update is over (since we need to display message)
    httpUpdate.rebootOnUpdate (false);
    //  this callback is used to draw a horizontal progress line on the OTA screen
    Update.onProgress (handleOTAProgress);

    // wait for WiFi connection
    while ((WiFiMulti1.run () != WL_CONNECTED)) {
        Serial.print (" .");
    }
    if ((WiFiMulti1.run () == WL_CONNECTED)) {
        WiFiClient client;

        // httpUpdate.setLedPin(LED_BUILTIN, LOW);
        //t_httpUpdate_return ret = httpUpdate.update(client, "http://server/file.bin");
        t_httpUpdate_return ret = httpUpdate.update(client, String (UPDATE_SERVER_URL) + 
            "handle_update_VER.php", String(BIN_PROGNAME) + ".bin"+ " " + TimestampedVersion);
        // Or:
        //t_httpUpdate_return ret = httpUpdate.update(client, "server", 80, "/file.bin");

        switch (ret) {
        case HTTP_UPDATE_FAILED:
            Serial.printf ("JAN-NEW HTTP_UPDATE_FAILED Error (%d): %s\n", 
                httpUpdate.getLastError (), httpUpdate.getLastErrorString().c_str());
            break;

        case HTTP_UPDATE_NO_UPDATES:
            Serial.println("JAN-NEW HTTP_UPDATE_NO_UPDATES");
            break;

        case HTTP_UPDATE_OK:
            Serial.println("JAN-NEW HTTP_UPDATE_OK");
            delay (3000);
            ESP.restart ();
            break;
        }
    }
    fastledBlink (0);
}

void loop() {
    loopADXL ();
}
