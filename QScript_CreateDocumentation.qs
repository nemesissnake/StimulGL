function tr(s) { return s; }
//Construct a new Plugin object
//ExampleExtPluginObj = new _extensionname_();

//Example usage of the Example property
//ExampleExtPluginObj.setExampleProperty(99);
//Log(ExampleExtPluginObj.ExampleProperty);
//ExampleExtPluginObj.ExampleProperty = 88;
//Log(ExampleExtPluginObj.ExampleProperty);

//There are some global standard Script Functions which can be used like Log(), Beep(), ThrowError(), Include(), Pause(), ExitScript() and StimQT()
//Beep();

//Almost all ECMA(Java)-script compatible code can be used here, see below String
//var txt = new String("ECMA string");

//Almost all QT SDK libraries can be used trough automatically loaded script bindings
//We can even tell the main program to open an QT script binding example
//var strPath = new String(StimulGL.getSelectedScriptFileLocation());
//strPath = strPath + "/" + "StandardDialogs.qs"
//StimulGL.write2OutputWindow(strPath);
//StimulGL.openFiles(strPath);
//StimulGL.executeScript();
//StimulGL.write2OutputWindow(StimulGL.getSelectedScriptFileName());
//StimulGL.closeSelectedScriptFile();

//Define a custom function
//function myBeeps(n) 
//{ 
//	for(var i=0 ;i<n;i++)
//	{
//		Beep();
//		txt = txt + i;
//		Log(txt);
//	}
//} 
//Call the newly created function
//myBeeps(5);
//Make sure to cleanup your functions before exiting!
//myBeeps = null;



//var fileName = QFileDialog.getOpenFileName(this, tr("Open a File"), QDir.currentPath(), tr("txt Files (*.txt *.bat)"));
var fileName = "D:\\Projects\\StimulGL documentation\\test.txt";
if (fileName != "")
{
	
	var file = new QFile(fileName);
	if (!file.open(QIODevice.OpenMode(QIODevice.ReadOnly, QIODevice.Text))) 
	{
		QMessageBox.warning(this, tr("Open a File"),
			tr("Cannot read file %1:\n%2.")
			.arg(fileName)
			.arg(file.errorString()));
	}	
	else
	{
		Log("file is opened!");
//		var inStream = new QTextStream();
//		inStream(file);
//		var line = new QString();
//		line = inStream.readLine();
//		while (!line.isNull()) 
//		{
//			Log(line);
//			line = inStream.readLine();
//		}
		
		
		var strLine = new String;
		while (!file.atEnd()) 
		{
			var baLine = new QByteArray();
			baLine = file.readLine();
			strLine = baLine.at(1);
			Log(baLine.count(baLine) + ", " + baLine.size() + ", " + strLine);
			baLine = null;
		}
		file.close();
		//inStream = null;
		//line = null;
		strLine = null;
	}
	file = null;
}

	
//Signal/Slot functionality
var timer = new QTimer(this);
var nCounter = 0;
function cleanup() //First we define a function for the necessary cleanup before exiting the script
{
	timer.stop(); //Stop the timer
	timer.timeout.disconnect(this, this.myFunction1); //Disconnect again
	myFunction1 = null; //cleanup the function
	//ExampleExtPluginObj.ExampleSignalTriggered.disconnect(this, this.myFunction2); //Disconnect again
	//myFunction2 = null; //Cleanup the function
	cleanup = null; //Cleanup the function
	tr = null; //Cleanup the function
}
function myFunction1() //The first function demonstrates how a QTimer constructed from within the script can be connected to a custom defined script function
{ 
	nCounter++;
	Log("myFunction1() has been called " + nCounter + " times" ) 
	//ExampleExtPluginObj.setExampleProperty(nCounter);
	if (nCounter==5)
	{
		cleanup();
		StimulGL.clearOutputWindow();
		StimulGL.write2OutputWindow("done...");
	}
}
//function myFunction2() //The second function demonstrates how a Plugin constructed from within the script can be connected to a custom defined script function
//{ 
//	Log("myFunction2() has been called" ) 
//}
//function PauseMills(millis)
//{
//	var date = new Date();
//	var curDate = null;
     
