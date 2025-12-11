QT       += core gui
QT       += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17


SOURCES += \
    databuffer.cpp \
    dataprocessor.cpp \
    main.cpp \
    mycombobox.cpp \
    serialworker.cpp \
    widget.cpp

HEADERS += \
    databuffer.h \
    dataprocessor.h \
    mycombobox.h \
    serialconfig.h \
    serialworker.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rec.qrc

RC_ICONS = recf/wave.ico
