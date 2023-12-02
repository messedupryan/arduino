#include <LiquidCrystal.h>
#include <StateMachine.h>
#include <pitches.h>
#include "IRremote.h"

#define ELEMENTS(x)   (sizeof(x) / sizeof(x[0]))


// Musical Notes
const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;

int pacman_melody[][2] = {
  {NOTE_B4,16}, {NOTE_B5,16}, {NOTE_FS5,16}, {NOTE_DS5,16},
  {NOTE_B5,32}, {NOTE_FS5,16}, {NOTE_DS5,8}, {NOTE_C5,16},
  {NOTE_C6,16}, {NOTE_G6,16}, {NOTE_E6,16}, {NOTE_C6,32}, {NOTE_G6,16}, {NOTE_E6,8},
  
  {NOTE_B4,16}, {NOTE_B5,16}, {NOTE_FS5,16}, {NOTE_DS5,16}, {NOTE_B5,32},
  {NOTE_FS5,16}, {NOTE_DS5,8}, {NOTE_DS5,32}, {NOTE_E5,32}, {NOTE_F5,32},
  {NOTE_F5,32}, {NOTE_FS5,32}, {NOTE_G5,32}, {NOTE_G5,32}, {NOTE_GS5,32}, {NOTE_A5,16}, {NOTE_B5,8}
};
 
 // Buzzer, Button, and LED Pins
const byte button_pin = 2;
const byte buzzer_pin = 5;
const byte led_pin_1 = 3;
const byte led_pin_2 = 4;
          
// LCD Settings
const byte lcd_rs_pin = 7;
const byte lcd_enable_pin = 8;
const byte lcd_d4_pin = 9;
const byte lcd_d5_pin = 10;
const byte lcd_d6_pin = 11;
const byte lcd_d7_pin = 12;
const byte lcd_col_width = 16;
const byte lcd_rows = 2;

// Hacky atttempt at some musical notes
const byte black_note[8] = {
  B00001,
  B00001,
  B00001,
  B00001,
  B01111,
  B11111,
  B11111,
  B01110
};

const byte white_note[8] = {
  B00001,
  B00001,
  B00001,
  B00001,
  B01111,
  B10001,
  B10001,
  B01110
};

// initialize counter variables
int vader_counter = 0;
int pacman_counter = 0;
int last_note = 0;

// Create LCD and StateMachine objects
LiquidCrystal lcd(lcd_rs_pin, lcd_enable_pin, lcd_d4_pin, lcd_d5_pin, lcd_d6_pin, lcd_d7_pin);
StateMachine machine = StateMachine();

State* WAIT = machine.addState(&wait_state);
State* VADER = machine.addState(&vader_state);
State* PACMAN = machine.addState(&pacman_state);

void setup() {
  //Setup pin modes
  pinMode(buzzer_pin, OUTPUT);
  pinMode(button_pin, INPUT_PULLUP);  
  pinMode(led_pin_1, OUTPUT);
  pinMode(led_pin_2, OUTPUT);
  lcd.createChar(0,black_note);
  lcd.createChar(1,white_note);
  lcd.begin(lcd_col_width, lcd_rows);
  lcd.print("Push the button...");

  WAIT->addTransition([](){
    return true;
  },VADER);

  VADER->addTransition([](){
    updateCounter();
    return true;
  },PACMAN);

  PACMAN->addTransition([](){
    updateCounter();
    return true;
  },WAIT);
}
 
void loop() {
  machine.run();
}

/* ** Functions ** */

// Waits for a button to be pushed  
//  assumes INPUT_PULLUP so that LOW means it has been triggered
void buttonWait(int button_pin) {
  int buttonState = 0;
  while(1){
    buttonState = digitalRead(button_pin);
    if (buttonState == LOW) {
      return;
    }
  }
}

//
void wait_state() {
    buttonWait(button_pin);
}

