//PiezoStimDevice
//Copyright (C) 2014  Sven Gijsen
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


#include "PiezoStimDevice.h"

QScriptValue PiezoStimDevice::ctor__extensionname(QScriptContext* context, QScriptEngine* engine)
{
	Q_UNUSED(context);
	return engine->newQObject(new PiezoStimDevice(), QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
} 

PiezoStimDevice::PiezoStimDevice(QObject *parent) : QObject(parent)
{
	currentScriptEngine = NULL;
	libPiezoStim = NULL;
	bLibraryIsLoaded = false;
	bStopLibraryLoading = false;
}

PiezoStimDevice::~PiezoStimDevice()
{
	UnloadPiezoLibrary();
}

bool PiezoStimDevice::makeThisAvailableInScript(QString strObjectScriptName, QObject *engine)
{
	if (engine)
	{
		currentScriptEngine = reinterpret_cast<QScriptEngine *>(engine);
		QScriptValue objectValue = currentScriptEngine->newQObject(this);
		currentScriptEngine->globalObject().setProperty(strObjectScriptName, objectValue);
		return true;
	}
	return false;
}

bool PiezoStimDevice::LoadPiezoLibrary(const QString &sFileName)
{
	if(bStopLibraryLoading)
		return false;
	if(bLibraryIsLoaded == false)
	{
		if (QLibrary::isLibrary(sFileName)) 
		{
			libPiezoStim = new QLibrary(sFileName);
			bLibraryIsLoaded = libPiezoStim->load();
			if(bLibraryIsLoaded == false)
			{
				qDebug() << __FUNCTION__ << libPiezoStim->errorString();
				bStopLibraryLoading = true;
				return false;
			}
			if(resolveFunctionSignatures())
				setupReturnCodesTable();
			else
				bStopLibraryLoading = true;
		}
		else
		{
			bStopLibraryLoading = true;
		}
	}
	return bLibraryIsLoaded;
}

void PiezoStimDevice::UnloadPiezoLibrary()
{
	if(libPiezoStim)
	{
		if(bLibraryIsLoaded)
		{
			libPiezoStim->unload();
		}
		delete libPiezoStim;
		libPiezoStim = NULL;
	}
	bLibraryIsLoaded = false;
	bStopLibraryLoading = true;
	mReturnCodes.clear();
}

QString PiezoStimDevice::ReturnCodeToString(const int &nRetval)
{
	return mReturnCodes.value(nRetval,QString(RETURNCODETABLEVALUEUNAVAILABLE) + ":" + QString::number(nRetval));	
}

void PiezoStimDevice::setupReturnCodesTable()
{
	mReturnCodes.clear();
	mReturnCodes.insert(RISING_EDGE,"RISING_EDGE");
	mReturnCodes.insert(FALLING_EDGE,"FALLING_EDGE");
	mReturnCodes.insert(OK,"OK");
	mReturnCodes.insert(E_ERROR ,"E_ERROR ");
	mReturnCodes.insert(E_NO_LICENSE,"E_NO_LICENSE");
	mReturnCodes.insert(E_DAC_OUT_OF_RANGE,"E_DAC_OUT_OF_RANGE");
	mReturnCodes.insert(E_VALUE_OUT_OF_RANGE,"E_VALUE_OUT_OF_RANGE");
	mReturnCodes.insert(E_SLOT_OUT_OF_RANGE,"E_SLOT_OUT_OF_RANGE");
	mReturnCodes.insert(E_TRIGGER_OUT_OF_RANGE,"E_TRIGGER_OUT_OF_RANGE");
	mReturnCodes.insert(E_PORT_OUT_OF_RANGE,"E_PORT_OUT_OF_RANGE");
	mReturnCodes.insert(E_NO_PROPERTY,"E_NO_PROPERTY");
	mReturnCodes.insert(E_READONLY_PROPERTY,"E_READONLY_PROPERTY");
	mReturnCodes.insert(E_NOT_ENOUGH_MEMORY,"E_NOT_ENOUGH_MEMORY");
	mReturnCodes.insert(E_VAR_OUT_OF_RANGE,"E_VAR_OUT_OF_RANGE");
	mReturnCodes.insert(E_BUFFER_OVERFLOW,"E_BUFFER_OVERFLOW");
	mReturnCodes.insert(E_MODE_OUT_OF_RANGE,"E_MODE_OUT_OF_RANGE");
	mReturnCodes.insert(E_DAC_LOCKED,"E_DAC_LOCKED");
	mReturnCodes.insert(E_2P_MODULE_OUT_OF_RANGE,"E_2P_MODULE_OUT_OF_RANGE");
	mReturnCodes.insert(E_1100,"E_1100");
	mReturnCodes.insert(E_1200,"E_1200");
	mReturnCodes.insert(E_1300,"E_1300");
	mReturnCodes.insert(E_1400,"E_1400");
	mReturnCodes.insert(E_1500,"E_1500");
	mReturnCodes.insert(E_1600,"E_1600");
	mReturnCodes.insert(E_1700,"E_1700");
	mReturnCodes.insert(E_1800,"E_1800");
}

bool PiezoStimDevice::resolveFunctionSignatures()
{
	MyFunction_initStimulator = (MyPrototype_initStimulator) libPiezoStim->resolve("initStimulator");
	if(!MyFunction_initStimulator)
		return false;
	MyFunction_closeStimulator = (MyPrototype_closeStimulator) libPiezoStim->resolve("closeStimulator");
	if(!MyFunction_closeStimulator)
		return false;
	MyFunction_resetStimulator = (MyPrototype_resetStimulator) libPiezoStim->resolve("resetStimulator");
	if(!MyFunction_resetStimulator)
		return false;
	MyFunction_setProperty = (MyPrototype_setProperty) libPiezoStim->resolve("setProperty");
	if(!MyFunction_setProperty)
		return false;
	MyFunction_getProperty = (MyPrototype_getProperty) libPiezoStim->resolve("getProperty");
	if(!MyFunction_getProperty)
		return false;
	MyFunction_startStimulation = (MyPrototype_startStimulation) libPiezoStim->resolve("startStimulation");
	if(!MyFunction_startStimulation)
		return false;
	MyFunction_stopStimulation = (MyPrototype_stopStimulation) libPiezoStim->resolve("stopStimulation");
	if(!MyFunction_stopStimulation)
		return false;
	MyFunction_setDAC = (MyPrototype_setDAC) libPiezoStim->resolve("setDAC");
	if(!MyFunction_setDAC)
		return false;
	MyFunction_setPinBlock8 = (MyPrototype_setPinBlock8) libPiezoStim->resolve("setPinBlock8");
	if(!MyFunction_setPinBlock8)
		return false;
	MyFunction_setPinBlock = (MyPrototype_setPinBlock) libPiezoStim->resolve("setPinBlock");
	if(!MyFunction_setPinBlock)
		return false;
	MyFunction_setPinBlock10 = (MyPrototype_setPinBlock10) libPiezoStim->resolve("setPinBlock10");
	if(!MyFunction_setPinBlock10)
		return false;
	MyFunction_wait = (MyPrototype_wait) libPiezoStim->resolve("wait");
	if(!MyFunction_wait)
		return false;
	MyFunction_setVar = (MyPrototype_setVar) libPiezoStim->resolve("setVar");
	if(!MyFunction_setVar)
		return false;
	MyFunction_setVarImmediate = (MyPrototype_setVarImmediate) libPiezoStim->resolve("setVarImmediate");
	if(!MyFunction_setVarImmediate)
		return false;
	MyFunction_getVarImmediate = (MyPrototype_getVarImmediate) libPiezoStim->resolve("getVarImmediate");
	if(!MyFunction_getVarImmediate)
		return false;
	MyFunction_incVar = (MyPrototype_incVar) libPiezoStim->resolve("incVar");
	if(!MyFunction_incVar)
		return false;
	MyFunction_decVar = (MyPrototype_decVar) libPiezoStim->resolve("decVar");
	if(!MyFunction_decVar)
		return false;
	MyFunction_outPort8 = (MyPrototype_outPort8) libPiezoStim->resolve("outPort8");
	if(!MyFunction_outPort8)
		return false;
	MyFunction_outPort16 = (MyPrototype_outPort16) libPiezoStim->resolve("outPort16");
	if(!MyFunction_outPort16)
		return false;
	MyFunction_outPortVar16 = (MyPrototype_outPortVar16) libPiezoStim->resolve("outPortVar16");
	if(!MyFunction_outPortVar16)
		return false;
	MyFunction_inPortVar16 = (MyPrototype_inPortVar16) libPiezoStim->resolve("inPortVar16");
	if(!MyFunction_inPortVar16)
		return false;
	MyFunction_setTriggerMode = (MyPrototype_setTriggerMode) libPiezoStim->resolve("setTriggerMode");
	if(!MyFunction_setTriggerMode)
		return false;
	MyFunction_setTriggerLength = (MyPrototype_setTriggerLength) libPiezoStim->resolve("setTriggerLength");
	if(!MyFunction_setTriggerLength)
		return false;
	MyFunction_waitForTrigger = (MyPrototype_waitForTrigger) libPiezoStim->resolve("waitForTrigger");
	if(!MyFunction_waitForTrigger)
		return false;
	MyFunction_triggerOut = (MyPrototype_triggerOut) libPiezoStim->resolve("triggerOut");
	if(!MyFunction_triggerOut)
		return false;
	MyFunction_set2PDProperties = (MyPrototype_set2PDProperties) libPiezoStim->resolve("set2PDProperties");
	if(!MyFunction_set2PDProperties)
		return false;
	MyFunction_set2PDCalibrationX = (MyPrototype_set2PDCalibrationX) libPiezoStim->resolve("set2PDCalibrationX");
	if(!MyFunction_set2PDCalibrationX)
		return false;
	MyFunction_set2PDCalibrationZ = (MyPrototype_set2PDCalibrationZ) libPiezoStim->resolve("set2PDCalibrationZ");
	if(!MyFunction_set2PDCalibrationZ)
		return false;
	MyFunction_set2PDDistance = (MyPrototype_set2PDDistance) libPiezoStim->resolve("set2PDDistance");
	if(!MyFunction_set2PDDistance)
		return false;
	MyFunction_set2PDHeight = (MyPrototype_set2PDHeight) libPiezoStim->resolve("set2PDHeight");
	if(!MyFunction_set2PDHeight)
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//QString PiezoStimDevice::Test(const QString &sParam)
//{
//	if(LoadPiezoLibrary(LIBRARYFILENAME))
//	{
//		typedef int (*MyPrototype)(char *);
//
//		MyPrototype myFunction = (MyPrototype) libPiezoStim->resolve("initStimulator");
//		if (myFunction)
//			return QString::number(myFunction(sParam.toLatin1().data()));
//		return "Function failed";
//	}
//	return LIBRARYLOADFAILEDMESSAGE;
//}

int PiezoStimDevice::initStimulator(const QString &sLicense)
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		QByteArray baLicense = sLicense.toLocal8Bit();
		int nRetval = MyFunction_initStimulator(baLicense.data());
		return nRetval;
	}
	return -1;
}

