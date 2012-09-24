var scriptRootPath = StimulGL.getSelectedScriptFileLocation();
var qsFilePath = scriptRootPath + "/" + "08_FFA_PPA_LOC_ImageArrays.qs";
var exmlFilePath = scriptRootPath + "/" + "08_FFA_PPA_LOC.exml";

Include(qsFilePath);

var ExperimentManagerObj = new ExperimentManager();
var arg;
var nTriggerCounter = 0;
var tmpColor = new QColor ("#ffff00");
var tmpPixMap = new QPixmap(100,100);
var tmpString = new String();
var retVal;
var functionName = new String();
var MainItem = new String();
var Param1

function CleanupScript()
{
	ConnectDisconnectScriptFunctions(false);
	ExperimentManagerObj = null;
	ExperimentStateChanged = null;
	NewInitBlockTrial = null;
	ExternalTriggerIncremented = null;
	ExperimentStructureChanged = null;
	ConnectDisconnectScriptFunctions = null;
	CleanupScript = null;
	preCacheImages = null;
	preProcessImages = null;
	tmpPixMap = null;
	tmpString = null;
	tmpColor = null;
	StimulGL.cleanupScript();
}

function NewInitBlockTrial()
{
	Log("--- Starting to initialize a new Block Trial (" + arguments.length + " argument(s))");
}

function ExternalTriggerIncremented()
{
	Log("--- The External Trigger is incremented (" + arguments.length + " argument(s): " + arguments[0] +")");	
}

function preCacheImages()
{
	functionName = "setImageFunction";
	MainItem = "functions";
	for(var i=0;i<ImagePathArray.length;i++)
	{
		Param1 = ImagePathArray[i];
		retVal = qmlWidget_Object_1.invokeQmlMethod(MainItem,functionName,Param1);
		//Log("<<< " + functionName + "(" + Param1 + ") returned: " + retVal + " >>>");	
	}
	Log("... pre-Caching images done(" + ImagePathArray.length + " item(s) cached)!");
}

function preProcessImages()
{
	Log("... pre-Processing images");
//	var retVal;
//	var MainItem = "functions";
//	var Param1
	//for(var i=0;i<ImagePathArray.length;i++)
	//{
		//tmpPixMap.
		//var tmpPixMap = new QPixmap(101,99);
		//tmpPixMap.fill(tmpColor);
	var currLenght = 10;//ImagePathArray.length;
	for(var i=0;i<currLenght;i++)
	{		
		tmpString = ImagePathArray[i];
		if(qmlWidget_Object_1.getPixmapFromImageBuffer(tmpPixMap, tmpString))
		{
			//tmpPixMap.fill(tmpColor);
			tmpPixMap = tmpPixMap.scaledToWidth(100);
			Log("... getPixmapFromImageBuffer done!");	
			Log("... addPixmapToImageBuffer returned: " + qmlWidget_Object_1.addPixmapToImageBuffer(tmpPixMap));
			if(i<5)
			{
				Log("... removePixmapFromImageBuffer returned: " + qmlWidget_Object_1.removePixmapFromImageBuffer(i));
			}
			else
			{
				if(qmlWidget_Object_1.getPixmapFromImageBuffer(tmpPixMap, ImagePathArray[4]))
					Log("... updatePixmapFromImageBuffer returned: " + qmlWidget_Object_1.updatePixmapFromImageBuffer(tmpPixMap,i));
			
			}
		}
	}
	Log("... pre-Processing images done(" + ImagePathArray.length + " item(s) processed)!");	
}

