#define LED_PIN1 13
#define LEDP_IN2 12
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

int score;
bool inGame;
bool endGame;
bool outGame;
int turnedOffOrder[4];
int pressedOrder[4] = {0, 0, 0, 0};
int i;
int factor;
int t2;
int t3;

void setup() { 
  Serial.begin(9600);
  score = 0;
  inGame = false;
  endGame = false;
  outGame = true; 
  i = 0;
  t2 = 4000;
  t3 = 5000;
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN1), button1pressed, FALLING); 
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN2), button2pressed, FALLING); 
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN3), button3pressed, FALLING); 
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN4), button4pressed, FALLING); 


}

void loop() {
  if (outGame) {
    //mostra led
    outGame = false;
    inGame = true;
  } else {
    if (endGame) {
      //mostro led rosso
      //mostro punteggio
      //leggo valore potenziometro
      factor = analogRead(POT_PIN);
      factor = map(factor, 0, 1023, 0, 4);
      //aspetta 10 sec
      delay(T_OUT);
      endGame = false;
      outGame = true;
    } else {
      if (inGame) {
        //controlla length array
        if (pressedOrder[3] != 0) {
          //stoppa timer
          for(int x = 0; x < 4; x++) {
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