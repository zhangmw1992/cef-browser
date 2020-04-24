QT += core

CONFIG += c++11

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
        APP_PATH = $$PWD/../bin/win/debug
        APP_NAME = cef_rendererd

    } else {
        APP_PATH = $$PWD/../bin/win/release
        APP_NAME = cef_renderer
    }
}

macx {
    INCLUDEPATH += \
        $$PWD/../3rdparty/lib-mac/cef/release/include

    LIBS += \
        -L$$PWD/../3rdparty/lib-mac/cef/release/lib -lcef_dll_wrapper -F$$PWD/../3rdparty/lib-mac/cef/release/lib -framework "Chromium Embedded Framework"

    CONFIG(debug, debug|release) {
        APP_PATH = $$PWD/../bin/macx/debug
        APP_NAME = cef_rendererd

    } else {
        APP_PATH = $$PWD/../bin/macx/release
        APP_NAME = "cef_renderer Helper"
    }
}

include(./default_delegates/default_delegates.pri)

DESTDIR = $$APP_PATH
TARGET = $$APP_NAME

HEADERS += \
    client_renderer.h \
    web_define.h

SOURCES += \
    client_renderer.cpp \
    main.cpp
