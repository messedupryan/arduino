# arduino

This is a collection of random junk I have created while learning how to use an arduino

## darth_vader

This project uses an ardino to play the Darth Vader March from Star Wars. It will wait for the button to be pressed and then will play the song via the buzzer while displaying text/characters on and LCD. It keeps track of how many times the song has been played.

##### components:

- 1 x arduino uno
- 1 x LCD1602 LCD Panel (16 char by 2 Rows)
- 1 x piezo Buzzer
- 1 x Potentiometer (For LCD contrast control)
- 1 x Push Button
- 2 x Color LED's
- 2 x 220 ohm resistor for LED


## frankieBoard

This project is called frankieBoard beacause this is a Frankenstein like device where I am trying to test/integrate as many components as I can from an [Elegoo Uno E3 Super Starter Kit](https://www.elegoo.com/collections/arduino-learning-sets/products/elegoo-uno-r3-super-starter-kit).  _frankieBoard_ uses an ardino to play multiple "songs". I have taken music that was converted to notes using a library for [pitches](https://github.com/hibit-dev/buzzer/blob/master/lib/pitches.zip) to play some popular theme songs. I am using songs converted by [HiBit](https://github.com/hibit-dev). 

##### Song List
- pacman theme
- Star Wars: A New Hope theme

##### components
> (this is subject to change frequently as I swap components and struggle to fit everything on a single breadboard)

- 1 x arduino uno
- 1 x LCD1602 LCD Panel (16 char by 2 Rows)
- 1 x piezo Buzzer
- 1 x Potentiometer (For LCD contrast control)
- 1 x Push Button
- 2 x Color LED's
- 2 x 220 ohm resistor for LED
- 1 x IR Receiver
- 1 x IR Remote

## thermometerBoard

This project again leverages an Elegoo Uno E3 Super Starter Kit](https://www.elegoo.com/collections/arduino-learning-sets/products/elegoo-uno-r3-super-starter-kit).  _thermometerBoard_ uses an arduino to collect temperature and humidity data. It has an LCD Panel to display current values. It supports a serial connections at 9600 baud for raw data logging as well. This project uses a DHT11 sensor, and I am using the [aidafruit DHT-Sensor-Library](https://github.com/adafruit/DHT-sensor-library/tree/master) to access sensor data. There is a 7-Segment display combined with 2 buttons to allow the user to set and adjust a threshold temperature. This threshold controls some basic feedback for the user.  

_User Experience_
- When the temperature is below the threshold a Green LED is lit
- When the temp rises above the threshold a Red LED is lit. 
- An active buzzer is included to play an audible alert periodically when above a critical level.

##### components
> (this is subject to change frequently as I swap components and struggle to fit everything on a single breadboard)

- 1 x arduino uno
- 1 x LCD1602 LCD Panel (16 char by 2 Rows)
- 1 x piezo Buzzer
- 1 x Potentiometer (For LCD contrast control)
- 2 x Push Button
- 1 x Red LED
- 1 x Green LED
- 2 x 220 ohm resistor for LEDs
- 1 x 4 digit 7-Segment display
- 1 x DHT11 Temperature sensor
