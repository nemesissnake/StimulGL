//KeyBoardplugin
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

#include "SystemKeyboardReadWrite.h"

//SystemKeyboardReadWrite::SystemKeyboardReadWrite(QObject *parent)
//	: QObject(parent)
//{
//
//}

//SystemKeyboardReadWrite::~SystemKeyboardReadWrite()
//{
//
//}

SystemKeyboardReadWrite::SystemKeyboardReadWrite() : QObject()
{
	keyboardHook = NULL;// Assign to null
}

LRESULT CALLBACK SystemKeyboardReadWrite::keyboardProcedure(int nCode, WPARAM wParam, LPARAM lParam)
{	
	if (nCode == HC_ACTION)// Check for a key down press
	{
		//KeyBoardNameSpace::CaptureKeyMethod tmpMethod = instance()->captureMethod;
		if ((wParam == WM_KEYDOWN))// && ((tmpMethod == KeyBoardNameSpace::KeyPressed) || (tmpMethod == KeyBoardNameSpace::KeyPressedReleased)))
		{
			KBDLLHOOKSTRUCT *pKeyboard = (KBDLLHOOKSTRUCT*)lParam;
			emit SystemKeyboardReadWrite::instance()->keyPressedSignal((quint32) pKeyboard->vkCode);
		}
		else if ((wParam == WM_KEYUP)) //&& ((tmpMethod == KeyBoardNameSpace::KeyReleased) || (tmpMethod == KeyBoardNameSpace::KeyPressedReleased)))
		{			
			KBDLLHOOKSTRUCT *pKeyboard = (KBDLLHOOKSTRUCT*)lParam;
			emit SystemKeyboardReadWrite::instance()->keyReleasedSignal((quint32) pKeyboard->vkCode);
		}		
		instance()->setConnected( false );
		instance()->setConnected( true );//reconnect
	}
	return false;
}

bool SystemKeyboardReadWrite::connected()
{
	return keyboardHook;
}

bool SystemKeyboardReadWrite::setConnected(bool state)
{
	if(state && keyboardHook == NULL)
	{
		keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardProcedure, GetModuleHandle(NULL), 0);
		return keyboardHook;
	}
	else
	{
		UnhookWindowsHookEx(keyboardHook);
		keyboardHook = NULL;
		return keyboardHook;
	}
}

SystemKeyboardReadWrite* SystemKeyboardReadWrite::instance()
{
	static SystemKeyboardReadWrite* pKeyboardReadWriteInstance = new SystemKeyboardReadWrite();    
	//pKeyboardReadWriteInstance->setCaptureMethod(captureMethod);
	return pKeyboardReadWriteInstance;
}