int PiezoStimDevice::closeStimulator()
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_closeStimulator();
		return nRetval;
	}
	return 0;
};

int PiezoStimDevice::resetStimulator() 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_resetStimulator();
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::setProperty(char* property, uint32_t value) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_setProperty(property,value);
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::getProperty(char* property) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_getProperty(property);
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::startStimulation() 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_startStimulation();
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::stopStimulation() 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_stopStimulation();
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::setDAC(uint8_t dac, uint16_t wert) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_setDAC(dac,wert);
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::setPinBlock8(uint8_t slot, uint8_t int_trigger, uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin5, uint8_t pin6, uint8_t pin7) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_setPinBlock8(slot, int_trigger, pin0, pin1, pin2, pin3, pin4, pin5, pin6, pin7);
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::setPinBlock(uint8_t slot, uint8_t int_trigger, uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin5, uint8_t pin6, uint8_t pin7) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_setPinBlock(slot, int_trigger, pin0, pin1, pin2, pin3, pin4, pin5, pin6, pin7);
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::setPinBlock10(uint8_t slot, uint8_t int_trigger, uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin5, uint8_t pin6, uint8_t pin7, uint8_t pin8, uint8_t pin9) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_setPinBlock10(slot, int_trigger, pin0, pin1, pin2, pin3, pin4, pin5, pin6, pin7, pin8, pin9);
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::wait(uint8_t int_trigger, uint32_t time) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_wait(int_trigger, time);
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::setVar(uint8_t var, uint16_t value) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_setVar(var, value);
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::setVarImmediate(uint8_t var, uint16_t value) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_setVarImmediate(var, value);
		return nRetval;
	}
	return 0;
}

