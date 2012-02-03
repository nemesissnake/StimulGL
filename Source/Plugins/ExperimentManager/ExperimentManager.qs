var ExperimentManagerObj = new ExperimentManager();
//var TriggerTimerObj = new TriggerTimer();
//var RetinoMap_glwidgetObj = new RetinoMap_glwidget();
var ImageProcessorObj = new ImageProcessor();

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
		ExperimentManagerObj.ExperimentStateHasChanged.disconnect(this, this.ExperimentStateChanged);
		ExperimentManagerObj.WriteToLogOutput.disconnect(this, this.Log);
		ExperimentManagerObj.deletelater();
		ExperimentStateChanged = null;
		//TriggerTimerObj = null;
		//RetinoMap_glwidgetObj = null;
		ImageProcessorObj.ConvertPngToDatFile("Source", "Dest");
		ImageProcessorObj.deleteLater();
		Log("Experiment stopped");
	}
}

ExperimentManagerObj.ExperimentStateHasChanged.connect(this, this.ExperimentStateChanged);
ExperimentManagerObj.WriteToLogOutput.connect(this, this.Log);
ExperimentManagerObj.openExperiment("D:\\Projects\\StimulGL\\Install\\experiments\\test.exml",false);
ExperimentManagerObj.runExperiment();
//bool ImageProcessor::ConvertPngToDatFile(QString strSource, QString strDestination)


//Polar1.exml
//Polar2.exml
//Eccentricity1.exml
//Eccentricity2.exml
//MovingBar1.exml
//MovingBar2.exml