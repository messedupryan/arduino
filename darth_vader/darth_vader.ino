#include <LiquidCrystal.h>

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
 
 // Buzzer, Button, and LED Pins
const int button_pin = 2;
const int buzzer_pin = 5;
const int led_pin_1 = 3;
const int led_pin_2 = 4;

// LCD Settings
const int lcd_rs_pin = 7;
const int lcd_enable_pin = 8;
const int lcd_d4_pin = 9;
const int lcd_d5_pin = 10;
const int lcd_d6_pin = 11;
const int lcd_d7_pin = 12;

const int lcd_col_width = 16;
const int lcd_rows = 2;

// LCD Settings

LiquidCrystal lcd(lcd_rs_pin, lcd_enable_pin, lcd_d4_pin, lcd_d5_pin, lcd_d6_pin, lcd_d7_pin);
int counter = 0;
int last_note = 0;

byte black_note[8] = {
  B00001,
  B00001,
  B00001,
  B00001,
  B01111,
  B11111,
  B11111,
  B01110
};

byte white_note[8] = {
  B00001,
  B00001,
  B00001,
  B00001,
  B01111,
  B10001,
  B10001,
  B01110
};

void setup() {
  //Setup pin modes
  pinMode(buzzer_pin, OUTPUT);
  pinMode(button_pin, INPUT_PULLUP);  
  pinMode(led_pin_1, OUTPUT);
  pinMode(led_pin_2, OUTPUT);
  lcd.createChar(0,black_note);
  lcd.createChar(1,white_note);
  lcd.begin(lcd_col_width, lcd_rows);
  // Print a message to the LCD.
  lcd.print("Push the button...");
}
 
void loop() {
  buttonWait(button_pin);
  playDarthVaderMarch();
  updateCounter();
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

// Play the entire song
void playDarthVaderMarch() {
  lcd.clear();
  for (int i = 0; i < 5; i++) {
    lcd.write(byte(0));
    lcd.print(" ");
    lcd.write(byte(1));
    lcd.print(" ");
  }
  
  playIntro();

  playChorus();
 
  playNote(f, 250);  
  playNote(gS, 500);  
  playNote(f, 350);  
  playNote(a, 125);
  playNote(cH, 500);
  playNote(a, 375);  
  playNote(cH, 125);
  playNote(eH, 650);
 
  delay(500);
 
  playChorus();
 
  playNote(f, 250);  
  playNote(gS, 500);  
  playNote(f, 375);  
  playNote(cH, 125);
  playNote(a, 500);  
  playNote(f, 375);  
  playNote(cH, 125);
  playNote(a, 650);  
 
  delay(650);
  
  counter++;
}

// Play an individual note
void playNote(int note, int duration)
{
  // Play tone on buzzer_pin
  tone(buzzer_pin, note, duration);
 
  // Light different LED depending on value of note
  if(last_note > note)
  {
    digitalWrite(led_pin_1, HIGH);
    delay(duration);
    digitalWrite(led_pin_1, LOW);
  }else
  {
    digitalWrite(led_pin_2, HIGH);
    delay(duration);
    digitalWrite(led_pin_2, LOW);
  }
 
  //Stop tone on buzzer_pin
  noTone(buzzer_pin);
 
  last_note = note;
  delay(50);
}


// First 
void playIntro() {
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
}
 
void playChorus() {
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
  lcd.print("Darth Marches:");
  // Move to the second character of the second line
  lcd.setCursor(1, 1);
  lcd.print(counter);
}