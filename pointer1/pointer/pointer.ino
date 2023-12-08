/* Released into the public domain */
/* Earle F. Philhower, III <earlephilhower@yahoo.com> */

#include <KeyboardBT.h>

#define BLUEBUTTON 22                  //青いボタンはGPIO 0を使う
#define REDBUTTON 18               //黒いボタンはGPIO 1を使う

void ledCB(bool numlock, bool capslock, bool scrolllock, bool compose, bool kana, void *cbData) {
  (void) numlock;
  (void) scrolllock;
  (void) compose;
  (void) kana;
  (void) cbData;
  digitalWrite(LED_BUILTIN, capslock ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  KeyboardBT.begin("PicoW Pointer");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  KeyboardBT.onLED(ledCB);
  KeyboardBT.begin();
  delay(5000);
  Serial.printf("Arduino USB Password Typer\n");
  Serial.printf("Press BOOTSEL to enter your super-secure(not!) password\n\n");
  pinMode(BLUEBUTTON, INPUT_PULLDOWN);
  // pinMode(REDBUTTON, INPUT_PULLDOWN);
}

void loop() {
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
}