
# Arduino Mine Sweeper

[![IMAGE ALT TEXT HERE](http://i.imgur.com/Apb5agb.jpg?1)](https://www.youtube.com/watch?v=fyGGfZsNYQI)



This is a simple mine sweeper game where you have a 3x3 LED array and one of them is marked as a mine.
Your goal is to 'defuse' all the lights without activating the mine.    
The code includes game logic and win/lose/defuse animations.

NOTE: I used two potentiometers as controllers because 4 push buttons (Up/Down, Left/Right) are just too boring, and I couldn't get my hands on a joystick.
This ended up being rather cool, since it adds to the general feeling of 'defusing' mines. Inspiration by: Etch-A-Sketch <3.


## Arduino Scheme
![IMAGE ALT TEXT HERE](http://i.imgur.com/6ngIo2J.png)

#### Materials:
1. 1x Arduino Uno
2. 1x Half+ sized breadboard
3. 2x 10KOhm Potentiometer
4. 1x Push Button
5. 9x Red LED

## Build Instructions
As long as you allow separate control of the nine LEDs, you can set up the board however you'd like.
However, for the code to work properly it is important to connect the LEDs in the following manner:

1   4   7     
2   5   8    
3   6   9

where each number corresponds to an output pin on the Arduino Uno board.

Once you have setup your board, simply download the code and run it in Arduino IDE!


### Have fun :)

