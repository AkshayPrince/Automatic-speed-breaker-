#include <ESP32Servo.h>  // Include Servo library

Servo servo1;  // Servo 1 on GPIO18
Servo servo2;  // Servo 2 on GPIO19
const int led1 = 22; // LED 1 on GPIO22
const int led2 = 23; // LED 2 on GPIO23

void setup() {
  Serial.begin(115200);  // High baud rate for fast communication

  // Attach servos
  servo1.attach(18);
  servo2.attach(19);

  // Initial Servo Position
  servo1.write(180);
  servo2.write(0);

  // Set LED pins as outputs
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  Serial.println("ESP32 Ready");
}

void blinkLEDs(int ledPin) {
  digitalWrite(ledPin, HIGH);
  delay(500);  // LED ON for 500ms
  digitalWrite(ledPin, LOW);
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();  // Read only one character

    if (command == '1') {
      servo1.write(180);  
      servo2.write(0);    
      Serial.println("Access Granted: Servo1 180°, Servo2 0°");

      // Blink both LEDs once
      blinkLEDs(led1);
      blinkLEDs(led2);
    } 
    else if (command == '2') {
      servo1.write(0);  
      servo2.write(180);  
      Serial.println("Access Denied: Servo1 0°, Servo2 180°");

      // Blink both LEDs once
      blinkLEDs(led1);
      blinkLEDs(led2);
    }
  }
}
