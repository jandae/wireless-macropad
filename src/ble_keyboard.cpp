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
    2. Unpair first when changes are made (true for non-beta)
    3. Hold boot button when uploading to esp32 (rewired to a switch for ease)
    4. Using pin 12 causes it to not flash (remove first then flash)
    5. space hex is 0x20

    Modes:
    1 - regular unique keys
    2 - limited 5 sec config mode
    3 - keyboard mode (reverse everything)

 */

#include <Arduino.h>
#include <BleKeyboard.h>
#include "AiEsp32RotaryEncoder.h"

#include <Arduino.h>
int longPressDelay = 350;           //customizable encoderValues
int spamSpeed = 15;

byte inputs[] = {22, 21, 19, 18};          //declaring inputs and outputs column
const int inCount = sizeof(inputs)/sizeof(inputs[0]);
byte outputs[] = {17, 16, 4, 2}; // row
const int outCount = sizeof(outputs)/sizeof(outputs[0]);

char layout[4][4] = {               //layout grid for characters
  {'1', '2', '3', '4'},
  {'5', '6', '7', '8'},
  {'9', 'a', 'b', 'c'},
  {'d', 'e', 'f', 'g'},
};

int keyDown[4][4];
bool keyLong[4][4];


BleKeyboard bleKeyboard("JankoPad", "thejandae", 100);

const int toggl = 23;
const int toggl2 = 15;
const int btn1 = 22;
const int btn2 = 21;
const int cup = 33;// 19;
const int btn4 = 18;
const int btn5 = 17;
const int btn6 = 16;
const int btn7 = 4;
const int btn8 = 2;
const int btn9 = 33;
const int left = 0;
const int right = 3;

int togglState = 0;                                                
int toggl2State = 0;
int btn1State = 0;
int btn2State = 0;
int cupState = 0;
int btn4State = 0;
int btn5State = 0;
int btn6State = 0;
int btn7State = 1;
int btn8State = 0;
int btn9State = 0;
int cupaaa = 0;
int leftState = 0;
int rightState = 0;

#define ROTARY_ENCODER_A_PIN 13
#define ROTARY_ENCODER_B_PIN 27
#define ROTARY_ENCODER_C_PIN 25
#define ROTARY_ENCODER_D_PIN 26
#define ROTARY_ENCODER_E_PIN 32
#define ROTARY_ENCODER_F_PIN 35
#define ROTARY_ENCODER_BUTTON_PIN 14
#define ROTARY_ENCODER_BUTTON_PIN2 5
#define ROTARY_ENCODER_BUTTON_PIN3 1
#define ROTARY_ENCODER_VCC_PIN -1

//depending on your encoder - try 1,2 or 4 to get expected behaviour
//#define ROTARY_ENCODER_STEPS 1
//#define ROTARY_ENCODER_STEPS 2
#define ROTARY_ENCODER_STEPS 4

int val = 0;
int prev_val = 0;
int val2 = 0;
int prev_val2 = 0;
int val3 = 0;
int prev_val3 = 0;

unsigned long previousMillis = 0;
int mode = 1;
int pressed = 0;

