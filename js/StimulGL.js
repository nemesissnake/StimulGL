// JavaScript Document
//
function createArray(length) 
{
	//createArray();     // [] or new Array()
	//createArray(2);    // new Array(2)
	//createArray(3, 2); // [new Array(2),
                    	 //  new Array(2),
                    	 //  new Array(2)]
    var a = new Array(length || 0);
    if (arguments.length > 1) 
	{
        var args = Array.prototype.slice.call(arguments, 1);
        for (var i = 0; i < length; i++) 
		{
            a[i] = createArray.apply(this, args);
        }
    }
    return a;
}

///////////////////////////////////////////////////////////
window.StimulGLInfo = new StimulGL();
function StimulGL()
{
	//alert('StimulGL constructor');
	this.StimulGLReleases = [['2.0.0.2','4.8.0'],['2.0.0.3','4.8.0'],['2.0.0.4','5.0.0']];
	var PropCounter = 0;
	var AuthorCounter = 0;
	var ComponentCounter = 0;
	var tmpArray;
	var nNumberOfComponentChanges = 0;
	
	this.Doc_FileName_Index = AuthorCounter++;
	this.Doc_Title_Index = AuthorCounter++;
	this.Doc_Version_Index = AuthorCounter++;
	this.Doc_Date_Index = AuthorCounter++;
	this.Doc_Authors_Index = AuthorCounter++;
	
	this.Name_Index = PropCounter++;
	this.Version_Index = PropCounter++;
	this._StimulGLVersion_Index = PropCounter++;
	this.EXML_Index = PropCounter++;
	this.DevInterface_Index = PropCounter++;
	this.ExtInterface_Index = PropCounter++;
	
	var ExperimentManagerPlugin_Comp_Index = ComponentCounter++;
	var ParallelPortPlugin_Comp_Index = ComponentCounter++;
	var SerialPortPlugin_Comp_Index = ComponentCounter++;
	var KeyBoardPlugin_Comp_Index = ComponentCounter++;
	var USBHIDDevicePlugin_Comp_Index = ComponentCounter++;
	var _extensionname_plugin_Comp_Index = ComponentCounter++;
	var DefaultQMLPlugin_Comp_Index = ComponentCounter++;
	var	StimulGLExtensions_Comp_Index = ComponentCounter++;	
	var MediaPlayer_Comp_Index = ComponentCounter++;
	
	
	this.componentVersioning = createArray(1,ComponentCounter,PropCounter);//Default constructor!
	//StimulGL version (2.0.0.2)
	///////////////////////////////////////////////////////////////
	this.componentVersioning[0][ExperimentManagerPlugin_Comp_Index][this.Name_Index] = 'ExperimentManagerPlugin';
	this.componentVersioning[0][ExperimentManagerPlugin_Comp_Index][this.Version_Index] = '2.0.0.2';
	this.componentVersioning[0][ExperimentManagerPlugin_Comp_Index][this._StimulGLVersion_Index] = 0;
	this.componentVersioning[0][ExperimentManagerPlugin_Comp_Index][this.EXML_Index] = '2.0.0.1';
	this.componentVersioning[0][ExperimentManagerPlugin_Comp_Index][this.DevInterface_Index] = '1.0';
	this.componentVersioning[0][ExperimentManagerPlugin_Comp_Index][this.ExtInterface_Index] = '1.0';
	///////////////////////////////////////////////////////////////
	this.componentVersioning[0][ParallelPortPlugin_Comp_Index][this.Name_Index] = 'ParallelPortPlugin';
	this.componentVersioning[0][ParallelPortPlugin_Comp_Index][this.Version_Index] = '2.0.0.1';
	this.componentVersioning[0][ParallelPortPlugin_Comp_Index][this._StimulGLVersion_Index] = 0; //-->2.0.0.2
	this.componentVersioning[0][ParallelPortPlugin_Comp_Index][this.EXML_Index] = 'x';
	this.componentVersioning[0][ParallelPortPlugin_Comp_Index][this.DevInterface_Index] = '1.0';
	this.componentVersioning[0][ParallelPortPlugin_Comp_Index][this.ExtInterface_Index] = '1.0';
	///////////////////////////////////////////////////////////////
	this.componentVersioning[0][SerialPortPlugin_Comp_Index][this.Name_Index] = 'SerialPortPlugin';
	this.componentVersioning[0][SerialPortPlugin_Comp_Index][this.Version_Index] = '1.0.0.1';
	this.componentVersioning[0][SerialPortPlugin_Comp_Index][this._StimulGLVersion_Index] = 0; //-->2.0.0.2
	this.componentVersioning[0][SerialPortPlugin_Comp_Index][this.EXML_Index] = 'x';
	this.componentVersioning[0][SerialPortPlugin_Comp_Index][this.DevInterface_Index] = '1.0';
	this.componentVersioning[0][SerialPortPlugin_Comp_Index][this.ExtInterface_Index] = '1.0';
	///////////////////////////////////////////////////////////////
	this.componentVersioning[0][KeyBoardPlugin_Comp_Index][this.Name_Index] = 'KeyBoardPlugin';
	this.componentVersioning[0][KeyBoardPlugin_Comp_Index][this.Version_Index] = '1.0.0.1';
	this.componentVersioning[0][KeyBoardPlugin_Comp_Index][this._StimulGLVersion_Index] = 0; //-->2.0.0.2
	this.componentVersioning[0][KeyBoardPlugin_Comp_Index][this.EXML_Index] = 'x';
	this.componentVersioning[0][KeyBoardPlugin_Comp_Index][this.DevInterface_Index] = '1.0';
	this.componentVersioning[0][KeyBoardPlugin_Comp_Index][this.ExtInterface_Index] = '1.0';
	///////////////////////////////////////////////////////////////
	this.componentVersioning[0][USBHIDDevicePlugin_Comp_Index][this.Name_Index] = 'USBHIDDevicePlugin';
	this.componentVersioning[0][USBHIDDevicePlugin_Comp_Index][this.Version_Index] = '2.0.0.1';
	this.componentVersioning[0][USBHIDDevicePlugin_Comp_Index][this._StimulGLVersion_Index] = 0; //-->2.0.0.2
	this.componentVersioning[0][USBHIDDevicePlugin_Comp_Index][this.EXML_Index] = 'x';
	this.componentVersioning[0][USBHIDDevicePlugin_Comp_Index][this.DevInterface_Index] = '1.0';
	this.componentVersioning[0][USBHIDDevicePlugin_Comp_Index][this.ExtInterface_Index] = '1.0';
	///////////////////////////////////////////////////////////////
	this.componentVersioning[0][_extensionname_plugin_Comp_Index][this.Name_Index] = '_extensionname_plugin';
	this.componentVersioning[0][_extensionname_plugin_Comp_Index][this.Version_Index] = '1.0.0.1';
	this.componentVersioning[0][_extensionname_plugin_Comp_Index][this._StimulGLVersion_Index] = 0; //-->2.0.0.2
	this.componentVersioning[0][_extensionname_plugin_Comp_Index][this.EXML_Index] = 'x';
	this.componentVersioning[0][_extensionname_plugin_Comp_Index][this.DevInterface_Index] = '1.0';
	this.componentVersioning[0][_extensionname_plugin_Comp_Index][this.ExtInterface_Index] = '1.0';
	///////////////////////////////////////////////////////////////
	this.componentVersioning[0][DefaultQMLPlugin_Comp_Index][this.Name_Index] = 'DefaultQMLPlugin';
	this.componentVersioning[0][DefaultQMLPlugin_Comp_Index][this.Version_Index] = '1.0';
	this.componentVersioning[0][DefaultQMLPlugin_Comp_Index][this._StimulGLVersion_Index] = 0; //-->2.0.0.2
	this.componentVersioning[0][DefaultQMLPlugin_Comp_Index][this.EXML_Index] = 'x';
	this.componentVersioning[0][DefaultQMLPlugin_Comp_Index][this.DevInterface_Index] = 'x';
	this.componentVersioning[0][DefaultQMLPlugin_Comp_Index][this.ExtInterface_Index] = 'x';
	///////////////////////////////////////////////////////////////
	this.componentVersioning[0][StimulGLExtensions_Comp_Index][this.Name_Index] = 'StimulGLExtensions';
	this.componentVersioning[0][StimulGLExtensions_Comp_Index][this.Version_Index] = '1.0';
	this.componentVersioning[0][StimulGLExtensions_Comp_Index][this._StimulGLVersion_Index] = 0; //-->2.0.0.2
	this.componentVersioning[0][StimulGLExtensions_Comp_Index][this.EXML_Index] = 'x';
	this.componentVersioning[0][StimulGLExtensions_Comp_Index][this.DevInterface_Index] = 'x';
	this.componentVersioning[0][StimulGLExtensions_Comp_Index][this.ExtInterface_Index] = 'x';
	///////////////////////////////////////////////////////////////
	this.componentVersioning[0][MediaPlayer_Comp_Index][this.Name_Index] = 'MediaPlayer';
	this.componentVersioning[0][MediaPlayer_Comp_Index][this.Version_Index] = '1.0';
	this.componentVersioning[0][MediaPlayer_Comp_Index][this._StimulGLVersion_Index] = 0; //-->2.0.0.2
	this.componentVersioning[0][MediaPlayer_Comp_Index][this.EXML_Index] = 'x';
	this.componentVersioning[0][MediaPlayer_Comp_Index][this.DevInterface_Index] = 'x';
	this.componentVersioning[0][MediaPlayer_Comp_Index][this.ExtInterface_Index] = 'x';
	///////////////////////////////////////////////////////////////
		
	//StimulGL version (2.0.0.3)
	///////////////////////////////////////////////////////////////
	nNumberOfComponentChanges = 9;//See below, increment by adding new changes for this release!
	tmpArray = createArray(1,nNumberOfComponentChanges,PropCounter);
	this.componentVersioning.push.apply(this.componentVersioning, tmpArray)
	this.componentVersioning[1][ExperimentManagerPlugin_Comp_Index] = 	['ExperimentManagerPlugin',	'2.0.0.3',	1,'2.0.0.2','1.1',	'1.1'];
	this.componentVersioning[1][ParallelPortPlugin_Comp_Index] = 		['ParallelPortPlugin',		'2.0.0.2',	0,'x',		'1.1',	'1.1'];
	this.componentVersioning[1][SerialPortPlugin_Comp_Index] =          ['SerialPortPlugin',		'1.0.0.2',	0,'x',		'1.1',	'1.1'];
	this.componentVersioning[1][KeyBoardPlugin_Comp_Index] = 			['KeyBoardPlugin',			'1.0.0.2',	0,'x',		'1.1',	'1.1'];
	this.componentVersioning[1][USBHIDDevicePlugin_Comp_Index] = 		['USBHIDDevicePlugin',		'2.0.0.2',	0,'x',		'1.1',	'1.1'];
	this.componentVersioning[1][_extensionname_plugin_Comp_Index] = 	['_extensionname_plugin',	'1.0.0.2',	0,'x',		'1.1',	'1.1'];
	this.componentVersioning[1][DefaultQMLPlugin_Comp_Index] = 			['DefaultQMLPlugin',		'1.1',		0,'x',		'x',	'x'];
	this.componentVersioning[1][StimulGLExtensions_Comp_Index] = 		['StimulGLExtensions',		'1.1',		0,'x',		'x',	'x'];
	this.componentVersioning[1][MediaPlayer_Comp_Index] = 				['MediaPlayer',				'1.1',		0,'x',		'x',	'x'];
	
	this.DocumentAuditing = createArray(2,5);//Default constructor!
	///////////////////////////////////////////////////////////////
	this.DocumentAuditing[0][this.Doc_FileName_Index] = 'GettingStartedGuide.html';
	this.DocumentAuditing[0][this.Doc_Title_Index] = 'Getting Started Guide';
	this.DocumentAuditing[0][this.Doc_Version_Index] = '1.0.0.2';
	this.DocumentAuditing[0][this.Doc_Date_Index] = 'November 2012';
	this.DocumentAuditing[0][this.Doc_Authors_Index] = 'Sven Gijsen';
	
	this.DocumentAuditing[1] = ['PreBuildDevelopmentSetup.html','Pre Build Development Setup','1.0.0.2','November 2012','Sven Gijsen'];
}

