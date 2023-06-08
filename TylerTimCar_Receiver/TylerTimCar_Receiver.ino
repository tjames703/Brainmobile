/**********
  Written by Tyler James
**********/

#include "BLEDevice.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  displayInit();
  driveInit();
}

void loop() {
  // put your main code here, to run repeatedly:

  // Starts car after press of button
  if(readButton()){
    if (readSwitch()){
      while(1){
        WIFIHandle();
        delay(100);
      }
    }
    else{
      while(1){
        // driveTest(readPot()/410);
        BLEHandle();
        delay(100);
      }
    }
  }

  updateIdleDisplay();
  delay(1000);
}
