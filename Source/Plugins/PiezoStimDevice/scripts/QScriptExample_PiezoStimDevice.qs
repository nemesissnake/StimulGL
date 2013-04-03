//Construct a new Plugin object
var PiezoStimDeviceobject = new PiezoStimDevice();
var sLicenseCode = "";//Please fill-in your license code here!
var nCounter;
var nRetval;
var nInteger = 1;
var sString = "Test";
var dDouble = 1.87;

//Construct a StimulGL Plugin KeyBoard Object
var KeyBoardCaptureObj = new KeyBoardCapture(); 
var activationArray = new Array();

////Test whether we can invoke some standard template example slots
//PiezoStimDeviceobject.setExampleProperty(99);
//Log("	 +PiezoStimDeviceobject.getExampleProperty());
//PiezoStimDeviceobject.ExampleProperty = 0;
//Log("	 +PiezoStimDeviceobject.ExampleProperty);

//This function makes sure that everything gets nicely cleaned before the script ends
function ScriptCleanupFunction()//Cleanup
{
	KeyBoardCaptureObj.StopCaptureThread();
	//Disconnect the signal/slots
	KeyBoardCaptureObj.CaptureThreadKeyPressed.disconnect(this, this.KeyCaptureDetectFunction);
	//Set all constructed objects to null 
	KeyBoardCaptureObj = null;
	PiezoStimDeviceobject = null;
	activationArray = null;
	//Set all functions to null
	KeyCaptureDetectFunction = null;
	ScriptCleanupFunction = null;
	PiezoStimulation = null;
	//Write something to the Log Output Pane so we know that this Function executed successfully.
	Log("\nFinished script Cleanup!");
	StimulGL.cleanupScript();
}

//This function is called whenever the user presses a key
function KeyCaptureDetectFunction(keyCode)
{
	Log("A key press was detected: " + keyCode); 	
	
	if (keyCode==118)//F7 key
	{
		Log("- Configure Stimulator....");
		nRetval = PiezoStimDeviceobject.closeStimulator();
		Log("\t" + PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - closeStimulator()");
		nRetval = PiezoStimDeviceobject.setProperty("local_buffer_size" , 5000000 );		//PC buffer in bytes (default 50 000). This property must be set before initStimulator
		Log("\t" + PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - setProperty()");
		nRetval = PiezoStimDeviceobject.initStimulator(sLicenseCode);
		Log("\t" + PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - initStimulator()");
		nRetval = PiezoStimDeviceobject.setTriggerMode(16, 1, 1); 					//slot (address of controller), port (0=in, 1=out), mode(1=rising edge, 0=falling)
		Log("\t" + PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - setTriggerMode()");
		nRetval = PiezoStimDeviceobject.setTriggerLength(16, 1, 50);					//slot, port, duration of trigger (multiples of 0.5ms)
		Log("\t" + PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - setTriggerLength()");
		//nRetval = PiezoStimDeviceobject.setDAC(1 , 4095);
		//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
		nRetval = PiezoStimDeviceobject.setPinBlock10(0, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1);		
		Log("\t" + PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - setPinBlock10()");
	}
	else if (keyCode==119)//F8 key
	{
		Log("- Programming....");
		PiezoStimulation(1);	
	}
	else if (keyCode==120)//F9 key
	{	
		Log("- Start Stimulation....");
		nRetval = PiezoStimDeviceobject.startStimulation();
		Log("\t" + PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - startStimulation()");		
	}
	else if (keyCode==121)//F10 key
	{
		Log("- Stop Stimulation....");
		nRetval = PiezoStimDeviceobject.stopStimulation();
		Log("\t" + PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - stopStimulation()");	
	}
	else if (keyCode==122)//F11 key
	{
		Log("- Resetting....");
		nRetval = PiezoStimDeviceobject.resetStimulator();
		Log("\t" + PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - resetStimulator()");	
	}
	else if (keyCode==27)//Escape key
	{			
		ScriptCleanupFunction();
	}
}

function PiezoStimulation(TriggerValue)
{	
	Log("\tPiezoStimulation()");
	for(nCounter=0;nCounter<50;nCounter++)
	{
		activationArray[nCounter] = nCounter*80;
	}
	for(nCounter=0;nCounter<activationArray.length;nCounter++)
	{	
		nRetval = PiezoStimDeviceobject.setDAC(1 , activationArray[nCounter]);
		//Log("\t" + PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - setDAC()");	
		//Pause(10);
		nRetval = PiezoStimDeviceobject.wait(TriggerValue, 80);
		//Log("\t" + PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - wait()");		
	}
	nRetval = PiezoStimDeviceobject.setDAC(1 , 0);
	Log("\t" + PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - setDAC()");
}

KeyBoardCaptureObj.CaptureThreadKeyPressed.connect(this, this.KeyCaptureDetectFunction);
KeyBoardCaptureObj.StartCaptureThread(0, true);
Log("");



//nRetval = PiezoStimDeviceobject.setProperty(sString , nInteger );
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.getProperty(sString );
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.startStimulation();
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.stopStimulation();
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.setDAC(nInteger , nInteger );
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.setPinBlock8(nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger );
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.setPinBlock(nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger );		
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.setPinBlock10(nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger , nInteger );		
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.wait(nInteger , nInteger );
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.setVar(nInteger , nInteger );
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.setVarImmediate(nInteger , nInteger );
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.getVarImmediate(nInteger );
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.incVar(nInteger );
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.decVar(nInteger );
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.outPort8(nInteger , nInteger , nInteger );
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.outPort16(nInteger , nInteger , nInteger , nInteger );
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.outPortVar16(nInteger , nInteger , nInteger , nInteger );
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.inPortVar16(nInteger , nInteger , nInteger , nInteger );
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.setTriggerMode(nInteger , nInteger , nInteger );
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.setTriggerLength(nInteger , nInteger , nInteger );
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.waitForTrigger(nInteger , nInteger );
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.triggerOut(nInteger , nInteger );
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.set2PDProperties(nInteger , nInteger , nInteger , nInteger , nInteger , nInteger );
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.set2PDCalibrationX(nInteger , nInteger , dDouble , dDouble , dDouble , dDouble , dDouble , dDouble , dDouble , dDouble , dDouble , dDouble );
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.set2PDCalibrationZ(nInteger , dDouble , dDouble , dDouble , dDouble );
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.set2PDDistance(nInteger , dDouble );
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");
//nRetval = PiezoStimDeviceobject.set2PDHeight(nInteger , nInteger , nInteger );
//Log("	 +PiezoStimDeviceobject.ReturnCodeToString(nRetval) + " - ");


//Log();
//Set the constructed object to null
//PiezoStimDeviceobject = null;

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