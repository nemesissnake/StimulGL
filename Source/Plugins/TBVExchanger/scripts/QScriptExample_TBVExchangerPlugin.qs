//Construct a new Plugin object
var TBVExchangerobject = new TBVExchanger();
var KeyBoardCaptureObj = new KeyBoardCapture();

//This function is called whenever the user presses a key
function KeyCaptureDetectFunction(keyCode)
{
	Log("A key press was detected: " + keyCode);
	if(keyCode == 27)//escape key
	{
		KeyBoardCaptureObj.StopCaptureThread();
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
KeyBoardCaptureObj.StartCaptureThread(0, true);
//if(TBVExchangerobject.connectToServer("127.0.0.1",55555) == false)
if(TBVExchangerobject.activateAutoConnection() == false)
	CleanupScript();