//PiezoStimDevice
//Copyright (C) 2013  Sven Gijsen
//
//This file is part of StimulGL.
//StimulGL is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//


//This file defines the script binding interface, all below function are scriptable except for the destructor

#ifndef PIEZOSTIMDEVICE_H
#define PIEZOSTIMDEVICE_H
#include <QObject>
#include <QString>
#include <QtScript>
#include <QScriptable>
#include "./../../StimulGL/mainappinfo.h"
#include "./QuaeroSys/StimLibV2-0.3.3-Win/stimlibrel.h"

#define LIBRARYFILENAME					"stimlib0.dll"
#define LIBRARYLOADFAILEDMESSAGE		"The " LIBRARYFILENAME " library could not be loaded!"
#define RETURNCODETABLEVALUEUNAVAILABLE	"Return Code Lookup Table value is unavailable"

typedef int (*MyPrototype_initStimulator)(char *);
typedef int (*MyPrototype_closeStimulator)();
typedef int (*MyPrototype_resetStimulator)();
typedef int (*MyPrototype_setProperty)(char* property, uint32_t value);
typedef int (*MyPrototype_getProperty)(char* property);
typedef int (*MyPrototype_startStimulation)();
typedef int (*MyPrototype_stopStimulation)();
typedef int (*MyPrototype_setDAC)(uint8_t dac, uint16_t wert);
typedef int (*MyPrototype_setPinBlock8)(uint8_t slot, uint8_t int_trigger, uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin5, uint8_t pin6, uint8_t pin7);
typedef int (*MyPrototype_setPinBlock)(uint8_t slot, uint8_t int_trigger, uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin5, uint8_t pin6, uint8_t pin7);
typedef int (*MyPrototype_setPinBlock10)(uint8_t slot, uint8_t int_trigger, uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin5, uint8_t pin6, uint8_t pin7, uint8_t pin8, uint8_t pin9);
typedef int (*MyPrototype_wait)(uint8_t int_trigger, uint32_t time);
typedef int (*MyPrototype_setVar)(uint8_t var, uint16_t value);
typedef int (*MyPrototype_setVarImmediate)(uint8_t var, uint16_t value);
typedef uint16_t (*MyPrototype_getVarImmediate)(uint8_t var);
typedef int (*MyPrototype_incVar)(uint8_t var);
typedef int (*MyPrototype_decVar)(uint8_t var);
typedef int (*MyPrototype_outPort8)(uint8_t slot, uint8_t port, uint8_t value);
typedef int (*MyPrototype_outPort16)(uint8_t slot, uint8_t portH, uint8_t portL, uint16_t value);
typedef int (*MyPrototype_outPortVar16)(uint8_t slot, uint8_t portH, uint8_t portL, uint8_t var);
typedef int (*MyPrototype_inPortVar16)(uint8_t slot, uint8_t portH, uint8_t portL, uint8_t var);
typedef int (*MyPrototype_setTriggerMode)(uint8_t slot, uint8_t port, uint8_t mode);
typedef int (*MyPrototype_setTriggerLength)(uint8_t slot, uint8_t port, uint8_t length);
typedef int (*MyPrototype_waitForTrigger)(uint8_t slot, uint8_t port);
typedef int (*MyPrototype_triggerOut)(uint8_t slot, uint8_t port);
typedef int (*MyPrototype_set2PDProperties)(uint8_t module, uint8_t slot, uint8_t subslot, uint8_t dac_x, uint8_t dac_z0, uint8_t dac_z1);
typedef int (*MyPrototype_set2PDCalibrationX)(uint8_t module, uint16_t homeDACPos, double co0, double co1, double co2, double co3, double co4, double co5, double co6, double co7, double co8, double co9);
typedef int (*MyPrototype_set2PDCalibrationZ)(uint8_t module, double Z0_co0, double Z0_co1, double Z1_co0, double Z1_co1);
typedef int (*MyPrototype_set2PDDistance)(uint8_t module, double distance);
typedef int (*MyPrototype_set2PDHeight)(uint8_t module, uint16_t promille_Z0, uint16_t promille_Z1);

