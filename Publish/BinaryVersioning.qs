var sScriptPath = StimulGL.getActiveDocumentFileLocation() + "/";
var tmpString = new String();
var sTimeObject = new Date();//gettime() returns Milliseconds since midnight January 1, 1970
//Log(sTimeObject.toTimeString());
//Log(sTimeObject.toTimeString().replace(/.*(\d{2}:\d{2}:\d{2}).*/, "$1"));//returns 08:55:57

function tr(s) { return s; }

function CreateTimeStamp(sTime)
{
	var sep = "";//".";
	return (sTime.getFullYear()-2000) + sep + sTime.getMonth() + sep + sTime.getDate() + sep + sTime.getHours() + sep + sTime.getMinutes() + sep + sTime.getSeconds();
}

function CreateArray(length) 
{
	var a = new Array(length || 0);
	if (arguments.length > 1) 
	{
		var args = Array.prototype.slice.call(arguments, 1);
		for (var i = 0; i < length; i++) 
		{
			a[i] = CreateArray.apply(this, args);
		}
	}
	return a;
}

QByteArray.prototype.toString = function()
{
	ts = new QTextStream( this, QIODevice.ReadOnly );
	return ts.readAll();
}

function quotedString(sValue)
{
	return "\"" + sValue + "\"";
}

function CreateNewContent(strTemplate,changeSetArray)
{	
	var arrLines = strTemplate.split(/\r?\n|\r/);
	Log("  - Number of lines: " + arrLines.length);
	var nStartIndex = 0;
	var sSearchval = "";
	for(var nChanges=0;nChanges<changeSetArray[0].length;nChanges++)//For all changes
	{
		sSearchval = changeSetArray[0][nChanges];
		for(var nLines=0;nLines<arrLines.length;nLines++)//For all lines
		{
			if (arrLines[nLines].indexOf(sSearchval, nStartIndex) != -1) 
			{
				Log("    - Value(\"" + sSearchval + "\") found @ line:" + nLines);
				arrLines[nLines] = changeSetArray[1][nChanges];
			}
		}
	}
	strTemplate = arrLines.join("\n");
	return strTemplate;
}

function ProcessInFiles(strTemplate, strDestination, changeSetArr)
{
	var templateFile = new QFile(strTemplate);
	templateFile.open(QIODevice.OpenMode(QIODevice.ReadOnly) || QIODevice.OpenMode(QIODevice.Text));
	var inStream = new QTextStream(templateFile);
	var inData = new String();
	//inData = inStream.readLine();
	inData = inStream.readAll();
	//Log(inData);
	templateFile.close();
	///////////////////////////////////////////////////////////////////////////////////////
	var outputFile = new QFile(strDestination);
	outputFile.open(QIODevice.OpenMode(QIODevice.WriteOnly) || QIODevice.OpenMode(QIODevice.Text));
	var outStream = new QTextStream(outputFile);
	inData = CreateNewContent(inData,changeSetArr);
	var outData = new QByteArray(inData);// or 'text'
	//Log(outData.GDCF_toString());
	outputFile.write(outData);
	outputFile.close();
	//ofile.rename("E:/workplace/file2.txt","E:/workplace/file1.txt");	
	templateFile = null;
	inStream = null;
	inData = null;
	outputFile = null;
	outStream = null;
	outData = null;
	return true;
}

function getFileName()
{
	var options = 0;
	//options |= QFileDialog.DontUseNativeDialog;
	var selectedFilter;
	//( parent, caption, dir, filter, selectedFilter, options)
	var fileName = QFileDialog.getOpenFileName(this,
				tr("QFileDialog::getOpenFileName()"),
				sScriptPath,
				tr("JavaScript Files (*.js)"),
				selectedFilter,
				QFileDialog.Options(options));
	if (fileName.length != 0)
		return fileName;
	else
		return "";
}

