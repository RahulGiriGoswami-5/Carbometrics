QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    carbonanalyzer.cpp \
    electricityactivity.cpp \
    foodactivity.cpp \
    main.cpp \
    mainwindow.cpp \
    transportactivity.cpp \
    user.cpp

HEADERS += \
    activity.h \
    carbonanalyzer.h \
    electricityactivity.h \
    foodactivity.h \
    mainwindow.h \
    transportactivity.h \
    user.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
