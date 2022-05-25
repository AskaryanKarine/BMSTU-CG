QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cut.cpp \
    cut.cpp \
    drawing.cpp \
    figure.cpp \
    main.cpp \
    mainwindow.cpp \
    request.cpp

HEADERS += \
    cut.h \
    drawing.h \
    figure.h \
    mainwindow.h \
    request.h \
    structurs.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc

FORMS += \
    mainwindow.ui

DISTFILES += \
    lab_09.pro.user
