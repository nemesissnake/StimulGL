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


#ifndef SERIALGLOBAL_H
#define SERIALGLOBAL_H

/*! The namespace for the global SerialPortDevice definition */
namespace SerialPortDeviceNameSpace
{
	/*! The enum (OpenMode) describes the the mode in which a serial port device is opened. It is also returned by SerialPortDevice.openMode(). */
	enum OpenMode  {
		NotOpen = 0x0000,						//!<   0: The device is not open.
		ReadOnly = 0x0001,						//!<   1: The device is open for reading.
		WriteOnly = 0x0002,						//!<   2: The device is open for writing.
		ReadWrite =	ReadOnly | WriteOnly,		//!<   3: The device is open for reading and writing.
		Append = 0x0004,						//!<   4: The device is opened in append mode, so that all data is written to the end of the file.
		Truncate = 0x0008,						//!<   8: If possible, the device is truncated before it is opened. All earlier contents of the device are lost.
		Text = 0x0010,							//!<  16: When reading, the end-of-line terminators are translated to '\\n'. When writing, the end-of-line terminators are translated to the local encoding, for example '\\r\\n' for Win32.
		Unbuffered = 0x0020						//!<  32: Any buffer in the device is bypassed.
	};

	/*! The enum (Direction) describes the possible directions of the data transmission. */
	enum Direction  {
		Input = 1,							//!<  1: Input.
		Output = 2,							//!<  2: Output.
		AllDirections = Input | Output		//!<  3: AllDirections.
	};

	/*! The enum (BaudRate) describes the baud rate which the communication device operates with. Note: only the most common standard baud rates are listed in this enum. */
	enum BaudRate {
		Baud1200 = 1200,		//!<   1200: 1200 baud.
		Baud2400 = 2400,		//!<   2400: 2400 baud.
		Baud4800 = 4800,		//!<   4800: 4800 baud.
		Baud9600 = 9600,		//!<   9600: 9600 baud.
		Baud19200 = 19200,		//!<  19200: 19200 baud.
		Baud38400 = 38400,		//!<  38400: 38400 baud.
		Baud57600 = 57600,		//!<  57600: 57600 baud.
		Baud115200 = 115200,	//!< 115200: 115200 baud.
		UnknownBaud = -1		//!<     -1: unknown baud.
	};

	/*! The enum (DataBits) describes the number of data bits used. */
	enum DataBits {
		Data5 = 5,				//!<   5: five bits.
		Data6 = 6,				//!<   6: six bits.
		Data7 = 7,				//!<   7: seven bits.
		Data8 = 8,				//!<   8: eight bits.
		UnknownDataBits = -1	//!<  -1: unknown data bits.
	};

	/*! The enum (Parity) describes the parity scheme used. */
	enum Parity {
		NoParity = 0,				//!<   0: No parity.
		EvenParity = 2,				//!<   2: Even parity.
		OddParity = 3,				//!<   3: Odd parity.
		SpaceParity = 4,			//!<   4: Space parity.
		MarkParity = 5,				//!<   5: Mark parity.
		UnknownParity = -1			//!<  -1: Unknown parity.
	};

	/*! The enum (StopBits) describes the number of stop bits used. */
	enum StopBits {
		OneStop = 1,				//!<   1: 1 stop bit.
		TwoStop = 2,				//!<   2: 2 stop bits.
		OneAndHalfStop = 3,			//!<   3: 1.5 stop bits.
		UnknownStopBits = -1		//!<  -1: Unknown number of stop bit.
	};

	/*! The enum (FlowControl) describes the desired flow control mode. */
	enum FlowControl {
		NoFlowControl = 0,			//!<   0: No flow control.
		HardwareControl = 1,		//!<   1: Hardware flow control (RTS/CTS).
		SoftwareControl = 2,		//!<   2: Software flow control (XON/XOFF)
		UnknownFlowControl = -1		//!<  -1: Unknown flow control.
	};

	/*! The enum (PinoutSignal) describes the possible RS-232 pinout signals. */
	enum PinoutSignal {
		NoSignal = 0x00,							//!<    0: No line active.
		TransmittedDataSignal = 0x01,				//!<    1: TxD (Transmitted Data).
		ReceivedDataSignal = 0x02,					//!<    2: RxD (Received Data).
		DataTerminalReadySignal = 0x04,				//!<    4: DTR (Data Terminal Ready).
		DataCarrierDetectSignal = 0x08,				//!<    8: DCD (Data Carrier Detect).
		DataSetReadySignal = 0x10,					//!<   16: DSR (Data Set Ready).
		RingIndicatorSignal = 0x20,					//!<   32: RNG (Ring Indicator).
		RequestToSendSignal = 0x40,					//!<   64: RTS (Request To Send).
		ClearToSendSignal = 0x80,					//!<  128: CTS (Clear To Send).
		SecondaryTransmittedDataSignal = 0x100,		//!<  256: STD (Secondary Transmitted Data).
		SecondaryReceivedDataSignal = 0x200			//!<  512: SRD (Secondary Received Data).
	};

	/*! The enum (DataErrorPolicy) describes the policies for the received symbols while parity errors were detected.. */
	enum DataErrorPolicy {
		SkipPolicy = 0,				//!<   0: Skips the bad character.
		PassZeroPolicy = 1,			//!<   1: Replaces bad character to zero.
		IgnorePolicy = 2,			//!<   2: Ignores the error for a bad character.
		StopReceivingPolicy = 3,	//!<   3: Stops data reception on error.
		UnknownPolicy = -1			//!<  -1: Unknown policy.
	};

	/*! The enum (SerialPortError) describes the serial port errors. */
	enum SerialPortError {
		NoError,					//!<   0: No error occurred.
		DeviceNotFoundError,		//!<   1: An error occurred while attempting to open an non-existing device.
		PermissionError,			//!<   2: An error occurred while attempting to open an already opened device by another process or a user not having enough permission and credentials to open.
		OpenError,					//!<   3: An error occurred while attempting to open an already opened device in this object.
		ParityError,				//!<   4: Parity error detected by the hardware while reading data.
		FramingError,				//!<   5: Framing error detected by the hardware while reading data.
		BreakConditionError,		//!<   6: Break condition detected by the hardware on the input line.
		WriteError,					//!<   7: An I/O error occurred while writing the data.
		ReadError,					//!<   8: An I/O error occurred while reading the data.
		ResourceError,				//!<   9: An I/O error occurred when a resource becomes unavailable, e.g. when the device is unexpectedly removed from the system.
		UnsupportedOperationError,	//!<  10: The requested device operation is not supported or prohibited by the running operating system.
		UnknownError				//!<  11: An unidentified error occurred.
	};
}

#endif//SERIALGLOBAL_H