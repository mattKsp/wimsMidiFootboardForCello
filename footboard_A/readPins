/*----------------------------read data from pins---------------------*/

void analogGet() {
  //sub-loop - reads all analog pins
  
}

void digitalGet() {
  //sub-loop - reads all digital pins / buttons
  //fallingEdge/risingEdge may be wrong way round cos using NC switches ?
  for (int i = 0; i < btTotal; i++) {
    bt[i].update(); //update buttons (this handles everything to do with reading the pins)
    //SAVE THIS FOR v2 - whether the note on/cc gets triggered on the falling or rising edge
    if (bt[i].fallingEdge()) {
      //button pressed (connected to ground) - NC ON STATE
      if (eepromCurrentBankSettings[curBank][i*4] == 0 | eepromCurrentBankSettings[curBank][i*4] == 1) {  
        //button sends Note messages
        usbMIDI.sendNoteOn(eepromCurrentBankSettings[curBank][i*4+1], 127, eepromCurrentBankSettings[curBank][i*4+3]); //note, val, midi channel
        usbMIDI.send_now();
        flashMessage("ON", btMessageTime); //"bt"

      } else if (eepromCurrentBankSettings[curBank][i*4] == 3) {
        //button sends CC messages
        usbMIDI.sendControlChange(eepromCurrentBankSettings[curBank][i*4+2], 127, eepromCurrentBankSettings[curBank][i*4+3]);  //control, value, channel
        usbMIDI.send_now();
        flashMessage("CC", btMessageTime); //"bt"
      }
    } //end fallingEdge
    
    if (bt[i].risingEdge()) {
      //button released (internal pullup to HIGH) - NC OFF STATE
      //if (eepromCurrent[i*4] == 0) {
      if (eepromCurrentBankSettings[curBank][i*4] == 0 | eepromCurrentBankSettings[curBank][i*4] == 1) { 
        //button sends Note messages
        usbMIDI.sendNoteOff(eepromCurrentBankSettings[curBank][i*4+1], 0, eepromCurrentBankSettings[curBank][i*4+3]); //note, val, midi channel
        usbMIDI.send_now();
        //flashMessage("OFF", 1, 2, btMessageTime); //"bt"
      } else if (eepromCurrentBankSettings[curBank][i*4] == 3) {
        //button sends CC messages
        //don't need to send anything else, already sent on falling edge
        //..will add triggering on falling/rising in v2
        //usbMIDI.send_now();
        //flashMessage("CC", 1, 2, btMessageTime); //"bt"
      }
    } //end risingEdge
    
  } //end for
} //end digitalGet
