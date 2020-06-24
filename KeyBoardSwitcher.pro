#-------------------------------------------------
#
# Project created by QtCreator 2019-07-01T12:07:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KeyBoardSwitcher

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QMAKE_LFLAGS += -F/System/Library/Frameworks/
LIBS += -framework AppKit
LIBS += -framework Carbon
LIBS += -framework Foundation
LIBS += -framework ApplicationServices

CONFIG += c++17

SOURCES += \
        adminaccessinstructions.cpp \
        main.cpp \
        mainwindow.cpp \
        switchKeyBoard.mm \
        utilities.cpp

HEADERS += \
        adminaccessinstructions.h \
        mainwindow.h \
        switchkeyboard.h \
        utilities.h

FORMS += \
        adminaccessinstructions.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    icon.png

RESOURCES += \
    imgs.qrc

QMAKE_INFO_PLIST = $$PWD/Info.plist
    plist.target = ".$${DESTDIR}/$${TARGET}.app/Contents/Info.plist"
    plist.depends = $$PWD/Info.plist
    plist.commands = $(DEL_FILE) \".$${DESTDIR}/$${TARGET}.app/Contents/Info.plist\" $$escape_expand(\n\t) \
                     $(COPY_FILE) $$PWD/Info.plist \".$${DESTDIR}/$${TARGET}.app/Contents/Info.plist\"
    QMAKE_EXTRA_TARGETS = plist
    PRE_TARGETDEPS += $$plist.target

ICON = logo.icns
