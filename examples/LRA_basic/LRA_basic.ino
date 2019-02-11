#include "DRV2605_LRA.h"



DRV2605_LRA drv;

#define DEBUG

void setup() {

  drv.begin();
  drv.enablePin(18); // defines the enable pin
  drv.autoCal(102, 105); // execute the auto Calibration Process
  drv.setupDone(); // save the calibration data to the registers of the IC and finish




}

void loop() {

  // loop through different effects
  for ( int i = 1; i < 16; i++) {
    drv.play(i); // play the selected effect
    Serial.print("i:\t");
    Serial.println(i);
    delay(1000);

  }


  delay(3000);

  drv.play(10); // play one specific effect. The corosponting names to the effect number can be found in the datasheet

  delay(3000);

}
