/* @file    WearableTurnSignal.ino
 * @author  Hayden Cornwell
 * @version 1.0
 * @brief   Requires active-high toggle switches for proper operation.
 *          To use, simply click the toggle switch for left or right, or click both for
 *          flashing hazard lights.
 *
 * @details This sketch implements sequential turn signals controlled by an Arduino.
 *
 *          For most Arduinos (Uno, Pro Micro, etc.), digital pins 2--5 and 6--9 can be
 *          used for the left and right sequential turn signals, with pins 10 and 11
 *          being the left and right latching/toggle switches and pin 12 being the buzzer.
 *          
 *          On the Pro Trinket, however, pins 2 and 7 are unavailable, and 9--13 are on
 *          the other side of the Trinket, so a suggested pinout would be to have pins
 *          3--6 and 9--12 controlling the left and right blinkers, respectively, with
 *          pins 8 and 13 used as matching button control pins and analog pin A0 used to
 *          control the buzzer.
 *
 *          Due to the current and voltage requirements, the use of an external power
 *          source and transistors to switch the LEDs and buzzer on and off is recommended.
 *          It is also recommended to use a 3.3V Arduino and 3.3V voltage regulators for
 *          improved efficiency and battery life.
 *
 *          A TinyCAD/Windows EMF/PNG schematic is included in the Github repository, using
 *          an Uno R3 as an example.
 */

#include<Arduino.h>

// button pins
const byte leftButtonPin = 13;
const byte rightButtonPin = 8;
// buzzer pin
const byte buzzerPin = A0;
// how many turn signal lights there are to turn on in sequence
const byte num_signal_lights = 4;
// the sequences of turn signal lights
const byte leftSignalPins[num_signal_lights] = {9, 10, 11, 12};
const byte rightSignalPins[num_signal_lights] = {3, 4, 5, 6};
// cycle step interval in ms
long unsigned int interval = 500;

// enumerated type listing possible blinker states for all possible button combinations
enum blinkerstate : byte {
    off,
    left,
    right,
    hazards
};

// function prototypes
blinkerstate readButtonStates(void);
void leftTurnSignal(void);
void rightTurnSignal(void);
void engageHazardLights(void);
void disableSignals(void);
void buzzerClick(void);

// function run at Arduino boot
void setup(void) {
    // set pin modes
    pinMode(leftButtonPin, INPUT);
    pinMode(rightButtonPin, INPUT);
    pinMode(buzzerPin, OUTPUT);
    for (int i = 0; i < num_signal_lights; i++) {
        pinMode(leftSignalPins[i], OUTPUT);
        pinMode(rightSignalPins[i], OUTPUT);
    }
}

// function looped forever
void loop(void) {
    // get the button state
    blinkerstate selection = readButtonStates();

    if (selection == left) {
        // flash the left turn signal
        leftTurnSignal();
    }
    else if (selection == right) {
        // flash the right turn signal
        rightTurnSignal();
    }
    else if (selection == hazards) {
        // flash both lights if hazards are engaged
        engageHazardLights();
    }
    else {
        // kill the lights if they're not already off
        disableSignals();
    }
}

/*  @function       readButtonStates()
 *  @param          (this function takes no arguments)
 *  @return         blinkerstate
 *  @brief          reads the states of the two buttons and interprets the result
 */
blinkerstate readButtonStates(void) {
    // the state of the blinker (derp)
    blinkerstate state;
    // if the left blinker button is pressed . . .
    if (digitalRead(leftButtonPin) == HIGH) {
        // . . . and the right button is also pressed, it's hazard lights
        if (digitalRead(rightButtonPin) == HIGH) {
            state = hazards;
        }
        // . . . and the right button is not, it's a left turn signal
        else {
            state = left;
        }
    }
    // else, if only the right button is pressed, it's a right turn signal
    else if (digitalRead(rightButtonPin) == HIGH) {
        state = right;
    }
    // if neither is pressed, it's off
    else {
        state = off;
    }
    // wait 1 ms for debouncing
    delay(1);

    return state;
}

/*  @function       leftTurnSignal()
 *  @param          (this function takes no arguments)
 *  @return         (this function returns no value)
 *  @brief          generates a sequential left turn signal
 */
void leftTurnSignal(void) {
    // whether the left turn signal is still on
    bool left_turn = true;
    // keep cycling while the signal is on
    while (left_turn) {
        // turn off all the lights
        disableSignals();
        // cycle through the lights
        for (int i = 0; i < num_signal_lights && left_turn; i++) {
            digitalWrite(leftSignalPins[i], HIGH);
            // click the buzzer
            buzzerClick();
            // watch the buttons while waiting
            long unsigned int start_time = millis();
            while (millis() - start_time < interval && left_turn) {
                readButtonStates() == left ? left_turn = true : left_turn = false;
            }
        }
    }
}

/*  @function       rightTurnSignal()
 *  @param          (this function takes no arguments)
 *  @return         (this function returns no value)
 *  @brief          generates a sequential right turn signal
 */
void rightTurnSignal(void){
    // whether the left turn signal is still on
    bool right_turn = true;
    // keep cycling while the signal is on
    while (right_turn) {
        // turn off all the lights
        disableSignals();
        // cycle through the lights
        for (int i = 0; i < num_signal_lights && right_turn; i++) {
            digitalWrite(rightSignalPins[i], HIGH);
            // click the buzzer
            buzzerClick();
            // watch the buttons while waiting
            long unsigned int start_time = millis();
            while (millis() - start_time < interval && right_turn) {
                readButtonStates() == right ? right_turn = true : right_turn = false;
            }
        }
    }
}

/*  @function       engageHazardLights()
 *  @param          (this function takes no arguments)
 *  @return         (this function returns no value)
 *  @brief          flashes all turn signal lights
 */
void engageHazardLights(void) {
    bool hazard_lights = true;
    while (hazard_lights) {
        // turn off all the lights
        disableSignals();
        // click the buzzer
        buzzerClick();
        // wait briefly
        long unsigned int start_time = millis();
        while (millis() - start_time < interval && hazard_lights) {
            readButtonStates() == hazards ? hazard_lights = true : hazard_lights = false;
        }
        // turn on all the lights
        for (int i = 0; i < num_signal_lights; i++) {
            digitalWrite(leftSignalPins[i], HIGH);
            digitalWrite(rightSignalPins[i], HIGH);
        }
        // click the buzzer
        buzzerClick();
        // wait briefly
        start_time = millis();
        while (millis() - start_time < interval && hazard_lights) {
            readButtonStates() == hazards ? hazard_lights = true : hazard_lights = false;
        }
    }
}

/*  @function       disableSignals()
 *  @param          (this function takes no arguments)
 *  @return         (this function returns no value)
 *  @brief          turns off all turn signal lights
 */
void disableSignals(void) {
    // turn off all the lights
    for (int i = 0; i < num_signal_lights; i++) {
        digitalWrite(leftSignalPins[i], LOW);
        digitalWrite(rightSignalPins[i], LOW);
    }
}

/*  @function       buzzerClick()
 *  @param          (this function takes no arguments)
 *  @return         (this function returns no value)
 *  @brief          generates a click from a piezo buzzer at 3.90 KHz by quickly cycling the potential
 */
void buzzerClick(void) {
    // click the buzzer by turning it on and then off quickly.
    for (int i = 0; i < 8; i++) {
        digitalWrite(buzzerPin, HIGH);
        delayMicroseconds(128);
        digitalWrite(buzzerPin, LOW);
        delayMicroseconds(128);
    }
}