uint16_t PiezoStimDevice::getVarImmediate(uint8_t var) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		uint16_t nRetval = MyFunction_getVarImmediate(var);
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::incVar(uint8_t var) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_incVar(var);
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::decVar(uint8_t var) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_decVar(var);
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::outPort8(uint8_t slot, uint8_t port, uint8_t value) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_outPort8(slot, port, value);
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::outPort16(uint8_t slot, uint8_t portH, uint8_t portL, uint16_t value) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_outPort16(slot, portH, portL, value);
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::outPortVar16(uint8_t slot, uint8_t portH, uint8_t portL, uint8_t var) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_outPortVar16(slot, portH, portL, var);
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::inPortVar16(uint8_t slot, uint8_t portH, uint8_t portL, uint8_t var) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_inPortVar16(slot, portH, portL, var);
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::setTriggerMode(uint8_t slot, uint8_t port, uint8_t mode) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_setTriggerMode(slot, port, mode);
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::setTriggerLength(uint8_t slot, uint8_t port, uint8_t length) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_setTriggerLength(slot,port,length);
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::waitForTrigger(uint8_t slot, uint8_t port) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_waitForTrigger(slot,port);
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::triggerOut(uint8_t slot, uint8_t port) 
{
		if(LoadPiezoLibrary(LIBRARYFILENAME))
		{
			int nRetval = MyFunction_triggerOut(slot,port);
			return nRetval;
		}
		return 0;
}

