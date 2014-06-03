//Includes
Include("QtScript/BasicQMLExperiment.qs"); //Do not use a backslash (\) in a string, use forward slashes for a directory definition!//../../include/
//Prepare Output Window
StimulGL.addOutputWindow("RealTimeFeedback");
StimulGL.clearOutputWindow("RealTimeFeedback");
//Configuration
(function( RealTimeFeedback, undefined )
{ 
//Public Property Variabeles
	RealTimeFeedback.TBVExchangerObject = new TBVExchanger();//Construct a new Plugin object
	
//Public Property Functions
	RealTimeFeedback.__proto__.executePreStep = function(nTimePoint)
	{
		BasicExperiment.LogFunctionSignature("RealTimeFeedback","executePreStep", arguments, false);
	}

	RealTimeFeedback.__proto__.executePostStep = function(nTimePoint)
	{
		BasicExperiment.LogFunctionSignature("RealTimeFeedback","executePostStep", arguments, false);
	}
	
	RealTimeFeedback.__proto__.executePostRun = function(nTimePoint)
	{
		BasicExperiment.LogFunctionSignature("RealTimeFeedback","executePostRun", arguments, false);	
	}

	RealTimeFeedback.__proto__.disconnected = function()
	{
		BasicExperiment.LogFunctionSignature("RealTimeFeedback","disconnected", arguments, false);	
	}

	RealTimeFeedback.__proto__.connected = function()
	{
		BasicExperiment.LogFunctionSignature("RealTimeFeedback","connected", arguments, false);
	}

	RealTimeFeedback.__proto__.connectionError = function(sError)
	{
		BasicExperiment.LogFunctionSignature("RealTimeFeedback","connectionError", arguments, false);
	}
	
//*!This wrapper function uses the wrap method because the original function has more than one parameter
BasicExperiment.__proto__.preExperimentStateChanged = _.wrap(BasicExperiment.__proto__.preExperimentStateChanged, function(OriginalFunction,currentState,sDateTimeStamp) 
{	
	//*!Remove the first element and store the arguments in a seperate args array
	var args = [].slice.call(arguments);
	[].shift.apply(args);
	
	//ExperimentManager_NoState			= 0, /*!< this state is only internally used by the object while it is constructing, thus not yet fully available and therefore cannot be detected/used in the script environment. */
	//ExperimentManager_Constructed			= 1, /*!< this state is used to determine whenever the object has been constructed, this state also cannot be detected/used in the script environment. */
	//ExperimentManager_Loaded			= 2, /*!< after the Experiment file (*.exml) is successfully loaded (and validated) the ExperimentManager state changes to this state.*/
	//ExperimentManager_PreParsed			= 3, /*!< when the Experiment could be successfully preparsed (after loading) then the ExperimentManager state changes to this state.*/
	//ExperimentManager_Configured			= 4, /*!< after the function runExperiment() is called the ExperimentManager (validates if it has not been done before) and configures the experiment, it then changes to this state if the configuration was done.*/
	//ExperimentManager_Initialized			= 5, /*!< after the Experiment configuration the ExperimentManager tries to initialized and changes the state to this state if this step was successful.*/
	//ExperimentManager_IsStarting			= 6, /*!< after the Experiment initialization the ExperimentManager is ready to actually start the Experiment but first it changes the state to this state.*/
	//ExperimentManager_Started			= 7, /*!< after the Experiment is successfully started the ExperimentManager changes to this state.*/
	//ExperimentManager_IsStopping			= 8, /*!< before the ExperimentManager tries to stop the Experiment it first changes to this state.*/
	//ExperimentManager_Stopped			= 9  /*!< after the ExperimentManager could finalize and stop the Experiment it changes to this state.*/
	
	//*!Call the below BasicExperiment.LogFunctionSignature method with the args variabele
	BasicExperiment.LogFunctionSignature("RealTimeFeedback","preExperimentStateChanged", args, true);
	if(currentState == ExperimentManager.ExperimentState.ExperimentManager_Initialized)
	{
		try 
		{	
			RealTimeFeedback.TBVExchangerObject.executePreStep.connect(RealTimeFeedback, RealTimeFeedback.executePreStep);  
			RealTimeFeedback.TBVExchangerObject.executePostStep.connect(RealTimeFeedback, RealTimeFeedback.executePostStep);  
			RealTimeFeedback.TBVExchangerObject.executePostRun.connect(RealTimeFeedback, RealTimeFeedback.executePostRun);  
			RealTimeFeedback.TBVExchangerObject.disconnected.connect(RealTimeFeedback, RealTimeFeedback.disconnected);  
			RealTimeFeedback.TBVExchangerObject.connected.connect(RealTimeFeedback, RealTimeFeedback.connected);  
			RealTimeFeedback.TBVExchangerObject.connectionError.connect(RealTimeFeedback, RealTimeFeedback.connectionError);  	
		} 
		catch (e) 
		{
			StimulGL.write2OutputWindow(".*. Something went wrong connecting the Signal/Slots:" + e, "RealTimeFeedback");
		}			
	}	
	else if(currentState == ExperimentManager.ExperimentState.ExperimentManager_IsStopping)
	{	
		try 
		{	
			RealTimeFeedback.TBVExchangerObject.executePreStep.disconnect(RealTimeFeedback, RealTimeFeedback.executePreStep);  
			RealTimeFeedback.TBVExchangerObject.executePostStep.disconnect(RealTimeFeedback, RealTimeFeedback.executePostStep);  
			RealTimeFeedback.TBVExchangerObject.executePostRun.disconnect(RealTimeFeedback, RealTimeFeedback.executePostRun);  
			RealTimeFeedback.TBVExchangerObject.disconnected.disconnect(RealTimeFeedback, RealTimeFeedback.disconnected);  
			RealTimeFeedback.TBVExchangerObject.connected.disconnect(RealTimeFeedback, RealTimeFeedback.connected);  
			RealTimeFeedback.TBVExchangerObject.connectionError.disconnect(RealTimeFeedback, RealTimeFeedback.connectionError);  	
		} 
		catch (e) 
		{
			StimulGL.write2OutputWindow(".*. Something went wrong disconnecting the Signal/Slots:" + e, "RealTimeFeedback");
		}			
	}		
	
	//*!Call the original function (wrap method, with more that one arguments) and return
	var vResult = OriginalFunction.apply(this,args);
	return vResult;
})	

}( this.RealTimeFeedback = this.RealTimeFeedback || {} ));

