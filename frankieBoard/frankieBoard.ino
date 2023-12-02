#include <IRremote.h>
#include <LiquidCrystal.h>
#include <pitches.h>
#include <StateMachine.h>

#define ELEMENTS(x)   (sizeof(x) / sizeof(x[0]))

// Buzzer, Button, and LED Pins
const byte BUTTON_PIN = 2;
const byte BUZZER_PIN = 5;
const byte LED_PIN_A = 3;
const byte LED_PIN_B = 4;
          
// LCD Settings
const byte LCD_RS_PIN = 7;
const byte LCD_ENABLE_PIN = 8;
const byte LCD_DATA_PIN_4 = 9;
const byte LCD_DATA_PIN_5 = 10;
const byte LCD_DATA_PIN_6 = 11;
const byte LCD_DATA_PIN_7 = 12;
const byte LCD_COLS = 16;
const byte LCD_ROWS = 2;

// Hacky atttempt at some musical notes
//  According to wikipedia these are Half/Quarter Notes https://en.wikipedia.org/wiki/List_of_musical_symbols
const byte HALF_NOTE[8] = {
  B00001,
  B00001,
  B00001,
  B00001,
  B01111,
  B10001,
  B10001,
  B01110
};

const byte QUARTER_NOTE[8] = {
  B00001,
  B00001,
  B00001,
  B00001,
  B01111,
  B11111,
  B11111,
  B01110
};

// ** Song Definitions ** //
//  These songs were converted to notes by https://github.com/hibit-dev/
//  We are taking it one step further by taking the note and duration arrays and combining them into a multi-dimensional array.
//  The structure is song_array[][2] = {{NOTE_CONSTANT, TIMING}, {NOTE_CONSTANT, TIMING}}
//  where TIMING is given in fractions of a second where we only provide the divisor 
//  e.g., 2, 4, 8, 16 for 1/2th, 1/4th, 1/8th, 1/16th of a second

// Notes taken from https://github.com/hibit-dev/buzzer/blob/master/src/games/pacman/pacman.ino
int pacman_melody[][2] = {
  {NOTE_B4,16}, {NOTE_B5,16}, {NOTE_FS5,16}, {NOTE_DS5,16},
  {NOTE_B5,32}, {NOTE_FS5,16}, {NOTE_DS5,8}, {NOTE_C5,16},
  {NOTE_C6,16}, {NOTE_G6,16}, {NOTE_E6,16}, {NOTE_C6,32}, {NOTE_G6,16}, {NOTE_E6,8},
  
  {NOTE_B4,16}, {NOTE_B5,16}, {NOTE_FS5,16}, {NOTE_DS5,16}, {NOTE_B5,32},
  {NOTE_FS5,16}, {NOTE_DS5,8}, {NOTE_DS5,32}, {NOTE_E5,32}, {NOTE_F5,32},
  {NOTE_F5,32}, {NOTE_FS5,32}, {NOTE_G5,32}, {NOTE_G5,32}, {NOTE_GS5,32}, {NOTE_A5,16}, {NOTE_B5,8}
};
 
// Notes taken from https://github.com/hibit-dev/buzzer/blob/master/src/movies/star_wars/star_wars.ino
int new_hope_melody[][2] = {
  {NOTE_AS4,8}, {NOTE_AS4,8}, {NOTE_AS4,8},
  {NOTE_F5,2}, {NOTE_C6,2},
  {NOTE_AS5,8}, {NOTE_A5,8}, {NOTE_G5,8}, {NOTE_F6,2}, {NOTE_C6,4},
  {NOTE_AS5,8}, {NOTE_A5,8}, {NOTE_G5,8}, {NOTE_F6,2}, {NOTE_C6,4},
  {NOTE_AS5,8}, {NOTE_A5,8}, {NOTE_AS5,8}, {NOTE_G5,2}, {NOTE_C5,8}, {NOTE_C5,8}, {NOTE_C5,8},
  {NOTE_F5,2}, {NOTE_C6,2},
  {NOTE_AS5,8}, {NOTE_A5,8}, {NOTE_G5,8}, {NOTE_F6,2}, {NOTE_C6,4},

  {NOTE_AS5,8}, {NOTE_A5,8}, {NOTE_G5,8}, {NOTE_F6,2}, {NOTE_C6,4},
  {NOTE_AS5,8}, {NOTE_A5,8}, {NOTE_AS5,8}, {NOTE_G5,2}, {NOTE_C5,8}, {NOTE_C5,16},
  {NOTE_D5,4}, {NOTE_D5,8}, {NOTE_AS5,8}, {NOTE_A5,8}, {NOTE_G5,8}, {NOTE_F5,8},
  {NOTE_F5,8}, {NOTE_G5,8}, {NOTE_A5,8}, {NOTE_G5,4}, {NOTE_D5,8}, {NOTE_E5,4}, {NOTE_C5,8}, {NOTE_C5,16},
  {NOTE_D5,4}, {NOTE_D5,8}, {NOTE_AS5,8}, {NOTE_A5,8}, {NOTE_G5,8}, {NOTE_F5,8},

  {NOTE_C6,8}, {NOTE_G5,16}, {NOTE_G5,2}, {REST,8}, {NOTE_C5,8},
  {NOTE_D5,4}, {NOTE_D5,8}, {NOTE_AS5,8}, {NOTE_A5,8}, {NOTE_G5,8}, {NOTE_F5,8},
  {NOTE_F5,8}, {NOTE_G5,8}, {NOTE_A5,8}, {NOTE_G5,4}, {NOTE_D5,8}, {NOTE_E5,4}, {NOTE_C6,8}, {NOTE_C6,16},
  {NOTE_F6,4}, {NOTE_DS6,8}, {NOTE_CS6,4}, {NOTE_C6,8}, {NOTE_AS5,4}, {NOTE_GS5,8}, {NOTE_G5,4}, {NOTE_F5,8},
  {NOTE_C6,1}
};

