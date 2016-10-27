# W!Ms MIDI footboard for cello
MIDI foot controller for use with Cello and guitar<br>
Repository is an Arduino project using Teensy MIDI<br>
Transmits and receives MIDI Notes, CC & Sysex messages over USB

### Equipment
#### Hardware
- Teensy 3.2 board
- 5x Normally Closed foot switches (pins 0-4)
- 1x LED (pin 6)
- 2x 8x8 LED matrix (more coming) (pins CS 10, DOUT 11, CLK 13)
- USB cable
- Case (of some description)

#### Software
- Arduino 
- Teensyduino plugin (for Arduino) - https://www.pjrc.com/teensy/td_download.html
  * Make sure Arduino software is set to 'MIDI'
  * You must select MIDI from the "Tools > USB Type" menu
- Parola display library - https://parola.codeplex.com/
- Bounce library
- EEPROM libary
- usbMIDI library (teensy)

### Licence
- Written by M.Standish (Thurstan/mattKsp) 2016 (..this is available to anyone for free, i don't care that much, just don't steal my name)
- Released under GNU GPLv3 (see LICENCE file)

This software is provided on an "AS-IS-BASIS".

### Reference
https://www.pjrc.com/teensy/td_midi.html<br>
https://forum.pjrc.com/threads/24537-Footsy-Teensy-2-based-MIDI-foot-pedal-controller<br>
https://learn.sparkfun.com/tutorials/midi-tutorial/all
