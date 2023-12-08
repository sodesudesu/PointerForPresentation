#include <KeyboardBT.h>

// ここから関数などの定義（もともとライブラリだった）
// static const uint8_t nCombo = 3;
const uint8_t portd_shift = 2;
const uint8_t nKeyMask = 0x0f << portd_shift;
const uint8_t BLUE = 22, RED = 18; // ここを変更した
// uint8_t anPass[nCombo];

//// Timer Functions
uint32_t ulMillis_10ms = 0;
uint32_t ulMillis_100ms = 0;
uint32_t ulMillis_1s = 0;

//// Key Input Functions
uint8_t nKeyPrev = nKeyMask;
uint8_t nKeyOff = nKeyMask, nKeyTurnOff = 0, nKeyTurnOn = 0;

uint8_t nKeyStatus = 0;
uint8_t nEventKeyPressed = 0;  // Event raised by State Machine 1


uint8_t KEY(uint8_t n)  { return (0x01 << (n + portd_shift)); };
/* LEDの点灯に関する関数だからとりあえずコメントアウト
//// (simulate) LED Functions
void lock() {  digitalWrite(nLED2, false); };
void unlock() {  digitalWrite(nLED2, true); };

void start_beep() {  digitalWrite(nLED1, true); };
void stop_beep() {  digitalWrite(nLED1, false); };
*/

void getKey(void) // ここの変更うまくいっているか不安
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
void timer1s_start(void) {  ulMillis_1s = millis(); };
bool timer10ms_expired(void) {  return (millis() - ulMillis_10ms) >= 10; };
bool timer100ms_expired(void) {  return (millis() - ulMillis_100ms) >= 100; };
bool timer1s_expired(void) {  return (millis() - ulMillis_1s) >= 1000; };

// bool isCorrectEntered(uint8_t n) { return (n > 0 && n <= nCombo) ? (nEventKeyPressed & anPass[n-1]) : false; }
// bool isAnyEntered(void) { return nEventKeyPressed; }

// ライブラリ終了

// stateを設定
enum { ST_IDLE, ST_DEBOUNCE, ST_ACTION } state1;
enum { ST_LOCKED, ST_BLUE, ST_RED} state2;

// #define BLUEBUTTON 22              //青いボタンはGPIO 22を使う
// #define REDBUTTON 18               //黒いボタンはGPIO 18を使う

/* ここ削除して何か問題は起きるかな？
void ledCB(bool numlock, bool capslock, bool scrolllock, bool compose, bool kana, void *cbData) {
  (void) numlock;
  (void) scrolllock;
  (void) compose;
  (void) kana;
  (void) cbData;
  digitalWrite(LED_BUILTIN, capslock ? HIGH : LOW);
}
*/

void setup() {
  Serial.begin(9600);
  KeyboardBT.begin("PicoW Pointer");
  // pinMode(LED_BUILTIN, OUTPUT);
  // digitalWrite(LED_BUILTIN, LOW);
  // KeyboardBT.onLED(ledCB);
  KeyboardBT.begin();
  delay(5000);
  Serial.printf("Arduino USB Password Typer\n");
  Serial.printf("Press BOOTSEL to enter your super-secure(not!) password\n\n");
  pinMode(BLUE, INPUT_PULLDOWN);
  pinMode(RED, INPUT_PULLDOWN);
}

void loop() {
  getKey();  // Needed to get key status

  // State Machine 1
  switch (state1) {
    case ST_IDLE:
    if(key_on2off()){
      timer10ms_start();
      key_record();
      state1=ST_DEBOUNCE;
      Serial.print(" \n key on to off \n");
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
        state1=ST_ACTION;
      }
    }
    break;

    case ST_ACTION:
    if(timer100ms_expired()){
      state1=ST_IDLE;
      Serial.print("raise an event \n");
      key_raiseEntered();
    }
    break;
  }

  // State Machine 2
  switch (state2) {
    case ST_LOCKED:
    if(digitalRead(BLUE) == 1){
      timer1s_start();
      state2 = ST_BLUE;
      KeyboardBT.write(KEY_RIGHT_ARROW); // 右矢印キーを送信
      Serial.print("brue pressed \n");
    }
    else if(digitalRead(RED) == 1){
      timer1s_start();
      state2 = ST_RED;
      KeyboardBT.write(KEY_LEFT_ARROW); // 右矢印キーを送信
      Serial.print("red pressed \n");
    }
    break;

    case ST_BLUE:
    if(timer1s_expired()){
      state2 = ST_LOCKED;
      Serial.print("back to ST_LOCKED \n");
    }
    break;

    case ST_RED:
    if(timer1s_expired()){
      state2 = ST_LOCKED;
      Serial.print("back to ST_LOCKED \n");
    }
    break;

  }
  /*
  int blueButtonState = digitalRead(BLUEBUTTON);
  // Serial.print("Blue Button State: ");
  // Serial.println(blueButtonState);

  if (blueButtonState == 1) {
    Serial.println("Advancing to the next page...");
    KeyboardBT.write(KEY_RIGHT_ARROW); // 右矢印キーを送信
  }

  delay(100); // デバウンスのための適切な遅延を設定


  int redButtonState = digitalRead(REDBUTTON);
  if (redButtonState == 1) {
    Serial.println("Back to the previnius page...");
    KeyboardBT.write(KEY_LEFT_ARROW); // 右矢印キーを送信
  }

  delay(100); // デバウンスのための適切な遅延を設定
  */
}
