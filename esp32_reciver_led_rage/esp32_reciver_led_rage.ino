#include <esp_now.h>
#include <WiFi.h>

#define LED_PIN 2  // Built-in LED (GPIO 2)

typedef struct {
    bool ledState;
} Message;

Message receivedData;

// ✅ Updated function signature
void onReceiveData(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
    memcpy(&receivedData, incomingData, sizeof(receivedData));
    Serial.print("Received LED State: ");
    Serial.println(receivedData.ledState);

    digitalWrite(LED_PIN, receivedData.ledState);  // Turn LED ON/OFF
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);  // Start with LED OFF

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW Initialization Failed");
        return;
    }

    // ✅ Fix: Register receive callback with correct function signature
    esp_now_register_recv_cb(onReceiveData);
}

void loop() {
    // Nothing needed here, ESP-NOW automatically calls the callback function when data is received
}
