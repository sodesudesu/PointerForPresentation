// #include <JoystickBT.h>
#include <MouseBT.h>

int xAxisPin = 26; // define X pin of Joystick
int yAxisPin = 27; // define Y pin of Joystick
int zAxisPin = 28; // define Z pin of Joystick
int xVal, yVal, zVal; // define 3 variables to store the values of 3 direction
void setup() {
MouseBT.begin("BarrelMouse");
pinMode(zAxisPin, INPUT_PULLUP); // initialize the port to pull-up input
Serial.begin(9600); // initialize the serial port with baud rate 9600
}

void loop() {
// read analog value in XY axis
xVal = analogRead(xAxisPin);
yVal = analogRead(yAxisPin);
// read digital value of switch in Z axis
zVal = digitalRead(zAxisPin);

// データを送信 する必要ないよね、マウスで送信すればよいんだよね
// JoystickBT.X(xVal);
// JoystickBT.Y(yVal);

if(xVal > 700 || yVal > 700 || xVal <300 || yVal < 300){
  MouseBT.move((xVal - 500) / 20, (yVal - 500) / 20, 0);
}

//print the data read above
Serial.print("X : ");
Serial.print(xVal);
Serial.print(" \t Y : ");
Serial.print(yVal);
Serial.print(" \t Z : ");
Serial.println(zVal);
delay(200);
}

/*
#include <JoystickBT.h>

#define X 26       // アナログ入力ピン
#define Y 27       // アナログ入力ピン

void setup() {
  Serial.begin(115200);
  Serial.println("Use BOOTSEL to start the Joystick demo.");
  JoystickBT.begin("PicoJoy Demo");
}

void loop() {
  if (BOOTSEL) {
    Serial.println("Joystick buttons");
    for (uint8_t i = 1; i <= 32; i++) {
      JoystickBT.button(i, true);
      delay(250);
      JoystickBT.button(i, false);
      delay(10); //we need a short delay here, sending packets with less than 1ms leads to packet loss!
    }
    //alternativ with manual send:
    JoystickBT.useManualSend(true);
    Serial.println("Joystick buttons - manual send");
    for (uint8_t i = 1; i <= 32; i++) {
      JoystickBT.button(i, true);
      JoystickBT.send_now();
      delay(250);
      JoystickBT.button(i, false);
    }
    JoystickBT.useManualSend(false);

    //iterate all joystick axis
    //Note: although you can use 0-1023 here (10bit), internally 8bits are used (-127 to 127)
    Serial.println("Joystick X");
    for (uint16_t i = 0; i < 1023; i++) {
      JoystickBT.X(i);
      delay(2);
    } JoystickBT.X(512);
    Serial.println("Joystick Y");
    for (uint16_t i = 0; i < 1023; i++) {
      JoystickBT.Y(i);
      delay(2);
    } JoystickBT.Y(512);
    Serial.println("Joystick Z");
    for (uint16_t i = 0; i < 1023; i++) {
      JoystickBT.Z(i);
      delay(2);
    } JoystickBT.Z(512);
    Serial.println("Joystick Zrotate");
    for (uint16_t i = 0; i < 1023; i++) {
      JoystickBT.Zrotate(i);
      delay(2);
    } JoystickBT.Zrotate(512);
    Serial.println("Joystick sliderLeft");
    for (uint16_t i = 0; i < 1023; i++) {
      JoystickBT.sliderLeft(i);
      delay(2);
    } JoystickBT.sliderLeft(0);
    Serial.println("Joystick sliderRight");
    for (uint16_t i = 0; i < 1023; i++) {
      JoystickBT.sliderRight(i);
      delay(2);
    } JoystickBT.sliderRight(0);
    Serial.println("Joystick hat");
    for (uint16_t i = 0; i < 360; i++) {
      JoystickBT.hat(i);
      delay(20);
    } JoystickBT.hat(-1);

    //use int8 mode for the axis.
    //Note: hat is not used differently.
    Serial.println("Now all axis in 8bit mode, -127 to 127");
    JoystickBT.use8bit(true);
    Serial.println("Joystick X");
    for (int16_t i = -127; i < 128; i++) {
      JoystickBT.X(i);
      delay(2);
    } JoystickBT.X(0);
    Serial.println("Joystick Y");
    for (int16_t i = -127; i < 128; i++) {
      JoystickBT.Y(i);
      delay(2);
    } JoystickBT.Y(0);
    Serial.println("Joystick Z");
    for (int16_t i = -127; i < 128; i++) {
      JoystickBT.Z(i);
      delay(2);
    } JoystickBT.Z(0);
    Serial.println("Joystick Zrotate");
    for (int16_t i = -127; i < 128; i++) {
      JoystickBT.Zrotate(i);
      delay(2);
    } JoystickBT.Zrotate(0);
    Serial.println("Joystick sliderLeft");
    for (int16_t i = -127; i < 128; i++) {
      JoystickBT.sliderLeft(i);
      delay(2);
    } JoystickBT.sliderLeft(0);
    Serial.println("Joystick sliderRight");
    for (int16_t i = -127; i < 128; i++) {
      JoystickBT.sliderRight(i);
      delay(2);
    } JoystickBT.sliderRight(0);
    JoystickBT.use8bit(false);
  }
}
*/