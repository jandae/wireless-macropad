/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 */
#include <Arduino.h>
#include <BleKeyboard.h>

BleKeyboard bleKeyboard;

const int toggl = 23;
const int btn1 = 22;
int togglState = 1;
int btn1State = 1;


void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  pinMode(toggl, INPUT);
  bleKeyboard.begin();
}

void loop() {
  static uint32_t oldtime=millis();

  if(bleKeyboard.isConnected()) {
    togglState = digitalRead(toggl);
    btn1State = digitalRead(btn1);
    // Serial.print(togglState);
    // Serial.print('-');
    // Serial.print(btn1State);
    // Serial.println('+');
    //test change

    if ( (millis()-oldtime) > 100) {
      if (togglState == 1) {
        bleKeyboard.press(KEY_F14);
        Serial.println("F14");
      } else if (togglState == 0) {
        bleKeyboard.release(KEY_F14);
      }

      if (btn1State == 1) {
        bleKeyboard.press(KEY_F13);
        Serial.println("F13");
      } else if (btn1State == 0) {
        bleKeyboard.release(KEY_F13);
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
