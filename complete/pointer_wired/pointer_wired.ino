//// wired pointer (remote controler?) for power point and libreoffice impress...

//// Definition of functions used in a state machine for a while from here

// Definition of states
enum { ST_IDLE, ST_DEBOUNCE, ST_ACTION } state1; // this is to deal with debouncing
enum { ST_LOCKED, ST_BLUE, ST_RED} state2; // buttun pressed detections

const uint8_t portd_shift = 2;
const uint8_t nKeyMask = 0x0f << portd_shift;
const uint8_t BLUE = 0, RED = 1; // pin number 

// Timer functions
uint32_t ulMillis_10ms = 0;
uint32_t ulMillis_100ms = 0;
uint32_t ulMillis_500ms = 0;

// Key input functions
uint8_t nKeyPrev = nKeyMask;
uint8_t nKeyOff = nKeyMask, nKeyTurnOff = 0, nKeyTurnOn = 0;

uint8_t nKeyStatus = 0;
uint8_t nEventKeyPressed = 0;  // Event raised by State Machine 1

// A function for detecting pin input
void getKey(void) 
{
  bool PIND;
  PIND = gpio_get(BLUE) | gpio_get(RED);
  nEventKeyPressed = 0;
  nKeyOff = PIND & nKeyMask;
  nKeyTurnOff = (~nKeyPrev & nKeyMask) & nKeyOff;
  nKeyTurnOn = nKeyPrev & (~nKeyOff & nKeyMask);
  nKeyPrev = nKeyOff;
};

uint8_t key_on2off() { return nKeyTurnOff; };
uint8_t key_off2on() { return nKeyTurnOn; };
uint8_t key_on() { return ~nKeyOff & nKeyMask; };
uint8_t key_off() { return nKeyOff; };
void key_record() {  nKeyStatus = key_on2off(); };
void key_raiseEntered() { nEventKeyPressed = nKeyStatus; }

void timer10ms_start(void) {  ulMillis_10ms = millis(); };
void timer100ms_start(void) {  ulMillis_100ms = millis(); };
void timer500ms_start(void) {  ulMillis_500ms = millis(); };
bool timer10ms_expired(void) {  return (millis() - ulMillis_10ms) >= 10; };
bool timer100ms_expired(void) {  return (millis() - ulMillis_100ms) >= 100; };
bool timer500ms_expired(void) {  return (millis() - ulMillis_500ms) >= 500; };

//// End of definition of functions in a state machine

//// pin numbers in mouse function
int xAxisPin = 26; // define X pin of Joystick
int yAxisPin = 27; // define Y pin of Joystick
int zAxisPin = 28; // define Z pin of Joystick
int xVal, yVal, zVal; // define 3 variables to store the values of 3 direction

#include <Mouse.h>
#include <Keyboard.h>

void setup() {
Mouse.begin();
pinMode(zAxisPin, INPUT_PULLUP); // initialize the port to pull-up input
Serial.begin(9600); // initialize the serial port with baud rate 9600
Keyboard.begin();
pinMode(BLUE, INPUT_PULLDOWN); // initialize the port to pull-up input
pinMode(RED, INPUT_PULLDOWN); // initialize the port to pull-up input
}

void loop() {

  //// state machine (key board function)
  getKey();  // Needed to get key status

  // State Machine 1 (to deal with debouncing)
  switch (state1) {
    case ST_IDLE:
    if(key_on2off()){
      timer10ms_start();
      key_record();
      state1 = ST_DEBOUNCE;
    }
    break;

    case ST_DEBOUNCE:
    if(timer10ms_expired()){
      Serial.print("timer10ms expired \n");
      if(key_on()){
        timer10ms_start();
      }
      if(key_off()){
        timer100ms_start();
        state1 = ST_ACTION;
      }
    }
    break;

    case ST_ACTION:
    if(timer100ms_expired()){
      state1 = ST_IDLE;
      Serial.print("raise an event \n");
      key_raiseEntered();
    }
    break;
  }

  // State Machine 2
  switch (state2) {
    case ST_LOCKED:
    if(digitalRead(BLUE) == 1){
      timer500ms_start();
      state2 = ST_BLUE;
      Keyboard.write(KEY_RIGHT_ARROW); // Send the right arrow key
      Serial.print("brue pressed \n");
    }
    else if(digitalRead(RED) == 1){
      timer500ms_start();
      state2 = ST_RED;
      Keyboard.write(KEY_LEFT_ARROW); // Send the left arrow key
      Serial.print("red pressed \n");
    }
    break;

    case ST_BLUE:
    if(timer500ms_expired()){
      state2 = ST_LOCKED;
      Serial.print("back to ST_LOCKED \n");
    }
    break;

    case ST_RED:
    if(timer500ms_expired()){
      state2 = ST_LOCKED;
      Serial.print("back to ST_LOCKED \n");
    }
    break;
  }

  //// mouse function
  // read analog value in XY axis
  xVal = analogRead(xAxisPin);
  yVal = analogRead(yAxisPin);
  // read digital value of switch in Z axis
  zVal = digitalRead(zAxisPin);

  // detect key input then move mouse cursor
  if(xVal > 700 || yVal > 700 || xVal <300 || yVal < 300){
    Mouse.move((xVal - 500) / 50, (yVal - 500) / 50, 0);
  }
}