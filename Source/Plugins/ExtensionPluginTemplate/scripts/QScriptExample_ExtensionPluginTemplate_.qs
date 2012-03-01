//Construct a new Plugin object
ExampleExtPluginObj = new _extensionname_();

//Example usage of the Example property
ExampleExtPluginObj.setExampleProperty(99);
Log(ExampleExtPluginObj.ExampleProperty);
ExampleExtPluginObj.ExampleProperty = 88;
Log(ExampleExtPluginObj.ExampleProperty);

//There are some global standard Script Functions which can be used like Log(), Beep(), ThrowError(), Include(), Pause(), ExitScript() and StimQT()
Beep();

//Almost all ECMA(Java)-script compatible code can be used here, see below String
var txt = new String("ECMA string");

//Almost all QT SDK libraries can be used trough automatically loaded script bindings
//We can even tell the main program to open an QT script binding example
var strPath = new String(StimulGL.getSelectedScriptFileLocation());
strPath = strPath + "/" + "StandardDialogs.qs"
Log(strPath);
StimulGL.openFiles(strPath);
StimulGL.executeScript();
Log(StimulGL.getSelectedScriptFileName());
StimulGL.closeSelectedScriptFile();

//Define a custom function
function myBeeps(n) 
{ 
	for(var i=0 ;i<n;i++)
	{
		Beep();
		txt = txt + i;
		Log(txt);
	}
} 
//Call the newly created function
myBeeps(5);
//Make sure to cleanup your functions before exiting!
myBeeps = null;

//Signal/Slot functionality
var timer = new QTimer(this);
var nCounter = 0;
function cleanup() //First we define a function for the necessary cleanup before exiting the script
{
	timer.stop(); //Stop the timer
	timer.timeout.disconnect(this, this.myFunction1); //Disconnect again
	myFunction1 = null; //cleanup the function
	ExampleExtPluginObj.ExampleSignalTriggered.disconnect(this, this.myFunction2); //Disconnect again
	myFunction2 = null; //Cleanup the function
	cleanup = null; //Cleanup the function
}
function myFunction1() //The first function demonstrates how a QTimer constructed from within the script can be connected to a custom defined script function
{ 
	nCounter++;
	Log("myFunction1() has been called " + nCounter + " times" ) 
	ExampleExtPluginObj.setExampleProperty(nCounter);
	if (nCounter==5)
	{
		cleanup();
	}
}
function myFunction2() //The second function demonstrates how a Plugin constructed from within the script can be connected to a custom defined script function
{ 
	Log("myFunction2() has been called" ) 
}
function PauseMills(millis)
{
	var date = new Date();
	var curDate = null;
     
	do { curDate = new Date(); }
	while(curDate-date < millis)
}

timer.timeout.connect(this, this.myFunction1); //The same as: connect(timer, SIGNAL(timeout()), this, SLOT(myFunction()));
ExampleExtPluginObj.ExampleSignalTriggered.connect(this, this.myFunction2); //Connect a Signal from the Plugin object dynamically to a custom script Function
timer.start(1000);
PauseMills(8000);//This function doesn't block the Timer thread, but has a high CPU load, it's better to use Timers/Signal/Slots
//Pause(5000);//This function does block the remaining!
PauseMills = null;
Log("Ready");
//ThrowError("Fake Error");//Interrupts the script at this line and opens the script parser


//The ExampleExtPluginObj object gets automatically destructed
