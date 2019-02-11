#include "DRV2605_LRA.h"



DRV2605_LRA drv;


 #define DEBUG // define debug if you want to see some informations based on the processed which are handled inside of the library

void setup() {
  Serial.begin(115200);

  drv.begin(21, 22); // .begin(SDA Pin, SCL Pin)
  drv.enablePin(18); // defines the enable pin

  //drv.enableIC(false); you can disable the IC with true or false statments while runtime


 

  //drv.test(); // just a test function if the registers of the IC are not configured. Use this function if the LRA will not play any effect
  uint8_t compensation;
  uint8_t backEMF;
  uint8_t feedback;
  // the values 102 and 105 are dedicated to the 
  Serial.print("result: ");
  Serial.println(drv.autoCal(102, 105, &compensation, &backEMF, &feedback)); // execute the auto Calibration Process
  Serial.print("comp:\t");
  Serial.println(compensation, BIN);

  Serial.print("backEMF:\t");
  Serial.println(backEMF, BIN);

  Serial.print("fb:\t");
  Serial.println(feedback, BIN);


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
