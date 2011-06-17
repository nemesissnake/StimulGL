//Construct a new RetinotopicMapping Plugin object
RetinotopicMappingObject = new Retinotopic_Mapping();
PPDevice = new ParallelPort(); //Construct a Parallel Port Object

//Retrieve the current scripts location
var strScriptPath = new String(StimQT.getSelectedScriptFileLocation());
var triggerTimer = new QTimer(this);

function myFinalCleanup()//Cleanup
{
	Log("Starting to Cleanup");
	triggerTimer.stop(); //Stop the timer
	triggerTimer = null;
	RetinotopicMappingObject.ExperimentStopped.disconnect(this, this.myFinalCleanup);
	RetinotopicMappingObject = null;
	if (bUseParallelPort)
	{
		//PPDevice.StopGenerateThread();
		PPDevice.StopCaptureThread();
		PPDevice.CaptureThreadTriggered.disconnect(this, this.myTrigger2);	
		//PPDevice.CaptureThreadStarted.disconnect(this, this.myTrigger2);
	}
	PPDevice = null;
	myTrigger = null;
	myTrigger2 = null;
	myFinalCleanup = null;
	Log("Finished script Cleanup!");
}

function myTrigger()
{
	Log("Triggered: "+ RetinotopicMappingObject.incrementTriggerCount());
}

function myTrigger2()
{
	Log("Triggered2: "+ RetinotopicMappingObject.incrementTriggerCount());
}

//Set the Experiment File to use
//if(RetinotopicMappingObject.setExperimentFileName(strScriptPath + "/" + "Retinotopic_mapping_files/polar_mapping.exp"))
//{
//	Log("The ExperimentFileName = " + RetinotopicMappingObject.getExperimentFileName());
//	if(RetinotopicMappingObject.setExperimentType(1))
//		Log("The experiment Type has been set(polar_mapping = 1)");
//}
if(RetinotopicMappingObject.setExperimentFileName(strScriptPath + "/" + "Retinotopic_mapping_files/eccentricity_mapping.exp"))
{
	Log("The ExperimentFileName = " + RetinotopicMappingObject.getExperimentFileName());
	if(RetinotopicMappingObject.setExperimentType(::bgf))
		Log("The experiment Type has been set(eccentricity_mapping = 0)");
}
//Set to use FullScreen mode
if(RetinotopicMappingObject.setFullScreenMode(true))
	Log("Full Screen Mode has changed");
//Set to Debug mode
if(RetinotopicMappingObject.setDebugMode(true))
	Log("Debug Mode has changed");
//Load the Experiment
if(RetinotopicMappingObject.loadExperiment())
	Log("The experiment has been loaded");

var bInternalTriggering = false;
var bUseParallelPort = true;

if (bInternalTriggering)//Use of internal Triggering
{
	if(RetinotopicMappingObject.setTriggerMode(0))
		Log("The experiment Trigger mode (0=internal) has been set");		
	//Set the Experiment Tr Time to use
	if(RetinotopicMappingObject.setTrTime(2000))
		Log("The experiment Tr Time has been set");	
}
else//Use of external Triggering (custom QTimer or Parallel Port)
{
	if(RetinotopicMappingObject.setTriggerMode(1))
	Log("The experiment Trigger mode (1=external) has been set");
	//Set the Experiment Tr Time to use
	if(RetinotopicMappingObject.setTrTime(1000))
		Log("The experiment Tr Time has been set");	
	if(bUseParallelPort)//Use of the parallel port plugin object(connect pin 12 with ground to activate!)
	{
		Log("Connecting the parallel port plugin object");
		PPDevice.BaseAddress = 4368; //4368 (decimal) == 1110 (hexadecimal) 
		PPDevice.CaptureThreadTriggered.connect(this, this.myTrigger2);
		//PPDevice.CaptureThreadStarted.connect(this, this.myTrigger2);
		//PPDevice.StartGenerateThread(4368, 2, 255, 2, 16, 250, 500);	
		PPDevice.StartCaptureThread(4369, 32, 2, 100, 100);
	}
	else//Use of an Script QTimer constructed object
	{
		triggerTimer.timeout.connect(this, this.myTrigger);
		triggerTimer.start(1000);
	}
}

RetinotopicMappingObject.ExperimentStopped.connect(this, this.myFinalCleanup);

if(RetinotopicMappingObject.runExperiment())
{
	Log("The experiment succesfully started");
}
else
{
	Log("The experiment could not start");
	myFinalCleanup();
}