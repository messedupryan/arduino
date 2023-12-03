#include <IRremote.h>
#include <LiquidCrystal.h>
#include <pitches.h>

#define ELEMENTS(x)   (sizeof(x) / sizeof(x[0]))

// Various Device PIN settings
const byte BUZZER_PIN = 5;
const byte IR_SENSOR_PIN = 13; 
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

/* 🎉 Lets get this party started! 🚀*/

// initialize counter and other utility variables
int new_hope_counter = 0;
int pacman_counter = 0;
// Keep track of the last note that was played
int last_note = 0;
//vairable uses to store the last decodedRawData
uint32_t last_decodedRawData = 0;


// Create LCD and IR Receiver objects
IRrecv irrecv(IR_SENSOR_PIN);
LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_DATA_PIN_4, LCD_DATA_PIN_5, LCD_DATA_PIN_6, LCD_DATA_PIN_7);

// Create State Machines
//  see https://github.com/jrullan/StateMachine for more detail on the state machine library
// StateMachine machine = StateMachine();
// State* WAIT = machine.addState(&wait_state);
// State* NEWHOPE = machine.addState(&new_hope_state);
// State* PACMAN = machine.addState(&pacman_state);
// State* nextState = nullptr;

// arduino setup - only executed once
void setup() {
  //Setup pin modes
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN_A, OUTPUT);
  pinMode(LED_PIN_B, OUTPUT);
  pinMode(IR_SENSOR_PIN, INPUT);

  // Initialize LCD Screen
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.print("Use the remote,");
  lcd.setCursor(1,1);
  lcd.print(" Luke...");

  // Define 2 musical notes as custom LCD Chars
  lcd.createChar(0,QUARTER_NOTE);
  lcd.createChar(1,HALF_NOTE);

  // Inititalize Serial connection for monitoring IR
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode");
  irrecv.enableIRIn(); // Start the receiver
  
}
 
// arduino loop - executed after setup()
void loop() {
  if (irrecv.decode()) { // have we received an IR signal?
    translateIR();
    irrecv.resume(); // receive the next value
  }
}

/* ** Other Functions ** */

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
    if(last == current) {
      digitalWrite(LED_PIN_A, HIGH);
      digitalWrite(LED_PIN_B, HIGH);
      delay(duration);
      digitalWrite(LED_PIN_A, LOW);
      digitalWrite(LED_PIN_B, LOW);
    } else if(last > current){
      digitalWrite(LED_PIN_A, HIGH);
      delay(duration);
      digitalWrite(LED_PIN_A, LOW);
    } else {
      digitalWrite(LED_PIN_B, HIGH);
      delay(duration);
      digitalWrite(LED_PIN_B, LOW);
    }
}

// Play a song based on a multi-dimensional array containing
//  each song contains couplets of Notes and Durations
//  see song definitions above for more detail
void playSong(int notes[][2], int numNotes) {
  for (int i = 0; i < numNotes; i++) {
    //to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int duration = 1000 / notes[i][1];
    tone(BUZZER_PIN, notes[i][0], duration);
    flashLED(notes[i][0], last_note, duration);
    //to distinguish the notes, set a minimum time between them.
    //the note's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * (100/85);
    // int pauseBetweenNotes = 100;
    delay(pauseBetweenNotes);
    // Store the last note played. We are using this to control the blinking LED's
    last_note = notes[i][0];
  }
}

void translateIR() // takes action based on IR code received
{
  // Check if it is a repeat IR code 
  if (irrecv.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
    //set the current decodedRawData to the last decodedRawData 
    irrecv.decodedIRData.decodedRawData = last_decodedRawData;
    Serial.println("REPEAT!");
  } else {
    //output the IR code on the serial monitor
    // if (irrecv.decodedIRData.decodedRawData != 0) {
      Serial.print("IR code:0x");
      Serial.println(irrecv.decodedIRData.decodedRawData, HEX);
    // }
  }
  //map the IR code to the remote key
  switch (irrecv.decodedIRData.decodedRawData) {
    case 0xB847FF00:
      Serial.println("FUNC/STOP"); 
      // machine.transitionTo(WAIT);
      break;
    case 0xF30CFF00: 
      Serial.println("1");
      draw_notes();
      playSong(pacman_melody, ELEMENTS(pacman_melody));
      pacman_counter++;
      updateCounters();
      break;
    case 0xE718FF00:
      Serial.println("2");
      draw_notes();
      playSong(new_hope_melody, ELEMENTS(new_hope_melody));
      new_hope_counter++;
      updateCounters();
      break;
    case 0xA15EFF00: Serial.println("3");
      draw_notes();
      delay(1000);
      new_hope_counter++;
      updateCounters();
      break;
    // To-Do: figure these state transitions out and a method to pause
    case 0xBB44FF00:
      Serial.println("FAST BACK");
      break;
    case 0xBF40FF00:
      Serial.println("PAUSE");
      break;
    case 0xBC43FF00:
      Serial.println("FAST FORWARD");
      break;
    // The rest of the buttons will simply log the press via serial out
    case 0xB946FF00: Serial.println("VOL+");    break;
    case 0xBA45FF00: Serial.println("POWER");   break;
    case 0xF807FF00: Serial.println("DOWN");    break;
    case 0xEA15FF00: Serial.println("VOL-");    break;
    case 0xF609FF00: Serial.println("UP");      break;
    case 0xE619FF00: Serial.println("EQ");      break;
    case 0xF20DFF00: Serial.println("ST/REPT"); break;
    case 0xE916FF00: Serial.println("0");       break;
    case 0xF708FF00: Serial.println("4");       break;
    case 0xE31CFF00: Serial.println("5");       break;
    case 0xA55AFF00: Serial.println("6");       break;
    case 0xBD42FF00: Serial.println("7");       break;
    case 0xAD52FF00: Serial.println("8");       break;
    case 0xB54AFF00: Serial.println("9");       break;
    case 0x0: Serial.println("9");              break;
    default:
      Serial.println(" other button   ");
  }
  //store the last decodedRawData
  last_decodedRawData = irrecv.decodedIRData.decodedRawData;
  delay(500); // Do not get immediate repeat
}

// Update LCD Screen with current Counters
void updateCounters() {
  lcd.clear();
  lcd.print("New Hope: ");
  lcd.setCursor(10, 0);
  lcd.print(new_hope_counter);
  
  lcd.setCursor(0, 1);
  lcd.print("PacMan: ");
  lcd.setCursor(9, 1);
  lcd.print(pacman_counter);
}