/*
   W!Ms MIDI footboard A for cello
   by M.Standish (Thurstan/mattKsp)

   Teensy 3.2 board
   Teensuduino plugin for Arduino software

   Make sure Arduino software is set to 'MIDI'
   You must select MIDI from the "Tools > USB Type" menu

   5x Normally Closed foot switches (pins 0-4)
   1x LED (pin 6)
   2x 8x8 LED matrix (more coming) (pins CS 10, DOUT 11, CLK 13)

   Parola display library - https://parola.codeplex.com/
   Bounce library
   EEPROM libary
   usbMIDI library (teensy)
   
   https://www.pjrc.com/teensy/td_midi.html
   https://forum.pjrc.com/threads/24537-Footsy-Teensy-2-based-MIDI-foot-pedal-controller
   https://learn.sparkfun.com/tutorials/midi-tutorial/all
*/


int statusLED = 6; //PWM LED with resistor on pin 6 //10  //this has to go higher than 8, cos 8 bts start at 0..

/*-----------------------------------buttons-------------------------------*/

//don't need to do interrupts for this project, overkill, scanning on main loop is fast enough.
#include <Bounce.h>  //(de-)Bounce library for buttons
#include<EEPROM.h>  //EEPROM library to store tiny amount of persistant data on Teensy
//eeprom data is persistant between uses, but is re-written every time a sketch is uploaded
//if saving settings to eeprom everytime startup, the memory will be good for about 10,000 cycles
//to put into perspective..
//on/off once a day for 27.4 years
//on/off 4 times a day for 6.8 years

unsigned long debounceTime = 5; //5 milli-seconds debounce time
const int mainLoopDelay = 0;  //delay in milli-seconds if needed for tweaking / reduces message frequency
const int btTotal = 1;   //total count of buttons used (max 5 to start with). cannot set Bounce using this unfortunately!
//Bounce bt[btTotal]; //initialise array of 'n' buttons
//!!! change to match 'btTotal' !!!
Bounce bt[1] = {
  Bounce(0, debounceTime),
  //Bounce(1, debounceTime),
  //Bounce(2, debounceTime),
  //Bounce(3, debounceTime),
  //Bounce(4, debounceTime),
  };

/*-----------------------------------saved settings-------------------------------*/

//loading/saving settings to eeprom
//eeprom pos 0 to 8 (9 total)
//copy eepromCurrentSystemSettings to individual vars
int displayBrightness = 128;  //brightness of any displays/LEDs   //eeprom 0  //divide by 256 for parola display
int defaultMidiChannel = 1; //the default midi channel            //eeprom 4
volatile int prevBank = 0; //the previous bank selected                    //...this doesn't need loading/saving!!!
volatile int curBank = 0;  //the current bank selected                     //eeprom 6
//system settings saved - read straight from memory each time     //eeprom 7
//bank settings saved - read straight from memory each time       //eeprom 8
//eeprom pos 9 to 32 | 33 to 56 | 57 to  80 | 81 to 104 | 105 to 128 (24 per bank)
//5 rows of 24 columns
const int bankTotal = 5;  
//could use..
//bankTotal = (sizeof(eepromCurrentBankSettings / sizeof(eepromCurrentBankSettings[0]));  
//..but not necessary cos bank size won't change
int eepromCurrentBankSettings[][24] = {
  //see spreadsheet
  {0,  0,   22,  1, 0, 1,   23,  1, 0, 2,   24,  1, 0, 3,   25,  1, 0, 4,   26,  1, 0, 0, 0, 0 },
  {0,  5,   27,  1, 1, 6,   28,  2, 1, 7,   29,  2, 1, 8,   30,  2, 1, 9,   31,  2, 0, 0, 0, 0 },
  {0,  10,  102, 1, 2, 11,  103, 3, 2, 12,  104, 3, 2, 13,  105, 3, 2, 14,  106, 3, 0, 0, 0, 0 },
  {0,  15,  107, 1, 3, 16,  108, 4, 3, 17,  109, 4, 3, 18,  110, 4, 3, 19,  111, 4, 0, 0, 0, 0 },
  {0,  20,  112, 1, 4, 21,  113, 5, 4, 22,  114, 5, 4, 23,  115, 5, 4, 24,  116, 5, 0, 0, 0, 0 }
};