StimulGL.prototype.GetStrippedFileName = function (strFileUrl)//var strFileUrl=window.location.pathname;
{
	return strFileUrl.replace(/^.*[\\\/]/, '');
}

StimulGL.prototype.GetDocumentIndex = function (strFileUrl)//var strFileUrl=window.location.pathname;
{
	//alert('GetDocumentIndex ' + strFileUrl);
	if ((strFileUrl===undefined) || (strFileUrl==''))
		return -1;
	var filename = this.GetStrippedFileName(strFileUrl)
	var nDocuments = this.DocumentAuditing.length;
	for (var i=0;i<nDocuments;i++) 
	{
		if(this.DocumentAuditing[i][this.Doc_FileName_Index] == filename)
		{
			//alert('found it!');
			return i;
		}
	}
	//alert('not found it...');
	return -1;
}

StimulGL.prototype.GetDocumentTitle = function (strFileUrl)
{
	//alert('GetDocumentTitle');
	var nDocIndex = this.GetDocumentIndex(strFileUrl);
	if(nDocIndex >= 0)
		return this.DocumentAuditing[nDocIndex][this.Doc_Title_Index];
	return this.GetStrippedFileName(strFileUrl);
}

StimulGL.prototype.GetDocumentHeader = function (strFileUrl)
{
	//alert('GetDocumentHeader ' + strFileUrl);
	var nDocIndex = this.GetDocumentIndex(strFileUrl);
	if(nDocIndex >= 0)
	{
		document.write('<h1>' + this.DocumentAuditing[nDocIndex][this.Doc_Title_Index] + '</h1>');
		document.write('<p><em><strong>Version ' + this.DocumentAuditing[nDocIndex][this.Doc_Version_Index] + '</strong></em></p>'); //+ ', for StimulGL v' + + '</p>');
		document.write('<p><em><strong>' + this.DocumentAuditing[nDocIndex][this.Doc_Date_Index] + ', by ' + this.DocumentAuditing[nDocIndex][this.Doc_Authors_Index] + '</strong></em></p>');
	}
	else
	{
		document.write('<h1>' + this.GetStrippedFileName(strFileUrl) + '</h1>');
	}
	document.write('<p>&nbsp;</p>');
}

