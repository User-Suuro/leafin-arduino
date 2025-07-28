// Important: Set your Serial Monitor Line Ending to "Both NL & CR"
// Pins: RX (Arduino receives from ESP TX), TX (Arduino sends to ESP RX)

#include <SoftwareSerial.h>

SoftwareSerial espSerial(3,2); // RX, TX

void setup() {
  Serial.begin(9600);       // Monitor Serial
  espSerial.begin(9600);    // ESP-01S default baud rate

  delay(2000);
  espSerial.println("AT"); 

  Serial.println("ESP-01S AT Command Test Ready");
}

void loop() {
  // Forward data from ESP to Serial Monitor
  if (espSerial.available()) {
    Serial.write(espSerial.read());
  }

  // Forward data from Serial Monitor to ESP
  if (Serial.available()) {
    espSerial.write(Serial.read());
  }

  // Type AT in the Serial Monitor for Testing, it should return "OK"
}
