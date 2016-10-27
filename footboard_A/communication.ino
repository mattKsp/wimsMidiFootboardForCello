/*----------------------------communication---------------------*/

/*MIDI message types:
 * 0 - Note ON
 * 1 - Note OFF
 * 2 - 
 * 3 - Control Change (CC)
 * 4 - 
 * 5 - 
 * 6 - 
 * 7 - System Exclusive (SysEx)
 * ..?
*/
//CC 0 - bank select (MSB) | CC 32 - bank select (LSB)

void readMidiData() {
  if(usbMIDI.read()) {  //check if there is a MIDI message available (returns 0/1)
    if (usbMIDI.getType() == 7) { //if message is sysEx
      readMidiSysExData(); // look for sysEx msg
    } else {
      usbMIDI.read(); //auto process and trigger callback functions - i think this will work
    }
  }
  //while (usbMIDI.read()) { }  //ignore/discard incoming messages - TEMP solution
}

/*----------------------------CC---------------------*/

//void readMidiCcData() {
//  byte *sysExBytes = usbMIDI.getSysExArray();
//  
//}

void OnControlChange(byte channel, byte ctrl, byte value) {
  //callback function (see setup)
  //---You should declare as volatile any variables that you modify within the attached function.---interupts
  if (ctrl == 0x00) { //bank select
    int tb = (int)value; //cast (covert) byte to int
    changeBank(tb);
  }
}

/*----------------------------sysEx---------------------*/

//currently only 2 sysex messages
//1x 32byte - systemSettings
//1x 16byte - bankSettings

void readMidiSysExData() {
  byte *sysExBytes = usbMIDI.getSysExArray();
  
  if (sysExBytes[0] == 0xf0   //START sysex message
  && sysExBytes[1] == 0x7D    // 7D is private use (placeholder for non-commercial entities)
  && sysExBytes[2] == 0x4C    // START 4-byte 'key' (not really needed via USB)
  && sysExBytes[3] == 0x65
  && sysExBytes[4] == 0x69
  && sysExBytes[5] == 0x66)   // END 'key'
    {
      //END sysex message. 16byte message and 32byte. last byte in message
      if (sysExBytes[15] == 0xf7) { processSystemSettingsMessage(sysExBytes); }
      else if (sysExBytes[31] == 0xf7) { processBankSettingsMessage(sysExBytes); }
    }
}

void processSystemSettingsMessage(byte *sysExBytes) {
  saveSystemSettings(sysExBytes);
  sendConfirmationMessage();
}
void processBankSettingsMessage(byte *sysExBytes) {
  saveBankSettings(sysExBytes);
  sendConfirmationMessage();
}

/*----------------------------extra---------------------*/

void sendConfirmationMessage() {
  byte data[] = { 0xF0, 0x7D, 0xF7 }; // return confirmation ACK msg - should be safe for any device even if listening for 7D
  usbMIDI.sendSysEx(3, data);         // SEND confirmation message back to computer
}
