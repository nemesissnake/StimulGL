(function( RealTimeVisualFeedback, undefined )
{ 
//Public Property Variabeles
	RealTimeVisualFeedback.TBVExchangerObject = new TBVExchanger();//Construct a new Plugin object
	
//Public Property Functions
	RealTimeVisualFeedback.__proto__.executePreStep = function executePreStep(nTimePoint)
	{
		Log("RealTimeVisualFeedback.executePreStep");
	}

	RealTimeVisualFeedback.__proto__.executePostStep = function executePostStep(nTimePoint)
	{
		Log("RealTimeVisualFeedback.executePostStep");
	}
	
	RealTimeVisualFeedback.__proto__.executePostRun = function executePostRun(nTimePoint)
	{
		Log("RealTimeVisualFeedback.executePostRun: " + nTimePoint);	
	}

	RealTimeVisualFeedback.__proto__.disconnected = function disconnected()
	{
		Log("RealTimeVisualFeedback.disconnected");	
	}

	RealTimeVisualFeedback.__proto__.connected = function connected()
	{
		Log("RealTimeVisualFeedback.connected");	
	}

	RealTimeVisualFeedback.__proto__.connectionError = function connectionError(sError)
	{
		Log("RealTimeVisualFeedback.connectionError: " + sError);	
	}
	
	preExperimentStateChanged = _.compose(preExperimentStateChanged, function(currentState) 
	{
		if(currentState == ExperimentManager.ExperimentState.ExperimentManager_Initialized)
		{
			try 
			{	
				RealTimeVisualFeedback.TBVExchangerObject.executePreStep.connect(RealTimeVisualFeedback, RealTimeVisualFeedback.executePreStep);  
				RealTimeVisualFeedback.TBVExchangerObject.executePostStep.connect(RealTimeVisualFeedback, RealTimeVisualFeedback.executePostStep);  
				RealTimeVisualFeedback.TBVExchangerObject.executePostRun.connect(RealTimeVisualFeedback, RealTimeVisualFeedback.executePostRun);  
				RealTimeVisualFeedback.TBVExchangerObject.disconnected.connect(RealTimeVisualFeedback, RealTimeVisualFeedback.disconnected);  
				RealTimeVisualFeedback.TBVExchangerObject.connected.connect(RealTimeVisualFeedback, RealTimeVisualFeedback.connected);  
				RealTimeVisualFeedback.TBVExchangerObject.connectionError.connect(RealTimeVisualFeedback, RealTimeVisualFeedback.connectionError);  	
			} 
			catch (e) 
			{
				Log(".*. RealTimeVisualFeedback::Something went wrong connecting the Signal/Slots:" + e);
			}			
		}	
		else if(currentState == ExperimentManager.ExperimentState.ExperimentManager_IsStopping)
		{	
			try 
			{	
				RealTimeVisualFeedback.TBVExchangerObject.executePreStep.disconnect(RealTimeVisualFeedback, RealTimeVisualFeedback.executePreStep);  
				RealTimeVisualFeedback.TBVExchangerObject.executePostStep.disconnect(RealTimeVisualFeedback, RealTimeVisualFeedback.executePostStep);  
				RealTimeVisualFeedback.TBVExchangerObject.executePostRun.disconnect(RealTimeVisualFeedback, RealTimeVisualFeedback.executePostRun);  
				RealTimeVisualFeedback.TBVExchangerObject.disconnected.disconnect(RealTimeVisualFeedback, RealTimeVisualFeedback.disconnected);  
				RealTimeVisualFeedback.TBVExchangerObject.connected.disconnect(RealTimeVisualFeedback, RealTimeVisualFeedback.connected);  
				RealTimeVisualFeedback.TBVExchangerObject.connectionError.disconnect(RealTimeVisualFeedback, RealTimeVisualFeedback.connectionError);  	
			} 
			catch (e) 
			{
				Log(".*. RealTimeVisualFeedback::Something went wrong connecting the Signal/Slots:" + e);
			}			
		}	
		return arguments;
	});	

}( this.RealTimeVisualFeedback = this.RealTimeVisualFeedback || {} ));

