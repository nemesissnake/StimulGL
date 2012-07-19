//StimulGL
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


#ifndef MAINDEFINES_H
#define MAINDEFINES_H

#define QUOTE(name) #name
#define MAKESTRING(macro) QUOTE(macro)
#define VERSION_STRING_INTERMEDIATE(a,b,c,d) MAKESTRING(a) "." MAKESTRING(b) "." MAKESTRING(c) "." MAKESTRING(d)
//#define JOIN_STRINGS(a,b) a " - " EXPANDMACROx1(b)

//Only edit the below!
#define MAIN_PROGRAM_COMPANY_NAME						"MBIC, Maastricht Brain Imaging Center"
#define MAIN_PROGRAM_LEGAL_COPYRIGHT					"Copyright (C) 2012"
#define MAIN_PROGRAM_FILE_VERSION_STRING_MAJOR			2
#define MAIN_PROGRAM_FILE_VERSION_STRING_MINOR			0
#define MAIN_PROGRAM_FILE_VERSION_STRING_REVISION		0
#define MAIN_PROGRAM_FILE_VERSION_STRING_BUILD			1
#define MAIN_PROGRAM_PRODUCT_VERSION_STRING_MAJOR		1
#define MAIN_PROGRAM_PRODUCT_VERSION_STRING_MINOR		0
#define MAIN_PROGRAM_PRODUCT_VERSION_STRING_REVISION	0
#define MAIN_PROGRAM_PRODUCT_VERSION_STRING_BUILD		1
#define MAIN_PROGRAM_INTERNAL_NAME						"StimulGL"
#define MAIN_PROGRAM_FULL_NAME							MAIN_PROGRAM_INTERNAL_NAME "(v" MAIN_PROGRAM_FILE_VERSION_STRING ")"
#define MAIN_PROGRAM_PRODUCT_NAME                       MAIN_PROGRAM_INTERNAL_NAME
#define MAIN_PROGRAM_AUTHOR_NAME						"Sven Gijsen"
#define MAIN_PROGRAM_AUTHOR_EMAIL						"svengijsen@gmail.com"
#define MAIN_PROGRAM_ORIGINAL_FILENAME					"StimulGL.exe"
#define MAIN_PROGRAM_LOGFILE_NAME						"logfile.txt"
#define MAIN_PROGRAM_DOC_DIRNAME						"documents"
#define MAIN_PROGRAM_EXAMPLES_DIRNAME					"examples"
#define MAIN_PROGRAM_DOC_BINARYCOLLFILE					"MainDocumentation.qhc"
#define MAIN_PROGRAM_FILE_DESCRIPTION					"Open Source Cross Platform Scriptable Application"
#define MAIN_PROGRAM_FILESEXTENSION_LIST				MAIN_PROGRAM_INTERNAL_NAME "standard files (*.qs *.svg *.svgz *.svg.gz);;QT Script files (*.qs);;SVG files (*.svg *.svgz *.svg.gz);;"
#define MAIN_PROGRAM_ANYFILESEXTENSION_LIST				"Any files (*)"
#define MAIN_PROGRAM_UNTITLED_NAME						"Untitled"
#define MAIN_PROGRAM_STANDARD_DATETIME_FORMAT			"yyyyMMddHHmmsszzz"
#define MAIN_PROGRAM_DEVICE_INTERFACE					"StimulGL.Plugins.DeviceInterface/1.1"
#define MAIN_PROGRAM_EXTENSION_INTERFACE				"StimulGL.Plugins.ExtensionInterface/1.1"

//Only edit until here!
#define MAIN_PROGRAM_FILE_VERSION_STRING				VERSION_STRING_INTERMEDIATE(MAIN_PROGRAM_FILE_VERSION_STRING_MAJOR,MAIN_PROGRAM_FILE_VERSION_STRING_MINOR,MAIN_PROGRAM_FILE_VERSION_STRING_REVISION,MAIN_PROGRAM_FILE_VERSION_STRING_BUILD)
#define MAIN_PROGRAM_PRODUCT_VERSION_STRING				VERSION_STRING_INTERMEDIATE(MAIN_PROGRAM_PRODUCT_VERSION_STRING_MAJOR,MAIN_PROGRAM_PRODUCT_VERSION_STRING_MINOR,MAIN_PROGRAM_PRODUCT_VERSION_STRING_REVISION,MAIN_PROGRAM_PRODUCT_VERSION_STRING_BUILD)

#endif // MAINDEFINES_H


