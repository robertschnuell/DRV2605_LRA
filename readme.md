# DRV 2605 Arduino Library

## Why using a LRA
To create feedbacks in the field of human machine interaction,  haptic vibrations are commonly  used. Most of the used vibrations motores ab based on the EMA instead of an LRA. Instead of creating a vibration on two axis an Linear Resonant Actuator (LRA) creates a vibration only on one axis. The advantage of this is that you have better control of the created vibration as well as the vibration response can be much shorter and more precise. As one example of the usage of an LRA's the tapic engine (apple)  can be named. Which created the illusion of a click.

Be aware while buying an LRA. Recently bought one at aliexpress.com first which after removing the housing turned out to be an EMA instead.


A good guid about Linear Resonant Actuator can be found at the website of this company:
https://www.precisionmicrodrives.com/vibration-motors/linear-resonant-actuators-lras/


This is not an advising for the precision microdrives company, I just found there documentation and products interesting.

## Tested setup

This library was tested successfully with an own breakout for the DRV2605 as well as with the breakout from Adafruit.
As an Linear Resonant Actuator the C10-100 from precision microdrives was used. (https://www.precisionmicrodrives.com/product/c10-100-10mm-linear-resonant-actuator-4mm-type)

## Getting Started

As an easy starting point use a breakout for the DRV2605 which is breadboard friendly in case that you just want to try out the effects of an LRA. Adafruit and Sparkfun are offering such products.

### Connect
1. Connect the 2 Wires of the LRA to the Motor+ and Motor- Terminal.
2. Connect the VCC Terminal to a rated voltage source of the LRA (starting at 2.5V)
3. Connect the GND Terminal to the GND of your power supply as well as the GND of your microcontroller
4. Connect both I2C Terminals to the microcontroller (SDA,SCL)

### Calibrate the LRA
To get the best results out of the LRA the registers of the DRV2605 has to be set to the values of the used LRA. After this run the LRA_calibration.ino example sketch. The results of this test will be written into the registers after the test as well as will be printed via Serial so you can use them afterwards.

For this process you will need following informations:
- Rated Voltage
- Rated Clamp Voltage

All of this informations usually can be found in the datasheet of the used LRA.
If you are using the C10-100 from precision microdrives you can easy use the function calibrate("C10-100")  which will handle the rest for you and you don not have to search for this informations at all.

### Select effects
The easiest way to create a vibration is to use one of the build in effects  of the DRV2605. Use the function selectEffect(int id )
An overview about all of the available effects with there corresponding id is listed in the datasheet.


### Create Vibration
Finally use the function play() to create a vibration. Be aware to not trigger another play() directly afterwards. Use instead a delay() of the milliseconds the chosen effect needs to be finished after starting to play another one.

### The example code
Now you are done and can play around with this powerful IC. There are some examples added to use the DRV2605 releated to several input triggers.
 // insert example code

## More functions to drive an LRA
There are several more functions to control an LRA with the DRV2605, like realtime playback or playing an effect after an interrupt trigger without using the I2C bus.


## Documents
https://www.precisionmicrodrives.com/product/datasheet/c10-100-10mm-linear-resonant-actuator-4mm-type-datasheet.pdf
http://www.ti.com/lit/ds/symlink/drv2605.pdf
http://www.ti.com/lit/an/sloa189/sloa189.pdf
