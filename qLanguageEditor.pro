QT       += core gui
QT       += widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    baseutils.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# FOR STATIC BUILD ONLY!!!
QMAKE_LFLAGS_RELEASE += -static -static-libgcc -static-libstdc++
