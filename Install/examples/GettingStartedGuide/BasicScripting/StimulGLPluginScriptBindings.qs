// This example shows how you can access the features of an StimulGL Plugin from the script.

//Declare a global variabele to count the number of key presses while capturing
var KeyPressedCounter = 0;
//Construct a StimulGL Plugin KeyBoard Object
var KeyBoardDeviceObj = new KeyBoardDevice(); 

//This function makes sure that everything gets nicely cleaned before the script ends
function ScriptCleanupFunction()//Cleanup
{
	//Disconnect the signal/slots
	KeyBoardDeviceObj.CaptureThreadStarted.disconnect(this, this.KeyCaptureStartedFunction);
	KeyBoardDeviceObj.CaptureThreadKeyPressed.disconnect(this, this.KeyCaptureDetectFunction);
	KeyBoardDeviceObj.CaptureThreadKeyReleased.disconnect(this, this.KeyCaptureDetectFunction);	
	KeyBoardDeviceObj.CaptureThreadStopped.disconnect(this, this.KeyCaptureStoppedFunction);
	//Set all constructed objects to null 
	KeyBoardDeviceObj = null;
	//Set all functions to null
	KeyCaptureStartedFunction = null;
	KeyCaptureDetectFunction = null;
	KeyCaptureStoppedFunction = null;
	ScriptCleanupFunction = null;
	//Write something to the Log Output Pane so we know that this Function executed successfully.
	Log("Finished script Cleanup!");
	StimulGL.cleanupScript();
}

//This function is called whenever the user presses a key
function KeyCaptureDetectFunction(keyCode)
{
	Log("A key press was detected: " + keyCode); 	
	KeyPressedCounter++;
	Log("KeyPressedCounter incremented to: " + KeyPressedCounter + "\n");
	if (KeyPressedCounter==4)
	{
		KeyBoardDeviceObj.StopCaptureThread();		
	}
}

//This function is called whenever the Key capture status has been started
function KeyCaptureStartedFunction()
{
	Log("\n\n\nThe key capture thread was just started...\n");
	Log("KeyCaptureStartedFunction arguments count: " + arguments.length);
	for (var i = 0; i < arguments.length; ++i)
		Log("KeyCaptureStartedFunction argument(" + i + "): " + arguments[i]);
	Log("\n");
}

//This function is called whenever the Key capture status has been stopped
function KeyCaptureStoppedFunction(DateTimeStampStopped)
{
	Log("\nThe key capture thread was just stopped @ " + DateTimeStampStopped + "\n"); 
	ScriptCleanupFunction();
}

//Connect the signal/slots
KeyBoardDeviceObj.CaptureThreadStarted.connect(this, this.KeyCaptureStartedFunction);
KeyBoardDeviceObj.CaptureThreadKeyPressed.connect(this, this.KeyCaptureDetectFunction);
KeyBoardDeviceObj.CaptureThreadKeyReleased.connect(this, this.KeyCaptureDetectFunction);
KeyBoardDeviceObj.CaptureThreadStopped.connect(this, this.KeyCaptureStoppedFunction);
//Start the capture thread
KeyBoardDeviceObj.StartCaptureThread(0, false);
//StartCaptureThread(const short method, bool keyForwarding)
//method == 0 --> KeyPressed
//method == 1 --> KeyReleased
//method == 2 --> KeyPressedReleased