StimulGL.prototype.GetCurrentRelease = function ()
{
	//alert('GetCurrentRelease');
	return this.StimulGLReleases[this.StimulGLReleases.length-1][0];
}

StimulGL.prototype.GetReleaseByIndex = function (nIndex)
{
	//alert('GetReleaseByIndex');
	return this.StimulGLReleases[nIndex][0];
}

StimulGL.prototype.GetReleaseQtVersionByIndex = function (nIndex)
{
	//alert('GetReleaseQtVersionByIndex');
	return this.StimulGLReleases[nIndex][1];
}

StimulGL.prototype.GetNumberOfReleases = function ()
{
	//alert('GetNumberOfReleases');
	return this.StimulGLReleases.length;
}

StimulGL.prototype.GetNumberOfComponentsByReleaseIndex = function (nReleaseIndex)
{
	//alert('GetComponentRelease ' + nReleaseIndex);
	if(this.componentVersioning.length > nReleaseIndex)
	{
		return this.componentVersioning[nReleaseIndex].length;
	}
	return 0;
}

//StimulGL.prototype.GetComponentReleaseByName = function (strName)
//{
	//alert('GetComponentRelease');
//	return this.componentVersioning[0][0][2];
//}

StimulGL.prototype.CheckComponentByIndexes = function (nReleaseIndex, nComponentIndex)
{
	//alert('CheckComponentByIndexes ' + nReleaseIndex + ", " + nComponentIndex);
	if(this.componentVersioning.length > nReleaseIndex)
	{
		if(this.componentVersioning[nReleaseIndex].length > nComponentIndex)
		{		
			return true;
		}
	}
	return false;
}

