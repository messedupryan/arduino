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
