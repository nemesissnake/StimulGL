/* $Revision: 335 $ */

#ifndef STIMLIBREL_H
#define STIMLIBREL_H

#ifdef WIN32
#define IMPORT extern "C" __declspec(dllimport)
#else
#define IMPORT extern "C"
#endif

#define RISING_EDGE 1
#define FALLING_EDGE 0

#define OK 0
#define E_ERROR -1
#define E_NO_LICENSE 2000 
#define E_DAC_OUT_OF_RANGE 2010
#define E_VALUE_OUT_OF_RANGE 2011
#define E_SLOT_OUT_OF_RANGE 2012
#define E_TRIGGER_OUT_OF_RANGE 2013
#define E_PORT_OUT_OF_RANGE 2014
#define E_NO_PROPERTY 2015
#define E_READONLY_PROPERTY 2016
#define E_NOT_ENOUGH_MEMORY 2017
#define E_VAR_OUT_OF_RANGE 2018
#define E_BUFFER_OVERFLOW 2019
#define E_MODE_OUT_OF_RANGE 2020
#define E_DAC_LOCKED 2021
#define E_2P_MODULE_OUT_OF_RANGE 2022
#define E_1100 1100
#define E_1200 1200
#define E_1300 1300
#define E_1400 1400
#define E_1500 1500
#define E_1600 1600
#define E_1700 1700
#define E_1800 1800

typedef unsigned char		uint8_t;
typedef unsigned short int	uint16_t;
typedef unsigned int		uint32_t;

IMPORT int initStimulator(char *license);
IMPORT int closeStimulator();
IMPORT int resetStimulator();
IMPORT int setProperty(char* property, uint32_t value);
IMPORT int getProperty(char* property);
IMPORT int startStimulation();
IMPORT int stopStimulation();

IMPORT int setDAC(uint8_t dac, uint16_t wert);
IMPORT int setPinBlock8(uint8_t slot, uint8_t int_trigger, uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin5, uint8_t pin6, uint8_t pin7);
IMPORT int setPinBlock(uint8_t slot, uint8_t int_trigger, uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin5, uint8_t pin6, uint8_t pin7);
IMPORT int setPinBlock10(uint8_t slot, uint8_t int_trigger, uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin5, uint8_t pin6, uint8_t pin7, uint8_t pin8, uint8_t pin9);
IMPORT int wait(uint8_t int_trigger, uint32_t time);

IMPORT int setVar(uint8_t var, uint16_t value);
IMPORT int setVarImmediate(uint8_t var, uint16_t value);
IMPORT uint16_t getVarImmediate(uint8_t var);
IMPORT int incVar(uint8_t var);
IMPORT int decVar(uint8_t var);
IMPORT int outPort8(uint8_t slot, uint8_t port, uint8_t value);
IMPORT int outPort16(uint8_t slot, uint8_t portH, uint8_t portL, uint16_t value);
IMPORT int outPortVar16(uint8_t slot, uint8_t portH, uint8_t portL, uint8_t var);
IMPORT int inPortVar16(uint8_t slot, uint8_t portH, uint8_t portL, uint8_t var);

IMPORT int setTriggerMode(uint8_t slot, uint8_t port, uint8_t mode);
IMPORT int setTriggerLength(uint8_t slot, uint8_t port, uint8_t length);
IMPORT int waitForTrigger(uint8_t slot, uint8_t port);
IMPORT int triggerOut(uint8_t slot, uint8_t port);

IMPORT int set2PDProperties(uint8_t module, uint8_t slot, uint8_t subslot, uint8_t dac_x, uint8_t dac_z0, uint8_t dac_z1);
IMPORT int set2PDCalibrationX(uint8_t module, uint16_t homeDACPos, double co0, double co1, double co2, double co3, double co4, double co5, double co6, double co7, double co8, double co9);
IMPORT int set2PDCalibrationZ(uint8_t module, double Z0_co0, double Z0_co1, double Z1_co0, double Z1_co1);
IMPORT int set2PDDistance(uint8_t module, double distance);
IMPORT int set2PDHeight(uint8_t module, uint16_t promille_Z0, uint16_t promille_Z1);

#endif /*STIMLIBREL_H*/
