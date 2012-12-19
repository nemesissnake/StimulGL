//Construct a new Plugin object
var _extensionname_object = new _extensionname_();

//Test whether we can invoke some default implemented Slots
_extensionname_object.setExampleProperty(99);
Log(_extensionname_object.getExampleProperty());
_extensionname_object.ExampleProperty = 0;
Log(_extensionname_object.ExampleProperty);

//Set the constructed object to null
_extensionname_object = null;