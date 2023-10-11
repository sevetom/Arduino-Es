#define LED_PIN1 13
#define LED_PIN2 12
#define LED_PIN3 11
#define LED_PIN4 10
#define LED_ERRORPIN 9
#define BUTTON_PIN1 7
#define BUTTON_PIN2 6
#define BUTTON_PIN3 5
#define BUTTON_PIN4 4
#define POT_PIN A0
#define T1 3000
#define T_OUT 10000
#define N_LED 4

int score;
bool inGame;
bool endGame;
bool outGame;
int turnedOffOrder[4] = {0, 0, 0, 0};
int pressedOrder[4] = {0, 0, 0, 0};
int i;
int factor;
int t2;
int t3;

void setup() { 
  Serial.begin(9600);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_PIN3, OUTPUT);
  pinMode(LED_PIN4, OUTPUT);
  pinMode(LED_ERRORPIN, OUTPUT);
  score = 0;
  inGame = false;
  endGame = false;
  outGame = true; 
  i = 0;
  t2 = 4000;
  t3 = 5000;
  randomSeed(analogRead(4));
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN1), button1pressed, FALLING); 
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN2), button2pressed, FALLING); 
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN3), button3pressed, FALLING); 
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN4), button4pressed, FALLING); 
}

void loop() {
  //digitalWrite(LED_PIN1, HIGH);
  
  if (outGame) {
    //genero sequenza random
    randomizeOrder();
    //accendo tutti i led
    digitalWrite(LED_PIN1, HIGH);
    digitalWrite(LED_PIN2, HIGH);
    digitalWrite(LED_PIN3, HIGH);
    digitalWrite(LED_PIN4, HIGH);
    //spengo in base all'ordine randomizzato
    delay(1500);
    int led = 0;
    for (led = 0; led < N_LED; led++) {
      int current = turnedOffOrder[led];
      switch(current) {
        case 1:
          digitalWrite(LED_PIN1, LOW);
          break;
        case 2:
          digitalWrite(LED_PIN2, LOW);
          break;
        case 3:
          digitalWrite(LED_PIN3, LOW);
          break;
        case 4:
          digitalWrite(LED_PIN4, LOW);
          break;
      }
      delay(t2 / N_LED);
    }
    //parte timer
    outGame = false;
    inGame = true;
  } else {
    if (endGame) {
      //mostro led rosso
      digitalWrite(LED_ERRORPIN, HIGH);
      //mostro punteggio
      //leggo valore potenziometro
      factor = analogRead(POT_PIN);
      factor = map(factor, 0, 1023, 0, 4);
      //aspetta 10 sec
      delay(T_OUT);
      endGame = false;
      outGame = true;
      digitalWrite(LED_ERRORPIN, LOW);
    } else {
      if (inGame) {
        //controlla length array
        if (pressedOrder[3] != 0) {
          //stoppa timer
          for(int x = 0; x < N_LED ; x++) {
            if(pressedOrder[x] != turnedOffOrder[3-x]) {
              endGame = true;
            }
          }
          if(!endGame) {
            score++;
            //diminuisco tempi per difficoltà
          }
          outGame = true;
        }
      }
    }
  }
}

void button1pressed() {
  checkButton(1);
}

void button2pressed() {
  checkButton(2);
}

void button3pressed() {
  checkButton(3);
}

void button4pressed() {
  checkButton(4);
}

void checkButton(int n) {
  pressedOrder[i] = n;
}

void randomizeOrder() {
  int i = 1;
  while(i <= N_LED) {
    int choise = random(0, N_LED);
    if (turnedOffOrder[choise] == 0){
      turnedOffOrder[choise] = i;
      i++;
    }
  }
  Serial.println(turnedOffOrder[0]);
  Serial.println(turnedOffOrder[1]);
  Serial.println(turnedOffOrder[2]);
  Serial.println(turnedOffOrder[3]);
}