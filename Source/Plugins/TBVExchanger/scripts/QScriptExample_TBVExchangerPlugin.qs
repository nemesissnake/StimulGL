//Construct a new Plugin object
var TBVExchangerobject = new TBVExchanger();
var KeyBoardCaptureObj = new KeyBoardCapture();

//This function is called whenever the user presses a key
function KeyCaptureDetectFunction(keyCode)
{
	Log("A key press was detected: " + keyCode);
	//KeyPressedCounter++;
	//Log("KeyPressedCounter incremented to: " + KeyPressedCounter + "\n");
	//if (KeyPressedCounter==4)
	//{
	//	KeyBoardCaptureObj.StopCaptureThread();		
	//}
	if(keyCode == 27)
	{
		Log(TBVExchangerobject.deactivateAutoConnection());
		CleanupScript();//escape key
	}	
}

function CleanupScript() //Cleanup the script
{
	//Disconnect the signal/slots
	ConnectDisconnectScriptFunctions(false);
	//Set all functions and constructed objects to null
	executePreStep = null;
	executePostStep = null;
	executePostRun = null;
	disconnected = null;
	connected = null;
	connectionError = null;	
	ConnectDisconnectScriptFunctions = null;
	TBVExchangerobject = null;
	KeyBoardCaptureObj = null;
	KeyCaptureDetectFunction = null;
	CleanupScript = null;
	//Write something to the Log Output Pane so we know that this Function executed successfully.
	Log("Finished script cleanup, ready for garbage collection!");
	StimulGL.cleanupScript();
}

function ConnectDisconnectScriptFunctions(Connect)
//This function can connect or disconnect all signal/slot connections defined by the boolean parameter 
{
	if(Connect) //This parameter defines whether we should connect or disconnect the signal/slots.
	{
		Log("... Connecting Signal/Slots");
		try 
		{	
			KeyBoardCaptureObj.CaptureThreadKeyPressed.connect(this, this.KeyCaptureDetectFunction);
			TBVExchangerobject.executePreStep.connect(this, this.executePreStep);  
			TBVExchangerobject.executePostStep.connect(this, this.executePostStep);  
			TBVExchangerobject.executePostRun.connect(this, this.executePostRun);  
			TBVExchangerobject.disconnected.connect(this, this.disconnected);  
			TBVExchangerobject.connected.connect(this, this.connected);  
			TBVExchangerobject.connectionError.connect(this, this.connectionError);  
		} 
		catch (e) 
		{
			Log(".*. Something went wrong connecting the Signal/Slots:" + e); //If a connection fails warn the user!
		}		
	}
	else
	{
		Log("... Disconnecting Signal/Slots");
		try 
		{	
			KeyBoardCaptureObj.CaptureThreadKeyPressed.disconnect(this, this.KeyCaptureDetectFunction);
			TBVExchangerobject.executePreStep.disconnect(this, this.executePreStep);  
			TBVExchangerobject.executePostStep.disconnect(this, this.executePostStep);  
			TBVExchangerobject.executePostRun.disconnect(this, this.executePostRun);  
			TBVExchangerobject.disconnected.disconnect(this, this.disconnected);  
			TBVExchangerobject.connected.disconnect(this, this.connected);  
			TBVExchangerobject.connectionError.disconnect(this, this.connectionError);  
		} 
		catch (e) 
		{
			Log(".*. Something went wrong disconnecting the Signal/Slots:" + e); //If a disconnection fails warn the user!
		}		
	}	
}

function executePreStep()
{
	Log("executePreStep");
}

function executePostStep()
{
	Log("executePostStep");	
}

function executePostRun()
{
	Log("executePostRun");	
}

function disconnected()
{
	Log("disconnected");	
}

function connected()
{
	Log("connected");	
}

function connectionError(sError)
{
	Log("connectionError: " + sError);	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ConnectDisconnectScriptFunctions(true);

//Start the capture thread
KeyBoardCaptureObj.StartCaptureThread(0, false);
//StartCaptureThread(const short method, bool keyForwarding)
//method == 0 --> KeyPressed
//method == 1 --> KeyReleased
//method == 2 --> KeyPressedReleased

//	Basic Project Functions
//Log(TBVExchangerobject.tGetCurrentTimePoint());
//Log(TBVExchangerobject.tGetExpectedNrOfTimePoints());
//Log(TBVExchangerobject.tGetDimsOfFunctionalData());
//Log(TBVExchangerobject.tGetProjectName());
//Log(TBVExchangerobject.tGetWatchFolder());
//Log(TBVExchangerobject.tGetTargetFolder());
//Log(TBVExchangerobject.tGetFeedbackFolder());
//	Protocol, DM, GLM Functions:
//Log(TBVExchangerobject.tGetCurrentProtocolCondition());
//Log(TBVExchangerobject.tGetFullNrOfPredictors());
//Log(TBVExchangerobject.tGetCurrentNrOfPredictors());
//Log(TBVExchangerobject.tGetNrOfConfoundPredictors());
//Log(TBVExchangerobject.tGetValueOfDesignMatrix(1, 2));
//Log(TBVExchangerobject.tGetNrOfContrasts());
//	ROI Functions
//Log(TBVExchangerobject.tGetNrOfROIs());
//Log(TBVExchangerobject.tGetMeanOfROI(2));
//Log(TBVExchangerobject.tGetNrOfVoxelsOfROI(2));
//Log(TBVExchangerobject.tGetBetaOfROI(1,2));
//Log(TBVExchangerobject.tGetCoordsOfVoxelOfROI(1, 2));
//Log(TBVExchangerobject.tGetAllCoordsOfVoxelsOfROI(1));
//	Volume Data Access Functions
//Log(TBVExchangerobject.tGetValueOfVoxelAtTime(1,2,3,4));
//Log(TBVExchangerobject.tGetTimeCourseData(1));
//Log(TBVExchangerobject.tGetRawTimeCourseData(1));
//Log(TBVExchangerobject.tGetBetaOfVoxel(1,2,3,4));
//Log(TBVExchangerobject.tGetBetaMaps());
//Log(TBVExchangerobject.tGetMapValueOfVoxel(1,2,3,4));// {return tbvNetwIntFace->tGetMapValueOfVoxel(map, x, y, z);};
//Log(TBVExchangerobject.tGetContrastMaps());
Log(TBVExchangerobject.activateAutoConnection());
//Log(TBVExchangerobject.connectToServer("127.0.0.1",80));
//Log(TBVExchangerobject.disconnectFromServer());

//CleanupScript();