void draw_notes() {
  lcd.clear();
  for (int i = 0; i < 5; i++) {
    lcd.write(byte(0));
    lcd.print(" ");
    lcd.write(byte(1));
    lcd.print(" ");
  }
  lcd.setCursor(1, 1);
  for (int i = 0; i < 5; i++) {
    lcd.write(byte(0));
    lcd.print(" ");
    lcd.write(byte(1));
    lcd.print(" ");
  }
}

// Play the entire song
void vader_state() {
  draw_notes();
  
  playNote(a, 500);
  playNote(a, 500);    
  playNote(a, 500);
  playNote(f, 350);
  playNote(cH, 150);  
  playNote(a, 500);
  playNote(f, 350);
  playNote(cH, 150);
  playNote(a, 650);
 
  delay(500);
 
  playNote(eH, 500);
  playNote(eH, 500);
  playNote(eH, 500);  
  playNote(fH, 350);
  playNote(cH, 150);
  playNote(gS, 500);
  playNote(f, 350);
  playNote(cH, 150);
  playNote(a, 650);
 
  delay(500);

  playVaderChorus();
 
  playNote(f, 250);  
  playNote(gS, 500);  
  playNote(f, 350);  
  playNote(a, 125);
  playNote(cH, 500);
  playNote(a, 375);  
  playNote(cH, 125);
  playNote(eH, 650);
 
  delay(500);
 
  playVaderChorus();
 
  playNote(f, 250);  
  playNote(gS, 500);  
  playNote(f, 375);  
  playNote(cH, 125);
  playNote(a, 500);  
  playNote(f, 375);  
  playNote(cH, 125);
  playNote(a, 650);  
 
  delay(650);

  vader_counter++;
}

void pacman_state() {
  draw_notes();
  playSong(pacman_melody, ELEMENTS(pacman_melody));
  pacman_counter++;
}

// Play an individual note
 
void playVaderChorus() {
  playNote(aH, 500);
  playNote(a, 300);
  playNote(a, 150);
  playNote(aH, 500);
  playNote(gSH, 325);
  playNote(gH, 175);
  playNote(fSH, 125);
  playNote(fH, 125);    
  playNote(fSH, 250);
 
  delay(325);
 
  playNote(aS, 250);
  playNote(dSH, 500);
  playNote(dH, 325);  
  playNote(cSH, 175);  
  playNote(cH, 125);  
  playNote(b, 125);  
  playNote(cH, 250);  
 
  delay(350);
}

void updateCounter() {
  lcd.clear();
  lcd.print("Vader: ");
  lcd.setCursor(7, 0);
  lcd.print(vader_counter);
  
  lcd.setCursor(0, 1);
  lcd.print("PacMan: ");
  lcd.setCursor(9, 1);
  lcd.print(pacman_counter);
}

// Play a single Note
void playNote(int note, int duration) {
  // Play tone on buzzer_pin
  tone(buzzer_pin, note, duration);
  // Light different LED depending on value of note
  flashLED(note, last_note, duration);
  //Stop tone on buzzer_pin
  noTone(buzzer_pin);
  last_note = note;
  delay(50);
}

// Play a song based on notes[] and durations[] arrays
void playSong(int notes[][2], int count) {
  for (int i = 0; i < sizeof(notes); i++) {
    //to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int duration = 1000 / notes[i][1];
    tone(buzzer_pin, notes[i], duration);
    flashLED(notes[i][0], last_note, duration);

    //to distinguish the notes, set a minimum time between them.
    //the note's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);
    
    last_note = notes[i];
    //stop the tone playing:
    noTone(buzzer_pin);
  }
}

void flashLED(int current, int last, int duration) {
      // Light different LED depending on value of note
    if(last > current)
    {
      digitalWrite(led_pin_1, HIGH);
      delay(duration);
      digitalWrite(led_pin_1, LOW);
    } else if(last == current) {
      digitalWrite(led_pin_1, HIGH);
      digitalWrite(led_pin_2, HIGH);
      delay(duration);
      digitalWrite(led_pin_1, LOW);
      digitalWrite(led_pin_2, LOW);
    } else {
      digitalWrite(led_pin_2, HIGH);
      delay(duration);
      digitalWrite(led_pin_2, LOW);
    }
}