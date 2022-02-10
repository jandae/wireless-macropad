/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 * Library:
 * https://github.com/T-vK/ESP32-BLE-Keyboard/issues/11
 * Incase you shelf this project here are things to note:
 *
 * 1. Change Blekeyboard.cpp (enables F13+)
 *
*       LOGICAL_MINIMUM(1), 0x00,          //   LOGICAL_MINIMUM(0)
        LOGICAL_MAXIMUM(1), 0x73,          //   LOGICAL_MAXIMUM(0x65) ; 101 keys
        USAGE_PAGE(1),      0x07,          //   USAGE_PAGE (Kbrd/Keypad)
        USAGE_MINIMUM(1),   0x00,          //   USAGE_MINIMUM (0)
        USAGE_MAXIMUM(1),   0x73,          //   USAGE_MAXIMUM (0x65)
    2. Unpair first when changes are made
    3. Hold boot button when uploading to esp32
    4. Using pin 12 causes it to not flash (remove first then flash)

 */

#include <Arduino.h>
#include <BleKeyboard.h>
#include "AiEsp32RotaryEncoder.h"

BleKeyboard bleKeyboard("JankoPad", "thejandae", 100);

const int toggl = 23;
const int toggl2 = 15;
const int btn1 = 22;
const int btn2 = 21;
const int btn3 = 19;
const int btn4 = 18;
const int btn5 = 17;
const int btn6 = 16;
const int btn7 = 4;

int togglState = 0;
int toggl2State = 0;
int btn1State = 0;
int btn2State = 0;
int btn3State = 0;
int btn4State = 0;
int btn5State = 0;
int btn6State = 0;
int btn7State = 0;

#define ROTARY_ENCODER_A_PIN 13
#define ROTARY_ENCODER_B_PIN 27
#define ROTARY_ENCODER_C_PIN 25
#define ROTARY_ENCODER_D_PIN 26
#define ROTARY_ENCODER_BUTTON_PIN 14
#define ROTARY_ENCODER_BUTTON_PIN2 5
#define ROTARY_ENCODER_VCC_PIN -1

//depending on your encoder - try 1,2 or 4 to get expected behaviour
//#define ROTARY_ENCODER_STEPS 1
//#define ROTARY_ENCODER_STEPS 2
#define ROTARY_ENCODER_STEPS 4

int val = 0;
int prev_val = 0;
int val2 = 0;
int prev_val2 = 0;

//instead of changing here, rather change numbers above
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
AiEsp32RotaryEncoder rotaryEncoder2 = AiEsp32RotaryEncoder(ROTARY_ENCODER_C_PIN, ROTARY_ENCODER_D_PIN, ROTARY_ENCODER_BUTTON_PIN2, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

void rotary_onButtonClick()
{
	static unsigned long lastTimePressed = 0;
	//ignore multiple press in that time milliseconds
	if (millis() - lastTimePressed < 500)
	{
		return;
	}

  bleKeyboard.write(KEY_F20);
	// lastTimePressed = millis();
	// Serial.print("button pressed ");
	// Serial.print(millis());
	// Serial.println(" milliseconds after restart");
}

void rotary_onButtonClick2()
{
	static unsigned long lastTimePressed = 0;
	//ignore multiple press in that time milliseconds
	if (millis() - lastTimePressed < 500)
	{
		return;
	}

  bleKeyboard.write(KEY_F23);
}

void rotary_loop()
{
	//dont print anything unless value changed
	if (rotaryEncoder.encoderChanged())
	{
    val = rotaryEncoder.readEncoder();

    if (val < prev_val) {
      bleKeyboard.write(KEY_F18);
      Serial.println("CCW");
    } else {
      bleKeyboard.write(KEY_F19);
      Serial.println("CW");
    }

    prev_val = val;

		Serial.print("Value: ");
		Serial.println(val);
	}

  if (rotaryEncoder2.encoderChanged())
	{
    val2 = rotaryEncoder2.readEncoder();

    if (val2 < prev_val2) {
      bleKeyboard.write(KEY_F21);
      Serial.println("CCW2");
    } else {
      bleKeyboard.write(KEY_F22);
      Serial.println("CW2");
    }

    prev_val2 = val2;

		Serial.print("Value2: ");
		Serial.println(val2);
	}
	if (rotaryEncoder.isEncoderButtonClicked())
	{
		rotary_onButtonClick();
	}

  if (rotaryEncoder2.isEncoderButtonClicked())
	{
		rotary_onButtonClick2();
	}
}

void IRAM_ATTR readEncoderISR()
{
	rotaryEncoder.readEncoder_ISR();
  rotaryEncoder2.readEncoder_ISR();
}


void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  // Maybe loop these but maybe not for simplicity ;)
  pinMode(toggl, INPUT_PULLDOWN);
  pinMode(toggl2, INPUT_PULLDOWN);
  pinMode(btn1, INPUT_PULLDOWN);
  pinMode(btn2, INPUT_PULLDOWN);
  pinMode(btn3, INPUT_PULLDOWN);
  pinMode(btn4, INPUT_PULLDOWN);
  pinMode(btn5, INPUT_PULLDOWN);
  pinMode(btn6, INPUT_PULLDOWN);
  pinMode(btn7, INPUT_PULLDOWN);

  rotaryEncoder.begin();
	rotaryEncoder.setup(readEncoderISR);
  rotaryEncoder.setAcceleration(250); //or set the value - larger number = more accelearation; 0 or 1 means disabled acceleration

  rotaryEncoder2.begin();
	rotaryEncoder2.setup(readEncoderISR);
  rotaryEncoder2.setAcceleration(250);

  bleKeyboard.begin();
}

