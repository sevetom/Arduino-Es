/**
 * @author lorenzo.annibalini@studio.unibo.it
 * @author lorenzo.bacchini26@studio.unibo.it
 * @author emanuele.sanchi@studio.unibo.it
*/
#include <EnableInterrupt.h>
#include <TimerOne.h>
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
#define N_LED 4
#define FIXAMOUNT 300

int score;
int turnedOffOrder[4] = {0, 0, 0, 0};
int pressedOrder[4] = {0, 0, 0, 0};
int pos;
float factor;
int t2;
int t3;
unsigned long prevoiusTime;
int turnedOffLed;
enum gameState
{
    inGame,
    outGame,
    endGame
} gameState;

void setup()
{
    // inizialize serial monitor, leds and buttons
    Serial.begin(9600);
    pinMode(LED_PIN1, OUTPUT);
    pinMode(LED_PIN2, OUTPUT);
    pinMode(LED_PIN3, OUTPUT);
    pinMode(LED_PIN4, OUTPUT);
    pinMode(BUTTON_PIN1, INPUT);
    pinMode(BUTTON_PIN2, INPUT);
    pinMode(BUTTON_PIN3, INPUT);
    pinMode(BUTTON_PIN4, INPUT);
    pinMode(LED_ERRORPIN, OUTPUT);
    // initialize variables for timing and gloabl counters
    score = 0;
    pos = 0;
    t2 = 4;
    t3 = 5;
    prevoiusTime = 0;
    turnedOffLed = 0;
    factor = 0.2;
    gameState = outGame;
    randomSeed(analogRead(4));
    // azzero registers
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    // attached interrupts on buttons
    enableInterrupt(BUTTON_PIN1, button1pressed, CHANGE);
    enableInterrupt(BUTTON_PIN2, button2pressed, CHANGE);
    enableInterrupt(BUTTON_PIN3, button3pressed, CHANGE);
    enableInterrupt(BUTTON_PIN4, button4pressed, CHANGE);
}

void loop()
{
    switch (gameState)
    {
    case outGame:
        // randomize order of leds' turning off
        randomizeOrder();
        // turning on all leds
        digitalWrite(LED_PIN1, HIGH);
        digitalWrite(LED_PIN2, HIGH);
        digitalWrite(LED_PIN3, HIGH);
        digitalWrite(LED_PIN4, HIGH);
        // wait 1 second to starting turning off leds
        delay(1000);
        turnOffLeds();
        // start timer
        createTimer();
        // change state
        gameState = inGame;
        break;
    case endGame:
        // chiama funzione per mostrare punteggio e fare fade del led rosso
        showScore();
        fadeAmount();
        break;
    case inGame:

        // check array lenght by checking if last element is equal 0
        if (pressedOrder[3] != 0)
        {
            // stop the timer
            stopTimer();
            // check pressed button order
            checkPressOrder();
            if (gameState == inGame)
            {
                score++;
                // reduce games timers
                t2 = t2 - t2 * factor;
                t3 = t3 - t3 * factor;
            }
            // change state
            gameState = outGame;
        }
        break;
    default:
        break;
    }
}

/**
 * Function to turn off the leds by random order previously chose
 */
void turnOffLeds()
{
    // looping in the array of turning off order
    int i = 0;
    for (i = 0; i < 4; i++)
    {
        switch (turnedOffOrder[i])
        {
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
        delay(/*t2 / N_LED*/ 1000);
    }
}
/**
 * Function to create a pseudo-random order to turn off leds
 */
void randomizeOrder()
{
    int i = 1;
    while (i <= N_LED)
    {
        int choise = random(0, N_LED);
        if (turnedOffOrder[choise] == 0)
        {
            turnedOffOrder[choise] = i;
            i++;
        }
    }
    // print the order on serial line
    Serial.println(turnedOffOrder[0]);
    Serial.println(turnedOffOrder[1]);
    Serial.println(turnedOffOrder[2]);
    Serial.println(turnedOffOrder[3]);
}

/**
 * Function to check if the order of pressed buttons is right
 */
void checkPressOrder()
{
    for (int x = 0; x < N_LED; x++)
    {
        if (pressedOrder[x] != turnedOffOrder[3 - x])
        {
            gameState = endGame;
        }
    }
    pos = 0;
}

/**
 * Function for button 1 interrupt
 */
void button1pressed()
{
    insertButton(1);
}

/**
 * Function for button 2 interrupt
 */
void button2pressed()
{
    insertButton(2);
}

/**
 * Function for button 3 interrupt
 */
void button3pressed()
{
    insertButton(3);
}

/**
 * Function for button 4 interrupt
 */
void button4pressed()
{
    insertButton(4);
}

/**
 * Function to insert the pressed button into the array
 */
void insertButton(int n)
{
    // check for bouncing of phisical buttons
    if (millis() - prevoiusTime > FIXAMOUNT)
    {
        pressedOrder[pos] = n;
        Serial.println(pressedOrder[pos]);
        prevoiusTime = millis();
        pos++;
    }
}

/**
 * function to end the game after the timeout
 */
void goToEndGame()
{
    gameState = endGame;
    Serial.println("fine");
    pos = 0;
}

/**
 * function to create a timer and attach the interrupt
 */
void createTimer()
{
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    Timer1.initialize();
    Timer1.setPeriod(t3 * 1000000);
    Timer1.attachInterrupt(goToEndGame);
}

/**
 * funtcion to stop the timer and detach the interrupt
 */
void stopTimer()
{
    Timer1.stop();
    Timer1.detachInterrupt();
}

void showScore()
{
    Serial.println("Your score" + score);
    score = 0;
}

void fadeAmount()
{
    // fade
}