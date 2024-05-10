QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CC = gcc-13
QMAKE_CXX = g++-13
CONFIG += c++20

message($$QT_VERSION)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    HabTableView.cpp \
    HabitDelegate.cpp \
    HabitDialog.cpp \
    ImportDialog.cpp \
    ListViewCustom.cpp \
    ReccurenceDialog.cpp \
    SoundPlayer.cpp \
    StatusProxyModel.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    HabTableView.hpp \
    HabitDelegate.hpp \
    HabitDialog.hpp \
    ImportDialog.hpp \
    ListViewCustom.hpp \
    MainWindow.hpp \
    ReccurenceDialog.hpp \
    SoundPlayer.hpp \
    StatusProxyModel.hpp

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../app_core/release/ -lapp_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../app_core/debug/ -lapp_core
else:unix: LIBS += -L$$OUT_PWD/../app_core/ -lapp_core

INCLUDEPATH += $$PWD/../app_core
DEPENDPATH += $$PWD/../app_core

RESOURCES += \
	resource.qrc
