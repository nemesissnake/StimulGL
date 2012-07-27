var strGlobal = new String(""); //Just a string for global use
var nTriggerCounter = 0;
var triggerTimer = new QTimer();
var KeyBoardDeviceObj = new KeyBoardDevice(); //Construct a KeyBoard Object


function myFinalCleanup()//Cleanup
{
	KeyBoardDeviceObj.StopCaptureThread();	
	KeyBoardDeviceObj.CaptureThreadStarted.disconnect(this, this.mySignalFunction);
	KeyBoardDeviceObj.CaptureThreadKeyPressed.disconnect(this, this.myKeyProcessingFunction);
	KeyBoardDeviceObj.CaptureThreadKeyReleased.disconnect(this, this.myKeyProcessingFunction);	
	KeyBoardDeviceObj.CaptureThreadStopped.disconnect(this, this.mySignalFunction);
	triggerTimer.timeout.disconnect(this, this.mySignalFunction);
	KeyBoardDeviceObj = null;
	mySignalFunction = null;
	myFinalCleanup = null;
	triggerTimer = null;
	myKeyProcessingFunction = null;
	Log("Finished script Cleanup!");
}

function myKeyProcessingFunction()
{
	Log("myKeyProcessingFunction arguments count: " + arguments.length);
	for (var i = 0; i < arguments.length; ++i)tt
		Log("myKeyProcessingFunction argument(" + i + "): " + arguments[i]); 	
}

function mySignalFunction()
{
	Log("mySignalFunction arguments count: " + arguments.length);
	for (var i = 0; i < arguments.length; ++i)
		Log("myKeyProcessingFunction argument(" + i + "): " + arguments[i]); 	
	
	nTriggerCounter++;
	Log("nTriggerCounter = " + nTriggerCounter);
	if (nTriggerCounter==4)
	{
		KeyBoardDeviceObj.StopCaptureThread();
	}	
	if (nTriggerCounter==5)
	{
		myFinalCleanup();
	}
}

KeyBoardDeviceObj.CaptureThreadStarted.connect(this, this.mySignalFunction);
KeyBoardDeviceObj.CaptureThreadKeyPressed.connect(this, this.myKeyProcessingFunction);
KeyBoardDeviceObj.CaptureThreadKeyReleased.connect(this, this.myKeyProcessingFunction);
KeyBoardDeviceObj.CaptureThreadStopped.connect(this, this.mySignalFunction);
KeyBoardDeviceObj.StartCaptureThread(2);
triggerTimer.timeout.connect(this, this.mySignalFunction);
triggerTimer.start(1000);
