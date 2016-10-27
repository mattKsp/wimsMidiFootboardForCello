/*----------------------------setup---------------------*/

void setupButtons() {
  for (int i = 0; i < btTotal; i++) {
    //btToggled[i] = 0; //
    //bt[i] = Bounce(i, debounceTime);  //setup Bounce object for each button
    pinMode(i, INPUT_PULLUP); //activate internal pullup resistor on pin    
    // 'i' ..!!!!! ????? //as long as both button array 'virtual' and 'physical' start at '0' !!!!! 
  }
}

void setupDisplay() {
  usbMIDI.setHandleControlChange(OnControlChange);  //set callback for CC MIDI messages received
  
  inFX = 2;   //2 0 6 //sets up for scrolling intro message
  outFX = 2;  //0   1 //..
  P.begin();
  P.displayClear();
  P.displaySuspend(false);
  P.setInvert(false);
  P.setIntensity(displayBrightness/256);
  P.displayText(curMessage, CENTER, SPEED_TIME, PAUSE_TIME, effect[inFX], effect[outFX]);
  strcpy(curMessage, defMessage); //setup for first loop
  //newMessage[0] = '\0'; //???

  updateBankInIntroMessage(); //copy current bank to last char in message
}
