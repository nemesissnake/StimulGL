//Construct a new Plugin object
var TCPNetworkServerobject = new TCPNetworkServer();

//Test whether we can invoke some default implemented Slots
TCPNetworkServerobject.setExampleProperty(99);
Log(TCPNetworkServerobject.getExampleProperty());
TCPNetworkServerobject.ExampleProperty = 0;
Log(TCPNetworkServerobject.ExampleProperty);

//Set the constructed object to null
TCPNetworkServerobject = null;