function CleanupScript()
{
	jsConfigurationFile = null;
	sTimeObject = null;
	CreateTimeStamp = null;
	tmpString = null;
	changeSet= null;
	ProcessInFiles = null;
	QByteArray.prototype.toString =null;
	CreateNewContent = null;
	CreateArray = null;
	sSourceList = null;
	getFileName = null;
	CleanupScript = null;
	tr = null;
	StimulGLInfo = null;
	quotedString = null;
	Log("\nScript ended");
	StimulGL.cleanupScript();
}

var sSourcePath = "E:/Projects/StimulGL/Source/";
var sDestPath = sSourcePath;
//var sDestPath = "D:/Projects/StimulGL/Publish/";
var sSourceList = new CreateArray();
var sDestList = new CreateArray();
sSourceList.push(sSourcePath + "StimulGL/maindefines.h");//"E:/Projects/StimulGL/Source/StimulGL/maindefines.h");
sDestList.push(sDestPath + "StimulGL/maindefines.h");//0
sSourceList.push(sSourcePath + "Plugins/ExperimentManager/defines.h");
sDestList.push(sDestPath + "Plugins/ExperimentManager/defines.h");//1
sSourceList.push(sSourcePath + "Plugins/KeyBoardDevice/defines.h");
sDestList.push(sDestPath + "Plugins/KeyBoardDevice/defines.h");//2
sSourceList.push(sSourcePath + "Plugins/ParallelPortDevice/defines.h");
sDestList.push(sDestPath + "Plugins/ParallelPortDevice/defines.h");//3
sSourceList.push(sSourcePath + "Plugins/SerialPortDevice/defines.h");
sDestList.push(sDestPath + "Plugins/SerialPortDevice/defines.h");//4
sSourceList.push(sSourcePath + "Plugins/USBHIDDevice/defines.h");
sDestList.push(sDestPath + "Plugins/USBHIDDevice/defines.h");//5
sSourceList.push(sSourcePath + "QmlExtensions/Plugins/DefaultPlugin/defines.h");
sDestList.push(sDestPath + "QmlExtensions/Plugins/DefaultPlugin/defines.h");//6
sSourceList.push(sSourcePath + "Plugins/FirebirdClient/defines.h");
sDestList.push(sDestPath + "Plugins/FirebirdClient/defines.h");//7
sSourceList.push(sSourcePath + "Plugins/PiezoStimDevice/defines.h");
sDestList.push(sDestPath + "Plugins/PiezoStimDevice/defines.h");//8
sSourceList.push(sSourcePath + "Plugins/TBVExchanger/defines.h");
sDestList.push(sDestPath + "Plugins/TBVExchanger/defines.h");//9
sSourceList.push(sSourcePath + "Plugins/TCPNetworkServer/defines.h");
sDestList.push(sDestPath + "Plugins/TCPNetworkServer/defines.h");//10

//StimulGL.openFiles("",sSourceList);

var nFiles = sSourceList.length;
var changeSet;
var sComment = "   //Please do not edit this line manually, see BinaryVersioning.qs (" + CreateTimeStamp(sTimeObject) + ").";
var bDoProcess = true;
var nChangeCounter;
var jsConfigurationFile = new QFile("E:/Projects/StimulGL/Source/StimulGL/resources/StimulGL.js");

Log("");
if(jsConfigurationFile.exists() == false)
{
	jsConfigurationFile.setFileName("D:/Projects/StimulGL/Source/StimulGL/resources/StimulGL.js");
	if(jsConfigurationFile.exists() == false)
	{
		jsConfigurationFile.setFileName(getFileName());
		if(jsConfigurationFile.exists() == false)
		{
			Log("Javascript configuration file not found.\n");
			bDoProcess = false;
		}
	}
}
Include(jsConfigurationFile.fileName());
//StimulGL.openFiles(jsConfigurationFile.fileName());
var StimulGLInfo = new StimulGL_Information();
var StimulGL_CurrentReleaseString_Index = StimulGLInfo.GetNumberOfReleases() - 1;
var StimulGL_LatestCompReleaseString_Index = "";
var tmpCurrentReleaseString = "";
var tmpCurrentReleaseString_Array = "";
var tmpMinimalXMLString = "";
var tmpMinimalXMLString_Array = "";
var tmpProductVersionString = "";
var tmpProductVersionString_Array = "";
var sComponentName = "";
var sComponentIndex = "";

