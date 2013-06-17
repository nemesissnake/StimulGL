REM This command takes the current directory and moves 3 levels up the path.
cd /d %~dp0..\..\..\

REM This command starts StimulGL with the '-f' argument allowing you to let it automatically open one or more file(s),
REM providing the path(s) seperated with a ';' character.
Call StimulGL -f examples\GettingStartedGuide\BasicScripting\HelloWorld.qs;examples\GettingStartedGuide\BasicScripting\FunctionsAndIncludes1.qs