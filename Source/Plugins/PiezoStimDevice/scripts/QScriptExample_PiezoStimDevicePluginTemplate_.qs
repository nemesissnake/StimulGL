//Construct a new Plugin object
var PiezoStimDeviceobject = new PiezoStimDevice();
var sLicenseCode = "24OF1B661A7EW5116941EABAMDJSGO";
var nRetval;
Log();
////Test whether we can invoke some standard template example slots
//PiezoStimDeviceobject.setExampleProperty(99);
//Log(PiezoStimDeviceobject.getExampleProperty());
//PiezoStimDeviceobject.ExampleProperty = 0;
//Log(PiezoStimDeviceobject.ExampleProperty);

nRetval = PiezoStimDeviceobject.initStimulator(sLicenseCode);
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.resetStimulator();
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));
nRetval = PiezoStimDeviceobject.closeStimulator();
Log(PiezoStimDeviceobject.ReturnCodeToString(nRetval));



Log();
//Set the constructed object to null
PiezoStimDeviceobject = null;