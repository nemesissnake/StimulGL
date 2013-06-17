234var KeyBoardCaptureObj = new KeyBoardCapture();
var objTCPNetworkServer = new TCPNetworkServer();
var ServerAddress = new Array("127.0.0.1", "200");

ConnectDisconnectScriptFunctions(true);
Log("connectToServer() result -> " + objTCPNetworkServer.connectToServer(ServerAddress[0],ServerAddress[1]));
KeyBoardCaptureObj.StartCaptureThread(0, true);

function ServerDataAvailable(sData)
{
	Log("* ServerDataRecieved:" + sData);
}

function KeyCaptureDetectFunction(keyCode)
{
	Log("A key press was detected: " + keyCode); 
	if(keyCode == 27)//Escape
	{
		CleanupScript();
	}	
	else
	{
		Log("sendClientData() result -> " + objTCPNetworkServer.sendClientData("Log('The keycode from the key pressed at the client was: " + keyCode+ "')"));
	}
}

function CleanupScript()
{
	KeyBoardCaptureObj.StopCaptureThread();
	ConnectDisconnectScriptFunctions(false);
	ServerDataAvailable = null;
	ConnectDisconnectScriptFunctions = null;
	objTCPNetworkServer = null;
	ServerAddress = null;
	KeyBoardCaptureObj = null;
	KeyCaptureDetectFunction = null;
	Log("CleanupScript succeeded!");
	CleanupScript = null;
	StimulGL.cleanupScript();
}

function ConnectDisconnectScriptFunctions(Connect)
{
	if(Connect)
	{
		Log("... Connecting Signal/Slots");
		try 
		{	
			KeyBoardCaptureObj.CaptureThreadKeyPressed.connect(this, this.KeyCaptureDetectFunction);
			objTCPNetworkServer.ServerDataAvailable.connect(this, this.ServerDataAvailable);			
		} 
		catch (e) 
		{
			Log(".*. Something went wrong connecting the Signal/Slots:" + e);
		}		
	}
	else
	{
		Log("... Disconnecting Signal/Slots");
		try 
		{	
			KeyBoardCaptureObj.CaptureThreadKeyPressed.disconnect(this, this.KeyCaptureDetectFunction);
			objTCPNetworkServer.ServerDataAvailable.disconnect(this, this.ServerDataAvailable);
		} 
		catch (e) 
		{
			Log(".*. Something went wrong disconnecting the Signal/Slots:" + e);
		}		
	}	
}

