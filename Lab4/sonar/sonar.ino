const int trigPin = 3;
const int echoPin = 2;

/* supponendo di eseguire il test 
   in un ambiente a 20 °C 
   https://it.wikipedia.org/wiki/Velocità_del_suono
   */
//vs = velocita del suono
const float vs = 331.45 + 0.62*20;

void setup()
{
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);  
}

float getDistance()
{
    /* invio impulso */
    digitalWrite(trigPin,LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin,HIGH);
    delayMicroseconds(5);
    digitalWrite(trigPin,LOW);
    
    /* ricevi l’eco */
    //tUS = tempo in microsecondi
    float tUS = pulseIn(echoPin, HIGH);
    //t = tempo in secondi (10^-6/10^-6 = 10^0 = 1) ==> 1 secondo
    //poi dividiamo per due perche sarebbe il tempo di andata e il tempo di ritorno
    float t = tUS / 2000000.0;
    //d = distanza
    float d = t*vs;
    return d;
}

void loop()
{
  float d = getDistance();
  Serial.println(d);
  delay(50); 
}