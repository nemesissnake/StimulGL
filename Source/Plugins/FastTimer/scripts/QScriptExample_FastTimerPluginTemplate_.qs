//Construct a new Plugin object
var FastTimerobject = new FastTimer();

//Test whether we can invoke some default implemented Slots
FastTimerobject.setExampleProperty(99);
Log(FastTimerobject.getExampleProperty());
FastTimerobject.ExampleProperty = 0;
Log(FastTimerobject.ExampleProperty);

//Set the constructed object to null
FastTimerobject = null;