StimulGL.prototype.GetComponentNameByIndexes = function (nReleaseIndex, nComponentIndex)
{
	//alert('GetComponentNameByIndexes ' + nReleaseIndex + ", " + nComponentIndex);
	if(this.CheckComponentByIndexes(nReleaseIndex, nComponentIndex))
		return this.componentVersioning[nReleaseIndex][nComponentIndex][this.Name_Index];
	return '';
}

StimulGL.prototype.GetComponentStimGLVersionByIndexes = function (nReleaseIndex, nComponentIndex)
{
	//alert('GetComponentStimGLVersionByIndexes ' + nReleaseIndex + ", " + nComponentIndex);
	if(this.CheckComponentByIndexes(nReleaseIndex, nComponentIndex))
	{
		return this.StimulGLReleases[this.componentVersioning[nReleaseIndex][nComponentIndex][this._StimulGLVersion_Index]][0];
	}
	return '';
}

StimulGL.prototype.GetComponentVersionByIndexes = function (nReleaseIndex, nComponentIndex)
{
	//alert('GetComponentVersionByIndexes ' + nReleaseIndex + ", " + nComponentIndex);
	if(this.CheckComponentByIndexes(nReleaseIndex, nComponentIndex))
		return this.componentVersioning[nReleaseIndex][nComponentIndex][this.Version_Index];
	return '';
}

StimulGL.prototype.GetComponentMinimalEXMLVersionByIndexes = function (nReleaseIndex, nComponentIndex)
{
	//alert('GetComponentMinimalEXMLVersionByIndexes ' + nReleaseIndex + ", " + nComponentIndex);
	if(this.CheckComponentByIndexes(nReleaseIndex, nComponentIndex))
		return this.componentVersioning[nReleaseIndex][nComponentIndex][this.EXML_Index];
	return '';
}

StimulGL.prototype.GetComponentDeviceInterfaceVersionByIndexes = function (nReleaseIndex, nComponentIndex)
{
	//alert('GetComponentDeviceInterfaceVersionByIndexes ' + nReleaseIndex + ", " + nComponentIndex);
	if(this.CheckComponentByIndexes(nReleaseIndex, nComponentIndex))
		return this.componentVersioning[nReleaseIndex][nComponentIndex][this.DevInterface_Index];
	return '';
}

StimulGL.prototype.GetComponentExtensionInterfaceVersionByIndexes = function (nReleaseIndex, nComponentIndex)
{
	//alert('GetComponentExtensionInterfaceVersionByIndexes ' + nReleaseIndex + ", " + nComponentIndex);
	if(this.CheckComponentByIndexes(nReleaseIndex, nComponentIndex))
		return this.componentVersioning[nReleaseIndex][nComponentIndex][this.ExtInterface_Index];
	return '';
}
