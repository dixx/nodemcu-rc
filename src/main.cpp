#include <cstdint>
#include <list>
#include "Arduino.h"
#include "painlessMesh.h"
#include "core.h"
#include "my_secrets.h" // <-- here goes RC_SSID and RC_WLAN_PASSWORD

const uint32_t BUTTON_1 = D1;
const uint32_t BUTTON_2 = D2;
const uint32_t SPEED = A0;
const uint32_t LED_1 = D5;
const uint32_t LED_2 = D6;
bool clientsConnected = false;
bool inputAvailable = false;
std::list<uint32_t> nodes;
Scheduler userScheduler;
painlessMesh network;

void receivedCallback(uint32_t from, String& msg) {
    if (msg.equals("·")) {
        Serial.print("KeepAlive received from ");
        Serial.println(from);
    } else {
        Serial.printf("Received from %u: msg=%s\n", from, msg.c_str());
    }
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
    nodes = network.getNodeList();
    clientsConnected = (nodes.size() > 0);
    digitalWrite(LED_2, clientsConnected ? HIGH : LOW);
    Serial.printf("Changed connections, nodes: %d\n", nodes.size());
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", network.getNodeTime(), offset);
}

void delayReceivedCallback(uint32_t from, int32_t delay) {
    Serial.printf("Delay to node %u is %d us\n", from, delay);
}

void sendData() {
    if (!inputAvailable) { return; }

    digitalWrite(LED_1, HIGH);
    String message = "";
    message += "d1=";
    message += digitalRead(BUTTON_1) ? "1" : "0";
    message += ":d2=";
    message += digitalRead(BUTTON_2) ? "1" : "0";
    message += ":a=";
    message += analogRead(SPEED);
    network.sendBroadcast(message);
    digitalWrite(LED_1, LOW);
}

Task taskSendData(500, TASK_FOREVER, &sendData);

Task taskSendKeepAlive(TASK_SECOND * 3 , TASK_FOREVER, []() {
    onboard_led::on();
    network.sendBroadcast("·");
    onboard_led::off();
});

void setup() {
    serial::init();
    onboard_led::init();
    pinMode(BUTTON_1, INPUT);
    pinMode(BUTTON_2, INPUT);
    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);
    network.setDebugMsgTypes(ERROR | DEBUG);
    network.init(RC_SSID, RC_WLAN_PASSWORD, &userScheduler);
    network.onReceive(&receivedCallback);
    network.onNewConnection(&newConnectionCallback);
    network.onChangedConnections(&changedConnectionCallback);
    network.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
    network.onNodeDelayReceived(&delayReceivedCallback);
    userScheduler.addTask(taskSendKeepAlive);
    taskSendKeepAlive.enable();
    userScheduler.addTask(taskSendData);
    taskSendData.enable();
}

void loop() {
    network.update();
    inputAvailable = (clientsConnected && (digitalRead(BUTTON_1) || digitalRead(BUTTON_2)));
}
