/*
  Arduino Library to Control the TI DRV2605 IC with an LRA
  Author: Robert Schnüll
    @robertschnuell

    Based on this datasheet: http://www.ti.com/lit/ds/symlink/drv2605.pdf


*/

#ifndef H_DRV2605_LRA
#define H_DRV2605_LRA

#include "Arduino.h"
#include <Wire.h>


// DEBUG MODE


#if defined(DEBUG)
#define spl(a) (Serial.println(a))
#define sp(a) (Serial.print(a))
#define spt() (Serial.print("\t"))
#else
#define spl(a)
#define sp(a)
#define spt()
#endif



// DRV2605 Registers (Datasheet page 31)
#define DRV2605_ADDR 0x5A

#define DRV2605_ADRESS_STATUS 0x00
#define DRV2605_ADRESS_MODE 0x01
#define DRV2605_MODE_INTTRIG  0x00
#define DRV2605_MODE_EXTTRIGEDGE  0x01
#define DRV2605_MODE_EXTTRIGLVL  0x02
#define DRV2605_MODE_PWMANALOG  0x03
#define DRV2605_MODE_AUDIOVIBE  0x04
#define DRV2605_MODE_REALTIME  0x05
#define DRV2605_MODE_DIAGNOS  0x06
#define DRV2605_MODE_AUTOCAL  0x07


#define DRV2605_ADRESS_RTPIN 0x02
#define DRV2605_ADRESS_LIBRARY 0x03
#define DRV2605_ADRESS_WAVESEQ1 0x04
#define DRV2605_ADRESS_WAVESEQ2 0x05
#define DRV2605_ADRESS_WAVESEQ3 0x06
#define DRV2605_ADRESS_WAVESEQ4 0x07
#define DRV2605_ADRESS_WAVESEQ5 0x08
#define DRV2605_ADRESS_WAVESEQ6 0x09
#define DRV2605_ADRESS_WAVESEQ7 0x0A
#define DRV2605_ADRESS_WAVESEQ8 0x0B

#define DRV2605_ADRESS_GO 0x0C
#define DRV2605_ADRESS_OVERDRIVE 0x0D
#define DRV2605_ADRESS_SUSTAINPOS 0x0E
#define DRV2605_ADRESS_SUSTAINNEG 0x0F
#define DRV2605_ADRESS_BREAK 0x10
#define DRV2605_ADRESS_AUDIOCTRL 0x11
#define DRV2605_ADRESS_AUDIOLVL 0x12
#define DRV2605_ADRESS_AUDIOMAX 0x13
#define DRV2605_ADRESS_RATEDV 0x16
#define DRV2605_ADRESS_CLAMPV 0x17
#define DRV2605_ADRESS_AUTOCALCOMP 0x18
#define DRV2605_ADRESS_AUTOCALEMP 0x19
#define DRV2605_ADRESS_FEEDBACK 0x1A
#define DRV2605_ADRESS_CONTROL1 0x1B
#define DRV2605_ADRESS_CONTROL2 0x1C
#define DRV2605_ADRESS_CONTROL3 0x1D
#define DRV2605_ADRESS_CONTROL4 0x1E
#define DRV2605_ADRESS_VBAT 0x21
#define DRV2605_ADRESS_LRARESON 0x22


class DRV2605_LRA {
  private:
    void init();
    void writeRegister8(uint8_t reg, uint8_t val);
    uint8_t readRegister8(uint8_t reg);

    uint8_t enablePinId;


  public:
    DRV2605_LRA();
    void begin();
    void begin(uint8_t sda_pin, uint8_t scl_pin);
    void enablePin(uint8_t e);
    void enableIC(bool b);


    void selectLibrary(uint8_t lib);
    void go();


    void test();

    void play(uint8_t effectId);

    void setupDone() ;

    void autoCal(String name );
    bool autoCal( uint8_t ratedVoltage, uint8_t overdriveClamp,uint8_t* compensation, uint8_t* backEMF, uint8_t* feedback );
    void autoCal(uint8_t ratedVoltage, uint8_t overdriveClamp);

};


#endif
