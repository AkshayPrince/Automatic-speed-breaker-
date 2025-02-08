#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>


#define BUTTON_PIN 4  // Button connected to GPIO 4

uint8_t receiverAddress[] = {0x8C, 0x4F, 0x00, 0x12, 0x09, 0xE4};

// Data structure for sending message
typedef struct {
    bool ledState;
} Message;

Message dataToSend;
bool lastButtonState = HIGH;

void sendData(bool state) {
    dataToSend.ledState = state;
    esp_now_send(receiverAddress, (uint8_t*)&dataToSend, sizeof(dataToSend));
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);

    // Reduce Transmit Power to Limit Range
    esp_wifi_set_max_tx_power(78);  // Lower WiFi power (Max = 78)

    // ESP-NOW Initialization
    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW Initialization Failed");
        return;
    }

    // Register peer (Receiver)
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, receiverAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }

    pinMode(BUTTON_PIN, INPUT_PULLUP);  // Internal pull-up resistor
}

void loop() {
    bool buttonState = digitalRead(BUTTON_PIN);
    
    // Detect button press (debounced)
    if (buttonState == LOW && lastButtonState == HIGH) {
        delay(50);  // Simple debounce
        if (digitalRead(BUTTON_PIN) == LOW) {
            dataToSend.ledState = !dataToSend.ledState;  // Toggle LED state
            sendData(dataToSend.ledState);
            Serial.print("Sent LED State: ");
            Serial.println(dataToSend.ledState);
        }
    }
    
    lastButtonState = buttonState;
}
