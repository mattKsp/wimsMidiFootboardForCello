/*----------------------------memory load/save---------------------*/

//split memory into parts
//a - settings
//b - bank 0 settings
//c - bank 1..
//d - etc.

/*-----------loading------------*/

void eepromLoad() {
  //eeprom is wiped everytime a new sketch is uploaded, but will be persistant thru normal use.
  loadSystemSettings();
  loadBankSettings();
}

void loadSystemSettings() {
  //if system settings saved flag set at pos 8 is '1', then we have system data to be read from memory.. failsafe
  if (EEPROM.read(7) == 1) {
    displayBrightness = EEPROM.read(0);   //brightness of any displays/LEDs
    defaultMidiChannel = EEPROM.read(4);  //the default midi channel
    curBank = EEPROM.read(6);             //the current bank selected
  }
}

void loadBankSettings() {
  //load bank settings (5 banks)     
  //if bank settings saved flag set at pos 8 is '1', then we have bank data to be read from memory.. failsafe
  if (EEPROM.read(8) == 1) {
    for (int b = 0; b < bankTotal; b++) {
      for (int i = 0; i < 24; i++) {  //iterate through and read EEPROM for sysEx data. copy straight to int array.
        eepromCurrentBankSettings[b][i] = EEPROM.read((b*24)+9 +i);
        delay(10);  // give time for EEPROM read??  // 5 / 10/ 100 ?
      }
      //midi check/fix. for every bank, loops round 'x' (max 5) buttons. checks midi is 0-16, if outside range sets channel to 1.
      for (int f = 0; f < btTotal; f++) {
        int tMidi = eepromCurrentBankSettings[b][(f*4)+3];
        if (tMidi > 0 && tMidi < 17) { }  //inside-out
        else { 
          eepromCurrentBankSettings[b][(f*4)+3] = defaultMidiChannel; 
          EEPROM.write( ((b*24) +9 +(f*4) +3), defaultMidiChannel); //corrects the stored version
        }
      }
    } //END bank loop
  } //END bank settings read
} //END loadBankSettings

/*-----------saving------------*/

void saveSystemSettings(byte *sysExBytes) {
  displayBrightness = sysExBytes[0];   //brightness of any displays/LEDs
  defaultMidiChannel = sysExBytes[4];
  EEPROM.write(7, 1); //write a 1 to pos 7 to indicate that system data has been saved and is available for use.
  loadSystemSettings(); //reload/update vars from eeprom
}
void saveBankSettings(byte *sysExBytes) {
  for (int b = 0; b < bankTotal; b++) {
    for (int i = 0; i < 24; i++) {
      EEPROM.write((b*24)+9 +i, sysExBytes[i+6]);
    }
  }
  EEPROM.write(8, 1); //write a 1 to pos 8 to indicate that bank data has been saved and is available for use.
  loadBankSettings(); //reload/update vars from eeprom
}

/*-----------changing------------*/

void changeBank(int bank) {
    prevBank = curBank;   //save bank
    curBank = bank;         //set bank
    EEPROM.write(6, bank);  //save bank
    //updateBankInIntroMessage();   //bit overkill.. prob never play intro message again this session
    String m = "B"; //new string
    m += bank;   //value;   //add int to the end
    char mc[sizeof(m)]; //create pointer to char array the same size as string
    m.toCharArray(mc, sizeof(mc));  //copy string to char array
    flashMessage(mc, btMessageTime); //"Bx"
}
