#ifndef MAIN_H
#define MAIN_H

#include <EEPROM.h>
#include <stdio.h>

#define ADD_COUNTER 0


/**** khai bao chan loadcell *****/
#include <HX711.h>
#define LOADCELL_DT_PIN    4
#define LOADCELL_SCK_PIN   8
HX711 scale;


/****** khai bao chan A4988 ****/
#define STEP_STEP   3
#define STEP_EN     7
#define STEP_DIR    2



/****** KHAI BAO LCD 16x4 *****/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 4);


/****** KHAI BAO INPUT ******/
#define H92B4_PIN    12
#define IR1          A2
#define IR2          A0
#define START_BNT    6
#define STOP_BNT     5
#define POT2         A3


/****** KAHI BAO OUTPUT ********/
#define CTR          A1 // CTR3
#define BANG_TAI     10
#define DOUBLE_MOTOR 9



/****** DINH NGHIA *******/
#define CTR_ON      digitalWrite(CTR, HIGH)
#define CTR_OFF     digitalWrite(CTR, LOW)

#define BANG_TAI_ON    analogWrite(BANG_TAI, 75)
#define BANG_TAI_OFF   digitalWrite(BANG_TAI, LOW)

#define DOUBLE_MOTOR_ON   analogWrite(DOUBLE_MOTOR, 75)
#define DOUBLE_MOTOR_OFF   digitalWrite(DOUBLE_MOTOR, LOW)


void writeIntToEEPROM(int value, int address) {
  EEPROM.write(address, highByte(value));
  EEPROM.write(address + 1, lowByte(value));
}

int readIntFromEEPROM(int address) {
  int highByteValue = EEPROM.read(address);
  int lowByteValue = EEPROM.read(address + 1);
  return (highByteValue << 8) | lowByteValue;
}



















#endif
