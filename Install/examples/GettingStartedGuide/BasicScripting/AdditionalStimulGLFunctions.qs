// This example shows how you can make use of some additional StimulGL functions.

var result = "";

Log("\n\n\n");//This appends some empty lines (\n) to the Log Output Pane

result = StimulGL.getSelectedScriptFileLocation();
Log("The selected script file location is: " + result);

result = StimulGL.getApplicationRootDirPath();
Log("The StimulGL root directory is: " + result);

result = StimulGL.getSelectedScriptFileName();
Log("This script file is named: " + result);

result = StimulGL.getEnvironmentVariabele("USERNAME");
Log("User: " + result + " is currently logged on this system.");

Beep();//This function plays a default beep sound
Pause(500);//This function Pauses for 1000 milliseconds before it proceeds (Important, this blocks all script context execution! Therefore only for testing purpose.)
Beep();
StimulGL.write2OutputWindow("You can also make use of the StimulGL.write2OutputWindow() function \
													to write to the Log Output Pane");
Log("\tTAB is inserted first");//This appends an TAB (\t) and some text to the Log Output Pane
Log("\n\n\n");//This appends some empty lines (\n) to the Log Output Pane

//StimulGL.find(false,"getSelectedScriptFileName");
//StimulGL.clearOutputWindow();
//StimulGL.closeSelectedScriptFile();
//ThrowError("This is an dummy error message"); This function Throw an error, see output log window and aborts the script immediately (this is not a clean way of exiting your script!).
