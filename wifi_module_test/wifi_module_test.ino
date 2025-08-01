// No SoftwareSerial needed

void setup() {
  Serial.begin(9600);       // PC Serial Monitor
  Serial1.begin(9600);      // ESP-01 on TX1/RX1 (Mega pins 18, 19)

  delay(2000);
  Serial1.println("AT");
}

void loop() {
  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }

  if (Serial.available()) {
    Serial1.write(Serial.read());
  }
}
