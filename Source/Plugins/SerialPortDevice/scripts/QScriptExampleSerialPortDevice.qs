//Construct a new Plugin object
ExampleDevicePluginObj = new SerialPortDevice();
var s = new String();
var n;
var nTriggerCounter = 0;

function myFinalCleanup()//Cleanup
{
	ExampleDevicePluginObj.close();	
	ExampleDevicePluginObj.CaptureThreadReadyRead.disconnect(this, this.mySignalFunction);//readyRead
	ExampleDevicePluginObj = null;
	mySignalFunction = null;
	myFinalCleanup = null;
	Log("Finished script Cleanup!");
}

function mySignalFunction()
{
	Log("mySignalFunction arguments count: " + arguments.length);
	for (var i = 0; i < arguments.length; ++i)
	Log("mySignalFunction first argument: " + arguments[i]); 	
	nTriggerCounter++;
	Log("nTriggerCounter = " + nTriggerCounter);
	if (nTriggerCounter==5)
	{
		myFinalCleanup();
	}
}

Log("----------------------------setPortName");
s="Port99"
ExampleDevicePluginObj.setPortName(s);
Log(ExampleDevicePluginObj.portName());
s="COM5";
ExampleDevicePluginObj.setPortName(s);
Log(ExampleDevicePluginObj.portName());

Log("----------------------------setBaudRate");
s="BAUD38400";
ExampleDevicePluginObj.setBaudRate(s);
Log(ExampleDevicePluginObj.baudRateToString());
s="BAUD19200";
ExampleDevicePluginObj.setBaudRate(s);
Log(ExampleDevicePluginObj.baudRateToString());

Log("----------------------------setDataBits");
s="DATA_7";
ExampleDevicePluginObj.setDataBits(s);
Log(ExampleDevicePluginObj.dataBitsToString());
s="DATA_8";
ExampleDevicePluginObj.setDataBits(s);
Log(ExampleDevicePluginObj.dataBitsToString());

Log("----------------------------setParity");
s="PAR_ODD";
ExampleDevicePluginObj.setParity(s);
Log(ExampleDevicePluginObj.parityToString());
s="PAR_NONE";
ExampleDevicePluginObj.setParity(s);
Log(ExampleDevicePluginObj.parityToString());

Log("----------------------------setFlowControl");
s="FLOW_HARDWARE";
ExampleDevicePluginObj.setFlowControl(s);
Log(ExampleDevicePluginObj.flowControlToString());
s="FLOW_OFF";
ExampleDevicePluginObj.setFlowControl(s);
Log(ExampleDevicePluginObj.flowControlToString());

Log("----------------------------setStopBits");
s="STOP_2";
ExampleDevicePluginObj.setStopBits(s);
Log(ExampleDevicePluginObj.stopBitsToString());
s="STOP_1";
ExampleDevicePluginObj.setStopBits(s);
Log(ExampleDevicePluginObj.stopBitsToString());

Log("----------------------------setTimeout");
n=10;
ExampleDevicePluginObj.setTimeout(n);

Log("----------------------------setQueryMode");
s="Polling";
ExampleDevicePluginObj.setQueryMode(s);
Log(ExampleDevicePluginObj.queryModeToString());
s="EventDriven";
ExampleDevicePluginObj.setQueryMode(s);
Log(ExampleDevicePluginObj.queryModeToString());

Log("----------------------------connect");
ExampleDevicePluginObj.CaptureThreadReadyRead.connect(this, this.mySignalFunction);
//ExampleDevicePluginObj.CaptureThreadReadyRead.connect(this, ExampleDevicePluginObj.test);
//ExampleDevicePluginObj.readyRead.connect(this, ExampleDevicePluginObj.test);

Log("----------------------------open/close");
s="ReadOnly | WriteOnly";
ExampleDevicePluginObj.open(s);
Log(ExampleDevicePluginObj.openModeToString());
Log(ExampleDevicePluginObj.isOpen());
//ExampleDevicePluginObj.close();
//Log(ExampleDevicePluginObj.isOpen());

//ExampleDevicePluginObj.CaptureThreadReadyRead();
//ExampleDevicePluginObj.CaptureThreadReadyRead();
//ExampleDevicePluginObj.CaptureThreadReadyRead();
//ExampleDevicePluginObj.CaptureThreadReadyRead();
//ExampleDevicePluginObj.CaptureThreadReadyRead();

//ExampleDevicePluginObj.open("ReadWrite");//3

//ExampleDevicePluginObj.close();
//ExampleDevicePluginObj.open(7);//ReadOnly
//Log(ExampleDevicePluginObj.openMode());
//ExampleDevicePluginObj.close();


//ExampleDevicePluginObj = null;


//Configure the Plugin Object
//ExampleDevicePluginObj.SetVendorID(0x181b);
//ExampleDevicePluginObj.SetProductID(0x4002);



//Construct a QTimer
//var timer = new QTimer(this);


