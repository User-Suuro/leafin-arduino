// pH Sensor Reading Script
const int pH_Pin = A0;            // Analog input pin for pH sensor
const float voltageAtPH7 = 2.5;   // Voltage at pH 7.0
const float slope = -5.0;         // Sensor slope (approximate)

void setup() {
  Serial.begin(9600);             // Start serial communication
  Serial.println("pH Sensor Test Initialized");
}

void loop() {
  int analogValue = analogRead(pH_Pin);                    // Read analog value
  float voltage = analogValue * (5.0 / 1023.0);            // Convert to voltage
  float pH = 7.0 + ((voltage - voltageAtPH7) * slope);     // Convert voltage to pH

  // Display the results
  Serial.print("Analog: ");
  Serial.print(analogValue);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 2);
  Serial.print(" V | pH: ");
  Serial.println(pH, 2);

  delay(1000);  // Wait 1 second before next reading
}
