// Two Button Test with Internal Pull-ups
// No external resistors needed

const int button1Pin = 26;  // Button 1 connected to pin 2
const int button2Pin = 27;  // Button 2 connected to pin 3

void setup() {
  Serial.begin(9600);

  // Enable internal pull-up resistors
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);

  Serial.println("Button Test Ready.");
}

void loop() {
  // Read button states (LOW = pressed, HIGH = released)
  bool button1Pressed = (digitalRead(button1Pin) == LOW);
  bool button2Pressed = (digitalRead(button2Pin) == LOW);

  Serial.print("Button 1: ");
  Serial.print(button1Pressed ? "Pressed" : "Released");
  Serial.print(" | Button 2: ");
  Serial.println(button2Pressed ? "Pressed" : "Released");

  delay(200); // short delay for readability
}