void loop() {
  static uint32_t oldtime=millis();

  if(bleKeyboard.isConnected()) {
    togglState = digitalRead(toggl);
    toggl2State = digitalRead(toggl2);
    btn1State = digitalRead(btn1);
    btn2State = digitalRead(btn2);
    btn3State = digitalRead(btn3);
    btn4State = digitalRead(btn4);
    btn5State = digitalRead(btn5);
    btn6State = digitalRead(btn6);
    btn7State = digitalRead(btn7);

    rotary_loop();
    // Serial.println(currentStateCLK);
    // Serial.print(togglState);
    // Serial.print('-');
    // Serial.print(btn1State);
    // Serial.println('+');

    if ( (millis()-oldtime) > 100) {
      if (togglState == 1) {
        // bleKeyboard.press(KEY_F13);
        bleKeyboard.press('f');
        Serial.println("f");
      } else if (togglState == 0) {
        bleKeyboard.release('f');
        // bleKeyboard.release(KEY_F13);
      }

      if (toggl2State == 1) {
        bleKeyboard.press('g');
        Serial.println("g");
      } else if (togglState == 0) {
        bleKeyboard.release('g');
      }

      if (btn1State == 1) {
        bleKeyboard.press(KEY_F14);
        Serial.println("F14");
      } else if (btn1State == 0) {
        bleKeyboard.release(KEY_F14);
      }

      if (btn2State == 1) {
        bleKeyboard.press(KEY_F15);
        Serial.println("F15");
      } else if (btn2State == 0) {
        bleKeyboard.release(KEY_F15);
      }

      if (btn3State == 1) {
        bleKeyboard.press(KEY_F16);
        Serial.println("F16");
      } else if (btn3State == 0) {
        bleKeyboard.release(KEY_F16);
      }

      if (btn4State == 1) {
        bleKeyboard.press(KEY_F17);
        Serial.println("F17");
      } else if (btn4State == 0) {
        bleKeyboard.release(KEY_F17);
      }

      if (btn5State == 1) {
        bleKeyboard.press(KEY_F24);
        Serial.println("F24");
      } else if (btn5State == 0) {
        bleKeyboard.release(KEY_F24);
      }

      if (btn6State == 1) {
        bleKeyboard.press(KEY_LEFT_CTRL);
        bleKeyboard.press(KEY_F24);
        Serial.println("ctrl+f13");
      } else if (btn6State == 0) {
        bleKeyboard.release(KEY_LEFT_CTRL);
        bleKeyboard.release(KEY_F24);
      }

      if (btn7State == 1) {
        Serial.println("bk");
        bleKeyboard.press(KEY_MEDIA_WWW_BOOKMARKS);
        Serial.println("bk");
      } else if (btn7State == 0) {
        bleKeyboard.release(KEY_MEDIA_WWW_BOOKMARKS);
      }

      // if (btn6State == 1) {
      //   bleKeyboard.press(KEY_F25);
      //   Serial.println("CTRL+F2");
      // } else if (btn6State == 0) {
      //   bleKeyboard.release(KEY_F25);
      // }

      oldtime = millis();
    }




//    Serial.println("Sending 'Hello world'...");
//    bleKeyboard.print("Hello world");
//
//    delay(1000);
//
//    Serial.println("Sending Enter key...");
//    bleKeyboard.write('z');
//
//    delay(1000);
//
//    Serial.println("Sending Play/Pause media key...");
//    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
//
//    delay(1000);


   //
   // Below is an example of pressing multiple keyboard modifiers
   // which by default is commented out.
   //
   /* Serial.println("Sending Ctrl+Alt+Delete...");
    bleKeyboard.press(KEY_LEFT_CTRL);
    bleKeyboard.press(KEY_LEFT_ALT);
    bleKeyboard.press(KEY_DELETE);
    delay(100);
    bleKeyboard.releaseAll();
    */

  }
//  Serial.println("Waiting 5 seconds...");
//  delay(5000);
}
