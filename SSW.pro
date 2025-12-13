QT       += core gui
QT       += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

TARGET = SSWCOM

VERSION = 2.1.1

SOURCES += \
    appsettings.cpp \
    databuffer.cpp \
    dataprocessor.cpp \
    keywordhighlighter.cpp \
    main.cpp \
    mycombobox.cpp \
    serialworker.cpp \
    speedmonitor.cpp \
    widget.cpp

HEADERS += \
    appsettings.h \
    databuffer.h \
    dataprocessor.h \
    keywordhighlighter.h \
    mycombobox.h \
    serialconfig.h \
    serialworker.h \
    speedmonitor.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    font.qrc \
    rec.qrc

RC_ICONS = recf/wave.ico
