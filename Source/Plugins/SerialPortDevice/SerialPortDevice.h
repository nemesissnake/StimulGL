//SerialPortDeviceplugin
//Copyright (C) 2014  Sven Gijsen
//
//This file is part of StimulGL.
//StimulGL is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//


//This file defines the script binding interface, all below function are scriptable except for the destructor

#ifndef SerialPortDevice_H
#define SerialPortDevice_H
#include <QObject>
#include <QString>
#include <QtScript>
#include <QScriptable>
#include <QDesktopWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "maindefines.h"
#include "Global.h"

//!  The SerialPortDevice class. 
/*!
  The SerialPortDevice can be used to communicate with a Device connected to the Serial Port.
*/
class SerialPortDevice : public QObject, protected QScriptable
{
	Q_OBJECT
	Q_CLASSINFO("ScriptAPIClassName", "SerialPort");//Can't use defines here!, moc doesn't handle defines

signals:
	//! The SerialDataReceived Signal.
	/*!
	This Signal is emitted each time new Serial Data is received.
	The sData parameter contains the last received data stream.
	*/
	void SerialDataReceived(QString sData);

public:
	SerialPortDevice(QObject *parent = 0);//unsigned short vendor_id = 0x181b, unsigned short product_id = 0x4002, QObject *parent = 0);
	SerialPortDevice(const SerialPortDevice& other ){Q_UNUSED(other);}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro
	~SerialPortDevice();

	static QScriptValue ctor__extensionname(QScriptContext* context, QScriptEngine* engine);

	bool isInitialized(bool bDoInit = false);

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script
	
