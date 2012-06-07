var ExperimentManagerObj = new ExperimentManager();

function CleanupScript()
{
	ExperimentManagerObj.ExperimentStateHasChanged.disconnect(this, this.ExperimentStateChanged);
	ExperimentManagerObj.WriteToLogOutput.disconnect(this, this.Log);
	ExperimentManagerObj = null;
	ExperimentStateChanged = null;	
	CleanupScript = null;
	StimulGL.cleanupScript();
}

function ExperimentStateChanged()
{
//	Experiment_Constructed	= 0,
//	Experiment_Initialized	= 1,
//	Experiment_IsStarting	= 2,
//	Experiment_Started		= 3,
//	Experiment_IsStopping	= 4,
//	Experiment_Stopped		= 5	
	
//	Log("mySignalFunction arguments count: " + arguments.length);
//	for (var i = 0; i < arguments.length; ++i)
//		Log("mySignalFunction first argument: " + arguments[i]); 	
	
	if(arguments[0] == 5)//Experiment Stopped
	{
		Log("Experiment stopped");
		CleanupScript();
	}
}

ExperimentManagerObj.ExperimentStateHasChanged.connect(this, this.ExperimentStateChanged);
ExperimentManagerObj.WriteToLogOutput.connect(this, this.Log);
ExperimentManagerObj.setExperimentFileName("E:\\projects\\stimulgl\\Install\\experiments\\Eccentricity_Hye-Jin-Triggered.exml");//Eccentricity_Hye-Jin-Triggered.exml Polar_Hye-Jin-Triggered.exml
ExperimentManagerObj.runExperiment();

//Polar1.exml
//Polar2.exml
//Eccentricity1.exml
//Eccentricity2.exml
//MovingBar1.exml
//MovingBar2.exml
//MovingDots1.exml