/* Lets get this party started! */
// Main logic will follow

// initialize counter variables
int new_hope_counter = 0;
int pacman_counter = 0;
int last_note = 0;

// Create LCD and StateMachine objects
LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_DATA_PIN_4, LCD_DATA_PIN_5, LCD_DATA_PIN_6, LCD_DATA_PIN_7);
StateMachine machine = StateMachine();

State* WAIT = machine.addState(&wait_state);
State* NEWHOPE = machine.addState(&new_hope_state);
State* PACMAN = machine.addState(&pacman_state);

// arduino setup - only executed once
void setup() {
  //Setup pin modes
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  
  pinMode(LED_PIN_A, OUTPUT);
  pinMode(LED_PIN_B, OUTPUT);
  // Define 2 musical notes as custom LCD Chars
  lcd.createChar(0,QUARTER_NOTE);
  lcd.createChar(1,HALF_NOTE);
  // Initialize LCD Screen
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.print("Push the button...");

  // Define State Machine Transitions
  WAIT->addTransition([](){
    return true;
  },PACMAN);

  NEWHOPE->addTransition([](){
    updateCounters();
    return true;
  },WAIT);

  PACMAN->addTransition([](){
    updateCounters();
    return true;
  },NEWHOPE);
}
 
// arduino loop - executed after setup()
void loop() {
  // Trigger Our State Machine to run
  machine.run();
}

/* ** State Function Definitions ** */

// Patiently wait for a button push
void wait_state() {
    buttonWait(BUTTON_PIN);
}

// Play the theme song from Star Wars: A New Hope
void new_hope_state() {
  draw_notes();
  playSong(new_hope_melody, ELEMENTS(new_hope_melody));
  new_hope_counter++;
}

// Play the pacman theme song
void pacman_state() {
  draw_notes();
  playSong(pacman_melody, ELEMENTS(pacman_melody));
  pacman_counter++;
}

/* ** Other Functions ** */
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

// Draw 2 rows of alternating musical notes on the LCD
//  this seems less boring and gives the user something to look at besides the blinky lights
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

// Flash 2 LED lights by comparing the current note to the previous note
//  this is just an attempt to make the lighting more intersting than simply alternating.
void flashLED(int current, int last, int duration) {
      // Light different LED depending on value of note
    if(last > current)
    {
      digitalWrite(LED_PIN_1, HIGH);
      delay(duration);
      digitalWrite(LED_PIN_1, LOW);
    } else if(last == current) {
      digitalWrite(LED_PIN_1, HIGH);
      digitalWrite(LED_PIN_2, HIGH);
      delay(duration);
      digitalWrite(LED_PIN_1, LOW);
      digitalWrite(LED_PIN_2, LOW);
    } else {
      digitalWrite(LED_PIN_2, HIGH);
      delay(duration);
      digitalWrite(LED_PIN_2, LOW);
    }
}

// Play a song based on notes[] and durations[] arrays
void playSong(int notes[][2], int count) {
  for (int i = 0; i < count; i++) {
    //to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int duration = 1000 / notes[i][1];
    tone(BUZZER_PIN, notes[i][0], duration);
    flashLED(notes[i][0], last_note, duration);

    //to distinguish the notes, set a minimum time between them.
    //the note's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);
    
    last_note = notes[i][0];
    //stop the tone playing:
    noTone(BUZZER_PIN);
  }
}

// Update LCD Screen with current Counters
void updateCounters() {
  lcd.clear();
  lcd.print("New Hope: ");
  lcd.setCursor(11, 0);
  lcd.print(new_hope_counter);
  
  lcd.setCursor(0, 1);
  lcd.print("PacMan: ");
  lcd.setCursor(9, 1);
  lcd.print(pacman_counter);
}