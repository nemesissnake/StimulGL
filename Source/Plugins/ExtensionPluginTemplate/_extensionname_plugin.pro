# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = lib
QT += core gui script
DEFINES += _WINDOWS QT_LARGEFILE_SUPPORT QT_DLL QT_THREAD_SUPPORT
INCLUDEPATH += ./debug \
    . \
    ./GeneratedFiles
DEPENDPATH += .
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
RC_FILE = $$PWD/_extensionname_plugin.rc
QMAKE_RC = rc -D_MSC_VER
include(_extensionname_plugin.pri)

contains(QMAKE_HOST.os,Windows){
    contains(QMAKE_HOST.arch,x86_64){
        #message("$$QMAKE_HOST.arch on $$QMAKE_HOST.os")
        CONFIG(debug, debug|release) {
            DESTDIR = x64/Debug#"$$PWD/x64_Debug"
            TARGET = _extensionname_plugin_x64d
            INCLUDEPATH += x64/Debug
            MOC_DIR += x64/Debug
            OBJECTS_DIR += x64/Debug
            #message("x64_Debug")
        }
        CONFIG(release, debug|release) {
            DESTDIR = x64/Release#"$$PWD/x64_Release"
            TARGET = _extensionname_plugin_x64
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
                TARGET = _extensionname_plugin_Win32d
                INCLUDEPATH += Win32/Debug
                MOC_DIR += Win32/Debug
                OBJECTS_DIR += Win32/Debug
                #message("Win32_Debug")
            }
            CONFIG(release, debug|release) {
                DESTDIR = Win32/Release#"$$PWD/Win32_Release"
                TARGET = _extensionname_plugin_Win32
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

HEADERS += \
    resource.h
