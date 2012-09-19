//Construct a new Plugin object
ExampleDevicePluginObj = new SerialPortDevice();
var s = new String();
var n;
var retVal = false;
var nTriggerCounter = 0;

function myFinalCleanup()//Cleanup
{
	ExampleDevicePluginObj.CaptureThreadReadyRead.disconnect(this, this.mySignalFunction);//readyRead
	ExampleDevicePluginObj.close();	
	s = null;
	ExampleDevicePluginObj = null;
	mySignalFunction = null;
	myFinalCleanup = null;
	Log("Finished script Cleanup!");
	StimulGL.cleanupScript();
}

function mySignalFunction()
{
	Log("mySignalFunction arguments count: " + arguments.length);
	for (var i = 0; i < arguments.length; ++i)
	Log("mySignalFunction first argument: " + arguments[i]); 	
	nTriggerCounter++;
	Log("nTriggerCounter = " + nTriggerCounter);
	if (nTriggerCounter==5)
	{
		myFinalCleanup();
	}
}

Log("----------------------------setPortName");
s="Port99"
ExampleDevicePluginObj.setPortName(s);
Log(ExampleDevicePluginObj.portName());
s="COM5";
ExampleDevicePluginObj.setPortName(s);
Log(ExampleDevicePluginObj.portName());

Log("----------------------------setBaudRate");
s="BAUD38400";
ExampleDevicePluginObj.setBaudRate(s);
Log(ExampleDevicePluginObj.baudRateToString());
s="BAUD19200";
ExampleDevicePluginObj.setBaudRate(s);
Log(ExampleDevicePluginObj.baudRateToString());

Log("----------------------------setDataBits");
s="DATA_7";
ExampleDevicePluginObj.setDataBits(s);
Log(ExampleDevicePluginObj.dataBitsToString());
s="DATA_8";
ExampleDevicePluginObj.setDataBits(s);
Log(ExampleDevicePluginObj.dataBitsToString());

Log("----------------------------setParity");
s="PAR_ODD";
ExampleDevicePluginObj.setParity(s);
Log(ExampleDevicePluginObj.parityToString());
s="PAR_NONE";
ExampleDevicePluginObj.setParity(s);
Log(ExampleDevicePluginObj.parityToString());

Log("----------------------------setFlowControl");
s="FLOW_HARDWARE";
ExampleDevicePluginObj.setFlowControl(s);
Log(ExampleDevicePluginObj.flowControlToString());
s="FLOW_OFF";
ExampleDevicePluginObj.setFlowControl(s);
Log(ExampleDevicePluginObj.flowControlToString());

Log("----------------------------setStopBits");
s="STOP_2";
ExampleDevicePluginObj.setStopBits(s);
Log(ExampleDevicePluginObj.stopBitsToString());
s="STOP_1";
ExampleDevicePluginObj.setStopBits(s);
Log(ExampleDevicePluginObj.stopBitsToString());

Log("----------------------------setTimeout");
n=10;
ExampleDevicePluginObj.setTimeout(n);

Log("----------------------------setQueryMode");
s="Polling";
ExampleDevicePluginObj.setQueryMode(s);
Log(ExampleDevicePluginObj.queryModeToString());
s="EventDriven";
ExampleDevicePluginObj.setQueryMode(s);
Log(ExampleDevicePluginObj.queryModeToString());

Log("----------------------------connect");
ExampleDevicePluginObj.CaptureThreadReadyRead.connect(this, this.mySignalFunction);
//ExampleDevicePluginObj.CaptureThreadReadyRead.connect(this, ExampleDevicePluginObj.test);
//ExampleDevicePluginObj.readyRead.connect(this, ExampleDevicePluginObj.test);

Log("----------------------------open/close");
s="ReadOnly | WriteOnly";
retVal = ExampleDevicePluginObj.open(s);
Log(ExampleDevicePluginObj.openModeToString());
Log(ExampleDevicePluginObj.isOpen());
if(retVal == false)
	myFinalCleanup();
