//SerialPortDeviceplugin
//Copyright (C) 2013  Sven Gijsen
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


#ifndef SERIALPORTDEVICEDEFINES_H
#define SERIALPORTDEVICEDEFINES_H
#include "./../../StimulGL/maindefines.h"

//Only edit the below!
#define PLUGIN_FILE_VERSION_STRING_MAJOR	1   //Please do not edit this line manually, see BinaryVersioning.qs (1211595614).
#define PLUGIN_FILE_VERSION_STRING_MINOR	0   //Please do not edit this line manually, see BinaryVersioning.qs (1211595614).
#define PLUGIN_FILE_VERSION_STRING_REVISION	0   //Please do not edit this line manually, see BinaryVersioning.qs (1211595614).
#define PLUGIN_FILE_VERSION_STRING_BUILD	2   //Please do not edit this line manually, see BinaryVersioning.qs (1211595614).
#define PLUGIN_PRODUCT_VERSION_STRING_MAJOR	1   //Please do not edit this line manually, see BinaryVersioning.qs (1211595614).
#define PLUGIN_PRODUCT_VERSION_STRING_MINOR	0   //Please do not edit this line manually, see BinaryVersioning.qs (1211595614).
#define PLUGIN_PRODUCT_VERSION_STRING_REVISION	0   //Please do not edit this line manually, see BinaryVersioning.qs (1211595614).
#define PLUGIN_PRODUCT_VERSION_STRING_BUILD	1   //Please do not edit this line manually, see BinaryVersioning.qs (1211595614).
#define PLUGIN_LEGAL_COPYRIGHT						MAIN_PROGRAM_LEGAL_COPYRIGHT
#define PLUGIN_AUTHOR_NAME							MAIN_PROGRAM_AUTHOR_NAME
#define PLUGIN_AUTHOR_EMAIL							MAIN_PROGRAM_AUTHOR_EMAIL
#define PLUGIN_COMPANY_NAME							MAIN_PROGRAM_COMPANY_NAME
#define PLUGIN_INTERNAL_NAME						"SerialPortDevice"
#define PLUGIN_SCRIPTOBJECT_NAME					"SerialPortDevice"
#define PLUGIN_SERIALPORTINFO_OBJECT_NAME			"SerialPortDeviceInfo"
#define PLUGIN_PRODUCT_NAME							PLUGIN_INTERNAL_NAME
#define PLUGIN_INTERNAL_EXTENSION					"dll"
#define PLUGIN_ORIGINAL_FILENAME					PLUGIN_INTERNAL_NAME "." PLUGIN_INTERNAL_EXTENSION
#define PLUGIN_FULL_NAME							PLUGIN_INTERNAL_NAME "(v" PLUGIN_FILE_VERSION_STRING ")"
#define PLUGIN_FILE_DESCRIPTION						"Library that controls the communication with an Serial Port Device"
#define PLUGIN_INFORMATION							PLUGIN_INTERNAL_NAME " Plugin(v" PLUGIN_FILE_VERSION_STRING ") by " PLUGIN_AUTHOR_NAME
#define PLUGIN_MAIN_PROGRAM_MINIMAL_VERSION	"2.0.0.3"   //Please do not edit this line manually, see BinaryVersioning.qs (1211595614).
//Only edit until here!


#define PLUGIN_FILE_VERSION_STRING				VERSION_STRING_INTERMEDIATE(PLUGIN_FILE_VERSION_STRING_MAJOR,PLUGIN_FILE_VERSION_STRING_MINOR,PLUGIN_FILE_VERSION_STRING_REVISION,PLUGIN_FILE_VERSION_STRING_BUILD)
#define PLUGIN_PRODUCT_VERSION_STRING			VERSION_STRING_INTERMEDIATE(PLUGIN_PRODUCT_VERSION_STRING_MAJOR,PLUGIN_PRODUCT_VERSION_STRING_MINOR,PLUGIN_PRODUCT_VERSION_STRING_REVISION,PLUGIN_PRODUCT_VERSION_STRING_BUILD)
#endif // SERIALPORTDEVICEDEFINES_H




