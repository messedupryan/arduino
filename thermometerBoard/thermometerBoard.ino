#include <LiquidCrystal.h>
#include <DHT.h>
#define DHT_TYPE DHT11

// Configure settings
const int TEMP_THRESHOLD = 70;

// Pin Configuration
const byte BUZZER_PIN = 5;
const byte ENV_SENSOR_PIN = 2;
const byte LCD_RS_PIN = 7;
const byte LCD_ENABLE_PIN = 8;
const byte LCD_DATA_PIN_4 = 9;
const byte LCD_DATA_PIN_5 = 10;
const byte LCD_DATA_PIN_6 = 11;
const byte LCD_DATA_PIN_7 = 12;
const byte LCD_COLS = 16;
const byte LCD_ROWS = 2;
const byte LED_PIN_GREEN= 3;
const byte LED_PIN_RED = 4;


// Initialize Environment Sensor
DHT EnvSensor(ENV_SENSOR_PIN, DHT_TYPE);

// Initialize LCD
LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_DATA_PIN_4, LCD_DATA_PIN_5, LCD_DATA_PIN_6, LCD_DATA_PIN_7);

void setup() {
  Serial.begin(9600);
  Serial.println(F("Initializing LCD"));
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.print("Waiting for sensor");
  
  Serial.println(F("Configuring PIN modes"));
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN_GREEN, OUTPUT);
  pinMode(LED_PIN_RED, OUTPUT);
  pinMode(ENV_SENSOR_PIN, INPUT);
  
  Serial.println(F("Starting Sensor"));
  EnvSensor.begin();
}

void loop() {
  if (getTemp() > TEMP_THRESHOLD) {
    toggleLED(LED_PIN_RED, LED_PIN_GREEN);
  } else {
    toggleLED(LED_PIN_GREEN, LED_PIN_RED);
  }
  delay(1000);
}

float getTemp() {
  float humidity = EnvSensor.readHumidity();
  float temp_c = EnvSensor.readTemperature();
  // Compute heat index in Farenheit (isFahreheit = true)
  float temp_f = EnvSensor.readTemperature(true);
  // Compute heat index in Celsius (isFahreheit = false)
  float heat_index_c = EnvSensor.computeHeatIndex(temp_c, humidity, false);
  float heat_index_f = EnvSensor.computeHeatIndex(temp_f, humidity);

  if (isnan(humidity) || isnan(temp_c) || isnan(temp_f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Do some serial logging to be friendly
  Serial.print(F("%  Temperature: "));
  Serial.print(temp_f);
  Serial.print(F("°F "));
  // Serial.print(temp_c);
  // Serial.print(F("°C"))
  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.print(F("  Heat index: "));
  Serial.print(heat_index_f);
  Serial.print(F("°F"));
  // Serial.print(heat_index_c);
  // Serial.println(F("°C "));
  Serial.println("");

  lcd.clear();
  lcd.print(F("Humidity: "));
  lcd.print(humidity);
  lcd.setCursor(0,1);
  lcd.print(F("Temp: "));
  lcd.print(temp_f);

  
  // return temp in °F, because I am american and change is HARD
  return temp_f;
}

// toggleLED()
//    led_pin_on pin of LED to turn on
//    led_pin_off pin of LED to turn off
void toggleLED(byte led_pin_on, byte led_pin_off) {
  digitalWrite(led_pin_off, HIGH);
  digitalWrite(led_pin_on, LOW);
}
