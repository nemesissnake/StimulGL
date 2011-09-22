var ExperimentManagerObj = new ExperimentManager();

function ExperimentStateChanged()
{
	Log("mySignalFunction arguments count: " + arguments.length);
	for (var i = 0; i < arguments.length; ++i)
		Log("mySignalFunction first argument: " + arguments[i]); 	
	
	if(arguments[0] == 2)//Experiment Stopped
	{
		ExperimentManagerObj.ExperimentStateHasChanged.disconnect(this, this.ExperimentStateChanged);
		ExperimentManagerObj = null;
		ExperimentStateChanged = null;
		Log("Experiment stopped");
	}
}

ExperimentManagerObj.ExperimentStateHasChanged.connect(this, this.ExperimentStateChanged);
ExperimentManagerObj.openExperiment("C:\\Users\\John\\Desktop\\polar_mapping.exml", false);
ExperimentManagerObj.runExperiment();




//ExperimentStateHasChanged(Experiment_Stopped);

//
//ExperimentManagerObj.saveExperiment("C:\\Users\\John\\Desktop\\experiment_example1.exml");
//ExperimentManagerObj.animate();
//ExperimentManagerObj.startAnimationTimer(100);
//ExperimentManagerObj = null;
//ExperimentManagerObj.runExperiment();
//var a = new QGLWidget();