//	do { curDate = new Date(); }
//	while(curDate-date < millis)
//}

timer.timeout.connect(this, this.myFunction1); //The same as: connect(timer, SIGNAL(timeout()), this, SLOT(myFunction()));
//ExampleExtPluginObj.ExampleSignalTriggered.connect(this, this.myFunction2); //Connect a Signal from the Plugin object dynamically to a custom script Function
//timer.start(1000);

//copyFile("http://www.gemini.edu/images/stories/press_release/pr2008-6/fig1.jpg","D:\\Projects\\StimulGL documentation\\Downloaded\\fig1.jpg");

var i = 0;
var nTotalItems = 0;
strPath = "D:\\Projects\\StimulGL documentation\\Downloaded\\index.html";//ParallelPortDevicePlugin\\ParallelPortDevicePlugin.html";
StimulGL.openFiles(strPath);
//StimulGL.find();//Shows only the dialog
var findFlags = new DocFindFlags;
//findFlags.replace = false;//do not use this in the script, instead use the proper find or replace function instead!
findFlags.matchCase = true;
findFlags.backwards = false;
findFlags.isRegExp = true;
findFlags.wholeWords = false;
findFlags.multiLine = false;

var GoogleDocIDs;
GoogleDocIDs =  new Array(
"1zetAgALlRcb8GoOeGFne8R_91tkebtPMsA9jOIlEMxw",//href="[^"]{1,99}1zetAgALlRcb8GoOeGFne8R_91tkebtPMsA9jOIlEMxw"
"1g6aWTPp809ZIktUzfV3UN4QeyBAoSAgEm0SFuuQVg5M",
"1DXVkg8VezdsG2HHGeQvsNmMFEBqLwtEV5wPh0Ro13kQ",
"1ZUchDkHsMf3fb-oSjp_YKFUwatknYtnc1FMOK3NCWtk",
"1g6aWTPp809ZIktUzfV3UN4QeyBAoSAgEm0SFuuQVg5M",
"19h-paHzUYMd5PibM1NQHQ4GfyxfChZCqIlSofoSLJzM"
);

var LocalPaths;
LocalPaths =  new Array(
"index.html",
"ParallelPortDevicePlugin\ParallelPortDevicePlugin.html",
"ExperimentManagerPlugin\ExperimentManagerPlugin.html",
"ExtensionPluginTemplate\ExtensionPluginTemplate.html",
"ParallelPortDevicePlugin\ParallelPortDevicePlugin.html",
"USBHIDDevicePlugin\USBHIDDevicePlugin.html"
);

nTotalItems = GoogleDocIDs.length;
for(i=0;i<nTotalItems;i++)
{

//	void find(bool useParams = false, QString strFindString = "", DocFindFlags findFlags = _DocFindFlags());
//	void replace(bool bReplaceAll = false, bool useParams = false, QString strFindString = "", QString strReplaceString = "", DocFindFlags findFlags = _DocFindFlags());
//StimulGL.find(true,"margi",findFlags);
//Below only counts for the first equal level location
StimulGL.replace(true,true,"href=\"[^\"]{1,99}" + GoogleDocIDs[i] + "\"","href=\"..\\" + LocalPaths[i] + "\"",findFlags);

	
	
}	

//StimulGL.replace(true,true,"margina","margin",findFlags);
//StimulGL.closeSelectedScriptFile(true);//closeSelectedScriptFile(bool bAutoSaveChanges)
//StimulGL.replace();//Shows only the dialog
StimulGL.write2OutputWindow(StimulGL.getSelectedScriptFileName());
//StimulGL.closeSelectedScriptFile();
cleanup();
//PauseMills(8000);//This function doesn't block the Timer thread, but has a high CPU load, it's better to use Timers/Signal/Slots
//Pause(5000);//This function does block the remaining!
//PauseMills = null;
//Log("Ready");
//ThrowError("Fake Error");//Interrupts the script at this line and opens the script parser
//The ExampleExtPluginObj object gets automatically destructed
