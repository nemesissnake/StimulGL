#include "USBHIDDevice.h"
#include "hidcapturethread.h"

HIDCaptureThread::HIDCaptureThread(QObject *parent) : QThread(parent)
{
	isRunning = false;
	abortRunning = false;
	//nBaseAddress = baseAddress;
	//dMethod = method;
	//nMask = mask;
	//nPostLHDelay = postLHDelay;
	//nPostHLDelay = postHLDelay;
}

HIDCaptureThread::~HIDCaptureThread()
{
	stop();
}

void HIDCaptureThread::stop()
{
	abortRunning = true;
}

void HIDCaptureThread::run()
{
	//short currentValue;
	//short oldValue;
	//QString result = "";
	//ParallelPort portDev(nBaseAddress,this);

	//currentValue = (portDev.PortRead() & nMask);
	//oldValue = currentValue;
	isRunning = true;
	emit receiveThreadStarted(QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz"));

	//switch (dMethod)
	//{
	//case MaskedValueChanged :
	//	do 
	//	{
	//		if (oldValue != currentValue)//any change within masked measurement
	//		{
	//			//result = QString::number(currentValue);
	//			emit recieveThreadTriggered(currentValue);
	//			if (nPostLHDelay > 0)
	//			{
	//				if (currentValue > 0)//low->high was triggered
	//				{
	//					QThread::msleep(nPostLHDelay);
	//				}
	//			}
	//			if (nPostHLDelay > 0)
	//			{
	//				if (currentValue == 0)//high->low was triggered
	//				{
	//					QThread::msleep(nPostHLDelay);
	//				}
	//			}
	//			oldValue = currentValue;
	//		}
	//		currentValue = (portDev.PortRead() & nMask);
	//	} while (abortRunning==false);
	//	break;	
	//case MaskedValueChangedHigh :
	//	do 
	//	{
	//		if (oldValue != currentValue)
	//		{
	//			if (currentValue == nMask)//low->high was triggered
	//			{
	//				//result = QString::number(currentValue);
	//				emit recieveThreadTriggered(currentValue);	
	//				if (nPostLHDelay > 0) QThread::msleep(nPostLHDelay);
	//			}
	//			oldValue = currentValue;
	//		}
	//		currentValue = (portDev.PortRead() & nMask);
	//	} while (abortRunning==false);
	//	break;
	//case MaskedValueChangedLow :
	//	do 
	//	{
	//		if (oldValue != currentValue)
	//		{
	//			if (currentValue == 0)//high->low was triggered
	//			{
	//				//result = QString::number(currentValue);
	//				emit recieveThreadTriggered(currentValue);	
	//				if (nPostHLDelay > 0) QThread::msleep(nPostHLDelay);
	//			}
	//			oldValue = currentValue;
	//		}
	//		currentValue = (portDev.PortRead() & nMask);
	//	} while (abortRunning==false);
	//	break;
	//default :
	//	break;	
	//}

	int res, i;
	unsigned char buf[256];
	hid_device *handle;

	struct hid_device_info *devs, *cur_dev;
	devs = hid_enumerate(0x0, 0x0);
	cur_dev = devs;	
	//while (cur_dev) {
	//	printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
	//	printf("\n");
	//	printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
	//	printf("  Product:      %ls\n", cur_dev->product_string);
	//	printf("  Release:      %hx\n", cur_dev->release_number);
	//	printf("  Interface:    %d\n",  cur_dev->interface_number);
	//	printf("\n");
	//	cur_dev = cur_dev->next;
	//}
	hid_free_enumeration(devs);
	memset(buf,0x00,sizeof(buf));// Set up the command buffer.

	// Open the device using the VID, PID,
	// and optionally the Serial number.
	handle = hid_open(0x181b, 0x4002, NULL);
	if (!handle) {
		//printf("unable to open device\n");
		emit receiveThreadStopped(QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz"));
		isRunning = false;
		return;
	}

	// Set the hid_read() function to be non-blocking.
	hid_set_nonblocking(handle, 1);

	QStringList lines;
	QString line;
	QFile file ( "d:\\temp\\output.txt" );

	while(abortRunning==false)
	{
		// Try to read from the device. There should be no
		// data here, but execution should not block.
		// res = hid_read(handle, buf, 4);
		res = hid_read(handle, buf, sizeof(buf));
		if (res>1)
		{
			//emit captureThreadTriggered();
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
				{
					stream << *it << "\n";
					emit receiveThreadTriggered(QString(*it));
				}
				file.close();
			}
		}
	}

	abortRunning = false;
	isRunning = false;
	emit receiveThreadStopped(QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz"));
	return;
}

