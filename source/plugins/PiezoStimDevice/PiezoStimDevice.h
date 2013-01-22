//PiezoStimDevice
//Copyright (C) 2012  Sven Gijsen
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

class PiezoStimDevice : public QObject, protected QScriptable
{
	Q_OBJECT
	//Q_CLASSINFO("ScriptAPIClassName", "PiezoStimDevice");//Can't use defines here!, moc doesn't handle defines, not needed here
	//Q_PROPERTY( short ExampleProperty WRITE setExampleProperty READ getExampleProperty )

signals:
	void ExampleSignalTriggered(short);

public:
	PiezoStimDevice(QObject *parent = 0);
	~PiezoStimDevice();
	PiezoStimDevice(const PiezoStimDevice& other ){}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro

	static QScriptValue ctor__extensionname(QScriptContext* context, QScriptEngine* engine);

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script

	int initStimulator(const QString &sLicense);
	int closeStimulator();
	int resetStimulator();
	int setProperty(char* property, uint32_t value);
	int getProperty(char* property);
	int startStimulation();
	int stopStimulation();
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

	QString Test(const QString &sParam = QString(""));
	QString ReturnCodeToString(const int &nRetval);

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
};

#endif // PIEZOSTIMDEVICE_H
