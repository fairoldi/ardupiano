/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/
#include <Keypad.h>

#include <Button2.h>
#include <Rotary.h>

#include <LCD_I2C.h>
#include "led.h"
#include "rtttl.h"

#define INH 8
#define C_A 13
#define C_B 12
#define C_C 11
#define PIEZO 10

#define ROWS 2 
#define COLS 4
#define ROW_PINS {3, 2}
#define COL_PINS {7, 6, 5, 4}

#define ROTARY_PIN1	A2
#define ROTARY_PIN2	A1
#define BUTTON_PIN	A0

#define CLICKS_PER_STEP   4 

#define N_STATES 3

#define STOPPED -1
#define PLAYING 1

int state = 0;
int playback_state = STOPPED;
char states[N_STATES][2][41] = {
  {"Piano\0", "\0"},
  {"Twinkle twinkle\0", "little star\0"},
  {"Inno alla\0", "Gioia\0"}
};

char songs [N_STATES-1][256]= {
  "Twinkle:d=4,o=4,b=160:c4,c4,g4,g4,a4,a4,g4,p,f4,f4,e4,e4,d4,d4,c4\0",
  "Ode:d=4,o=4,b=120:e,e,f,g,g,f,e,d,c,c,d,e,e.,8d,2d,8p,e,e,f,g,g,f,e,d,c,c,d,e,d.,8c,2c"
};

LCD_I2C lcd(0x27, 16, 2);
Rotary r;
Button2 b;  

char hexaKeys[ROWS][COLS] = {
  {8,7,6,5},
  {4,3,2,1}
};
byte rowPins[ROWS] = ROW_PINS;
byte colPins[COLS] = COL_PINS;

int frequencies[] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5};

Keypad piano = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
LedArray led = LedArray(INH, C_A, C_B, C_C);


void update_lcd(){
  lcd.clear();
  lcd.print(states[state][0]);
  lcd.setCursor(0, 1);
  lcd.print(states[state][1]);
}

void setup(){
  Serial.begin(9600);
  pinMode(PIEZO, OUTPUT);

  delay(50);
  lcd.begin();
  lcd.backlight();
  update_lcd();

  r.begin(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP, 0, N_STATES-1);
  r.setChangedHandler(rotate);

  b.begin(BUTTON_PIN);
  b.setTapHandler(click);
  b.setLongClickHandler(resetPosition);
}
  
void loop(){
  r.loop();
  b.loop();

  switch (state) {
  case 0:
    pianoLoop();
    break;
  default:
    if (playback_state != STOPPED) {
      play_rtttl(songs[state-1], PIEZO, led);
      playback_state = STOPPED;
    }    
    break;
  }
}


void pianoLoop(){
  uint8_t key = piano.getKey();
  uint8_t state = piano.getState();
  switch(state){
    case HOLD:
      Serial.print("HOLD: ");
      Serial.println(key);
      break;
    case RELEASED:
      led.off();
      noTone(PIEZO);
      break;
    case PRESSED:
      if (key > 0) {
        led.on(key);
        Serial.println(key);
        tone(PIEZO, frequencies[key-1]);
      }
      break;
     //RELEASED
  }
}



void rotate(Rotary& r) {
  state = r.getPosition();
  update_lcd();
}

 
// single click
void click(Button2& btn) {
  Serial.println("Click!");
  playback_state = PLAYING;
}

// long click
void resetPosition(Button2& btn) {
  r.resetPosition();
  Serial.println("Reset!");
}
