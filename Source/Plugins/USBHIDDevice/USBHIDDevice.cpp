#include "USBHIDDevice.h"
#include <windows.h>
#include <QTextOStream>

QScriptValue USBHIDDevice::ctor__extensionname(QScriptContext* context, QScriptEngine* engine)
{
	//this function gets called first whenever a new object is constructed trough the script

	//	if (context->isCalledAsConstructor()) {
	//		// initialize the new object
	//		//context->thisObject().setProperty("bar", ...);
	//		// ...
	//		// return a non-object value to indicate that the
	//		// thisObject() should be the result of the "new Foo()" expression
	//		//return engine->undefinedValue();

	return engine->newQObject(new USBHIDDevice(), QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
} 

USBHIDDevice::USBHIDDevice(QObject *parent) : QObject(parent)
{

}

USBHIDDevice::~USBHIDDevice()
{

}

void USBHIDDevice::setExampleProperty( short sExampleProperty )
{
	m_ExampleProperty = sExampleProperty;
	emit ExampleSignalTriggered(m_ExampleProperty);//Signal that the value has been changed
}

short USBHIDDevice::getExampleProperty() const
{
	return m_ExampleProperty;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool USBHIDDevice::Calibrate()
{
	int nResult;
	bool bRetVal = false;
	HIDCalDiag = new HIDCalibrateDialog();
	nResult = HIDCalDiag->exec();   //->show();
	switch (nResult)
	{
	case QDialog::Accepted://OK Button Pressed
		nResult = 9;
		bRetVal = true;
		break;
	case QDialog::Rejected://Cancel or Close pressed
		nResult = 10;
		break;
	}	
	return bRetVal;
}

bool USBHIDDevice::StartCaptureThread()
{
	//ppCaptureThread::DetectionMethod CapDecMethod;
	//if (method == 0) 
	//{CapDecMethod = ppCaptureThread::DetectionMethod::MaskedValueChanged;}
	//else if (method == 1) 
	//{CapDecMethod = ppCaptureThread::DetectionMethod::MaskedValueChangedHigh;}
	//else 
	//{CapDecMethod = ppCaptureThread::DetectionMethod::MaskedValueChangedLow;}

	HIDCapThread = new HIDCaptureThread(this);

	connect(HIDCapThread, SIGNAL(receiveThreadTriggered(QString)), this, SIGNAL(CaptureThreadTriggered(QString)));
	connect(HIDCapThread, SIGNAL(receiveThreadStarted(QString)), this, SIGNAL(CaptureThreadStarted(QString)));
	connect(HIDCapThread, SIGNAL(receiveThreadStopped(QString)), this, SIGNAL(CaptureThreadStopped(QString)));

	HIDCapThread->start();
	return true;
}

void USBHIDDevice::StopCaptureThread()
{
	HIDCapThread->stop();
}

bool USBHIDDevice::doMeasurement()
{
	int res, i;
	unsigned char buf[256];
	hid_device *handle;

	struct hid_device_info *devs, *cur_dev;
	devs = hid_enumerate(0x0, 0x0);
	cur_dev = devs;	
	while (cur_dev) {
		printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
		printf("\n");
		printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
		printf("  Product:      %ls\n", cur_dev->product_string);
		printf("  Release:      %hx\n", cur_dev->release_number);
		printf("  Interface:    %d\n",  cur_dev->interface_number);
		printf("\n");
		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);
	memset(buf,0x00,sizeof(buf));// Set up the command buffer.

	// Open the device using the VID, PID,
	// and optionally the Serial number.
	handle = hid_open(0x181b, 0x4002, NULL);
	if (!handle) {
		printf("unable to open device\n");
		return 1;
	}

	//// Read the Manufacturer String
	//wstr[0] = 0x0000;
	//res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	//if (res < 0)
	//	printf("Unable to read manufacturer string\n");
	//printf("Manufacturer String: %ls\n", wstr);

	// Read the Product String
	//wstr[0] = 0x0000;
	//res = hid_get_product_string(handle, wstr, MAX_STR);
	//if (res < 0)
	//	printf("Unable to read product string\n");
	//printf("Product String: %ls\n", wstr);

	// Read the Serial Number String
	//wstr[0] = 0x0000;
	//res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	//if (res < 0)
	//	printf("Unable to read serial number string\n");
	//printf("Serial Number String: (%d) %ls", wstr[0], wstr);
	//printf("\n");

	//// Read Indexed String 1
	//wstr[0] = 0x0000;
	//res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
	//if (res < 0)
	//	printf("Unable to read indexed string 1\n");
	//printf("Indexed String 1: %ls\n", wstr);

	// Set the hid_read() function to be non-blocking.
	hid_set_nonblocking(handle, 1);

	//QFile f("c:\\output.txt");
	//QTextStream out(&f);

	//qsValue = "ccc";
	//out << qsValue;

	QStringList lines;
	QString line;
	QFile file ( "d:\\temp\\output.txt" );

	while(1)
	{
		// Try to read from the device. There should be no
		// data here, but execution should not block.
		// res = hid_read(handle, buf, 4);
		res = hid_read(handle, buf, sizeof(buf));
		if (res>1)
		{
			//printf("Data read:\n   ");
			// Print out the returned buffer.
			for (i = 0; i < res; i++)
			{
				//printf("%02hhx ", buf[i]);
				//csValue = "";
				line.append(QString("%02hhx ").arg(buf[i]));
				//OutputDebugString(qsValue.toStdString().c_str());
				//qDebug() << buf[i] << " ";
				//emit ExampleSignalTriggered(buf[i]);
				//qDebug((QString("%02hhx ").arg(buf[i])));
				//printf("%02hhx ", buf[i]);
			}	
			//printf("\n");
			lines += line;
			if ( file.open( QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text) )
			{
				QTextStream stream( &file );
				for ( QStringList::Iterator it = lines.begin(); it != lines.end(); ++it )
					stream << *it << "\n";
				file.close();
			}
		}
	}
	//file.close();


	//// Send a Feature Report to the device
	//buf[0] = 0x2;
	//buf[1] = 0xa0;
	//buf[2] = 0x0a;
	//buf[3] = 0x00;
	//buf[4] = 0x00;
	//res = hid_send_feature_report(handle, buf, 17);
	//if (res < 0) {
	//	printf("Unable to send a feature report.\n");
	//}

	//memset(buf,0,sizeof(buf));

	//buf[0] = 0x00;
	//while(1)
	//{
	//	//// Read a Feature Report from the device
	//	res = hid_get_feature_report(handle, buf, sizeof(buf));
	//	if (res < 0) {
	//		printf("Unable to get a feature report.\n");
	//		printf("%ls", hid_error(handle));
	//	}
	//	else {
	//		// Print out the returned buffer.
	//		printf("Feature Report\n   ");
	//		for (i = 0; i < res; i++)
	//			printf("%02hhx ", buf[i]);
	//		printf("\n");
	//	}
	//	buf[0] = buf[0] + 1;
	//}

	//memset(buf,0,sizeof(buf));

	//// Toggle LED (cmd 0x80). The first byte is the report number (0x1).
	//buf[0] = 0x1;
	//buf[1] = 0x80;
	//res = hid_write(handle, buf, 17);
	//if (res < 0) {
	//	printf("Unable to write()\n");
	//	printf("Error: %ls\n", hid_error(handle));
	//}
	//

	//// Request state (cmd 0x81). The first byte is the report number (0x1).
	//buf[0] = 0x1;
	//buf[1] = 0x81;
	//res = hid_write(handle, buf, 5);
	//if (res < 0)
	//	printf("Unable to write() (2)\n");

	//// Read requested state. hid_read() has been set to be
	//// non-blocking by the call to hid_set_nonblocking() above.
	//// This loop demonstrates the non-blocking nature of hid_read().
	//res = 0;
	//while (res == 0) {
	//	res = hid_read(handle, buf, sizeof(buf));
	//	if (res == 0)
	//		printf("waiting...\n");
	//	if (res < 0)
	//		printf("Unable to read()\n");
	//	#ifdef WIN32
	//	Sleep(500);
	//	#else
	//	usleep(500*1000);
	//	#endif
	//}

	//printf("Data read:\n   ");
	//// Print out the returned buffer.
	//for (i = 0; i < res; i++)
	//	printf("%02hhx ", buf[i]);
	//printf("\n");

//#ifdef WIN32
//	system("pause");
//#endif

	return false;
}



//// Register for device connect notification
//DEV_BROADCAST_DEVICEINTERFACE devInt;
//ZeroMemory( &devInt, sizeof(devInt) );
//devInt.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
//devInt.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
//devInt.dbcc_classguid = GUID_DEVINTERFACE_VOLUME;
//
//m_hDeviceNotify =
//RegisterDeviceNotification( winId(), &devInt, DEVICE_NOTIFY_WINDOW_HANDLE );   
//if(m_hDeviceNotify == NULL)
//{
//	qDebug() << "Failed to register device notification";
//} // end if
