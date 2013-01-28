@echo off

:START
SET DEBUGRELEASE=
SET ARCHITECTURE=

SET /p DEBUGRELEASE= Start the (d)ebug or (r)elease version?
SET /p ARCHITECTURE= Start the win32(1) or the x64(2) version?

ECHO Your input was: %ARCHITECTURE%
IF /I '%ARCHITECTURE%'=='1' GOTO WIN32
IF /I '%ARCHITECTURE%'=='2' GOTO X64
GOTO START

:WIN32
	ECHO WIN32 was choosen
	SET QT_QPA_PLATFORM_PLUGIN_PATH=C:\Qt\5.0.0\win32\plugins\platforms
	SET Path=C:\Qt\5.0.0\win32\bin;%Path%
	:: SET Path=C:\Qt\5.0.0\win32\plugins\platforms;%Path%
	IF /I '%DEBUGRELEASE%'=='d' StimulGL_win32_Debug.exe
	IF /I '%DEBUGRELEASE%'=='r' StimulGL_win32_Release.exe
	GOTO START
	
:X64
	ECHO X64 was choosen
	SET QT_QPA_PLATFORM_PLUGIN_PATH=C:\Qt\5.0.0\x64\plugins\platforms
	SET Path=C:\Qt\5.0.0\x64\bin;%Path%
	:: SET Path=C:\Qt\5.0.0\x64\plugins\platforms;%Path%
	IF /I '%DEBUGRELEASE%'=='d' StimulGL_x64_Debug.exe
	IF /I '%DEBUGRELEASE%'=='r' StimulGL_x64_Release.exe
	GOTO START

