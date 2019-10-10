#include <stdint.h>
#include "Arduino.h"
#include "Ticker.h"
#include "ESP8266WiFi.h"
// #include "ESP8266WebServer.h"
#include "core.h"
#include "my_secrets.h" // <-- here goes the SSID and WiFi passphrase

const uint32_t BUTTON_1 = D1;
const uint32_t BUTTON_2 = D2;
const uint32_t LED_1 = D5;
const uint32_t LED_2 = D6;
const uint32_t FLASH_DURATION = 200; // ms
const IPAddress IP(192, 168, 10, 101);
const IPAddress SUBNET(255, 255, 255, 0);
Ticker blocker;
bool writingAllowed = true; // TODO make class

void allowWriting() {
    writingAllowed = true;
    blocker.detach();
}

void denyWriting() {
    writingAllowed = false;
    blocker.attach(1.0, allowWriting);
}

void printA0() {
    if (writingAllowed) {
        Serial.println((analogRead(A0) * 3.3 ) / (4095));
        denyWriting();
    }
}

void processInput() {
    if (digitalRead(BUTTON_1)) {
        // do something
    } else {
        // do something
    }
    if (digitalRead(BUTTON_2)) {
        // do something
    } else {
        // do something
    }
}

void checkClients() {
    if (WiFi.softAPgetStationNum() > 0) {
        digitalWrite(LED_2, HIGH);
    } else {
        digitalWrite(LED_2, LOW);
    }
}

void setup() {
    serial::init();
    onboard_led::init();
    pinMode(BUTTON_1, INPUT);
    pinMode(BUTTON_2, INPUT);
    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);
    WiFi.softAPConfig(IP, IP, SUBNET);
    if (WiFi.softAP(RC_SSID, RC_WLAN_PASSWORD, 8 /* channel */, true /* hidden SSID */, 2 /* max connections */)) {
        Serial.println("WiFi on.");
        Serial.print("Soft-AP IP address = ");
        Serial.println(WiFi.softAPIP());
        Serial.print("MAC address = ");
        Serial.println(WiFi.softAPmacAddress().c_str());
    } else {
        Serial.println("WiFi creation failed.");
    }
}

void loop() {
    checkClients();
    processInput();
}
