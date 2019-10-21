#include <cstdint>
#include <list>
#include "Arduino.h"
#include "painlessMesh.h"
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
const uint32_t MESH_PORT = 5555;
bool inputAvailable = false;
bool clientsConnected = false;
painlessMesh network;

void processInput() {
    BUTTON_1.update();
    BUTTON_2.update();
    SPEED.update();
    inputAvailable = BUTTON_1.state() || BUTTON_2.state();
}

void checkClients() {
    std::list<uint32_t> nodes = network.getNodeList();
    clientsConnected = (nodes.size() > 0);
    digitalWrite(LED_2, clientsConnected ? HIGH : LOW);
}

void sendData() {
    digitalWrite(LED_1, HIGH);
    String message = "";
    message += "d1=";
    message += BUTTON_1.state() ? "1" : "0";
    message += ":d2=";
    message += BUTTON_2.state() ? "1" : "0";
    message += ":a=";
    message += SPEED.value();
    network.sendBroadcast(message);
    digitalWrite(LED_1, LOW);
}

void receivedCallback(uint32_t from, String &msg) {
    Serial.printf("startHere: Received from %d msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(bool adopt) {
    Serial.printf("startHere: New Connection, adopt=%d\n", adopt);
}

void setup() {
    serial::init();
    onboard_led::init();
    BUTTON_1.init();
    BUTTON_2.init();
    SPEED.init();
    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);
    network.setDebugMsgTypes(ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE); // all types on
    //network.setDebugMsgTypes(ERROR | STARTUP);  // set before init() so that you can see startup messages
    network.init(RC_SSID, RC_WLAN_PASSWORD, MESH_PORT);
    network.onReceive(&receivedCallback);
    network.onNewConnection(&newConnectionCallback);
}

void loop() {
    network.update();
    checkClients();
    processInput();
    if (clientsConnected) {
        if (inputAvailable) sendData();
    }
}