//!  The PiezoStimDevice class. 
/*!
  The PiezoStim Device can be used to communicate with the Piezo Stimulator device from QuaeroSys (see http://www.quaerosys.com/index.php?lang=en&page=piezostimulator).
*/
class PiezoStimDevice : public QObject, protected QScriptable
{
	Q_OBJECT
	//Q_CLASSINFO("ScriptAPIClassName", "PiezoStimDevice");//Can't use defines here!, moc doesn't handle defines, not needed here

public:
	PiezoStimDevice(QObject *parent = 0);
	~PiezoStimDevice();
	PiezoStimDevice(const PiezoStimDevice& other ){Q_UNUSED(other);}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro

	static QScriptValue ctor__extensionname(QScriptContext* context, QScriptEngine* engine);

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script
	
	//! \brief ReturnCodeToString slot.
	/*!  This function converts and returns the ReturnCode from the PiezoStimDevice API to a human readable String.
	 * @param nRetval the ReturnCode value to convert.
	 * @return a human readable String.
	 */
	QString ReturnCodeToString(const int &nRetval);
	//! \brief initStimulator slot.
	/*!  This function opens a new connection via USB to transmit data and commands. This command must be issued at the beginning of each stimulation program.
	 * @param sLicense a valid license-code as a string provided by the manufacturer.
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */
	int initStimulator(const QString &sLicense);
	//! \brief closeStimulator slot.
	/*!  This function terminates the connection to the stimulator.
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int closeStimulator();
	//! \brief resetStimulator slot.
	/*!  This function reboots the stimulator.
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int resetStimulator();
	//! \brief setProperty slot.
	/*!  This function sets a stimulator property.
	 * @param property the property, e.g. "local_buffer_size". The local_buffer_size property must be set before PiezoStimDevice::initStimulator.
	 * @param value the value, e.g. 50000.
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int setProperty(QString property, int value) {return setProperty((char*)property.toLatin1().data(),(uint32_t)value);};
	//! \brief getProperty slot.
	/*!  This function reads the given property. These properties can be read at any time the library is loaded. They are not depending on a connection with the stimulation device.
	 * @param property the property, e.g. 'local_buffer_size'.
	 * - 'local_buffer_size': Available local buffer in bytes
	 * - 'local_buffer_status': Filling of the PC buffer in bytes
	 * - 'remote_buffer_underruns': Number of steps of 0.5 ms the remote buffer (stimulator) was empty
	 * - 'remote_buffer_status': Number of bytes in remote buffer (possibly inaccurate due to program execution)
	 * @return the property value.
	 */	
	int getProperty(QString property) {return getProperty((char*)property.toLatin1().data());};
	//! \brief startStimulation slot.
	/*!  This function starts the eradication of the stimulation program while starting the internal clock of the stimulator.
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int startStimulation();
	//! \brief stopStimulation slot.
	/*!  This function Stops the eradication of the stimulation program.
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int stopStimulation();
	//! \brief setDAC slot.
	/*!  This function sets one of eight DACs (0 to 7) to a value between 0 (pin down) and 4095 (pin up). 
	 *   After a reset, all DACs are set to 0, so all pins should be down.
	 *   Avoid using DAC 0, because all pins refer at startup to this DAC.
	 *   The command will be executed instantly when calling wait (no waiting for a special internal trigger signal).
	 * @param dac the DAC to use.
	 * @param wert the new value.	 
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int setDAC(int dac, int wert) {return setDAC((uint8_t)dac, (uint16_t)wert);};
	//! \brief setPinBlock8 slot.
	/*!  This function sets the pins of the chosen eight pin stimulation card (first parameter) to a DAC of 0 to 7. 
	 *   The command will be executed as soon as the defined internal trigger signal (second parameter) is emitted by wait. 
	 *   Requires: Stimulation card for eight pins.
	 * @param slot the pin simulation card to use.
	 * @param int_trigger the internal trigger signal.
	 * @param pin<index> the value(s) of pins (0-7).
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int setPinBlock8(int slot, int int_trigger, int pin0, int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, int pin7)
		{return setPinBlock8((uint8_t)slot, (uint8_t)int_trigger, (uint8_t)pin0, (uint8_t)pin1, (uint8_t)pin2, (uint8_t)pin3, (uint8_t)pin4, (uint8_t)pin5, (uint8_t)pin6, (uint8_t)pin7);};
	//! \brief setPinBlock slot, this function is an alias to PiezoStimDevice::setPinBlock8. It only exists for compatibility reasons..
	int setPinBlock(int slot, int int_trigger, int pin0, int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, int pin7)
		{return setPinBlock((uint8_t)slot, (uint8_t)int_trigger, (uint8_t)pin0, (uint8_t)pin1, (uint8_t)pin2, (uint8_t)pin3, (uint8_t)pin4, (uint8_t)pin5, (uint8_t)pin6, (uint8_t)pin7);}
	//! \brief setPinBlock10 slot.
	/*!  This function sets the pins of the chosen ten pin stimulation card (first parameter) to a DAC of 0 to 9. 
	 *   The command will be executed as soon as the defined internal trigger signal (second parameter) is emitted by wait.
	 *   Requires: Stimulation card for ten pins.
	 * @param slot the DAC to use.
	 * @param int_trigger the internal trigger signal.
	 * @param pin<index> the value(s) of pins (0-9).
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int setPinBlock10(int slot, int int_trigger, int pin0, int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, int pin7, int pin8, int pin9)
		{return setPinBlock10((uint8_t)slot, (uint8_t)int_trigger, (uint8_t)pin0, (uint8_t)pin1, (uint8_t)pin2, (uint8_t)pin3, (uint8_t)pin4, (uint8_t)pin5, (uint8_t)pin6, (uint8_t)pin7, (uint8_t)pin8, (uint8_t)pin9);}
	//! \brief wait slot.
	/*!  This function completes the actual clock cycle and emits one or more (count) internal trigger signals. 
	 *   Commands waiting for this signal will be executed immediately.
	 * @param int_trigger the internal trigger to signal.
	 * @param count then number of trigger signal counts.
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int wait(int int_trigger, int count) {return wait((uint8_t)int_trigger, (uint32_t)count);};
	//! \brief setVar slot.
	/*!  This function sets the given variable when program execution reaches the command. 
	 *   In the stimulator there a 100 user defined variables of 16 bit available which are numbered from 0 to 99.
	 * @param var the variable to set.
	 * @param value the new value for the variable.
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int setVar(int var, int value) {return setVar((uint8_t)var, (uint16_t)value);};
	//! \brief setVarImmediate slot.
	/*!  This function sets the given variable immediately.	This command does not queue in the command buffer, but is executed directly.
	 * @param var the variable to set.
	 * @param value the new value for the variable.
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int setVarImmediate(int var, int value) {return setVarImmediate((uint8_t)var, (uint16_t)value);};
	//! \brief getVarImmediate slot.
	/*!  This function fetches the value of the given variable from the stimulator.
	 *   This command does not queue in the command buffer, but is executed directly.
	 * @param var the variable to fetch.
	 * @return the variable value.
	 */	
	int getVarImmediate(int var) {return (int)getVarImmediate((uint8_t)var);};
	//! \brief incVar slot.
	/*!  This function increments the given variable by one.
	 * @param var the variable to increment.
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int incVar(int var) {return incVar((uint8_t)var);};
	//! \brief decVar slot.
	/*!  This function decrements the given variable by one.
	 * @param var the variable to decrement.
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int decVar(int var) {return decVar((uint8_t)var);};	
	//! \brief outPort8 slot.
	/*!  This function outputs the given value to a I/O card port.
	 *   Requires an I/O card
	 * @param slot the I/O card slot.
	 * @param port the I/O card's port.
	 * @param value the value(8 bit) to output.
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int outPort8(int slot, int port, int value) {return outPort8((uint8_t) slot, (uint8_t) port, (uint8_t) value);};
	//! \brief outPort16 slot.
	/*!  This function outputs the given value to two I/O card ports.
	 *   Requires an I/O card
	 * @param slot the I/O card slot.
	 * @param portH the high I/O card's port.
	 * @param portL the low I/O card's port.
	 * @param value the value(16 bit) to output.	
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int outPort16(int slot, int portH, int portL, int value) {return outPort16((uint8_t) slot, (uint8_t) portH, (uint8_t) portL, (uint16_t) value);};
	//! \brief outPortVar16 slot.
	/*!  This function outputs a variable with a 16 bit value to two I/O card ports. If only 8 bit are used set the other port to 0xFF.
	 *   Requires an I/O card.
	 * @param slot the I/O card slot.
	 * @param portH the high I/O card's port.
	 * @param portL the low I/O card's port.
	 * @param var contains the value (16 bit) to output.
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int outPortVar16(int slot, int portH, int portL, int var) {return outPortVar16((uint8_t) slot, (uint8_t) portH, (uint8_t) portL, (uint8_t) var);};
	//! \brief inPortVar16 slot.
	/*!  This function reads the value of two I/O card ports into the internal 16 bit variable.
	 *   Requires an I/O card.
	 * @param slot the I/O card slot.
	 * @param portH the high I/O card's port.
	 * @param portL the low I/O card's port.
	 * @param var the variable where the value should be stored.
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int inPortVar16(int slot, int portH, int portL, int var) {return inPortVar16((uint8_t) slot, (uint8_t) portH, (uint8_t) portL, (uint8_t) var);};
	//! \brief setTriggerMode slot.
	/*!  This function sets the trigger output mode to one of the following modes.
	 *   Trigger mode is changed immediately after sending the command. 
	 *   It should only be called once at the beginning of the program to apply the right settings for the receiving device.
	 *   The command only affects HW versions >= 1.0, otherwise (default or HW version <1.0) trigger mode is RISING_EDGE.
	 * @param slot the I/O card slot.
	 * @param port the I/O card's port.
	 * @param mode the trigger mode:
	 * - FALLING_EDGE Mode 0: Trigger starts with falling edge (1->0)
	 * - RISING_EDGE Mode 1: Trigger starts with rising edge (0->1)
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int setTriggerMode(int slot, int port, int mode) {return setTriggerMode((uint8_t) slot, (uint8_t) port, (uint8_t) mode);};
	//! \brief setTriggerLength slot.
	/*!  This function sets the stimulator to generate trigger outputs of a given length. 
	 *   The length is measured in steps of 0.5 ms. Trigger length is changed after finishing the last trigger. 
	 *   It should only be called once at the beginning of the program to apply the right settings for the receiving device.
	 *   This command only affects HW versions>= 1.0, otherwise trigger length is 0.5 ms (default) or 500 ns (HW version <1.0).
	 * @param slot the I/O card slot.
	 * @param port the I/O card's port.
	 * @param length the trigger length.	
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int setTriggerLength(int slot, int port, int length) {return setTriggerLength((uint8_t) slot, (uint8_t) port, (uint8_t) length);};
	//! \brief waitForTrigger slot.
	/*!  This function waits for an external trigger signal (rising edge) on the selected port. 
	 *   The device pauses command execution when reaching this command. 
	 *   It does not block the stimulation PC or any program on it until the trigger signal arrives.
	 *   If there is only a single controller card the only possible arguments for this function are slot 0x10 (decimal 16) and port 0x00.
	 * @param slot the I/O card slot.
	 * @param port the I/O card's port.
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int waitForTrigger(int slot, int port) {return waitForTrigger((uint8_t) slot, (uint8_t) port);};
	//! \brief triggerOut slot.
	/*!  This function emits an external trigger signal on the selected port.
	 *   In hardware version <1.0 trigger output is always a positive pulse (logical ’1’) with a width of 500 ns. 
	 *   In hardware version version 1.0 the pulse is configurable. It can be positive (logical	’1’) or negative (logical ’0’) with a variable length in multiples of 500 µs. 
	 *   To modify these configuration parameters the commands setTriggerMode and setTriggerLength are used.
	 *   The command will be executed instantly on calling wait, where the number of wait cycles has to be at least as high as the current trigger length for complete execution of the trigger.
	 *   If there is only a single controller card the only possible arguments for this function are slot 0x10 (decimal 16) and port 0x01.
	 * @param slot the I/O card slot.
	 * @param port the I/O card's port.
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int triggerOut(int slot, int port) {return triggerOut((uint8_t) slot, (uint8_t) port);};
	//! \brief set2PDProperties slot.
	/*!  This function sets basic parameters for two point discrimination modules.
	 *   A call to this function is needed for the usage of each two point discrimination module identified by the first parameter. 
	 *   The slot gives the card where the module is connected, subslot	specifies the connection on the adapter (if applicable, otherwise this is 1). 
	 *   The following three parameters assign DACs to the axes of the module. 
	 *   The first DAC controls the distance and is occupied by the driver if X-calibration is used (see PiezoStimDevice::set2PDCalibrationX). 
	 *   DACs assigned to the pins moving in Z-direction (up/down) are user specific like pins on normal matrix modules. 
	 *   If both pins should always move in the same way, they can simply bound to the same DAC. 
	 *   If Z-calibration (see PiezoStimDevice::set2PDCalibrationZ) is used there have to be given two different DACs.
	 * @param module the module.
	 * @param slot the I/O card slot.
	 * @param the I/O card subslot.
	 * @param dac_x the DAC for X-calibration. 
	 * @param dac_z0 the first DAC for Z-calibration.	
	 * @param dac_z1 the second DAC for Z-calibration.
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int set2PDProperties(int module, int slot, int subslot, int dac_x, int dac_z0, int dac_z1) {return set2PDProperties((uint8_t) module, (uint8_t) slot, (uint8_t) subslot, (uint8_t) dac_x, (uint8_t) dac_z0, (uint8_t) dac_z1);};
	//! \brief set2PDCalibrationX slot.
	/*!  This function assigns calibration data for X-axis to a specific module.
	 *   Calibration data is given as 10+1 values used for the right interpretation of the distance given by PiezoStimDevice::set2PDDistance. 
	 *   The module has to be identified with the first parameter as each individual module has a set of calibration data. 
	 *   As the driver needs full control over the DAC assigned to the X-axis, it will be locked and cannot be used otherwise.
	 * @param module the module.
	 * @param homeDACPos
	 * @param coX
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int set2PDCalibrationX(int module, int homeDACPos, double co0, double co1, double co2, double co3, double co4, double co5, double co6, double co7, double co8, double co9) {return set2PDCalibrationX((uint8_t) module, (uint16_t) homeDACPos, co0, co1, co2, co3, co4, co5, co6, co7, co8, co9);};
	//! \brief set2PDCalibrationZ slot.
	/*!  This function assigns calibration data for both Z-axes to a specific module.
	 *   Command reference Calibration data is given as two values for each axis used for the right interpretation of the pin height given by set2PDHeight. 
	 *   The module has to be identified with the first parameter as each individual module has a set of calibration data. 
	 *   As the driver needs full control over both	DACs assigned to the Z-axes, they will be locked and cannot be used otherwise. 
	 *   Please be sure that there have been assigned two different DACs in set2PDProperties otherwise there will be an error.
	 * @param module the module.
	 * @param ZX_coY.
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int set2PDCalibrationZ(int module, double Z0_co0, double Z0_co1, double Z1_co0, double Z1_co1) {return set2PDCalibrationZ((uint8_t) module, Z0_co0, Z0_co1, Z1_co0, Z1_co1);};
	//! \brief set2PDDistance slot.
	/*!  This function move pins of a two point discrimination module to a given distance.
	 *   This function actually positions the pins to the given distance measured in millimeters. 
	 *   It	maintains retreating pins of the Z-axis inside of the case by smoothly setting the DACs assigned by set2PDProperties to zero. 
	 *	 Then it moves X-axis back to the starting position	and after that to the newly given position. 
	 *   It does not move pins of the Z-axis up, as this should be done by the user.
	 *   The whole positioning process takes one second, therefore it has influence on the timing of the program as there was a wait(1, 2000).
	 * @param module the module.
	 * @param distance the distance.
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int set2PDDistance(int module, double distance) {return set2PDDistance((uint8_t) module, distance);};
	//! \brief set2PDHeight slot.
	/*!  This function preset pin height measured in promille of full extend.
	 *   This function works like PiezoStimDevice::setDAC with the difference that it honours the calibration for Zaxis.
	 *   This function does not actually move the pins out, but waits for the next internal trigger	as setDAC does. 
	 *   The value of 0 has a special meaning as it does not take calibration data into	account, but sets the corresponding DAC to 0.
	 * @param module the module.
	 * @param promille_Z0 promille of full height extend.
	 * @param promille_Z1 promille of full height extend.	 
	 * @return the ReturnCode as an integer value, see PiezoStimDevice::ReturnCodeToString
	 */	
	int set2PDHeight(int module, int promille_Z0, int promille_Z1) {return set2PDHeight((uint8_t) module, (uint16_t) promille_Z0, (uint16_t) promille_Z1);};

private:
	QScriptEngine* currentScriptEngine;
	QMap<int,QString> mReturnCodes;

