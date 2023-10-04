#define FIRST 10
#define SECOND 11
#define THIRD 12
#define FOURTH 13
#define POTENTIOMETER A0

int index = 0;
bool bounce = true;
int pins[] = {FIRST, SECOND, THIRD, FOURTH};
int lenght = 3;

void setup() {
  // put your setup code here, to run once:
  pinMode(FIRST, OUTPUT);
  pinMode(SECOND, OUTPUT);
  pinMode(THIRD, OUTPUT);
  pinMode(FOURTH, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly
  digitalWrite(pins[index], HIGH);
  delay(analogRead(POTENTIOMETER));
  digitalWrite(pins[index], LOW);
  if (index >= lenght) { bounce = true; }
  else if (index <= 0) { bounce = false; }
  index += bounce ? -1 : 1;
}
