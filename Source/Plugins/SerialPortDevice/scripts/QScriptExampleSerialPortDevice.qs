//Construct a new Plugin object
var tTimer = new TriggerTimer();
var nMaxIdleTime = 2000;

//SerialDataReceived
var serialPortObject = new SerialPortDevice();

Log(serialPortObject.setPortName("COM6"));
serialPortObject.SerialDataReceived.connect(this, this.myDataReceivedFunction);
Log(serialPortObject.open(1));
//        NotOpen = 0x0000,
//        ReadOnly = 0x0001,
//        WriteOnly = 0x0002,
//        ReadWrite = ReadOnly | WriteOnly,
//        Append = 0x0004,
//        Truncate = 0x0008,
//        Text = 0x0010,
//        Unbuffered = 0x0020
Log(serialPortObject.portName());
Log(serialPortObject.setBaudRate(19200));
Log(serialPortObject.baudRate());
Log(serialPortObject.setFlowControl(0));//NoFlowControl = 0
Log(serialPortObject.flowControl());
Log(serialPortObject.setParity(0));//QSerialPort::NoParity));//NoParity = 0
Log(serialPortObject.parity());
Log(serialPortObject.setDataBits(8));//Data8 = 8
Log(serialPortObject.dataBits());
Log(serialPortObject.setStopBits(1));//OneStop = 1
Log(serialPortObject.stopBits());

tTimer.startTimer(nMaxIdleTime);
tTimer.timeout.connect(this, myFinalCleanup); 

function myDataReceivedFunction()
{
	Log("myDataReceivedFunction arguments count: " + arguments.length);
	for (var i = 0; i < arguments.length; ++i)
	{
		Log("myDataReceivedFunction argument(" + i + "): " + arguments[i]);
	}
	tTimer.startTimer(nMaxIdleTime);
}

function myFinalCleanup()//Cleanup
{
	serialPortObject.SerialDataReceived.disconnect(this, this.myDataReceivedFunction);
	tTimer.timeout.disconnect(this, myFinalCleanup);
	tTimer.stopTimer();
	serialPortObject.close();
	tTimer = null;
	myDataReceivedFunction = null;
	serialPortObject = null;
	myFinalCleanup = null;
	Log("Finished script Cleanup!");
	StimulGL.cleanupScript();
}