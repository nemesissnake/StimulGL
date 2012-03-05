REM The help project file is now finished. If you want to see the resulting documentation in Qt Assistant, you have to generate a Qt compressed help file out of it and register it with the default help collection of Qt Assistant:
REM qhelpgenerator MainDocumentation.qhp -o MainDocumentation.qch
REM assistant -register MainDocumentation.qch

REM As the last step, we have to generate the binary collection file out of the help collection project file. This is done by running the qcollectiongenerator tool:

qcollectiongenerator MainDocumentation.qhcp -o MainDocumentation.qhc

REM Copy all the documentation to the StimulGL repo
xcopy "MainDocumentation.qhc" "..\StimulGL\Install\documents\" /i /d /y

REM To test all our customizations made to Qt Assistant, we add the collection file name to the command line:
assistant -collectionFile MainDocumentation.qhc


