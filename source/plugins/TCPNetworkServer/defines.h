//TCPNetworkServer 
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


#ifndef TCPNetworkServerDEFINES_H
#define TCPNetworkServerDEFINES_H
#include "./../../StimulGL/maindefines.h"

//Only edit the below!
#define PLUGIN_FILE_VERSION_STRING_MAJOR	1   //Please do not edit this line manually, see BinaryVersioning.qs (1381317323).
#define PLUGIN_FILE_VERSION_STRING_MINOR	0   //Please do not edit this line manually, see BinaryVersioning.qs (1381317323).
#define PLUGIN_FILE_VERSION_STRING_REVISION	0   //Please do not edit this line manually, see BinaryVersioning.qs (1381317323).
#define PLUGIN_FILE_VERSION_STRING_BUILD	1   //Please do not edit this line manually, see BinaryVersioning.qs (1381317323).
#define PLUGIN_PRODUCT_VERSION_STRING_MAJOR	1   //Please do not edit this line manually, see BinaryVersioning.qs (1381317323).
#define PLUGIN_PRODUCT_VERSION_STRING_MINOR	0   //Please do not edit this line manually, see BinaryVersioning.qs (1381317323).
#define PLUGIN_PRODUCT_VERSION_STRING_REVISION	0   //Please do not edit this line manually, see BinaryVersioning.qs (1381317323).
#define PLUGIN_PRODUCT_VERSION_STRING_BUILD	1   //Please do not edit this line manually, see BinaryVersioning.qs (1381317323).
#define PLUGIN_LEGAL_COPYRIGHT						"Copyright (C) 2014"
#define PLUGIN_AUTHOR_NAME							"Sven Gijsen"
#define PLUGIN_AUTHOR_EMAIL							"sven.gijsen@maastrichtuniversity.nl"
#define PLUGIN_COMPANY_NAME							"MBIC, Maastricht Brain Imaging Center"
#define PLUGIN_INTERNAL_NAME						"TCPNetworkServer"
#define PLUGIN_SCRIPTOBJECT_NAME					"TCPNetworkServer"
#define PLUGIN_PRODUCT_NAME							PLUGIN_INTERNAL_NAME
#define PLUGIN_INTERNAL_EXTENSION					"dll"
#define PLUGIN_ORIGINAL_FILENAME					PLUGIN_INTERNAL_NAME "." PLUGIN_INTERNAL_EXTENSION
#define PLUGIN_FULL_NAME							PLUGIN_INTERNAL_NAME "(v" PLUGIN_FILE_VERSION_STRING ")"
#define PLUGIN_FILE_DESCRIPTION						"The TCPNetworkServer plugin makes it possible to create and configure a TCP server in the script."
#define PLUGIN_INFORMATION							PLUGIN_INTERNAL_NAME " Plugin(v" PLUGIN_FILE_VERSION_STRING ") by " PLUGIN_AUTHOR_NAME
#define PLUGIN_MAIN_PROGRAM_MINIMAL_VERSION	"2.1.0.0"   //Please do not edit this line manually, see BinaryVersioning.qs (1381317323).
//Only edit until here!


#define PLUGIN_FILE_VERSION_STRING				VERSION_STRING_INTERMEDIATE(PLUGIN_FILE_VERSION_STRING_MAJOR,PLUGIN_FILE_VERSION_STRING_MINOR,PLUGIN_FILE_VERSION_STRING_REVISION,PLUGIN_FILE_VERSION_STRING_BUILD)
#define PLUGIN_PRODUCT_VERSION_STRING			VERSION_STRING_INTERMEDIATE(PLUGIN_PRODUCT_VERSION_STRING_MAJOR,PLUGIN_PRODUCT_VERSION_STRING_MINOR,PLUGIN_PRODUCT_VERSION_STRING_REVISION,PLUGIN_PRODUCT_VERSION_STRING_BUILD)
#endif // TCPNetworkServerDEFINES_H




