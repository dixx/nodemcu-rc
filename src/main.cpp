#include <cstdint>
#include <list>
#include "Arduino.h"
#include "painlessMesh.h"
#include "core.h"
#include "analog_input.h"
#include "digital_input.h"
#include "my_secrets.h" // <-- here goes RC_SSID and RC_WLAN_PASSWORD

// DigitalInput BUTTON_1(D1);
// DigitalInput BUTTON_2(D2);
// AnalogInput SPEED(A0);
// const uint32_t LED_1 = D5;
// const uint32_t LED_2 = D6;
bool inputAvailable = false;
bool clientsConnected = false;
std::list<uint32_t> nodes;
Scheduler userScheduler;
painlessMesh network;

void processInput() {
    // BUTTON_1.update();
    // BUTTON_2.update();
    // SPEED.update();
    // inputAvailable = BUTTON_1.state() || BUTTON_2.state();
}

// void checkClients() {
//     std::list<uint32_t> nodes = network.getNodeList();
//     clientsConnected = (nodes.size() > 0);
//     digitalWrite(LED_2, clientsConnected ? HIGH : LOW);
// }

void receivedCallback(uint32_t from, String& msg) {
    Serial.printf("Received from %u: msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
    Serial.printf("Changed connections, nodes: %d\n", network.getNodeList().size());
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", network.getNodeTime(), offset);
}

void delayReceivedCallback(uint32_t from, int32_t delay) {
    Serial.printf("Delay to node %u is %d us\n", from, delay);
}

void sendData() {
    //     digitalWrite(LED_1, HIGH);
    //     String message = "";
    //     message += "d1=";
    //     message += BUTTON_1.state() ? "1" : "0";
    //     message += ":d2=";
    //     message += BUTTON_2.state() ? "1" : "0";
    //     message += ":a=";
    //     message += SPEED.value();
    //     network.sendBroadcast(message);
    //     digitalWrite(LED_1, LOW);
}

Task taskSendKeepAlive(TASK_SECOND * 3 , TASK_FOREVER, []() {
    onboard_led::on();
    network.sendBroadcast( "·" );
    onboard_led::off();
});

void setup() {
    serial::init();
    onboard_led::init();
    // BUTTON_1.init();
    // BUTTON_2.init();
    // SPEED.init();
    // pinMode(LED_1, OUTPUT);
    // pinMode(LED_2, OUTPUT);
    network.setDebugMsgTypes(ERROR | DEBUG);
    network.init(RC_SSID, RC_WLAN_PASSWORD, &userScheduler);
    network.onReceive(&receivedCallback);
    network.onNewConnection(&newConnectionCallback);
    network.onChangedConnections(&changedConnectionCallback);
    network.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
    network.onNodeDelayReceived(&delayReceivedCallback);
    userScheduler.addTask(taskSendKeepAlive);
    taskSendKeepAlive.enable();

    randomSeed(analogRead(A0));
}

void loop() {
    network.update();
//     checkClients();
    processInput();
//     if (clientsConnected) {
//         if (inputAvailable) sendData();
//     }
}
