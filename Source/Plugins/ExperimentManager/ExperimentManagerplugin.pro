# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = lib
QT += core gui script
DEFINES += _WINDOWS QT_LARGEFILE_SUPPORT QT_DLL QT_THREAD_SUPPORT
INCLUDEPATH += ./debug \
    . \
    $(QTDIR)/mkspecs/win32-msvc2008 \
    ./GeneratedFiles
DEPENDPATH += .
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(ExperimentManagerplugin.pri)

contains(QMAKE_HOST.os,Windows){
    contains(QMAKE_HOST.arch,x86_64){
        #message("$$QMAKE_HOST.arch on $$QMAKE_HOST.os")
        CONFIG(debug, debug|release) {
            DESTDIR = x64/Debug#"$$PWD/x64_Debug"
            TARGET = ExperimentManagerplugin_x64_Debug
            INCLUDEPATH += x64/Debug
            MOC_DIR += x64/Debug
            OBJECTS_DIR += x64/Debug
            #message("x64_Debug")
        }
        CONFIG(release, debug|release) {
            DESTDIR = x64/Release#"$$PWD/x64_Release"
            TARGET = ExperimentManagerplugin_x64_Release
            INCLUDEPATH += x64/Release
            MOC_DIR += x64/Release
            OBJECTS_DIR += x64/Release
            #message("x64_Release")
        }
    }
    else{
        contains(QMAKE_HOST.arch,x86){
            #message("$$QMAKE_HOST.arch on $$QMAKE_HOST.os")
            CONFIG(debug, debug|release) {
                DESTDIR = Win32/Debug#"$$PWD/Win32_Debug"
                TARGET = ExperimentManagerplugin_Win32_Debug
                INCLUDEPATH += Win32/Debug
                MOC_DIR += Win32/Debug
                OBJECTS_DIR += Win32/Debug
                #message("Win32_Debug")
            }
            CONFIG(release, debug|release) {
                DESTDIR = Win32/Release#"$$PWD/Win32_Release"
                TARGET = ExperimentManagerplugin_Win32_Release
                INCLUDEPATH += Win32/Release
                MOC_DIR += Win32/Release
                OBJECTS_DIR += Win32/Release
                #message("Win32_Release")
            }
        }
        else{
            error("Your OS architecture is $$QMAKE_HOST.arch .The library for this architecture hasn't been built")
        }
    }
}
else:error("Your OS is $$QMAKE_HOST.os . The library for this OS hasn't been built.")