int PiezoStimDevice::set2PDProperties(uint8_t module, uint8_t slot, uint8_t subslot, uint8_t dac_x, uint8_t dac_z0, uint8_t dac_z1) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_set2PDProperties(module, slot, subslot, dac_x, dac_z0, dac_z1);
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::set2PDCalibrationX(uint8_t module, uint16_t homeDACPos, double co0, double co1, double co2, double co3, double co4, double co5, double co6, double co7, double co8, double co9) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_set2PDCalibrationX(module, homeDACPos, co0, co1, co2, co3, co4, co5, co6, co7, co8, co9);
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::set2PDCalibrationZ(uint8_t module, double Z0_co0, double Z0_co1, double Z1_co0, double Z1_co1) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_set2PDCalibrationZ(module, Z0_co0, Z0_co1, Z1_co0, Z1_co1);
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::set2PDDistance(uint8_t module, double distance) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_set2PDDistance(module, distance);
		return nRetval;
	}
	return 0;
}

int PiezoStimDevice::set2PDHeight(uint8_t module, uint16_t promille_Z0, uint16_t promille_Z1) 
{
	if(LoadPiezoLibrary(LIBRARYFILENAME))
	{
		int nRetval = MyFunction_set2PDHeight(module, promille_Z0, promille_Z1);
		return nRetval;
	}
	return 0;
}
