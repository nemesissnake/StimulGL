var ExperimentManagerObj = new ExperimentManager();

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
		ExperimentManagerObj = null;
		ExperimentStateChanged = null;
		Log("Experiment stopped");
	}
}

ExperimentManagerObj.ExperimentStateHasChanged.connect(this, this.ExperimentStateChanged);
ExperimentManagerObj.WriteToLogOutput.connect(this, this.Log);
ExperimentManagerObj.openExperiment("D:/Projects/StimulGL/Install/experiments/MovingBar2.exml",false);//test  run_1 "D:\\Projects\\StimulGL\\Install\\examples\\retino_mapping_timer.exml", false);//retino_mapping_movdots_timer.exml, retino_mapping_polar_timer, retino_mapping_movbar_timer.exml
ExperimentManagerObj.runExperiment();

//test.exml
//Polar1.exml
//Polar2.exml
//Eccentricity1.exml
//Eccentricity2.exml
//MovingBar1.exml
//MovingBar2.exml