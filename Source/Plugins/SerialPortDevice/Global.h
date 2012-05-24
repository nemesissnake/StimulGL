//SerialPortDeviceplugin
//Copyright (C) 2012  Sven Gijsen
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

//#include <QThread>

/*! The namespace for the global SerialPortDevice definition */
namespace SerialPortDeviceNameSpace
{
	//// The enum (SerialDevDialogState) can store the state of a SerialPort Device.
	//enum SerialDevDialogState
	//{
	//	NotInitialized					= 0, // 0: The NotInitialized state
	//	ReadyToCapture					= 1, // 1: The ReadyToCapture state
	//	CaptureStarting					= 2, // 2: The CaptureStarting state
	//	CaptureStarted					= 3, // 3: The CaptureStarted state
	//	CaptureStopping					= 4  // 4: The CaptureStopping state
	//};
	//Q_DECLARE_FLAGS(SerialDevDialogStates, SerialDevDialogState)
	//Q_DECLARE_OPERATORS_FOR_FLAGS(SerialDevDialogStates)

	//struct strcCalibrationData {
	//	int Xmin;
	//	int Xmax;
	//	int Ymin;
	//	int Ymax;
	//	int XAbsOffset;
	//	int YAbsOffset;
	//	float XRelScaleFactor;
	//	float YRelScaleFactor;
	//};

	///*! The enum (CaptureMethods) can store a USB HID Device Capture thread method */
	//enum CaptureMethod
	//{
	//	MaskedValueChanged				= 0, //!< 0: The MaskedValueChanged	method
	//	MaskedValueChangedHigh			= 1, //!< 1: The MaskedValueChangedHig method
	//	MaskedValueChangedLow			= 2  //!< 2: The MaskedValueChangedLow method
	//};
	//Q_DECLARE_FLAGS(CaptureMethods, CaptureMethod)
	//Q_DECLARE_OPERATORS_FOR_FLAGS(CaptureMethods)

	//struct strcJoystickPosition 
	//{
	//	unsigned char Xpos;
	//	unsigned char Ypos;
	//};  
}

#endif//SERIALGLOBAL_H