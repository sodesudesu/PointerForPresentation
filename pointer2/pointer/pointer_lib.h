#ifndef pointer_lib
#define ponter_lib

class Lab8 {
// static const uint8_t nCombo = 3;
const uint8_t portd_shift = 2;
const uint8_t nKeyMask = 0x0f << portd_shift;
const uint8_t BLUE = 21, RED = 9; // ここを変更した
// uint8_t anPass[nCombo];

//// Timer Functions
uint32_t ulMillis_10ms = 0;
uint32_t ulMillis_100ms = 0;
uint32_t ulMillis_3s = 0;

//// Key Input Functions
uint8_t nKeyPrev = nKeyMask;
uint8_t nKeyOff = nKeyMask, nKeyTurnOff = 0, nKeyTurnOn = 0;

uint8_t nKeyStatus = 0;
uint8_t nEventKeyPressed = 0;  // Event raised by State Machine 1


uint8_t KEY(uint8_t n)  { return (0x01 << (n + portd_shift)); };

public:
  Lab8(uint8_t c0, uint8_t c1, uint8_t c2) {
    pinMode(BLUE, INPUT_PULLDOWN);
    pinMode(RED, INPUT_PULLDOWN);
    // anPass[0] = KEY(c0);
    // anPass[1] = KEY(c1);
    // anPass[2] = KEY(c2);
    // DDRD &= ~nKeyMask;  // Set keys to input

  };

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
void timer3s_start(void) {  ulMillis_3s = millis(); };
bool timer10ms_expired(void) {  return (millis() - ulMillis_10ms) >= 10; };
bool timer100ms_expired(void) {  return (millis() - ulMillis_100ms) >= 100; };
bool timer3s_expired(void) {  return (millis() - ulMillis_3s) >= 3000; };

// bool isCorrectEntered(uint8_t n) { return (n > 0 && n <= nCombo) ? (nEventKeyPressed & anPass[n-1]) : false; }
bool isAnyEntered(void) { return nEventKeyPressed; }
};

#endif