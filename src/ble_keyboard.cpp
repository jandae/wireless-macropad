/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 *
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


 */

#include <Arduino.h>
#include <BleKeyboard.h>

BleKeyboard bleKeyboard;

const int toggl = 23;
const int btn1 = 22;
const int btn2 = 21;
const int btn3 = 19;
const int btn4 = 18;

int togglState = 1;
int btn1State = 1;
int btn2State = 1;
int btn3State = 1;
int btn4State = 1;


void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  pinMode(toggl, INPUT_PULLDOWN);
  pinMode(btn1, INPUT_PULLDOWN);
  pinMode(btn2, INPUT_PULLDOWN);
  pinMode(btn3, INPUT_PULLDOWN);
  pinMode(btn4, INPUT_PULLDOWN);

  bleKeyboard.begin();
}

void loop() {
  static uint32_t oldtime=millis();

  if(bleKeyboard.isConnected()) {
    togglState = digitalRead(toggl);
    btn1State = digitalRead(btn1);
    btn2State = digitalRead(btn2);
    btn3State = digitalRead(btn3);
    btn4State = digitalRead(btn4);
    // Serial.print(togglState);
    // Serial.print('-');
    // Serial.print(btn1State);
    // Serial.println('+');

    if ( (millis()-oldtime) > 100) {
      if (togglState == 1) {
        bleKeyboard.press(KEY_F13);
        Serial.println("F13");
      } else if (togglState == 0) {
        bleKeyboard.release(KEY_F13);
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
