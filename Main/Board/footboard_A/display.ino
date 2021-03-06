/*----------------------------main display setting---------------------*/

void displaySet() {
  if (flashMessageActive) {
    if (millis() >= flashMessageOffAt) {
      //changeEffect(prevInFX, prevOutFX, true);  //change back
      //changeMessage(prevMessage);
      changeMessage(defMessage);
      flashMessageActive = false; //reset
    }
  }

  if (P.displayAnimate()) 
  {
    if (newMessageAvailable)
    {
      strcpy(prevMessage, curMessage); //backup
      strcpy(curMessage, newMessage);
      //P.setTextBuffer(pc[curString]); //this alt way is to set position in an array of text
      newMessageAvailable = false;
    }
    P.displayReset();
  }
} //end displaySet

/*----------------------------display messages---------------------*/

void updateBankInIntroMessage() {
  //called from setup
  String s = curBank;
  char mc[sizeof(s)];
  s.toCharArray(mc, sizeof(mc));
  introMessage[67] = mc[0];
}

void changeMessage(char msg[BUF_SIZE]) {
  strcpy(newMessage, msg);
  newMessageAvailable = true;
}


void flashMessage(char msg[BUF_SIZE], uint8_t FXi, uint8_t FXo, long t) {
  if (!firstTimeRun) {
    if (flashMessageActive) { 
      //changeEffect(FXi, FXo, false); //problem is this changes now.. //don't backup
    } else {
      //changeEffect(FXi, FXo, true); //problem is this changes now.. //backup
    }
    flashMessage(msg, t);
  }
}
void flashMessage(char msg[BUF_SIZE], long t) {
  //show some txt for short period with effects
  //needs to change the display buffer straight away,
  //none of this newMessage stuff, bypass that totally,
  //make every flashMessage interruptable, attach interrupts to pins etc. 
  //use 'flashMessageCanInterrupt' which was set by previous flash Message
  if (!firstTimeRun) {
    if (flashMessageActive) { 
      //changeEffect(FXi, FXo, false); //problem is this changes now.. //don't backup
      //keep prevMessage
    } else {
      //changeEffect(FXi, FXo, true); //problem is this changes now.. //backup
      strcpy(prevMessage, curMessage); //backup
    }
    strcpy(curMessage, msg);
    P.displayReset();
    flashMessageActive = true;  //set for main loop
    flashMessageOffAt = millis() + t;   //5000;  //now + t secs
  }
}

void changeEffect(uint8_t FXi, uint8_t FXo, bool backup) {
  if (backup) {
    prevInFX = inFX;    //backup
    prevOutFX = outFX;  //backup
  }
  inFX = FXi;
  outFX = FXo;
  P.setTextEffect(effect[FXi], effect[FXo]);
  P.displayReset();
}

/*----------------------------LEDs---------------------*/

void blinkStatusLED() {
  //this delays the whole script. use sparingly
  digitalWrite(statusLED, HIGH);
  delay(250);
  digitalWrite(statusLED, LOW);
  delay(250);
  digitalWrite(statusLED, HIGH);
  delay(250);
  digitalWrite(statusLED, LOW);
  delay(250);
  digitalWrite(statusLED, HIGH);
  delay(250);
  digitalWrite(statusLED, LOW);
}
