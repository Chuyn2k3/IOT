#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "main_define.h"

// Tạo client WiFi và MQTT
WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);

// Hàm thiết lập kết nối WiFi
void setupWiFi() {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" Connected!");
}



// Hàm kiểm tra và kết nối lại MQTT nếu mất kết nối
void reconnectMQTT() {
    while (!mqttClient.connected()) {
        Serial.print("Connecting to MQTT...");
        if (mqttClient.connect("ESP32_Client", MQTT_USER, MQTT_PASSWORD)) {  // Kết nối MQTT với thông tin người dùng
            Serial.println(" Connected to MQTT Server!");
            mqttClient.subscribe(MQTT_TOPIC);  // Đăng ký topic (nếu cần)
        } else {
            Serial.print(" Failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" retrying in 5 seconds...");
            delay(5000);
        }
    }
}

// Hàm publish dữ liệu lên MQTT
void publishData(const char *payload) {
    if (!mqttClient.connected()) {
        reconnectMQTT();
    }
    mqttClient.publish(MQTT_TOPIC, payload);
    Serial.print("Data published: ");
    Serial.println(payload);
}

// Hàm callback để xử lý các tin nhắn đến từ MQTT (nếu cần)
void mqttCallback(char *topic, byte *message, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++) {
        messageTemp += (char)message[i];
    }
    Serial.println(messageTemp);
}

// Hàm thiết lập kết nối MQTT
void setupMQTT() {
    espClient.setCACert(CERTIFICATE);             // Đặt chứng chỉ CA
    espClient.setCertificate(CLIENT_CERTIFICATE);  // Đặt chứng chỉ client
    espClient.setPrivateKey(PRIVATE_KEY);          // Đặt khóa riêng

    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);  // Đặt máy chủ và cổng MQTT
    mqttClient.setCallback(mqttCallback);          // Đặt hàm callback nếu cần nhận dữ liệu
    reconnectMQTT();                               // Kết nối đến MQTT Broker
}