//instead of changing here, rather change numbers above
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
AiEsp32RotaryEncoder rotaryEncoder2 = AiEsp32RotaryEncoder(ROTARY_ENCODER_C_PIN, ROTARY_ENCODER_D_PIN, ROTARY_ENCODER_BUTTON_PIN2, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
AiEsp32RotaryEncoder rotaryEncoder3 = AiEsp32RotaryEncoder(ROTARY_ENCODER_E_PIN, ROTARY_ENCODER_F_PIN, ROTARY_ENCODER_BUTTON_PIN3, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
unsigned long Time1;

void pressRelease1(uint16_t key, bool press) {
  if(press) {
    bleKeyboard.press(key);
  } else {
    bleKeyboard.release(key);            
  }
}

void pressRelease(uint8_t key, bool press) {
  if(press) {
    bleKeyboard.press(key);
  } else {
    bleKeyboard.release(key);            
  }
}

// this is new to me, apparently you could name the same function but different parameter data type "function overloading"
void pressRelease(char key, bool press) {
  if(press) {
    bleKeyboard.press(key);
  } else {
    bleKeyboard.release(key);            
  }
}


void sendKey(char key, bool press) {  
  
  switch(key) {
    case '1':
      if(mode == 1) {                
        pressRelease(KEY_F13, press);
      } else if (mode == 3) {
        pressRelease1(0x20, press);        
      }

      break;
    case '2':
      if(mode == 1) {                
        pressRelease(KEY_F14, press);    
      } else if (mode == 3) {
        pressRelease('c', press);        
      }      
      // bleKeyboard.write(KEY_F14);
      break;
    case '3':
      if(mode == 1) {                        
        pressRelease(KEY_F15, press);    
      } else if (mode == 3) {
        pressRelease('x', press);        
      }   
      // bleKeyboard.write(KEY_F15);
      break;
    case '4':
      if(mode == 1) {                        
        pressRelease(KEY_F17, press);   
      } else if (mode == 3) {
        pressRelease('z', press);        
      }        
      // bleKeyboard.write(KEY_F17);
      break;
    case '5':
      if(mode == 1) {                                
        pressRelease(KEY_F24, press); 
      } else if (mode == 3) {
        pressRelease('f', press);        
      }  
      // bleKeyboard.write(KEY_F24);
      break;
    case '6':
      // pressRelease(KEY_MEDIA_WWW_BOOKMARKS, press); 
      if(mode == 1) {        
        bleKeyboard.write(KEY_MEDIA_WWW_BOOKMARKS);
      } else if (mode == 3) {
        pressRelease('d', press);
      }
      break;
    case '7':
      if(mode == 1) {
        pressRelease(KEY_F23, press);
      } else if (mode == 3) {
        pressRelease('s', press);
      }
      
      // bleKeyboard.write(KEY_F23);
      break;
    case '8':
      if(mode == 1) {
        pressRelease(KEY_F20, press);
      } else if (mode == 3) {
        pressRelease('a', press);
      }
      // bleKeyboard.write(KEY_F20);
      break;
    case '9':    
      // Serial.print("9");  
      if(mode == 1) {
        bleKeyboard.press(KEY_LEFT_CTRL);
        bleKeyboard.press(KEY_LEFT_ALT);
        bleKeyboard.press(KEY_F2);
        delay(100);       
        bleKeyboard.release(KEY_LEFT_CTRL);
        bleKeyboard.release(KEY_LEFT_ALT);      
        bleKeyboard.release(KEY_F2);      
      } else if (mode == 3) {
        pressRelease('r', press);
      }
      
            
      break;
    case 'a':
      if(mode == 1) {
        bleKeyboard.press(KEY_LEFT_CTRL);
        bleKeyboard.press(KEY_LEFT_ALT);
        bleKeyboard.press(KEY_F3);
        delay(100);       
        bleKeyboard.release(KEY_LEFT_CTRL);
        bleKeyboard.release(KEY_LEFT_ALT);      
        bleKeyboard.release(KEY_F3);    
      } else if (mode == 3) {
        pressRelease('e', press);
      }
      
      break;
    case 'b':
      if(mode == 1) {
        bleKeyboard.press(KEY_LEFT_CTRL);
        bleKeyboard.press(KEY_LEFT_ALT);
        bleKeyboard.press(KEY_F4);
        delay(100);       
        bleKeyboard.release(KEY_LEFT_CTRL);
        bleKeyboard.release(KEY_LEFT_ALT);      
        bleKeyboard.release(KEY_F4);
      } else if (mode == 3) {
        pressRelease('w', press);
      }
     
      break;
    case 'c':
      if(mode == 1) {
        bleKeyboard.press(KEY_LEFT_CTRL);
        bleKeyboard.press(KEY_LEFT_ALT);
        bleKeyboard.press(KEY_F5);
        delay(100);       
        bleKeyboard.release(KEY_LEFT_CTRL);
        bleKeyboard.release(KEY_LEFT_ALT);      
        bleKeyboard.release(KEY_F5);
      } else if (mode == 3) {
        pressRelease('q', press);
      }
      
      break;
    case 'd':   
      if(mode == 1 && press) {
        bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
      } else if (mode == 3) {
        pressRelease('g', press);
      }   
      break;
    case 'e':
      if(mode == 1 && press) {
        bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
      } else if (mode == 3) {
        pressRelease('v', press);
      }
      break;
    case 'f':
      if(mode == 1 && press) {
        bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
      } else if (mode == 3) {
        pressRelease(KEY_RETURN, press);
      }
      break;
    case 'g':
      if(mode == 1 && press) {
        bleKeyboard.write(KEY_PRTSC);
      } else if (mode == 3) {
        pressRelease(KEY_ESC, press);
      }
      break;  
  } 
}

//if a key is pressed, this Funtion is called and outputs to serial the key location, it also sends the keystroke if not already done so
void keyPressed(int row, int col){  
  

  if(keyDown[row][col]==0){         //if the function is called for the first time for this key
    Serial.print(row);
    Serial.print("x");
    Serial.print(col);
    Serial.print(": ");
    Serial.println(layout[row][col]);
    sendKey(layout[row][col], true);           
  }
  else if(keyLong[row][col] && keyDown[row][col] > spamSpeed){ //if the key has been held long enough to warrant another keystroke set
    // sendKey(layout[row][col]); 
    keyDown[row][col] = 1;
  }
  else if(keyDown[row][col] > longPressDelay){ //if the key has been held for longer that longPressDelay, it switches into spam mode
    keyLong[row][col] = true;
  }

  keyDown[row][col]++;
}

void resetKey(int row, int col){ //resetting the variables after key is released
  keyDown[row][col] = 0;
  keyLong[row][col] = false;
  sendKey(layout[row][col], false); 
}
// first
void rotary_onButtonClick()
{
	static unsigned long lastTimePressed = 0;
	//ignore multiple press in that time milliseconds
	if (millis() - lastTimePressed < 500)
	{
		return;
	}

  if(mode == 2) {    
    mode = 3;
    Serial.print("mode ");
    Serial.println(mode);
    bleKeyboard.write(KEY_MEDIA_WWW_BACK);
  } else {
    bleKeyboard.write(KEY_MEDIA_WWW_BACK);
  }
}

// third
void rotary_onButtonClick2()
{
	static unsigned long lastTimePressed = 0;
	//ignore multiple press in that time milliseconds
	if (millis() - lastTimePressed < 500)
	{
		return;
	}

  if(mode == 2){
    mode = 1;
    Serial.print("mode ");
    Serial.println(mode);
    bleKeyboard.write(KEY_MEDIA_WWW_STOP);
  } else {
    bleKeyboard.write(KEY_MEDIA_WWW_STOP);
  }
}

// middle
void rotary_onButtonClick3()
{
	static unsigned long lastTimePressed = 0;
	//ignore multiple press in that time milliseconds
	if (millis() - lastTimePressed < 500)
	{
		return;
	}

  bleKeyboard.press(KEY_LEFT_CTRL);
  bleKeyboard.press(KEY_LEFT_ALT);
  bleKeyboard.press(KEY_F6);
  delay(100);       
  bleKeyboard.release(KEY_LEFT_CTRL);
  bleKeyboard.release(KEY_LEFT_ALT);      
  bleKeyboard.release(KEY_F6);

  mode = 2;
  Serial.print("mode ");
  Serial.println(mode);

  previousMillis = millis(); //start delay
  pressed = 1;

}

void rotary_loop()
{
  if(pressed) {    
    if (millis() - previousMillis >= 5000) {
      Serial.println("==================");

      // return mode after 5s but for mode 2 only
      if (mode == 2) {
        mode = 1;
      }
      pressed = 0;
      Serial.print("mode ");
      Serial.println(mode);
    }
  }

  // Serial.print("mode ");
  // Serial.println(mode);

  
	//dont print anything unless value changed
	if (rotaryEncoder.encoderChanged())
	{
    val = rotaryEncoder.readEncoder();

    if (val < prev_val) {
      
      if(mode == 1) {
        bleKeyboard.write(KEY_F18);
        Serial.println("CCW");
      } else if(mode == 2) {
        bleKeyboard.press(KEY_LEFT_SHIFT);        
        bleKeyboard.press(KEY_F18);
        delay(100);       
        bleKeyboard.release(KEY_LEFT_SHIFT);        
        bleKeyboard.release(KEY_F18);
      } else if(mode == 3) {
        bleKeyboard.write(KEY_F19);
      }
      

    } else {
      if(mode == 1) {
        bleKeyboard.write(KEY_F19);
        Serial.println("CW");
      } else if(mode == 2) {
        bleKeyboard.press(KEY_LEFT_SHIFT);        
        bleKeyboard.press(KEY_F19);
        delay(100);       
        bleKeyboard.release(KEY_LEFT_SHIFT);        
        bleKeyboard.release(KEY_F19);
      } else if(mode == 3) {
        bleKeyboard.write(KEY_F18);
      }
      

    }

    prev_val = val;

		Serial.print("Value: ");
		Serial.println(val);
	}

  if (rotaryEncoder2.encoderChanged())
	{
    val2 = rotaryEncoder2.readEncoder();

    if (val2 < prev_val2) {
      if(mode == 1) {
        bleKeyboard.write(KEY_F21);
        Serial.println("CCW2");
      } else if(mode == 2) {
        bleKeyboard.press(KEY_LEFT_SHIFT);        
        bleKeyboard.press(KEY_F21);
        delay(100);       
        bleKeyboard.release(KEY_LEFT_SHIFT);        
        bleKeyboard.release(KEY_F21);
      } else if(mode == 3) {
        bleKeyboard.write(KEY_F22);
      }
    } else {
      if(mode == 1) {
        bleKeyboard.write(KEY_F22);
        Serial.println("CW2");
      } else if(mode == 2) {
        bleKeyboard.press(KEY_LEFT_SHIFT);        
        bleKeyboard.press(KEY_F22);
        delay(100);       
        bleKeyboard.release(KEY_LEFT_SHIFT);        
        bleKeyboard.release(KEY_F22);
      } else if(mode == 3) {
        bleKeyboard.write(KEY_F21);
      }      
    }

    prev_val2 = val2;

		Serial.print("Value2: ");
		Serial.println(val2);
	}

  if (rotaryEncoder3.encoderChanged())
	{
    val3 = rotaryEncoder3.readEncoder();

    if (val3 < prev_val3) {
      if(mode == 1) {
        bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
        Serial.println("CCW3");
      } else if(mode == 2) {
        bleKeyboard.press(KEY_LEFT_CTRL);
        bleKeyboard.press(KEY_LEFT_ALT);
        bleKeyboard.press('[');
        delay(100);       
        bleKeyboard.release(KEY_LEFT_CTRL);
        bleKeyboard.release(KEY_LEFT_ALT);      
        bleKeyboard.release('[');
        Serial.println("NOT-");
      } else if(mode == 3) {
        bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
      } 
      
    } else {
      if(mode == 1) {
        bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
        Serial.println("CW3");
      } else if(mode == 2) {
        bleKeyboard.press(KEY_LEFT_CTRL);
        bleKeyboard.press(KEY_LEFT_ALT);
        bleKeyboard.press(']');
        delay(100);       
        bleKeyboard.release(KEY_LEFT_CTRL);
        bleKeyboard.release(KEY_LEFT_ALT);      
        bleKeyboard.release(']');
        Serial.println("NOT+");
      } else if(mode == 3) {
        bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
      } 
    }

    prev_val3 = val3;

		// Serial.print("Value3: ");
		// Serial.println(val3);
	}


	if (rotaryEncoder.isEncoderButtonClicked())
	{
		rotary_onButtonClick();
	}

  if (rotaryEncoder2.isEncoderButtonClicked())
	{
		rotary_onButtonClick2();
	}

  if (rotaryEncoder3.isEncoderButtonClicked())
	{
		rotary_onButtonClick3();
	}
}



void IRAM_ATTR readEncoderISR()
{
	rotaryEncoder.readEncoder_ISR();
  rotaryEncoder2.readEncoder_ISR();
  rotaryEncoder3.readEncoder_ISR();
}


void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  Time1 = millis();

  // Maybe loop these but maybe not for simplicity ;)
  pinMode(toggl, INPUT_PULLDOWN);
  pinMode(toggl2, INPUT_PULLDOWN);
  pinMode(cup, INPUT_PULLDOWN);
  pinMode(left, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);

  rotaryEncoder.begin();
	rotaryEncoder.setup(readEncoderISR);
  rotaryEncoder.setAcceleration(250); //or set the value - larger number = more accelearation; 0 or 1 means disabled acceleration

  rotaryEncoder2.begin();
	rotaryEncoder2.setup(readEncoderISR);
  rotaryEncoder2.setAcceleration(250);

  rotaryEncoder3.begin();
	rotaryEncoder3.setup(readEncoderISR);
  rotaryEncoder3.setAcceleration(250);

  for(int i=0; i<outCount; i++){    //declaring all the outputs and setting them high
    pinMode(outputs[i],OUTPUT);
    digitalWrite(outputs[i],HIGH);
  }
  for(int i=0; i<inCount; i++){     //declaring all the inputs and activating the internal pullup resistor
    pinMode(inputs[i],INPUT_PULLUP);
  }

  bleKeyboard.begin();
}

void loop() {
  static uint32_t oldtime=millis();  

  if(bleKeyboard.isConnected()) {
    togglState = digitalRead(toggl);
    toggl2State = digitalRead(toggl2);
    cupState = digitalRead(cup);
    leftState = digitalRead(left);
    rightState = digitalRead(right);
    

    rotary_loop();

    if ( (millis()-oldtime) > 100) {
      if (togglState == 1) {        
        bleKeyboard.press('f');
        Serial.println("f");
      } else if (togglState == 0) {
        bleKeyboard.release('f');        
      }

      if (toggl2State == 1) {
        bleKeyboard.press('g');
        Serial.println("g");
      } else if (togglState == 0) {
        bleKeyboard.release('g');
      }

      if (cupState == 1) {        
        cupaaa = 1;
      } else if (cupState == 0) {
        if (cupaaa == 1) {
          bleKeyboard.write(KEY_F16);
          cupaaa = 0;
        }
      }      

      if (leftState == 0) {
        bleKeyboard.press('[');
        Serial.println("[");
      } else if (leftState == 1) {
        bleKeyboard.release('[');
      }

      if (rightState == 0) {
        bleKeyboard.press(']');
        Serial.println("]");
      } else if (rightState == 1) {
        bleKeyboard.release(']');
      }

      oldtime = millis();
    }


    for (int i=0; i<4; i++)
    {    
      digitalWrite(outputs[i],LOW);   //setting one row low
      delayMicroseconds(5);           //giving electronics time to settle down
      
      for(int j=0; j<4; j++)
      {
        if(digitalRead(inputs[j]) == LOW)
        {
          keyPressed(i,j);            //calling keyPressed function if one of the inputs reads low
        }
        else if(keyDown[i][j] != 0)   //resetting the key if it is not pressed any more
        {  
          resetKey(i,j);
        }
      }
      
      digitalWrite(outputs[i],HIGH);
      delayMicroseconds(500);         //setting the row high and waiting 0.5ms until next cycle
    }

  }
}
