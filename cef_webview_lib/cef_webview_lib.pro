QT       += core widgets

TEMPLATE = lib
DEFINES += WEBVIEW_LIBRARY

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32 {
    INCLUDEPATH += \
        $$PWD/../3rdparty/lib-win/cef/release/include

    LIBS += \
        -L$$PWD/../3rdparty/lib-win/cef/release/lib -llibcef_dll_wrapper -llibcef

    CONFIG(debug, debug|release) {
        APP_PATH = $$PWD/../3rdparty/lib-win/webview/debug/lib
        APP_NAME = web_viewd

    } else {
        APP_PATH = $$PWD/../3rdparty/lib-win/webview/release/lib
        APP_NAME = web_view
    }
}

macx {
    INCLUDEPATH += \
        $$PWD/../3rdparty/lib-mac/cef/include

    LIBS += \
        $$PWD/../3rdparty/lib-mac/cef/release/lib/cef_sandbox.a \
        -L$$PWD/../3rdparty/lib-mac/cef/release/lib -lcef_dll_wrapper -F$$PWD/../3rdparty/lib-mac/cef/release/lib -framework "Chromium Embedded Framework"

    CONFIG(debug, debug|release) {
        APP_PATH = $$PWD/../3rdparty/lib-mac/webview/debug/lib
        APP_NAME = web_viewd

    } else {
        APP_PATH = $$PWD/../3rdparty/lib-mac/webview/release/lib
        APP_NAME = web_view
    }
}

include(./default_delegates/default_delegates.pri)
include(./client/client.pri)

DESTDIR = $$APP_PATH
TARGET = $$APP_NAME

HEADERS += \
    web_define.h \
    web_view.h \
    web_view_global.h

SOURCES += \
    web_view.cpp
