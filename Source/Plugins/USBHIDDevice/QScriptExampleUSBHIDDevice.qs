//Construct a new Plugin object
ExampleDevicePluginObj = new USBHIDDevice();
//Construct a QTimer
var timer = new QTimer(this);
//Connect the Signal/Slots
ExampleDevicePluginObj.ExampleSignalTriggered.connect(this, this.mySignalFunction); 
ExampleDevicePluginObj.CaptureThreadStarted.connect(this, this.mySignalFunction); 
ExampleDevicePluginObj.CaptureThreadStopped.connect(this, this.mySignalFunction); 
ExampleDevicePluginObj.CaptureThreadTriggered.connect(this, this.mySignalFunction); 

//Declare some functions
function mySignalFunction()
{
	Log("mySignalFunction arguments count: " + arguments.length);
	for (var i = 0; i < arguments.length; ++i)
	Log("mySignalFunction first argument: " + arguments[i]); 	
}

function PauseMills(millis)
{
	var date = new Date();
	var curDate = null;
     
	do { curDate = new Date(); }
	while(curDate-date < millis)
}

var nTimerCounter = 0;
function timerFunction() //The first function demonstrates how a QTimer constructed from within the script can be connected to a custom defined script function
{ 
	Log("timerFunction1() has been called " + nTimerCounter + "times" ) 
	if(nTimerCounter == 5)
	{
		ExampleDevicePluginObj.StopCaptureThread();
	}
	else if (nTimerCounter == 6)
	{	
		cleanup();
		return;
	}
	nTimerCounter++
}

function cleanup() //First we define a function for the necessary cleanup before exiting the script
{
	Log("Starting CleanUp");
	timer.stop(); //Stop the timer
	timer.timeout.disconnect(this, this.timerFunction); //Disconnect again
	timerFunction = null; //cleanup the function
	ExampleDevicePluginObj.ExampleSignalTriggered.disconnect(this, this.mySignalFunction);
	ExampleDevicePluginObj.CaptureThreadStarted.disconnect(this, this.mySignalFunction); 
	ExampleDevicePluginObj.CaptureThreadStopped.disconnect(this, this.mySignalFunction); 
	ExampleDevicePluginObj.CaptureThreadTriggered.disconnect(this, this.mySignalFunction); 
	mySignalFunction = null; //Cleanup the function
	PauseMills = null; //Cleanup the function
	cleanup = null; //Cleanup the function
}

//Example usage of the Example property
ExampleDevicePluginObj.setExampleProperty(99);
//Log(ExampleDevicePluginObj.ExampleProperty);
//ExampleDevicePluginObj.doMeasurement();
//ExampleDevicePluginObj.Calibrate();
ExampleDevicePluginObj.StartCaptureThread();
ExampleDevicePluginObj.ExampleProperty = 88;
//Log(ExampleDevicePluginObj.ExampleProperty);
timer.timeout.connect(this, this.timerFunction);
timer.start(1000);
//Pause(5000);//This function does block the remaining!
//PauseMills(5000);//This function doesn't block the Timer thread, but has a high CPU load, it's better to use Timers/Signal/Slots
//ExampleDevicePluginObj.StopCaptureThread();
//PauseMills(2000);//This function doesn't block the Timer thread, but has a high CPU load, it's better to use Timers/Signal/Slots
//Pause(4000);
//cleanup();



//There are some global standard Script Functions which can be used like Log(), Beep(), ThrowError(), Include(), Pause(), ExitScript() and StimQT()
//Beep();
//
//Almost all ECMA(Java)-script compatible code can be used here, see below String
//var txt = new String("ECMA string");
//
//Almost all QT SDK libraries can be used trough automatically loaded script bindings
//We can even tell the main program to open an QT script binding example
//var strPath = new String(StimQT.getSelectedScriptFileLocation());
//strPath = strPath + "/" + "StandardDialogs.qs"
//Log(strPath);
//StimQT.openFiles(strPath);
//StimQT.executeScript();
//Log(StimQT.getSelectedScriptFileName());
//StimQT.closeSelectedScriptFile();
//
//Define a custom function
//function myBeeps(n) 
//{ 
//	for(var i=0 ;i<n;i++)
//	{
//		Beep();
//		txt = txt + i;
//		Log(txt);
//	}
//} 
//Call the newly created function
//myBeeps(5);
//Make sure to cleanup your functions before exiting!
//myBeeps = null;
//
//Signal/Slot functionality
//var timer = new QTimer(this);
//var nCounter = 0;

//function myFunction1() //The first function demonstrates how a QTimer constructed from within the script can be connected to a custom defined script function
//{ 
//	nCounter++;
//	Log("myFunction1() has been called " + nCounter + " times" ) 
//	ExampleExtPluginObj.setExampleProperty(nCounter);
//	if (nCounter==5)
//	{
//		cleanup();
//	}
//}

//function PauseMills(millis)
//{
//	var date = new Date();
//	var curDate = null;
//     
//	do { curDate = new Date(); }
//	while(curDate-date < millis)
//}
//
//timer.timeout.connect(this, this.myFunction1); //The same as: connect(timer, SIGNAL(timeout()), this, SLOT(myFunction()));
//ExampleExtPluginObj.ExampleSignalTriggered.connect(this, this.myFunction2); //Connect a Signal from the Plugin object dynamically to a custom script Function
//timer.start(1000);
//PauseMills(8000);//This function doesn't block the Timer thread, but has a high CPU load, it's better to use Timers/Signal/Slots
//Pause(5000);//This function does block the remaining!
//PauseMills = null;
//Log("Ready");
//ThrowError("Fake Error");//Interrupts the script at this line and opens the script parser
//
//

//The ExampleExtPluginObj object gets automatically destructed