function ExperimentStructureChanged()
{
	var nCurrentBlock = arguments[0];
	var nCurrentTrial = arguments[1];
	var nCurrentTrigger = arguments[2];
	var textToShow = "Block " + nCurrentBlock + ", Trial " + nCurrentTrial + ", Trigger " + nCurrentTrigger;
	Log("--- The Experiment Structure Changed to : " + textToShow);

	try 
	{		
		if((nCurrentBlock == 0))
		{
			functionName = "setImageFunction";
			MainItem = "functions";
			Param1 = ImagePathArray[nCurrentTrigger];
			retVal = qmlWidget_Object_1.invokeQmlMethod(MainItem,functionName,Param1);
			Log("    <<< " + functionName + "(" + Param1 + ") returned: " + retVal + " >>>");			
//			functionName = "myQmlTextFunction";
//			retVal = qmlWidget_Object_1.invokeQmlMethod(MainItem,functionName,Param1);
//			Log("    <<< " + functionName + " returned: " + retVal + " >>>");
			MainItem = "DebugMode_Functions";
			functionName = "updateExperimentState";
			retVal = qmlWidget_Object_1.invokeQmlMethod(MainItem,functionName,nCurrentBlock,nCurrentTrial,nCurrentTrigger);
			Log("    <<< " + functionName + " returned: " + retVal + " >>>");
		}
		else if(nCurrentTrial == 1)
		{
			ExperimentManagerObj.abortExperiment();
		}
	} 
	catch (e) 
	{
		Log(".*. Something went wrong invoking the slot:" + e);
	}
	nTriggerCounter++;
}

function ExperimentStateChanged()
{
	//ExperimentManager_NoState	= 0,	This signal can't be catched in this script
	//ExperimentManager_Constructed	= 1, 	This signal can't be catched in this script
	//ExperimentManager_Loaded	= 2, 
	//ExperimentManager_Configured	= 3, 
	//ExperimentManager_Initialized	= 4, 	
	//ExperimentManager_IsStarting	= 5, 
	//ExperimentManager_Started	= 6, 
	//ExperimentManager_IsStopping	= 7, 
	//ExperimentManager_Stopped	= 8  
	
	var textToShow = "--- The Experiment State Changed to: ";
	
	if(arguments[0] == 0)
	{
		Log(textToShow + "ExperimentManager_NoState");
	}
	else if(arguments[0] == 1)
	{
		Log(textToShow + "ExperimentManager_Constructed");
	}
	else if(arguments[0] == 2)
	{
		Log(textToShow + "ExperimentManager_Loaded");
	}
	else if(arguments[0] == 3)
	{
		Log(textToShow + "ExperimentManager_Configured");
	}
	else if(arguments[0] == 4)
	{
		Log(textToShow + "ExperimentManager_Initialized");
		//Now all defined objects in the experiment file are constructed and therefore available in this script, so now we can make the connections between constructed the objects.
		ConnectDisconnectScriptFunctions(true);		
	}	
	else if(arguments[0] == 5)
	{	
		Log(textToShow + "ExperimentManager_IsStarting");
	}
	else if(arguments[0] == 6)
	{	
		Log(textToShow + "ExperimentManager_Started");
		preCacheImages();
		preProcessImages();
	}
	else if(arguments[0] == 7)
	{	
		Log(textToShow + "ExperimentManager_IsStopping");
	}	
	else if(arguments[0] == 8)
	{
		Log(textToShow + "ExperimentManager_Stopped");
		CleanupScript();
	}
}

function ConnectDisconnectScriptFunctions(Connect)
{
	if(Connect)
	{
		Log("... Connecting Signal/Slots");
		try 
		{	
			qmlWidget_Object_1.NewInitBlockTrial.connect(this, this.NewInitBlockTrial);
			qmlWidget_Object_1.ExternalTriggerIncremented.connect(this, this.ExternalTriggerIncremented);
			qmlWidget_Object_1.ExperimentStructureChanged.connect(this, this.ExperimentStructureChanged);
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
			ExperimentManagerObj.ExperimentStateHasChanged.disconnect(this, this.ExperimentStateChanged);
			qmlWidget_Object_1.NewInitBlockTrial.disconnect(this, this.NewInitBlockTrial);
			qmlWidget_Object_1.ExternalTriggerIncremented.disconnect(this, this.ExternalTriggerIncremented);
			qmlWidget_Object_1.ExperimentStructureChanged.disconnect(this, this.ExperimentStructureChanged);
		} 
		catch (e) 
		{
			Log(".*. Something went wrong disconnecting the Signal/Slots:" + e);
		}		
	}	
}

ExperimentManagerObj.ExperimentStateHasChanged.connect(this, this.ExperimentStateChanged);
if(ExperimentManagerObj.setExperimentFileName(exmlFilePath))
{
	ExperimentManagerObj.runExperiment();
}
else
{
	Log("*** Error: Could not locate the experiment file!");
}
