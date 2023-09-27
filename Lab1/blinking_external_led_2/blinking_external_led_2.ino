#define LED_PIN 13
#define LED_PIN2 12

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(LED_PIN2, LOW);
  Serial.println("ON");
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(LED_PIN2, HIGH);
  Serial.println("OFF");
  delay(1000);
}
