var KeyBoardCaptureObj = new KeyBoardCapture();
var objTCPNetworkServer1 = new TCPNetworkServer();
var objTCPNetworkServer2 = new TCPNetworkServer();
var ServerAddress;

function ServerDataAvailable(sData)
{
    Log("* ServerDataRecieved:" + sData);
}

function ClientDataAvailable(sData)
{
    Log("* ClientDataRecieved:" + sData);
}

function KeyCaptureDetectFunction(keyCode)
{
    Log("A key press was detected: " + keyCode);
    if(keyCode == 49)// 1
    {
        Log("1 pressed.");
        var retVal = objTCPNetworkServer1.startServer("","201");
        objTCPNetworkServer1.ClientDataAvailable.connect(this, this.ClientDataAvailable);
        Log("listen() result -> " + retVal);
        ServerAddress = retVal.split(":");
    }
    else if(keyCode == 50)// 2
    {
        Log("2 pressed.");
//		listen() result -> The server is running on
//	IP: 137.120.137.130
//	port: 56688
//Run the Client now.
        //Debug: "Network Server started @ 137.120.13.108:54855"  (mainwindow.cpp:468, void __thiscall MainWindow::setupNetworkServer(void))
        //Debug: "Network Server started @ 137.120.137.130:200"  (mainwindow.cpp:476, void __thiscall MainWindow::setupNetworkServer(const class QString &,unsigned short))
        //ServerAddress = new Array("137.120.137.130", "200");
        //ServerAddress = new Array("137.120.237.117", "200");
        ServerAddress = new Array("137.120.237.73", "200");
        if(ServerAddress.length > 1)
        {
            Log("connectToServer() result -> " + objTCPNetworkServer2.connectToServer(ServerAddress[0],ServerAddress[1]));
            objTCPNetworkServer2.ServerDataAvailable.connect(this, this.ServerDataAvailable);
            //KeyBoardCaptureObj.CaptureThreadKeyPressed.connect(this, this.KeyCaptureDetectFunction);
        }
        else
        {
            Log("No Server defined!");
        }
    }
    else if(keyCode == 51)// 3
    {
        Log("3 pressed.");//Client sends data to server
        Log("sendClientData() result -> " + objTCPNetworkServer2.sendClientData("Log('clientdata')"));
    }
    else if(keyCode == 52)// 4
    {
        Log("4 pressed.");//Client sends data to server
        Log("sendServerData() result -> " + objTCPNetworkServer1.sendServerData("serverdata"));
    }
    else if(keyCode == 27)//Escape
    {
        CleanupScript();
    }
    else if(keyCode == 87)//w
    {
        Log("w pressed.");//Client sends data to server
        Log("sendClientData() result -> " + objTCPNetworkServer2.sendClientData("changeStimuli(1,0)"));
    }
    else if(keyCode == 83)//s
    {
        Log("s pressed.");//Client sends data to server
        Log("sendClientData() result -> " + objTCPNetworkServer2.sendClientData("changeStimuli(2,0)"));
    }
}

function CleanupScript()
{
    KeyBoardCaptureObj.StopCaptureThread();
    ConnectDisconnectScriptFunctions(false);
    ServerDataAvailable = null;
    ClientDataAvailable = null;
    ConnectDisconnectScriptFunctions = null;
    objTCPNetworkServer1 = null;
    objTCPNetworkServer2 = null;
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
        }
        catch (e)
        {
            Log(".*. Something went wrong disconnecting the Signal/Slots:" + e);
        }
    }
}

ConnectDisconnectScriptFunctions(true);
KeyBoardCaptureObj.StartCaptureThread(0, false);

