var strGlobal = new String(""); //Just a string for global use
var nTriggerCounter = 0;
var triggerTimer = new QTimer();

function myFinalCleanup()//Cleanup
{
	PPDevice.StopCaptureThread();	
	PPDevice.CaptureThreadStarted.disconnect(this, this.mySignalFunction);
	//PPDevice.CaptureThreadTriggered.disconnect(this, this.mySignalFunction);
	PPDevice.CaptureThreadKeyPressed.disconnect(this, this.myKeyProcessingFunction);
	PPDevice.CaptureThreadKeyReleased.disconnect(this, this.myKeyProcessingFunction);	
	
	PPDevice.CaptureThreadStopped.disconnect(this, this.mySignalFunction);
	triggerTimer.timeout.disconnect(this, this.mySignalFunction);
	PPDevice = null;
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
		PPDevice.StopCaptureThread();
	}	
	if (nTriggerCounter==5)
	{
		myFinalCleanup();
	}
}

PPDevice = new KeyBoardDevice(); //Construct a KeyBoard Object
//Configure the Parallel Port
//Log(PPDevice.GetMinimalMainProgramVersion());//Test(""));
//Log("The default BaseAdress is: " + PPDevice.BaseAddress); //Read the default Parallel Port BaseAddress
//PPDevice.BaseAddress = 4368; //4368 (decimal) == 1110 (hexadecimal) 
//Log("The changed BaseAdress is: " + PPDevice.BaseAddress); //Read the changed Parallel Port BaseAddress again

//Log("The current Port Description" + PPDevice.GetPortDescription());

//Read/Write some Port Values at the new BaseAddress
//for (i=0;i<5;i++) //Create a simple for-loop
//{
//	PPDevice.PortWrite(64); //64 => only bit6 (0..7) is active
//	Log(PPDevice.PortRead());
//	Pause(100); //Wait some time, this blocks the script
//	PPDevice.PortWrite(33); //33(=1+32) => bit0 and bit5 (0..7) are active 
//	Log(PPDevice.PortRead());
//	Pause(100);
//}

PPDevice.CaptureThreadStarted.connect(this, this.mySignalFunction);
//PPDevice.CaptureThreadTriggered.connect(this, this.mySignalFunction);
PPDevice.CaptureThreadKeyPressed.connect(this, this.myKeyProcessingFunction);
PPDevice.CaptureThreadKeyReleased.connect(this, this.myKeyProcessingFunction);
PPDevice.CaptureThreadStopped.connect(this, this.mySignalFunction);
PPDevice.StartCaptureThread(4370, 1, 2, 0, 100);//(const shortbaseAddress, const short mask, const short method, const int postLHDelay = 0, const int postHLDelay = 0);
//triggerTimer.setInterval (1000);
triggerTimer.timeout.connect(this, this.mySignalFunction);
triggerTimer.start(1000);