//Declare some functions
//function mySignalFunction()
//{
//	Log("mySignalFunction arguments count: " + arguments.length);
//	for (var i = 0; i < arguments.length; ++i)
//	Log("mySignalFunction argument: " + arguments[i]); 	
//}
//
//function myTriggeredFunction()
//{
//	Log("myTriggeredFunction arguments count: " + arguments.length);
//	for (var i = 0; i < arguments.length; ++i)
//	Log("myTriggeredFunction argument: " + arguments[i]); 	
//}
//
//function myButtonTriggerFunction()
//{
//	Log("myButtonTriggerFunction arguments count: " + arguments.length);
//	for (var i = 0; i < arguments.length; ++i)
//	Log("myButtonTriggerFunction argument: " + arguments[i]); 	
//	nTimerCounter = 1;
//}
//
//function CalibrateStopped()
//{
//	Log("CalibrateStopped: " + arguments.length);
//	for (var i = 0; i < arguments.length; ++i)
//	Log("CalibrateStopped argument: " + arguments[i]); 	
//}
//
//function CaptureStopped()
//{
//	Log("CaptureStopped: " + arguments.length);
//	for (var i = 0; i < arguments.length; ++i)
//	Log("CaptureStopped argument: " + arguments[i]); 	
//	nTimerCounter = 0;
//}
//
//function PauseMills(millis)
//{
//	var date = new Date();
//	var curDate = null;
//     
//	do { curDate = new Date(); }
//	while(curDate-date < millis)
//}
//
//var nTimerCounter = 5;
//function timerFunction() //The first function demonstrates how a QTimer constructed from within the script can be connected to a custom defined script function
//{ 
//	Log("timerFunction1() is to be called " + nTimerCounter + "times" ) 
//	if(nTimerCounter == 1)
//	{
//		ExampleDevicePluginObj.StopCaptureThread();
//	}
//	else if (nTimerCounter == 0)
//	{	
//		cleanup();
//		return;
//	}
//	nTimerCounter--;
//}
//
//function cleanup() //First we define a function for the necessary cleanup before exiting the script
//{
//	timer.stop(); //Stop the timer
//	timer.timeout.disconnect(this, this.timerFunction); //Disconnect again
//	timerFunction = null; //cleanup the function
//	ExampleDevicePluginObj.CaptureThreadStarted.disconnect(this, this.mySignalFunction); 
//	ExampleDevicePluginObj.CaptureThreadStopped.disconnect(this, this.CaptureStopped); 
	//ExampleDevicePluginObj.CaptureThreadJoystickChanged.disconnect(this, this.myTriggeredFunction); 
//	ExampleDevicePluginObj.CaptureThreadButtonTriggered.disconnect(this, this.myButtonTriggerFunction);
//	CaptureStopped = null;
//	CalibrateStopped = null;
//	mySignalFunction = null; //Cleanup the function
//	myButtonTriggerFunction = null;
//	PauseMills = null; //Cleanup the function
//	cleanup = null; //Cleanup the function
	//ExampleDevicePluginObj = null;
//	Log("CleanUp Finished...");
//}

//Example usage of the Example property
//ExampleDevicePluginObj.setExampleProperty(99);
//Log(ExampleDevicePluginObj.ExampleProperty);
//ExampleDevicePluginObj.doMeasurement();
//ExampleDevicePluginObj.ExampleProperty = 88;
//Log(ExampleDevicePluginObj.ExampleProperty);



//ExampleDevicePluginObj.Calibrate();
//Log("Calibration done...");
//ExampleDevicePluginObj.Calibrate();
//Log("Calibration done...");


//Connect the Signal/Slots
//ExampleDevicePluginObj.CaptureThreadStarted.connect(this, this.mySignalFunction); 
//ExampleDevicePluginObj.CaptureThreadStopped.connect(this, this.CaptureStopped); 
//ExampleDevicePluginObj.CalibrateThreadStopped.connect(this, this.CalibrateStopped); 
//ExampleDevicePluginObj.CaptureThreadJoystickChanged.connect(this, this.myTriggeredFunction); 
//ExampleDevicePluginObj.CaptureThreadButtonTriggered.connect(this, this.myButtonTriggerFunction);//unsigned char
//
//ExampleDevicePluginObj.WriteCapturedDataToFile(true,"d:\\temp\\output.txt",true,true);//WriteCapturedDataToFile(bool bWriteToFile, QString qsFileName, bool bWriteHeaderInfo, bool bWriteCalibratedData)
//ExampleDevicePluginObj.ConfigureHIDTriggers(false,false,3,1);//=DetectionMethod::MaskedValueChangedHigh);
//ExampleDevicePluginObj.ConfigureHIDTriggers(true,true,3,1);//=DetectionMethod::MaskedValueChangedHigh);
//ConfigureHIDTriggers(bool bActivateJoystickTrigger = true, bool bActivateButtonTriggers = false, unsigned char cButtonMask = 255, DetectionMethod ButtonDetectDetectionMethod = DetectionMethod::MaskedValueChanged);
//ExampleDevicePluginObj.ConfigureHIDFiltering(true, 1, true, 10);
//void ConfigureHIDFiltering(bool bActivateJoystickStabilisation, int nJoystickStabilisationThreshold, bool bActivateJoystickHistory, int nJoystickHistorySize);
//
//ExampleDevicePluginObj.StartCaptureThread();
//timer.timeout.connect(this, this.timerFunction);
//timer.start(1000);
