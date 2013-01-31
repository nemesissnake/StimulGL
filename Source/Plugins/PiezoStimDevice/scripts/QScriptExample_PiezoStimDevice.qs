//Construct a new Plugin object
var PiezoStimDeviceobject = new PiezoStimDevice();
var sLicenseCode = "24OF1B661A7EW5116941EABAMDJSGO";
var nRetval;
var nInteger = 1;
var sString = "Test";
var dDouble = 1.87;

////Test whether we can invoke some standard template example slots
//PiezoStimDeviceobject.setExampleProperty(99);
//Log(PiezoStimDeviceobject.getExampleProperty());
//PiezoStimDeviceobject.ExampleProperty = 0;
//Log(PiezoStimDeviceobject.ExampleProperty);

nRetval = PiezoStimDeviceobject.initStimulator(sLicenseCode);
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.resetStimulator();
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.closeStimulator();
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));


nRetval = PiezoStimDeviceobject.setProperty(sString , nInteger );
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.getProperty(sString );
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.startStimulation();
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.stopStimulation();
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.setDAC(nInteger , nInteger );
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.setPinBlock8(nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger );
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.setPinBlock(nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger );		
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.setPinBlock10(nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger );		
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.wait(nInteger , nInteger );
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.setVar(nInteger , nInteger );
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.setVarImmediate(nInteger , nInteger );
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.getVarImmediate(nInteger );
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.incVar(nInteger );
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.decVar(nInteger );
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.outPort8(nInteger , nInteger , nInteger );
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.outPort16(nInteger , nInteger , nInteger , nInteger );
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.outPortVar16(nInteger , nInteger , nInteger , nInteger );
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.inPortVar16(nInteger , nInteger , nInteger , nInteger );
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.setTriggerMode(nInteger , nInteger , nInteger );
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.setTriggerLength(nInteger , nInteger , nInteger );
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.waitForTrigger(nInteger , nInteger );
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.triggerOut(nInteger , nInteger );
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.set2PDProperties(nInteger , nInteger , nInteger , nInteger , nInteger , nInteger );
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.set2PDCalibrationX(nInteger , nInteger , dDouble , dDouble , dDouble , dDouble , dDouble , dDouble , dDouble , dDouble , dDouble , dDouble );
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.set2PDCalibrationZ(nInteger , dDouble , dDouble , dDouble , dDouble );
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.set2PDDistance(nInteger , dDouble );
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.set2PDHeight(nInteger , nInteger , nInteger );
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));


Log();
//Set the constructed object to null
PiezoStimDeviceobject = null;

//	QString ReturnCodeToString(const int &nRetval);
//	int initStimulator(const QString &sLicense);
//	int closeStimulator();
//	int resetStimulator();
//	int setProperty(QString property, int value);
//	int getProperty(QString property);
//	int startStimulation();
//	int stopStimulation();
//	int setDAC(int dac, int wert);
//	int setPinBlock8(int slot, int int_trigger, int pin0, int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, int pin7);
//	int setPinBlock(int slot, int int_trigger, int pin0, int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, int pin7);		
//	int setPinBlock10(int slot, int int_trigger, int pin0, int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, int pin7, int pin8, int pin9);		
//	int wait(int int_trigger, int time);
//	int setVar(int var, int value);
//	int setVarImmediate(int var, int value);
//	int getVarImmediate(int var);
//	int incVar(int var);
//	int decVar(int var);
//	int outPort8(int slot, int port, int value);
//	int outPort16(int slot, int portH, int portL, int value);
//	int outPortVar16(int slot, int portH, int portL, int var);
//	int inPortVar16(int slot, int portH, int portL, int var);
//	int setTriggerMode(int slot, int port, int mode);
//	int setTriggerLength(int slot, int port, int length);
//	int waitForTrigger(int slot, int port);
//	int triggerOut(int slot, int port);
//	int set2PDProperties(int module, int slot, int subslot, int dac_x, int dac_z0, int dac_z1);
//	int set2PDCalibrationX(int module, int homeDACPos, double co0, double co1, double co2, double co3, double co4, double co5, double co6, double co7, double co8, double co9);
//	int set2PDCalibrationZ(int module, double Z0_co0, double Z0_co1, double Z1_co0, double Z1_co1);
//	int set2PDDistance(int module, double distance);
//	int set2PDHeight(int module, int promille_Z0, int promille_Z1);