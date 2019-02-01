#include "DRV2605_LRA.h"
#include "Arduino.h"
#include <Wire.h>



DRV2605_LRA::DRV2605_LRA() {

}

void DRV2605_LRA::begin() {
  Wire.begin();
  init();
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
}


void DRV2605_LRA::init() {
    writeRegister8(DRV2605_ADRESS_MODE, DRV2605_MODE_INTTRIG);
  uint8_t id = readRegister8(DRV2605_ADRESS_STATUS);
  Serial.print("Status 0x"); Serial.println(id, HEX);


  selectLibrary(6);
}

void DRV2605_LRA::selectLibrary(uint8_t lib) {
  writeRegister8(DRV2605_ADRESS_LIBRARY, lib);
}


void DRV2605_LRA::go() {
  writeRegister8(DRV2605_ADRESS_GO, 1);
}

uint8_t DRV2605_LRA::readRegister8(uint8_t reg) {
  uint8_t x ;
   // use i2c
    Wire.beginTransmission(DRV2605_ADDR);
    Wire.write((byte)reg);
    Wire.endTransmission();
    Wire.requestFrom((byte)DRV2605_ADDR, (byte)1);
    x = Wire.read();

  //  Serial.print("$"); Serial.print(reg, HEX);
  //  Serial.print(": 0x"); Serial.println(x, HEX);

  return x;
}

void DRV2605_LRA::writeRegister8(uint8_t reg, uint8_t val) {
   // use i2c
    Wire.beginTransmission(DRV2605_ADDR);
    Wire.write((byte)reg);
    Wire.write((byte)val);
    Wire.endTransmission();
}


#define FB_BRAKE_4x			0x30
#define FB_LOOP_FAST		0x08
#define DEFAULT_CTRL1		0x93
#define FB_MODE_LRA			0x80
#define MODE_AUTOCAL		0x07
#define GO				     	0x01
#define STOP			     	0x00
#define STAT_DIAG_BAD		0x08

bool DRV2605_LRA::autoCal( uint8_t ratedVoltage, uint8_t overdriveClamp, uint8_t* compensation, uint8_t* backEMF, uint8_t* feedback ){
	// Set defaults
	//setDefaults();

	// Work out control registers
	uint8_t fb = FB_BRAKE_4x | FB_LOOP_FAST;
	uint8_t control1 = DEFAULT_CTRL1;


	fb |= FB_MODE_LRA;


	// Write required registers
	writeRegister8( DRV2605_ADRESS_RATEDV,		ratedVoltage );
	writeRegister8( DRV2605_ADRESS_CLAMPV,		overdriveClamp );
	writeRegister8( DRV2605_ADRESS_FEEDBACK,		fb );
	writeRegister8( DRV2605_ADRESS_CONTROL1,			control1 );
	writeRegister8( DRV2605_ADRESS_MODE,			MODE_AUTOCAL );

	fb = GO;
	control1 = 0;
	writeRegister8( DRV2605_ADRESS_GO,			GO );


  Serial.print("fb Reg is: ");
  Serial.println(readRegister8(DRV2605_ADRESS_FEEDBACK),BIN);

	do
	{
		//I2C_Read( DRV2605_ADRESS_GO, &fb );
    fb =  readRegister8(DRV2605_ADRESS_GO);
    Serial.print("fbC: ");
    Serial.print(control1);
    Serial.print("\t");
    Serial.println(fb,HEX);

		delay( 100 );
		++control1;
	} while( fb & GO && control1 < 100 );

	// Read status bit
	//I2C_Read( DRV2605_ADRESS_STATUS, &fb );
  fb =  readRegister8(DRV2605_ADRESS_STATUS);

	if( fb & STAT_DIAG_BAD )
	{
		//Results did not converge
		Serial.print(F( "Fail: " ));
		Serial.print( control1 );
		Serial.print(F( " tries. Status " ));
		Serial.println( fb, BIN );
	//	return false;
	}

	Serial.print(F( "Status " ));
	Serial.println( fb, BIN );

	//I2C_Read( DRV2605_ADRESS_AUTOCALCOMP,			compensation );
	//I2C_Read( DRV2605_ADRESS_AUTOCALEMP,		backEMF );
	//I2C_Read( DRV2605_ADRESS_FEEDBACK,		feedback );

  *compensation =  readRegister8(DRV2605_ADRESS_AUTOCALCOMP);
	*backEMF =  readRegister8(DRV2605_ADRESS_AUTOCALEMP);
	*feedback =  readRegister8(DRV2605_ADRESS_FEEDBACK);



	return true;
}
