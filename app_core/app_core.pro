QT -= gui
QT += sql

TEMPLATE = lib
DEFINES += APP_CORE_LIBRARY

QMAKE_CC = gcc-13
QMAKE_CXX = g++-13
CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DatabaseManager.cpp \
    Habit.cpp \
    HabitDao.cpp \
    HabitModel.cpp \
    ImportLoader.cpp \
    Utility.cpp

HEADERS += \
    Constants.hpp \
    DatabaseManager.hpp \
    HabitDao.hpp \
    HabitModel.hpp \
    ImportLoader.hpp \
    Utility.hpp \
    app_core_global.hpp \
    Habit.hpp

# Default rules for deployment.
#unix {
#    target.path = /usr/lib
#}
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
