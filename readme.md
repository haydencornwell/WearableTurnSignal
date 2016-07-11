This sketch implements sequential turn signals controlled by an Arduino.

For most Arduinos (Uno, Pro Micro, etc.), digital pins 2--5 and 6--9 can be used for the left and right sequential turn signals, with pins 10 and 11 being the left and right latching/toggle switches and pin 12 being the buzzer.

On the Pro Trinket, however, pins 2 and 7 are unavailable, and 9--13 are on the other side of the Trinket, so a suggested pinout would be to have pins 3--6 and 9--12 controlling the left and right blinkers, respectively, with pins 8 and 13 used as matching button control pins and analog pin A0 used to control the buzzer.

Due to the current and voltage requirements, the use of an external power source and transistors to switch the LEDs and buzzer on and off is recommended. It is also recommended to use a 3.3V Arduino and 3.3V voltage regulators for
improved efficiency and battery life.

A TinyCAD/Windows EMF/PNG schematic is included in the Github repository, using an Uno R3 as an example.

A copy of this readme exists in the header comment in WearableTurnSignal.ino.