	bool bLibraryIsLoaded;
	bool bStopLibraryLoading;
	QLibrary *libPiezoStim;

	bool LoadPiezoLibrary(const QString &sFileName);
	void UnloadPiezoLibrary();
	void setupReturnCodesTable();
	bool resolveFunctionSignatures();

	MyPrototype_initStimulator MyFunction_initStimulator;
	MyPrototype_closeStimulator MyFunction_closeStimulator;
	MyPrototype_resetStimulator MyFunction_resetStimulator;
	MyPrototype_setProperty MyFunction_setProperty;
	MyPrototype_getProperty MyFunction_getProperty;
	MyPrototype_startStimulation MyFunction_startStimulation;
	MyPrototype_stopStimulation MyFunction_stopStimulation;
	MyPrototype_setDAC MyFunction_setDAC;
	MyPrototype_setPinBlock8 MyFunction_setPinBlock8;
	MyPrototype_setPinBlock MyFunction_setPinBlock;
	MyPrototype_setPinBlock10 MyFunction_setPinBlock10;
	MyPrototype_wait MyFunction_wait;
	MyPrototype_setVar MyFunction_setVar;
	MyPrototype_setVarImmediate MyFunction_setVarImmediate;
	MyPrototype_getVarImmediate MyFunction_getVarImmediate;
	MyPrototype_incVar MyFunction_incVar;
	MyPrototype_decVar MyFunction_decVar;
	MyPrototype_outPort8 MyFunction_outPort8;
	MyPrototype_outPort16 MyFunction_outPort16;
	MyPrototype_outPortVar16 MyFunction_outPortVar16;
	MyPrototype_inPortVar16 MyFunction_inPortVar16;
	MyPrototype_setTriggerMode MyFunction_setTriggerMode;
	MyPrototype_setTriggerLength MyFunction_setTriggerLength;
	MyPrototype_waitForTrigger MyFunction_waitForTrigger;
	MyPrototype_triggerOut MyFunction_triggerOut;
	MyPrototype_set2PDProperties MyFunction_set2PDProperties;
	MyPrototype_set2PDCalibrationX MyFunction_set2PDCalibrationX;
	MyPrototype_set2PDCalibrationZ MyFunction_set2PDCalibrationZ;
	MyPrototype_set2PDDistance MyFunction_set2PDDistance;
	MyPrototype_set2PDHeight MyFunction_set2PDHeight;

