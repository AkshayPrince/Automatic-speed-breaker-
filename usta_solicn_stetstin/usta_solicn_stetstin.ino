#include <ESP32Servo.h>

// Define Servos
Servo servo1;  // Servo 1 for opening
Servo servo2;  // Servo 2 for closing

// Define LEDs
const int led1 = 22; // Green LED for authorized
const int led2 = 23; // Red LED for unauthorized

// Define Ultrasonic Sensor Pins
const int trigPin = 4;    // HC-SR04 Trigger Pin
const int echoPin = 5;    // HC-SR04 Echo Pin

void setup() {
  Serial.begin(115200);  // High baud rate for fast communication

  // Attach servos
  servo1.attach(19);
  servo2.attach(18);

  // Set initial Servo Position
  servo1.write(180);  // Closed position
  servo2.write(0);

  // Set LED pins as outputs
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  // Set Ultrasonic Pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.println("ESP32 Ready with Ultrasonic Sensor");
}

void blinkLED(int ledPin) {
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
}

// Function to get distance from Ultrasonic Sensor
float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distance = (duration * 0.0343) / 2;  // Convert to cm
  return distance;
}

// Function to check for object and close the breaker
void checkForObjectAndClose() {
  // Check if an object is within 20 cm and close immediately
  float distance = getDistance();
  if (distance <= 20) {
    Serial.println("Object detected, closing breaker.");
    servo1.write(180);  // Close breaker
    servo2.write(0);
  }
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();  // Read only one character

    if (command == '1') {
      servo1.write(0);  // Open breaker
      servo2.write(180);
      Serial.println("Access Granted: Breaker Opened");

      // Blink Green LED
      blinkLED(led1);
      blinkLED(led2);

      // Keep checking for object while the breaker is open
      while (true) {
        float distance = getDistance(); // Continuously check distance

        if (distance <= 20) {
          Serial.println("Object detected, closing breaker.");
          servo1.write(180);  // Close breaker
          servo2.write(0);
          break;  // Exit the loop after closing the breaker
        }
        delay(10);  // Check every 100ms
      }

    } 
    else if (command == '2') {
      servo1.write(180);  // Keep breaker closed
      servo2.write(0);
      Serial.println("Access Denied: Breaker Closed");

      // Blink Red LED
      blinkLED(led1);
      blinkLED(led2);
    }
  }
}
