//This works
var a = 123;
Log(a);
StimulGL.executeScriptContent("Log(a);");

////This doesn't work
//StimulGL.executeScriptContent("var b=111;\n Log(a+b);");
//StimulGL.executeScriptContent("Log(b);");//This var b is not known here...

//Fixed, this does work
StimulGL.executeScriptContent("var c=222;\n Log(a+c);\n function myAdd(nNumber)\n { return c + nNumber; } \n StimulGL.saveContextState('aa');\n StimulGL.setContextState('aa');");
StimulGL.executeScriptContent("Log(c); \n Log(myAdd(3));");

a = a + 1;
Log(a);
StimulGL.executeScriptContent("myAdd = null;");

//function myAdd(nNumber)
//{
//	return c + nNumber;
//}

