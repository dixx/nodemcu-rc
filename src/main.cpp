#include <stdint.h>
#include "Arduino.h"
#include "Ticker.h"
#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"
#include "core.h"
#include "analog_input.h"
#include "digital_input.h"
#include "my_secrets.h" // <-- here goes RC_SSID and RC_WLAN_PASSWORD

DigitalInput BUTTON_1(D1);
DigitalInput BUTTON_2(D2);
AnalogInput SPEED(A0);
const uint32_t LED_1 = D5;
const uint32_t LED_2 = D6;
const uint32_t FLASH_DURATION = 200; // ms
const IPAddress IP(192, 168, 10, 101);
const IPAddress SUBNET(255, 255, 255, 0);
const String CLIENT_URL("http://192.168.10.73/");
Ticker blocker;
bool keepAlive = true; // TODO make class
bool inputChanged = false;
bool clientsConnected = false;
String data = "";

void allowSendingKeepAlive() {
    keepAlive = true;
    blocker.detach();
}

void denySendingKeepAlive() {
    keepAlive = false;
    blocker.attach(3.0, allowSendingKeepAlive);
}

void sendKeepAlive() {
    if (!keepAlive) return;

    WiFiClient client;
    HTTPClient http;
    http.begin(client, CLIENT_URL + "keep-alive");
    Serial.print("keep alive: ");
    Serial.print(http.GET());
    Serial.print(", ");
    Serial.println(http.getString());
    http.end();
    denySendingKeepAlive();
}

void processInput() {
    BUTTON_1.update();
    BUTTON_2.update();
    SPEED.update();
    inputChanged = BUTTON_1.hasChanged() || BUTTON_2.hasChanged();
}

void checkClients() {
    clientsConnected = (WiFi.softAPgetStationNum() > 0);
    digitalWrite(LED_2, clientsConnected ? HIGH : LOW);
}

void sendData() {
    WiFiClient client;
    client.setTimeout(200);
    HTTPClient http;
    http.begin(client, CLIENT_URL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    data = "rc";
    data += "?d1=";
    data += BUTTON_1.state() ? '1' : '0';
    data += "&d2=";
    data += BUTTON_2.state() ? '1' : '0';
    data += "&a=";
    data += SPEED.value();
    Serial.print("Send: ");
    Serial.print(CLIENT_URL);
    Serial.println(data);
    Serial.print("HTTP status code: ");
    Serial.println(http.POST(data));
    Serial.print("HTTP returned: ");
    Serial.println(http.getString());
    http.end();
}

void setup() {
    serial::init();
    onboard_led::init();
    BUTTON_1.init();
    BUTTON_2.init();
    SPEED.init();
    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);
    WiFi.mode(WIFI_AP);
    WiFi.setSleepMode(WIFI_NONE_SLEEP);
    WiFi.softAPConfig(IP, IP, SUBNET);
    if (WiFi.softAP(RC_SSID, RC_WLAN_PASSWORD, 8 /* channel */, true /* hidden SSID */, 2 /* max connections */)) {
        Serial.print("WiFi on: ");
        Serial.println(WiFi.softAPIP());
    } else {
        Serial.println("WiFi creation failed.");
    }
}

void loop() {
    checkClients();
    processInput();
    if (clientsConnected) {
        sendKeepAlive();
        if (inputChanged) sendData();
    }
}
