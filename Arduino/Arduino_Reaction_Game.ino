/*

 Brett's Reaction Game

 Plays a game where the player must hold down a switch to start the game LED
 fading in, and release it as close to the brightness wrap-around point as
 possible without it blanking. How well you do lights up the appropriate
 'scoring' LED.
 Refer to the components attached to the various pins.

 Created 12 January 2010
 By Brett Taylor

 Documentation and circuit layout available at:

http://inner.geek.nz/projects/reaction-game

 This work is licensed under a Creative Commons
 Attribution 3.0 New Zealand License.

http://creativecommons.org/licenses/by/3.0/nz/

*/

// the pins the brightness board pins attach to.
const int scorePins[] = {  12, // too quick, cold
 11, // too quick, warm
 10, // right on target, win!
 9,  // too high, warm
 8  // too high, cold
 }; 

const int scorePinCount = 5; // how many brightness pins there are 

const int buttonPin = 2;// button connects to this
const int gamePin = 5; // game fading pin, must be PWM

int oldState = 0; // used to track the changing of button state
int readState = 0; 

int brightness = 0; // current game brightness

int counter = 0; // used to check for a minimum hold time, prevents scoring of
 // short presses, and also used for the win mode flashing!

int i = 0; // used in the for loops in setup() and allScorePins();

int win = 0; // if the player has won, use this to keep track

void setup() {
 // set up score pins
 for (i = 0; i < scorePinCount; i++) {
 pinMode(scorePins[i], OUTPUT);
 }
 pinMode(buttonPin, INPUT);
 pinMode(gamePin, OUTPUT);
}

void loop() {
 readState = digitalRead(buttonPin);

 // this function calls other functions depending on the state of the button. Check below
 // for the code for pressed(), held(), and released().

 // check for button state transition
 if (readState == LOW && oldState == HIGH) { // button has just been pressed
 pressed(); // run the button pressed event

 }
 if (readState == LOW && oldState == LOW) { // button is held down
 held(); // run the button is being held event
 }
 if (readState == HIGH && oldState == LOW) { // button released
 released(); // run the button released event
 counter = 0; // reset the counter
 }

 if (win == 1) { // if the player has won the last game:

 // we want to flash the middle score LED

 counter++; // count cycles

 if (counter < 10) { // if it's under 50ms (5 * 10ms)...
 digitalWrite(scorePins[2], HIGH);
 } else if (counter < 20) { // otherwise if it's under 100ms...
 digitalWrite(scorePins[2], LOW);
 } else {
 counter = 0; // otherwise start the cycle over!
 }
 delay(5); // wait 10ms
 }
 oldState = readState;
}

void pressed() { // button has just been pressed:

 win = 0;  // new game, not winning any more

 brightness = 0;  // reset brightness to 0

 allScorePins(LOW); // turn all the score pins on
 //-- see below for the function

 counter = 0; // start counting how long the button
 // has been pressed for

 delay(10); // wait till button bouncing is likely to
 // have ended.
}

void held() { // button is being held:
 counter++; // increment the counter
 brightness++; // increment the brightness
 if (brightness > 255) { // PWM is ranged between 0-255, make it
 brightness = 0; // loop around
 }
 analogWrite(gamePin, brightness); // update the game pin's brightness
 delay(10); // wait a little
}

void released() { // button has been released:

 allScorePins(LOW); // turn all the score pins off

 // check to see if the game ran at least 128 cycles
 if (counter < 128) {
 analogWrite(gamePin, 0); // reset the game pin to off
 return; // return control to loop();
 }

 // now we check the value of brightness to see which
 // range it is within, and light up the appropriate LED.

 if (brightness > 128 && brightness <= 200 ) { // too quick, cold.
 digitalWrite(scorePins[0], HIGH);
 } else if (brightness > 200 && brightness <= 240 ) { // too quick, warm
 digitalWrite(scorePins[1], HIGH);
 } else if (brightness > 240 && brightness <= 255) { // right on the money! win!
 digitalWrite(scorePins[2], HIGH);
 win = 1;  // player wins!
 } else if (brightness >= 0 && brightness <= 30 ) { // too slow, warm
 digitalWrite(scorePins[3], HIGH);
 } else if (brightness > 30 && brightness <= 128 ) { // too slow, cold.
 digitalWrite(scorePins[4], HIGH);
 }
}

void allScorePins(int val) { 

 // this is a function that takes a value passed to it, stashes that in 'val'
 // and sets all the score pins to the value of 'val'

 for (i = 0; i < scorePinCount; i++) { // take each score pin in turn
 digitalWrite(scorePins[i], val); // set it to the value
 }
}
