#define N_LEDS 4
#define N_BUTTONS 4

int led_pins[N_LEDS] = { 3, 4, 5, 6};
int buttons_pins[N_BUTTONS] = {8,9,10,11};
int status_pin = 7;
int array_led[N_LEDS]={0,0,0,0};//sequenza led
int array_player[N_LEDS]={0,0,0,0};//Pulsanti premuti dal giocatore

bool gameStart=false;
bool perso = false;
int punteggio=0;
int current = 0;
int tmp=0;
int level=0;
int brightness=10;
int fadeAmount=10;
int step = 1;
int how_long_on_max = 1000;

void test(){
  for (int i = 0; i < N_LEDS; i++){
    digitalWrite(led_pins[i],HIGH);
  }
}

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < N_LEDS; i++){
    pinMode(led_pins[i],OUTPUT);
  }
  pinMode(status_pin,OUTPUT);
  for (int i = 0; i < N_BUTTONS; i++){
    pinMode(buttons_pins[i],INPUT);
  }
  /* testing */
  // test();
}

void dissolvenzaStatusLed(){
  for(int i=0; i<255;i++){
   	analogWrite(status_pin, brightness); // imposta la luminosità
   	brightness = brightness + fadeAmount; // cambia la luminosità attraverso il loop  
   	delay(1);
  }
  for(int i=0; i<255;i++){
   	//Serial.print("Welcome to the Restore the Light Game. Press Key B1 to Start");
   	analogWrite(status_pin, brightness); // imposta la luminosità
   	brightness = brightness - fadeAmount; // cambia la luminosità attraverso il loop  
   	delay(1);
  }
  delay(490);
}

bool testSequenza(int array_led[], int array_player[]){
 bool tmp = true;
  for(int i =0;i<N_LEDS;i++){
    if(array_led[i]!=array_player[N_LEDS-i]){tmp=false;}
  }
 return tmp;
}

void allOff(){
  for(int i =0;i<N_LEDS;i++){
    digitalWrite(led_pins[i],LOW);
  }
  digitalWrite(status_pin,LOW);
}

/*
void sleepNow()         // here we put the arduino to sleep
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here
    sleep_enable();          // enables the sleep bit in the mcucr registe
    attachInterrupt(0,2, LOW); // use interrupt 0 (pin 2) and run function
	sleep_mode();
	sleep_disable();
	detachInterrupt(0);
}
*/

void loop() {
  int speed = analogRead(A0);
  float factor = ((float)speed)/1024;
  if(gameStart==false && tmp<10){
    dissolvenzaStatusLed();
    tmp++;
  	Serial.print("Welcome to the Restore the Light Game. Press Key B1 to Start");
  }
  if(gameStart==false && tmp==10){
    //SLEEP
    digitalWrite(led_pins[0],HIGH);
  }
  if(digitalRead(buttons_pins[0])==HIGH && gameStart==false){gameStart=true;}
  if(gameStart==true){
  	//Sequenza Casuale Led
    Serial.print("GO!");
    allOff();
    
    
    //Sequenza Inversa Giocatore
   
   //Test Sequenzq
   // if(testSequenza(array_led,array_player)==true)
      
   //game Over
   if(perso==true){
    Serial.print("Game Over. Final Score:");
   	Serial.print(punteggio);
   	digitalWrite(status_pin,HIGH);
   	delay(1000*10);
    perso=false;
    gameStart=false;
   }
  }
}
          