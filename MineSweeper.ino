
// Define the two potentiometers' input numbers (0 and 1)
#define INPUT_PIN_X 0
#define INPUT_PIN_Y 1

// Used to save code and alternate between HIGH and LOW for blinking effects.
bool delaynow = false;

/*
 *		Two matrices are used to update the state
 *		of the 3x3 LED grid.
 *		currLights is used to decide which light should be off(0), on(1),
 *		or blinking(2).

 *		lightsValues is used to keep the static states of the lights.
 *		In this matrix, the lights are either off(0) or on(1), and it only
 *		changes ONCE per position when a light is turned off.
 */
int currLights[][3] = { {1,1,1,},
                        {1,1,1,},
                        {1,1,1,} };  
                        
int lightsValues[][3] = { {1,1,1,},
                          {1,1,1,},
                          {1,1,1,} }; 

// Parameters to assist in decision making.                         
int minesDefused = 0;
int mineI = 0;
int mineJ = 0;
int inputI = 0;
int inputJ = 0;
int delayVal = 200;

// the setup function runs once when you press reset or power the board
void setup() {
  randomSeed(analogRead(0));
  int mineI = random(3);
  int mineJ = random(3);
//  Serial.begin(9600);
  pinMode(13, INPUT_PULLUP);
  for (int i = 1; i < 10; ++i) {
    pinMode(i, OUTPUT);
  }
}


// the loop function runs over and over again forever
void loop() {
  
	/*
	 *		First, check if the button is pressed
	 *		in an attempt to defuse a light.
	 */
  int sensorVal = digitalRead(13);
  if(!sensorVal) {

		// If the light was not defused before
		//	 Run the defuse animation and then check if
		// the mine was triggered.
    if (lightsValues[inputI][inputJ] != 0) {
      defuseAnimation(inputI, inputJ);

      if (inputI == mineI && inputJ == mineJ) {
        loseAnimation();
        resetGame();
      } else {		// Successful defusals increase the defused lights counter.
        lightsValues[inputI][inputJ] = 0;
        ++minesDefused;
      }
    }
  }
	
	// If the player successfully defused 8 lights,
	// then the last light is the mine, and so the player
	// wins!
  if (minesDefused == 8) {
    delay(1000);
    winAnimation();
    resetGame();
  }
  
	/*
	 *		Second, synch the two matrices that are used to
	 * 	activate the 3x3 LED grid in preperation to process
	 *		the current potentiometers' input.

	 *		Once the input is recieved, update the current position
	 *		to 2 (blinking mode), and update the LED grid.
	 */ 
  currLights[inputI][inputJ] = lightsValues[inputI][inputJ];
  inputI = volts_to_index(analogRead(INPUT_PIN_X));
  inputJ = volts_to_index(analogRead(INPUT_PIN_Y));
  currLights[inputI][inputJ] = 2;
  

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      if (currLights[i][j] == 0) {
        digitalWrite(getPinNum(i,j), LOW);
      } else if (currLights[i][j] == 1) {
        digitalWrite(getPinNum(i,j), HIGH);
      } else if (currLights[i][j] == 2) {
        if (lightsValues[i][j] == 0) {		// if this light was turned off, blink in a different style
          delayVal = 1000;
        } else {
          delayVal = 200;
        }
        int pinToBlink = getPinNum(i,j);
        if (delaynow) {
          digitalWrite(pinToBlink, LOW);
        } else {
          if (lightsValues[i][j] == 0) {
            delayVal = 50;
          }
          digitalWrite(pinToBlink, HIGH);
        }
        delaynow = !delaynow;
      }
    }
  }
  delay(delayVal);
}


/*
 *		A simple function to call each time
 *  a player loses or wins the game.
 *  Simply returns the grid to its initial state.
 */
void resetGame() {
	// Set all the matrices to a 3x3 grid of ones. 
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      currLights[i][j] = 1;
      lightsValues[i][j] =1;
    }
  }
  
	// Reset all params and get a new random mine position.
  minesDefused = 0;
  mineI = random(3);
  mineJ = random(3);
  int inputI = 0;
  int inputJ = 0;
  int delayVal = 200;
}


/*
 *		Given a 3x3 array with input pins alligned like so:
					-----j----
				|	1		4		7
				i	2		5		8
				|	3		6		9
 *		calculate the pin number, given indices i and j as follows:
 *		if j == 0, return i+1
 *  else if j == 1, return i+4
 *  else if j == 2, return i+7

 *  example, when i == 1 and j == 2, output pin is 8.
 */
int getPinNum(int i, int j) {
  switch (j) {
    case 0:
      return i+1;
    case 1:
      return i+4;
    case 2:
      return i+7;
  }
}


/*
 *		A function to convert a potentiometer's
 *		input (0-1023) to 0, 1, or 2.
 */
int volts_to_index(int v) {
  if(v < 341)
    return 0;
  else if(v < 682)
    return 1;
  else
    return 2;
}


/*
 *		Animation that plays each time the player
 *  attempts to defuse one of the lights.
 */
void defuseAnimation(int i, int j) {
	//		Blink 25 times. Each time
	// decrease the delay time by 2.
  int delayTime = 70;
  while (delayTime > 20) {
    digitalWrite(getPinNum(i,j), HIGH);
    delay(delayTime);
    digitalWrite(getPinNum(i,j), LOW);
    delay(delayTime);
    delayTime -=2;
  }
}


/*
 *		Animation to play when a player
 *		wins the game (very rare...)
 */
void winAnimation() {
	/*
	 *		Initialize the grid to show only the mine.
	 */
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      int led = getPinNum(i,j);
      if (led == getPinNum(mineI, mineJ)) {
        digitalWrite(led, HIGH);
      } else {
        digitalWrite(led, LOW);
      }
    }
  }

  /*
	 *		'Flip' the grid 20 times.
	 */  
  int count = 20;
  while (count > 0) {
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        int led = getPinNum(i,j);
        digitalWrite(led, !digitalRead(led));
      }
    }
    delay(100);
    --count;
  }

	/*
	 *		Turn the grid off and wait for a second.
	 */
  for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        int led = getPinNum(i,j);
        digitalWrite(led, LOW);
      }
  }
  delay(1000);

}


/*
 *		Animation to play when a player
 *		loses the game (happens all the time.)
 */
void loseAnimation() {
	/*
	 *		Initialize the 3x3 grid to form an 'X'
	 */
  for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        int led = getPinNum(i,j);
        if (led%2 == 1) {
          digitalWrite(led, HIGH);
        } else {
          digitalWrite(led, LOW);
        }
      }
  }
  
  /*
	 *		'Flip' the grid 20 times.
	 */
  int count = 20;
  while (count > 0) {
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        int led = getPinNum(i,j);
        digitalWrite(led, !digitalRead(led));
      }
    }
    delay(70);
    --count;
  }

	/*
	 *		Turn the grid off and wait for a second.
	 */
  for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        int led = getPinNum(i,j);
        digitalWrite(led, LOW);
      }
  }
  delay(1000);
  
}
