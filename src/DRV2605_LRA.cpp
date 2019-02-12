#include "DRV2605_LRA.h"
#include "Arduino.h"
#include <Wire.h>


// basic constructor
DRV2605_LRA::DRV2605_LRA() {
  //limbus
}

void DRV2605_LRA::begin() {
  Wire.begin();
  init();
}

void DRV2605_LRA::enablePin(uint8_t e) {
  enablePinId = e;
  pinMode(enablePinId,HIGH);
}

void DRV2605_LRA::enableIC(bool b) {
  if(b) {
    pinMode(enablePinId,HIGH);
  } else {
    pinMode(enablePinId,LOW);
  }

}


void DRV2605_LRA::begin(uint8_t sda_pin, uint8_t scl_pin) {
  Wire.begin(sda_pin,scl_pin);
  init();

}


void DRV2605_LRA::test() {
  writeRegister8(DRV2605_ADRESS_LIBRARY, 6);
  delay(100);

  Serial.print("read:\t");
  Serial.println(readRegister8(DRV2605_ADRESS_LIBRARY),HEX);

  Serial.print("go:\t");
  Serial.println(readRegister8(DRV2605_ADRESS_GO),HEX);

  Serial.print("fb:\t");
  Serial.println(readRegister8(DRV2605_ADRESS_FEEDBACK),HEX);


}


void DRV2605_LRA::play(uint8_t effectId) {
  uint8_t seq = 0;
  writeRegister8(DRV2605_ADRESS_WAVESEQ1+0, effectId);
  writeRegister8(DRV2605_ADRESS_WAVESEQ1+1, 0);
  go();

}

void DRV2605_LRA::setupDone() {
  writeRegister8(DRV2605_ADRESS_MODE, DRV2605_MODE_INTTRIG);
  selectLibrary(6); // the library no. 6 is dedicated to be used for LRA's. All other can be used to control EMR's
}


void DRV2605_LRA::init() {
  writeRegister8(DRV2605_ADRESS_MODE, DRV2605_MODE_INTTRIG);
  uint8_t id = readRegister8(DRV2605_ADRESS_STATUS);
  Serial.print("Status 0x");
  Serial.println(id, HEX);


  selectLibrary(6);
}

void DRV2605_LRA::selectLibrary(uint8_t lib) {
  writeRegister8(DRV2605_ADRESS_LIBRARY, lib);
}


void DRV2605_LRA::go() {
  writeRegister8(DRV2605_ADRESS_GO, 1);
}


uint8_t DRV2605_LRA::readRegister8(uint8_t reg) {
  uint8_t ret ;
   // use i2c
    Wire.beginTransmission(DRV2605_ADDR);
    Wire.write((byte)reg);
    Wire.endTransmission();
    Wire.requestFrom((byte)DRV2605_ADDR, (byte)1);
    ret = Wire.read();


  return ret;
}

void DRV2605_LRA::writeRegister8(uint8_t reg, uint8_t val) {
    Wire.beginTransmission(DRV2605_ADDR);
    Wire.write((byte)reg);
    Wire.write((byte)val);
    Wire.endTransmission();
}


void DRV2605_LRA::autoCal(String name ) {
  if (name.equals("C10-100") ) {
    autoCal(102,105);
  }
}

void DRV2605_LRA::autoCal(uint8_t ratedVoltage, uint8_t overdriveClamp) {

  uint8_t compensation;
  uint8_t backEMF;
  uint8_t feedback;

  autoCal(ratedVoltage,overdriveClamp, &compensation, &backEMF, &feedback );

}


#define CMD_FB_BRAKE_4x			0x30
#define CMD_FB_LOOP_FAST		0x08
#define CMD_DEFAULT_CTRL1		0x93
#define CMD_FB_MODE_LRA			0x80
#define CMD_MODE_AUTOCAL		0x07
#define CMD_GO				     	0x01
#define CMD_STOP			     	0x00
#define CMD_STAT_DIAG_BAD		0x08

bool DRV2605_LRA::autoCal( uint8_t ratedVoltage, uint8_t overdriveClamp, uint8_t* compensation, uint8_t* backEMF, uint8_t* feedback ){

	uint8_t fb = CMD_FB_BRAKE_4x | CMD_FB_LOOP_FAST;
	uint8_t control1 = CMD_DEFAULT_CTRL1;


	fb |= CMD_FB_MODE_LRA;

	writeRegister8( DRV2605_ADRESS_RATEDV,		ratedVoltage );
	writeRegister8( DRV2605_ADRESS_CLAMPV,		overdriveClamp );
	writeRegister8( DRV2605_ADRESS_FEEDBACK,		fb );
	writeRegister8( DRV2605_ADRESS_CONTROL1,			control1 );
	writeRegister8( DRV2605_ADRESS_MODE,			CMD_MODE_AUTOCAL );

	fb = CMD_GO;
	control1 = 0;
	writeRegister8( DRV2605_ADRESS_GO,			CMD_GO );


  sp("feedback register: ");
  spl(readRegister8(DRV2605_ADRESS_FEEDBACK)); //readRegister8(DRV2605_ADRESS_FEEDBACK),BIN

	do
	{
    fb =  readRegister8(DRV2605_ADRESS_GO);
    sp("fbC: ");
    sp(control1);
    spt();
    spl(fb); // Serial.println(fb,HEX);

		delay( 100 );
		++control1;
	} while( fb & CMD_GO && control1 < 100 );

  fb =  readRegister8(DRV2605_ADRESS_STATUS);

	if( fb & CMD_STAT_DIAG_BAD )
	{
		sp(F( "Error: " ));
		sp( control1 );
		sp(F( " loops - code " ));
		spl( fb); //Serial.println(fb,BIN);
	}

	sp( "status ");
	spl( fb); // Serial.println(fb, BIN);


  *compensation =  readRegister8(DRV2605_ADRESS_AUTOCALCOMP);
	*backEMF =  readRegister8(DRV2605_ADRESS_AUTOCALEMP);
	*feedback =  readRegister8(DRV2605_ADRESS_FEEDBACK);



	return true;
}
