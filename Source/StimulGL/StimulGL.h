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


#ifndef STIMULGL_H
#define STIMULGL_H

#define STIMULGL_DEFAULT_WINDOW_BACKGROUND_COLOR_RED	235
#define STIMULGL_DEFAULT_WINDOW_BACKGROUND_COLOR_GREEN	235
#define STIMULGL_DEFAULT_WINDOW_BACKGROUND_COLOR_BLUE	235

#define FUNC_PLUGIN_GETSCRIPTMETAOBJECT				"GetScriptMetaObject"
#define FUNC_PLUGIN_GETSCRIPTMETAOBJECT_FULL		FUNC_PLUGIN_GETSCRIPTMETAOBJECT "(int)"
#define FUNC_PLUGIN_GETSCRIPTMETAOBJECTCOUNT		"GetScriptMetaObjectCount"
#define FUNC_PLUGIN_GETSCRIPTMETAOBJECTCOUNT_FULL	FUNC_PLUGIN_GETSCRIPTMETAOBJECTCOUNT "()"
#define FUNC_PLUGIN_ISCOMPATIBLE					"IsCompatible"
#define FUNC_PLUGIN_ISCOMPATIBLE_FULL				FUNC_PLUGIN_ISCOMPATIBLE "()"
#define FUNC_PLUGIN_GETADDFILEEXT					"GetAdditionalFileExtensions"
#define FUNC_PLUGIN_GETADDFILEEXT_FULL				FUNC_PLUGIN_GETADDFILEEXT "()"
#define FUNC_PLUGIN_GETADDFILE_SLOT_HANDLERS		"GetAdditionalFileSlotHandlers"
#define FUNC_PLUGIN_GETADDFILE_SLOT_HANDLERS_FULL	FUNC_PLUGIN_GETADDFILE_SLOT_HANDLERS "()"
#define FUNC_PLUGIN_GETADDFILE_TYPESTYLE			"GetAdditionalFileTypeStyle"
#define FUNC_PLUGIN_GETADDFILE_TYPESTYLE_FULL		FUNC_PLUGIN_GETADDFILE_TYPESTYLE "(QString)"
#define FUNC_PLUGIN_GETADDFILE_API_FILENAME			"GetAdditionalFileTypeApiName"
#define FUNC_PLUGIN_GETADDFILE_API_FILENAME_FULL	FUNC_PLUGIN_GETADDFILE_API_FILENAME "(QString)"

#endif // STIMULGL_H
//#ifdef Q_WS_WIN
//#ifdef Q_WS_X11
//#ifdef Q_WS_MACX