if(bDoProcess)
{
	Log("Total " + nFiles + " file(s).");
	for(var nFileCounter=0;nFileCounter<nFiles;nFileCounter++)
	{
		changeSet = CreateArray(nFiles,2,0);
		nChangeCounter = 0;
		bDoProcess = false;
		if(nFileCounter == 0)//"StimulGL/maindefines.h"
		{
			tmpCurrentReleaseString = StimulGLInfo.GetCurrentRelease();
			tmpCurrentReleaseString_Array = tmpCurrentReleaseString.split(".");
			tmpProductVersionString = StimulGLInfo.GetReleaseProductVersionByIndex(StimulGL_CurrentReleaseString_Index);
			tmpProductVersionString_Array = tmpProductVersionString.split(".");
			
			changeSet[nFileCounter][0][nChangeCounter] = "#define MAIN_PROGRAM_FILE_VERSION_STRING_MAJOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[0] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define MAIN_PROGRAM_FILE_VERSION_STRING_MINOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[1] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define MAIN_PROGRAM_FILE_VERSION_STRING_REVISION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[2] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define MAIN_PROGRAM_FILE_VERSION_STRING_BUILD";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[3] + sComment;
			nChangeCounter++;			
			
			changeSet[nFileCounter][0][nChangeCounter] = "#define MAIN_PROGRAM_PRODUCT_VERSION_STRING_MAJOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[0] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define MAIN_PROGRAM_PRODUCT_VERSION_STRING_MINOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[1] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define MAIN_PROGRAM_PRODUCT_VERSION_STRING_REVISION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[2] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define MAIN_PROGRAM_PRODUCT_VERSION_STRING_BUILD";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[3] + sComment;
			nChangeCounter++;
			
			changeSet[nFileCounter][0][nChangeCounter] = "#define MAIN_PROGRAM_LEGAL_COPYRIGHT";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + quotedString(StimulGLInfo.GetMainAppCopyrightString()) + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define MAIN_PROGRAM_COMPANY_NAME";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + quotedString(StimulGLInfo.GetMainAppCompanyName()) + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define MAIN_PROGRAM_INTERNAL_NAME";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + quotedString(StimulGLInfo.GetMainAppInternalName()) + sComment;
			nChangeCounter++;			

			changeSet[nFileCounter][0][nChangeCounter] = "#define MAIN_PROGRAM_DEVICE_INTERFACE";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + quotedString(StimulGLInfo.GetMainAppPluginDeviceInterfaceString()) + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define MAIN_PROGRAM_EXTENSION_INTERFACE";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + quotedString(StimulGLInfo.GetMainAppPluginExtensionInterfaceString()) + sComment;
			nChangeCounter++;	
			bDoProcess = true;
		}
		else if(nFileCounter == 1)//"Plugins/ExperimentManager/defines.h"
		{
			sComponentName = "ExperimentManagerPlugin";
			StimulGL_LatestCompReleaseString_Index = StimulGLInfo.GetLatestComponentReleaseByName(sComponentName);
			sComponentIndex = StimulGLInfo.GetLatestComponentIndexByName(StimulGL_LatestCompReleaseString_Index,sComponentName);
			
			tmpCurrentReleaseString = StimulGLInfo.GetComponentVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpCurrentReleaseString_Array = tmpCurrentReleaseString.split(".");
			tmpProductVersionString = StimulGLInfo.GetComponentProductVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpProductVersionString_Array = tmpProductVersionString.split(".");
			tmpMinimalXMLString = StimulGLInfo.GetComponentMinimalEXMLVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpMinimalXMLString_Array = tmpMinimalXMLString.split(".");			
					
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_MAJOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[0] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_MINOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[1] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_REVISION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[2] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_BUILD";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[3] + sComment;
			nChangeCounter++;			
			
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_MAJOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[0] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_MINOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[1] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_REVISION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[2] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_BUILD";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[3] + sComment;
			nChangeCounter++;
			
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_EXMLDOC_MINIMAL_VERSION_MAJOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpMinimalXMLString_Array[0] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_EXMLDOC_MINIMAL_VERSION_MINOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpMinimalXMLString_Array[1] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_EXMLDOC_MINIMAL_VERSION_REVISION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpMinimalXMLString_Array[2] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_EXMLDOC_MINIMAL_VERSION_BUILD";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpMinimalXMLString_Array[3] + sComment;
			nChangeCounter++;			
			
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_EXMLDOC_VALIDATION_NAME";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + "PLUGIN_EXMLDOC_EXTENSION \"_" + tmpMinimalXMLString_Array.join('') + ".xsd\"" + sComment;//PLUGIN_EXMLDOC_EXTENSION "_2001.xsd"
			nChangeCounter++;

			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_MAIN_PROGRAM_MINIMAL_VERSION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + quotedString(StimulGLInfo.GetReleaseByIndex(StimulGL_LatestCompReleaseString_Index)) + sComment;
			nChangeCounter++;	
			bDoProcess = true;
		}
		else if(nFileCounter == 2)//"Plugins/KeyBoardDevice/defines.h"
		{
			sComponentName = "KeyBoardPlugin";
			StimulGL_LatestCompReleaseString_Index = StimulGLInfo.GetLatestComponentReleaseByName(sComponentName);
			sComponentIndex = StimulGLInfo.GetLatestComponentIndexByName(StimulGL_LatestCompReleaseString_Index,sComponentName);
			
			tmpCurrentReleaseString = StimulGLInfo.GetComponentVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpCurrentReleaseString_Array = tmpCurrentReleaseString.split(".");
			tmpProductVersionString = StimulGLInfo.GetComponentProductVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpProductVersionString_Array = tmpProductVersionString.split(".");
			tmpMinimalXMLString = StimulGLInfo.GetComponentMinimalEXMLVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpMinimalXMLString_Array = tmpMinimalXMLString.split(".");			
					
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_MAJOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[0] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_MINOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[1] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_REVISION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[2] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_BUILD";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[3] + sComment;
			nChangeCounter++;			
			
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_MAJOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[0] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_MINOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[1] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_REVISION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[2] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_BUILD";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[3] + sComment;
			nChangeCounter++;

			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_MAIN_PROGRAM_MINIMAL_VERSION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + quotedString(StimulGLInfo.GetReleaseByIndex(StimulGL_LatestCompReleaseString_Index)) + sComment;
			nChangeCounter++;
			bDoProcess = true;	
		}
		else if(nFileCounter == 3)//"Plugins/ParallelPortDevice/defines.h"
		{
			sComponentName = "ParallelPortPlugin";			
			StimulGL_LatestCompReleaseString_Index = StimulGLInfo.GetLatestComponentReleaseByName(sComponentName);
			sComponentIndex = StimulGLInfo.GetLatestComponentIndexByName(StimulGL_LatestCompReleaseString_Index,sComponentName);
			
			tmpCurrentReleaseString = StimulGLInfo.GetComponentVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpCurrentReleaseString_Array = tmpCurrentReleaseString.split(".");
			tmpProductVersionString = StimulGLInfo.GetComponentProductVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpProductVersionString_Array = tmpProductVersionString.split(".");
			tmpMinimalXMLString = StimulGLInfo.GetComponentMinimalEXMLVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpMinimalXMLString_Array = tmpMinimalXMLString.split(".");			
					
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_MAJOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[0] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_MINOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[1] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_REVISION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[2] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_BUILD";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[3] + sComment;
			nChangeCounter++;			
			
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_MAJOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[0] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_MINOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[1] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_REVISION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[2] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_BUILD";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[3] + sComment;
			nChangeCounter++;

			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_MAIN_PROGRAM_MINIMAL_VERSION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + quotedString(StimulGLInfo.GetReleaseByIndex(StimulGL_LatestCompReleaseString_Index)) + sComment;
			nChangeCounter++;
			bDoProcess = true;	
		}
		else if(nFileCounter == 4)//"Plugins/SerialPortDevice/defines.h"
		{
			sComponentName = "SerialPortPlugin";			
			StimulGL_LatestCompReleaseString_Index = StimulGLInfo.GetLatestComponentReleaseByName(sComponentName);
			sComponentIndex = StimulGLInfo.GetLatestComponentIndexByName(StimulGL_LatestCompReleaseString_Index,sComponentName);
			
			tmpCurrentReleaseString = StimulGLInfo.GetComponentVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpCurrentReleaseString_Array = tmpCurrentReleaseString.split(".");
			tmpProductVersionString = StimulGLInfo.GetComponentProductVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpProductVersionString_Array = tmpProductVersionString.split(".");
			tmpMinimalXMLString = StimulGLInfo.GetComponentMinimalEXMLVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpMinimalXMLString_Array = tmpMinimalXMLString.split(".");			
					
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_MAJOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[0] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_MINOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[1] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_REVISION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[2] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_BUILD";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[3] + sComment;
			nChangeCounter++;			
			
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_MAJOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[0] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_MINOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[1] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_REVISION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[2] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_BUILD";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[3] + sComment;
			nChangeCounter++;

			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_MAIN_PROGRAM_MINIMAL_VERSION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + quotedString(StimulGLInfo.GetReleaseByIndex(StimulGL_LatestCompReleaseString_Index)) + sComment;
			nChangeCounter++;
			bDoProcess = true;	
		}
		else if(nFileCounter == 5)//"Plugins/USBHIDDevice/defines.h"
		{
			sComponentName = "USBHIDDevicePlugin";			
			StimulGL_LatestCompReleaseString_Index = StimulGLInfo.GetLatestComponentReleaseByName(sComponentName);
			sComponentIndex = StimulGLInfo.GetLatestComponentIndexByName(StimulGL_LatestCompReleaseString_Index,sComponentName);
			
			tmpCurrentReleaseString = StimulGLInfo.GetComponentVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpCurrentReleaseString_Array = tmpCurrentReleaseString.split(".");
			tmpProductVersionString = StimulGLInfo.GetComponentProductVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpProductVersionString_Array = tmpProductVersionString.split(".");
			tmpMinimalXMLString = StimulGLInfo.GetComponentMinimalEXMLVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpMinimalXMLString_Array = tmpMinimalXMLString.split(".");			
					
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_MAJOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[0] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_MINOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[1] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_REVISION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[2] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_BUILD";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[3] + sComment;
			nChangeCounter++;			
			
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_MAJOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[0] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_MINOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[1] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_REVISION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[2] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_BUILD";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[3] + sComment;
			nChangeCounter++;

			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_MAIN_PROGRAM_MINIMAL_VERSION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + quotedString(StimulGLInfo.GetReleaseByIndex(StimulGL_LatestCompReleaseString_Index)) + sComment;
			nChangeCounter++;	
			bDoProcess = true;	
		}
		else if(nFileCounter == 6)//"QmlExtensions/Plugins/DefaultPlugin/defines.h"
		{
			sComponentName = "StimulGL_QMLExtensions";			
			StimulGL_LatestCompReleaseString_Index = StimulGLInfo.GetLatestComponentReleaseByName(sComponentName);
			sComponentIndex = StimulGLInfo.GetLatestComponentIndexByName(StimulGL_LatestCompReleaseString_Index,sComponentName);
			tmpCurrentReleaseString = StimulGLInfo.GetComponentVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpCurrentReleaseString_Array = tmpCurrentReleaseString.split(".");
			
			changeSet[nFileCounter][0][nChangeCounter] = "#define QML_VERSION_STRING_MAJOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[0] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define QML_VERSION_STRING_MINOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[1] + sComment;
			nChangeCounter++;
			
			sComponentName = "MediaPlayer";			
			StimulGL_LatestCompReleaseString_Index = StimulGLInfo.GetLatestComponentReleaseByName(sComponentName);
			sComponentIndex = StimulGLInfo.GetLatestComponentIndexByName(StimulGL_LatestCompReleaseString_Index,sComponentName);
			tmpCurrentReleaseString = StimulGLInfo.GetComponentVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);

			changeSet[nFileCounter][0][nChangeCounter] = "#define PHONONPLAYER_VERSION_STRING_REVISION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString + sComment;
						
			nChangeCounter++;
			bDoProcess = true;	
		}
		else if(nFileCounter == 7)//"Plugins/FirebirdClient/defines.h"
		{
			sComponentName = "FireBirdClientPlugin";			
			StimulGL_LatestCompReleaseString_Index = StimulGLInfo.GetLatestComponentReleaseByName(sComponentName);
			
			Log(StimulGL_LatestCompReleaseString_Index);
			
			sComponentIndex = StimulGLInfo.GetLatestComponentIndexByName(StimulGL_LatestCompReleaseString_Index,sComponentName);
			
			tmpCurrentReleaseString = StimulGLInfo.GetComponentVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpCurrentReleaseString_Array = tmpCurrentReleaseString.split(".");
			tmpProductVersionString = StimulGLInfo.GetComponentProductVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpProductVersionString_Array = tmpProductVersionString.split(".");
			tmpMinimalXMLString = StimulGLInfo.GetComponentMinimalEXMLVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpMinimalXMLString_Array = tmpMinimalXMLString.split(".");			
					
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_MAJOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[0] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_MINOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[1] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_REVISION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[2] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_BUILD";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[3] + sComment;
			nChangeCounter++;			
			
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_MAJOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[0] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_MINOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[1] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_REVISION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[2] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_BUILD";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[3] + sComment;
			nChangeCounter++;

			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_MAIN_PROGRAM_MINIMAL_VERSION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + quotedString(StimulGLInfo.GetReleaseByIndex(StimulGL_LatestCompReleaseString_Index)) + sComment;
			nChangeCounter++;	
			bDoProcess = true;		
		}
		else if(nFileCounter == 8)//"Plugins/PiezoStimDevice/defines.h"
		{
			sComponentName = "PiezoStimDevicePlugin";			
			StimulGL_LatestCompReleaseString_Index = StimulGLInfo.GetLatestComponentReleaseByName(sComponentName);
			sComponentIndex = StimulGLInfo.GetLatestComponentIndexByName(StimulGL_LatestCompReleaseString_Index,sComponentName);
			
			tmpCurrentReleaseString = StimulGLInfo.GetComponentVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpCurrentReleaseString_Array = tmpCurrentReleaseString.split(".");
			tmpProductVersionString = StimulGLInfo.GetComponentProductVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpProductVersionString_Array = tmpProductVersionString.split(".");
			tmpMinimalXMLString = StimulGLInfo.GetComponentMinimalEXMLVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpMinimalXMLString_Array = tmpMinimalXMLString.split(".");			
					
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_MAJOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[0] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_MINOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[1] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_REVISION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[2] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_BUILD";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[3] + sComment;
			nChangeCounter++;			
			
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_MAJOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[0] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_MINOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[1] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_REVISION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[2] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_BUILD";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[3] + sComment;
			nChangeCounter++;

			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_MAIN_PROGRAM_MINIMAL_VERSION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + quotedString(StimulGLInfo.GetReleaseByIndex(StimulGL_LatestCompReleaseString_Index)) + sComment;
			nChangeCounter++;	
			bDoProcess = true;	
		}		
		else if(nFileCounter == 9)//"Plugins/TBVExchanger/defines.h"
		{
			sComponentName = "TBVExchangerPlugin";			
			StimulGL_LatestCompReleaseString_Index = StimulGLInfo.GetLatestComponentReleaseByName(sComponentName);
			sComponentIndex = StimulGLInfo.GetLatestComponentIndexByName(StimulGL_LatestCompReleaseString_Index,sComponentName);
			
			tmpCurrentReleaseString = StimulGLInfo.GetComponentVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpCurrentReleaseString_Array = tmpCurrentReleaseString.split(".");
			tmpProductVersionString = StimulGLInfo.GetComponentProductVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpProductVersionString_Array = tmpProductVersionString.split(".");
			tmpMinimalXMLString = StimulGLInfo.GetComponentMinimalEXMLVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpMinimalXMLString_Array = tmpMinimalXMLString.split(".");			
					
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_MAJOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[0] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_MINOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[1] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_REVISION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[2] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_BUILD";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[3] + sComment;
			nChangeCounter++;			
			
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_MAJOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[0] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_MINOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[1] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_REVISION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[2] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_BUILD";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[3] + sComment;
			nChangeCounter++;

			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_MAIN_PROGRAM_MINIMAL_VERSION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + quotedString(StimulGLInfo.GetReleaseByIndex(StimulGL_LatestCompReleaseString_Index)) + sComment;
			nChangeCounter++;	
			bDoProcess = true;	
		}		
		else if(nFileCounter == 10)//"Plugins/TCPNetworkServer/defines.h"
		{
			sComponentName = "TCPNetworkServerPlugin";			
			StimulGL_LatestCompReleaseString_Index = StimulGLInfo.GetLatestComponentReleaseByName(sComponentName);
			sComponentIndex = StimulGLInfo.GetLatestComponentIndexByName(StimulGL_LatestCompReleaseString_Index,sComponentName);
			
			tmpCurrentReleaseString = StimulGLInfo.GetComponentVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpCurrentReleaseString_Array = tmpCurrentReleaseString.split(".");
			tmpProductVersionString = StimulGLInfo.GetComponentProductVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpProductVersionString_Array = tmpProductVersionString.split(".");
			tmpMinimalXMLString = StimulGLInfo.GetComponentMinimalEXMLVersionByIndexes(StimulGL_LatestCompReleaseString_Index,sComponentIndex);
			tmpMinimalXMLString_Array = tmpMinimalXMLString.split(".");			
					
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_MAJOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[0] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_MINOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[1] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_REVISION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[2] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_FILE_VERSION_STRING_BUILD";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpCurrentReleaseString_Array[3] + sComment;
			nChangeCounter++;			
			
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_MAJOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[0] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_MINOR";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[1] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_REVISION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[2] + sComment;
			nChangeCounter++;
			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_PRODUCT_VERSION_STRING_BUILD";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + tmpProductVersionString_Array[3] + sComment;
			nChangeCounter++;

			changeSet[nFileCounter][0][nChangeCounter] = "#define PLUGIN_MAIN_PROGRAM_MINIMAL_VERSION";
			changeSet[nFileCounter][1][nChangeCounter] = changeSet[nFileCounter][0][nChangeCounter] + "\t" + quotedString(StimulGLInfo.GetReleaseByIndex(StimulGL_LatestCompReleaseString_Index)) + sComment;
			nChangeCounter++;	
			bDoProcess = true;	
		}		
		else
		{		
			bDoProcess = false;			
		}
		
		if(bDoProcess)
		{
			Log("\n- File: " + sSourceList[nFileCounter]);
			ProcessInFiles(sSourceList[nFileCounter],sDestList[nFileCounter],changeSet[nFileCounter]);		
		}
	}
}
CleanupScript();