/*-----------------------------------display-------------------------------*/

  //from footboard_parola_test1
#include <MD_Parola.h>
#include <MD_MAX72xx.h>

// Define the number of devices we have in the chain and the hardware interface
#define MAX_DEVICES 2
#define CS_PIN    10
#define DATA_PIN  11
#define CLK_PIN   13

// HARDWARE SPI
MD_Parola P = MD_Parola(CS_PIN, MAX_DEVICES);

#define SPEED_TIME      50 //25
#define PAUSE_TIME      3000  //1000
uint8_t  frameDelay = 50;  // default frame delay value
uint8_t  inFX, outFX;
uint8_t prevInFX, prevOutFX;
textEffect_t  effect[] =
{
  PRINT,
  SCAN_HORIZ,
  SCROLL_LEFT,
  WIPE,
  SCROLL_UP_LEFT,
  SCROLL_UP,
  OPENING_CURSOR,
  GROW_UP,
  SCROLL_UP_RIGHT,
  BLINDS,
  CLOSING,
  GROW_DOWN,
  SCAN_VERT,
  SCROLL_DOWN_LEFT,
  WIPE_CURSOR,
  DISSOLVE,
  OPENING,
  CLOSING_CURSOR,
  SCROLL_DOWN_RIGHT,
  SCROLL_RIGHT,
  SLICE,
  SCROLL_DOWN,
};

// Global message buffers shared by Serial and Scrolling functions   
#define BUF_SIZE  75
//char introMessage[BUF_SIZE] = "<  >    MIDI Cello footboard A  -  W!M | Thurstan    <  >    Bank 0   <  >"; //  |for cello |
char introMessage[BUF_SIZE] = "<  >     W!Ms MIDI footboard A   -   by Thurstan      |     Bank 0    <  >";   //'0' at end gets replaced with actual number
char defMessage[BUF_SIZE] = "W!M";
char prevMessage[BUF_SIZE];
char curMessage[BUF_SIZE];
char newMessage[BUF_SIZE];
bool newMessageAvailable = false;

// TIMING
volatile bool flashMessageActive = false;
volatile long flashMessageOffAt = 0;
bool firstTimeRun = true; //gets toggled to false when first time setup stuff done
bool firstTimeRun2 = true;  //cumbersome way of doing things really
long firstTimeRunMessageOffAt = 0;
const long btMessageTime = 10; //how long the button trigger message flashes up for


/* ----------------MAIN--------------------- */

void setup() {
  //Serial.begin(9600);
  eepromLoad(); //load anything from memory first (if anything available)
  //
  pinMode(statusLED, OUTPUT); //setup status LED pin
  setupButtons();
  setupDisplay();
  blinkStatusLED();
}

void loop() {
  //check for any midi messages
  readMidiData();

  //two loops to handle display of intro message
  //might change 'firstTimeRun' var to (something like) 'lock' ???
  if (firstTimeRun2) {
    strcpy(curMessage, introMessage);
    //changeEffect(1, 2, false);
    P.displayReset();
    firstTimeRun2 = false;
    firstTimeRunMessageOffAt = millis() + 15000;
  }
  if (firstTimeRun) {
    if (P.displayAnimate()) {
      if (millis() >= firstTimeRunMessageOffAt) {
         //P.setTextEffect(effect[0], effect[0]);
         P.setTextEffect(effect[6], effect[1]);
         strcpy(curMessage, defMessage);
        firstTimeRun = false; //reset
      }
    P.displayReset();
    }
  }
  
  if (!firstTimeRun) {
    analogGet();  //'analogGet' sub-loop
    digitalGet(); //'digitalGet' sub-loop
    displaySet(); //'displaySet' sub-loop
  }
  
  delay(mainLoopDelay); //just in case
}


/* ----------------stuff--------------------- */

//    String m = "b"; //new string
//    m += value;      //add int to the end
//    char mc[sizeof(m)]; //create pointer to char array the same size as string
//    m.toCharArray(mc, sizeof(mc));  //copy string to char array
  //int msgSize = sizeof(introMessage);
  //char mc;
  //mc = s;
  //s.toCharArray(mc, 1);
