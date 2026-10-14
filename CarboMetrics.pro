QT += widgets sql

CONFIG += c++17

SOURCES += \
    advisorysystem.cpp \
    carbonanalyzer.cpp \
    chartwidgets.cpp \
    databasemanager.cpp \
    electricityactivity.cpp \
    electricityadvisoryrule.cpp \
    foodactivity.cpp \
    foodadvisoryrule.cpp \
    footprintanalyzer.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    resultdialog.cpp \
    transportactivity.cpp \
    transportadvisoryrule.cpp \
    user.cpp

HEADERS += \
    activity.h \
    advisoryrule.h \
    advisorysystem.h \
    carbonanalyzer.h \
    chartwidgets.h \
    databasemanager.h \
    electricityactivity.h \
    electricityadvisoryrule.h \
    emissionrecord.h \
    foodactivity.h \
    foodadvisoryrule.h \
    footprintanalyzer.h \
    loginwindow.h \
    mainwindow.h \
    resultdialog.h \
    theme.h \
    transportactivity.h \
    transportadvisoryrule.h \
    user.h

FORMS += \
    mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target