	//! \brief baudRate slot.
	/*!  This function returns the data baud rate for the desired direction.
	 *   If the setting is successful, it returns the the data baud rate; otherwise it returns -1 and sets an error code which can be obtained by accessing the value of the SerialPortDevice::error property. 
	 *   To set the baud rate, use the enumeration SerialPortDeviceNameSpace::BaudRate or any positive integer value.
	 *   Warning: Only the SerialPortDeviceNameSpace::AllDirections flag is support for setting this property on Windows.
	 *   Warning: Returns equal baud rate in any direction on Windows.
	 * @param dir a integer value representing the direction of the requested baud rate, see SerialPortDeviceNameSpace::Direction.
	 * @return a integer value representing the baud rate or -1.
	 */
	int baudRate(int dir = (int)SerialPortDeviceNameSpace::AllDirections);
	//! \brief clear slot.
	/*!  This function discards all characters from the output or input buffer, depending on a given direction. 
	 *   Including clear an internal class buffers and the UART (driver) buffers. Also terminate pending read or write operations. 
	 * @param dir a integer value representing the direction of the requested baud rate, see SerialPortDeviceNameSpace::Direction.
	 * @return a Boolean value determing whether this function could be executed successfully.
	 */	
	bool clear(int dir = (int)SerialPortDeviceNameSpace::AllDirections);
	//! \brief setFlowControl slot.
	/*!  This function sets the desired flow control mode.
	 * If the setting is not successful then it returns false and sets an error code which can be obtained by accessing the value of the SerialPortDevice::error property.
	 * @param flow a integer value representing the flow control mode, see SerialPortDeviceNameSpace::FlowControl.
	 * @returns a Boolean value determing whether this function could be executed successfully.
	 */	
	bool setFlowControl(int flow);
	//! \brief clearError slot.
	/*!  This function sets the error code to the default SerialPortDeviceNameSpace::NoError after a call.
	 */	
	void clearError();
	//! \brief dataBits slot.
	/*!  This function returns the data bits in a frame.
	 *   If not successful, it sets an error code which can be obtained by accessing the value of the SerialPortDevice::error property.
	 * @return a integer value representing the data bits (see SerialPortDeviceNameSpace::DataBits) setting or -1 if unsuccessfully.
	 */	
	int dataBits();
	//! \brief dataErrorPolicy slot.
	/*!  This function returns the error policy how the process receives the character in case of parity error detection.
	 *   The default policy set is SerialPortDeviceNameSpace::IgnorePolicy.
	 * @return a integer value representing the error policy (see SerialPortDeviceNameSpace::DataErrorPolicy) setting or -1 if unsuccessfully.
	 */	
	int dataErrorPolicy();
	//! \brief error slot.
	/*!  This function returns the error status of the serial port, see SerialPortDeviceNameSpace::SerialPortError.
	 *   The I/O device status returns an error code. For example, if SerialPortDevice.open() returns false, or a read/write operation returns -1, this property can be used to figure out the reason why the operation failed.
	 *   The error code is set to the default SerialPortDeviceNameSpace::NoError after a call to SerialPortDevice.clearError().
	 * @return a integer value.
	 */	
	int error();
	//! \brief errorString slot.
	/*!  This function returns a human-readable description of the last device error that occurred, see SerialPortDevice.error().
	 * @returns a string value holding a human-readable description of the last device error that occurred.
	 */	
	QString errorString();
	//! \brief flowControl slot.
	/*!  This function returns the flow control mode.
	 * If the function could fetch the current flow control mode it return this mode (see SerialPortDeviceNameSpace::FlowControl), otherwise it returns -1 and sets an error code which can be obtained by accessing the value of the SerialPortDevice::error property.
	 * @return a integer value holding the current control mode or -1.
	 */	
	int flowControl();
	//! \brief flush slot.
	/*!  This function writes as much as possible from the internal write buffer to the underlying serial port without blocking. 
	 *   Call this function for sending the buffered data immediately to the serial port. 
	 *   The number of bytes successfully written depends on the operating system. In most cases, this function does not need to be called, 
	 *   because the SerialPortDevice class will start sending data automatically once control is returned to the event loop. 
	 * @return a Boolean value, if any data was written, this function returns true; otherwise returns false.
	 */	
	bool flush();
	//! \brief isDataTerminalReady slot.
	/*!  This function returns the state (high or low) of the line signal DTR.
	 * If the setting is successful, returns true; otherwise returns false. 
	 * @return a Boolean value determining whether the DTR signal is high.
	 */	
	bool isDataTerminalReady();
	//! \brief isRequestToSend slot.
	/*!  This function returns the state (high or low) of the line signal RTS.
	 * If the setting is successful, returns true; otherwise returns false. 
	 * @return a Boolean value determining whether the RTS signal is high.
	 */	
	bool isRequestToSend();
	//! \brief open slot.
	/*!  This function opens the serial port using SerialPortDeviceNameSpace::OpenMode mode, and then returns true if successful; otherwise returns false with and sets an error code which can be obtained by calling the SerialPortDevice::error() method.
	 * @param mode a integer value .
	 * @return a Boolean value determing whether the function executed successfully.
	 */	
	bool open(int mode);
	//! \brief close slot.
	/*!  This function first emits the aboutToClose() signal, then closes the serial port device and sets its OpenMode to SerialPortDeviceNameSpace::NotOpen.
	 *   The error string is also reset.
	 */	
	void close();
	//! \brief parity slot.
	/*!  This function returns the parity checking mode, see SerialPortDeviceNameSpace::Parity.
	 *   If the function could not return the parity checking mode it returns -1 and sets an error code which can be obtained by accessing the value of the SerialPortDevice::error property.
	 * @return a integer value representing the parity checking mode or -1 in case of an unsuccessfully result.
	 */	
	int parity();
	//! \brief pinoutSignals slot.
	/*!  This function returns the state of the line signals in a bitmap format, see SerialPortDeviceNameSpace::PinoutSignal.
	 *   From this result, it is possible to allocate the state of the desired signal by applying a mask "AND", where the mask is the desired enumeration value from SerialPortDevice::PinoutSignals.
	 *   Note that, this method performs a system call, thus ensuring that the line signal states are returned properly. This is necessary when the underlying operating systems cannot provide proper notifications about the changes.
	 * @return a integer value representing the line signals in a bitmap format.
	 */	
	int pinoutSignals();
	//! \brief portName slot.
	/*!  This function returns the name set by SerialPortDevice.setPort() or to the SerialPortDevice constructors. 
	 *   This name is short, i.e. it extract and convert out from the internal variable system location of the device. 
	 *   Conversion algorithm is platform specific:\n\n
	 *   Platform\t\t - Brief Description\n
	 *   Windows\t\t - Removes the prefix "\\\.\\" from the system location and returns the remainder of the string.\n
	 *	 Windows CE\t\t -	Removes the postfix ":" from the system location and returns the remainder of the string.\n
	 *	 Symbian\t\t - Returns the system location as it is, as it is equivalent to the port name.\n
	 *	 GNU/Linux\t\t - Removes the prefix "/dev/" from the system location and returns the remainder of the string.\n
	 *	 Mac OSX\t\t - Removes the prefix "/dev/cu." and "/dev/tty." from the system location and returns the remainder of the string.\n
	 *	 Other *nix\t\t -	The same as for GNU/Linux.
	 * @return a string value holding the port name.
	 */	
	QString	portName();
	//! \brief readBufferSize slot.
	/*!  This function returns the size of the internal read buffer. 
	 *   This limits the amount of data that the client can receive before calling the SerialPortDevice.read() or SerialPortDevice.readAll() methods.
	 *   A read buffer size of 0 (the default) means that the buffer has no size limit, ensuring that no data is lost.
	 * @return a integer(64 bit) value holding the size of the internal read buffer.
	 */	
	qint64 readBufferSize();
	//! \brief sendBreak slot.
	/*!  This function sends a continuous stream of zero bits during a specified period of time duration in msec if the terminal is using asynchronous serial data. 
	 *   If the duration is zero then zero bits are transmitted by at least 0.25 seconds, but no more than 0.5 seconds.
	 *   If the duration is non zero then zero bits are transmitted within a certain period of time depending on the implementation.
	 * @param duration a integer value representing the duration.
	 * @return a boolean value returning whether the function executed successful.
	 */	
	bool sendBreak(int duration = 0);
	//! \brief setBaudRate slot.
	/*!  This function sets the data baud rate for the desired direction.
	 *   If the setting is successful, it returns true; otherwise it returns false and sets an error code which can be obtained by accessing the value of the SerialPortDevice::error property. 
	 *   To set the baud rate, use the enumeration SerialPortDeviceNameSpace::BaudRate or any positive integer value.
	 *   Warning: Only the SerialPortDeviceNameSpace::AllDirections flag is support for setting this property on Windows.
	 * @param baudRate a integer value representing the baud rate, see SerialPortDeviceNameSpace::BaudRate.	
	 * @param dir a integer value representing the direction of the requested baud rate, see SerialPortDeviceNameSpace::Direction.
	 * @return a boolean value returning whether the function executed successful.
	 */	
	bool setBaudRate(int baudRate, int dir = (int)SerialPortDeviceNameSpace::AllDirections);
	//! \brief setBreakEnabled slot.
	/*!  This function controls the signal break, depending on the flag set.
	 *   If set is true then enables the break transmission; otherwise disables.
	 * @param set a boolean value determining whether the break transmission should be enabled or disabled.
	 * @return a boolean value returning whether the function executed successful.
	 */	
	bool setBreakEnabled(bool set = true);
	//! \brief setDataBits slot.
	/*!  This function sets the data bits in a frame.
	 *   If the setting is unsuccessful it returns false and sets an error code which can be obtained by accessing the value of the SerialPortDevice::error property.
	 * @param dataBits a integer value the data bits in a frame, see SerialPortDeviceNameSpace::DataBits.
	 * @return a boolean value returning whether the function executed successful.
	 */	
	bool setDataBits(int dataBits);
	//! \brief setDataErrorPolicy slot.
	/*!  This function sets the error policy how the process receives the character in case of parity error detection.
	 *   If the setting is unsuccessful, it returns false. The default policy set is SerialPortDeviceNameSpace::IgnorePolicy.
	 * @param policy a integer value representing the data error policy, see SerialPortDeviceNameSpace::DataErrorPolicy.
	 * @return a boolean value returning whether the function executed successful.
	 */	
	bool setDataErrorPolicy(int policy = (int)SerialPortDeviceNameSpace::IgnorePolicy);
	//! \brief setDataTerminalReady slot.
	/*!  This function configures the state (high or low) of the line signal DTR depending on the set flag.
	 *   If the flag is true then the DTR signal is set to high; otherwise low.
	 * @param set a boolean value holding the flag value.
	 * @return a boolean value returning whether the function executed successful.
	 */	
	bool setDataTerminalReady(bool set);
	//! \brief setParity slot.
	/*!  This function configures the parity checking mode.
	 * @param parity a integer value representing the parity checking mode, see SerialPortDeviceNameSpace::Parity.
	 * @return a boolean value returning whether the function executed successful.
	 */	
	bool setParity(int parity);
	//! \brief setPortName slot.
	/*!  This function configures the name of the serial port set by SerialPortDevice.setPort() or to the SerialPortDevice constructors. 
	 *   This name is short, i.e. it extract and convert out from the internal variable system location of the device. 
	 *   Conversion algorithm is platform specific:\n\n
	 *   Platform\t\t - Brief Description\n
	 *   Windows\t\t - Removes the prefix "\\\.\\" from the system location and returns the remainder of the string.\n
	 *	 Windows CE\t\t -	Removes the postfix ":" from the system location and returns the remainder of the string.\n
	 *	 Symbian\t\t - Returns the system location as it is, as it is equivalent to the port name.\n
	 *	 GNU/Linux\t\t - Removes the prefix "/dev/" from the system location and returns the remainder of the string.\n
	 *	 Mac OSX\t\t - Removes the prefix "/dev/cu." and "/dev/tty." from the system location and returns the remainder of the string.\n
	 *	 Other *nix\t\t -	The same as for GNU/Linux.
	 * @param name a string value holding the serial port name.
	 * @return a boolean value returning whether the function executed successful.
	 */	
	bool setPortName(const QString & name);
	//! \brief setReadBufferSize slot.
	/*!  This function sets the size of SerialPortDevice's internal read buffer to be size bytes.
	 *   If the buffer size is limited to a certain size, SerialPortDevice will not buffer more than this size of data. 
	 *   Exceptionally, a buffer size of 0 means that the read buffer is unlimited and all incoming data is buffered, this is the default.
	 *   This option is useful if the data is only read at certain points in time (for instance in a real-time streaming application) or 
	 *   if the serial port should be protected against receiving too much data, which may eventually causes that the application runs out of memory.
	 * @param size a integer(64 bit) value representing the buffer size.
	 */	
	void setReadBufferSize(qint64 size);
	//! \brief setRequestToSend slot.
	/*!  This function configures the state (high or low) of the line signal RTS.
	 *   If the flag is true then the RTS signal is set to high; otherwise low.
	 * @param set a boolean value holding the flag.
	 * @return a boolean value returning whether the function executed successful.
	 */	
	bool setRequestToSend(bool set);
	//! \brief setSettingsRestoredOnClose slot.
	/*!  This function configures the flag which allows to restore the previous settings while closing the serial port.
	 *   If this flag is true, the settings will be restored; otherwise not. 
	 *   The default state of the SerialPortDevice class is configured to restore the settings.
	 * @param restore a boolean value holding the flag.
	 */	
	void setSettingsRestoredOnClose(bool restore);
	//! \brief setStopBits slot.
	/*!  This function configures the number of stop bits in a frame.
	 *   If the setting is unsuccessful it returns false and sets an error code which can be obtained by accessing the value of the SerialPortDevice::error property.
	 * @param stopBits a integer value representing the stop bits in a frame, see SerialPortDeviceNameSpace::StopBits.
	 * @return a boolean value returning whether the function executed successful.
	 */	
	bool setStopBits(int stopBits);
	//! \brief settingsRestoredOnClose slot.
	/*!  This function returns the flag which allows to restore the previous settings while closing the serial port.
	 *   The default state of the SerialPortDevice class is configured to restore the settings.
	 * @return a boolean value returning the flag.
	 */	
	bool settingsRestoredOnClose();
	//! \brief stopBits slot.
	/*!  This function returns the number of stop bits in a frame.
	 * @return a integer value representing the number of stop bits, see SerialPortDeviceNameSpace::StopBits.
	 */	
	int stopBits();
	//! \brief writeData slot.
	/*!  This function writes/sends a string of data over the configured serial port.
	 * @param data a string value holding the data that should be written/send.
	 * @return a integer(64 bits) value holding the number of bytes successfully send.
	 */	
	qint64 writeData(const QString &data);
	//! \brief setPort slot.
	/*!  This function .
	 * @param serialPortInfo a QSerialPortInfo value .
	 */	
	void setPort(const QSerialPortInfo &serialPortInfo);
	//! \brief readAll slot.
	/*!  This function reads all available data from the serial device, and returns it as a QByteArray.
	 * This function has no way of reporting errors; returning an empty QByteArray() can mean either that no data was currently available for reading, or that an error occurred.
	 * @return a QByteArray value containing the data that was read.
	 */	
	QByteArray readAll();

private slots:
	void readData();
	void handleError(QSerialPort::SerialPortError error);

private:
	QScriptEngine* currentScriptEngine;
	QSerialPort* serialPort;
};

#endif // SerialPortDevice_H