	int setProperty(char* property, uint32_t value);
	int getProperty(char* property);
	int setDAC(uint8_t dac, uint16_t wert);
	int setPinBlock8(uint8_t slot, uint8_t int_trigger, uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin5, uint8_t pin6, uint8_t pin7);
	int setPinBlock(uint8_t slot, uint8_t int_trigger, uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin5, uint8_t pin6, uint8_t pin7);
	int setPinBlock10(uint8_t slot, uint8_t int_trigger, uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin5, uint8_t pin6, uint8_t pin7, uint8_t pin8, uint8_t pin9);
	int wait(uint8_t int_trigger, uint32_t time);
	int setVar(uint8_t var, uint16_t value);
	int setVarImmediate(uint8_t var, uint16_t value);
	uint16_t getVarImmediate(uint8_t var);
	int incVar(uint8_t var);
	int decVar(uint8_t var);
	int outPort8(uint8_t slot, uint8_t port, uint8_t value);
	int outPort16(uint8_t slot, uint8_t portH, uint8_t portL, uint16_t value);
	int outPortVar16(uint8_t slot, uint8_t portH, uint8_t portL, uint8_t var);
	int inPortVar16(uint8_t slot, uint8_t portH, uint8_t portL, uint8_t var);
	int setTriggerMode(uint8_t slot, uint8_t port, uint8_t mode);
	int setTriggerLength(uint8_t slot, uint8_t port, uint8_t length);
	int waitForTrigger(uint8_t slot, uint8_t port);
	int triggerOut(uint8_t slot, uint8_t port);
	int set2PDProperties(uint8_t module, uint8_t slot, uint8_t subslot, uint8_t dac_x, uint8_t dac_z0, uint8_t dac_z1);
	int set2PDCalibrationX(uint8_t module, uint16_t homeDACPos, double co0, double co1, double co2, double co3, double co4, double co5, double co6, double co7, double co8, double co9);
	int set2PDCalibrationZ(uint8_t module, double Z0_co0, double Z0_co1, double Z1_co0, double Z1_co1);
	int set2PDDistance(uint8_t module, double distance);
	int set2PDHeight(uint8_t module, uint16_t promille_Z0, uint16_t promille_Z1);
};

#endif // PIEZOSTIMDEVICE_H
