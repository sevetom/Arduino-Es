#define YELLOW 13
#define GREEN 12
#define BLU 11
#define RED 10
#define POTENZIOMETRO A0

int switchValue = -1;
int count = YELLOW;
int delayTime;

void setup() {                
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLU, OUTPUT);
  pinMode(RED, OUTPUT);
  Serial.begin(115200);     
  Serial.println("READY"); 
  
}

void loop() {
  delayTime = analogRead(POTENZIOMETRO);
  digitalWrite(count, HIGH);
  delay(delayTime);
  digitalWrite(count, LOW);
  count = count+switchValue;
  if(count < 11 || count > 12){
    switchValue = -switchValue;
  }
       
}
