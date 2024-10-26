#include <Arduino.h>
#include "wifi_mqtt_setup.cpp"

void setup() {
    Serial.begin(115200);
    setupWiFi();
    setupMQTT();
}

void loop() {
    if (!mqttClient.connected()) {
        reconnectMQTT();
    }
    mqttClient.loop();
    publishData("30");
    // Thêm logic khác tại đây (ví dụ: gửi dữ liệu)
    delay(5000);
}
