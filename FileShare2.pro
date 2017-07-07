QT += widgets core network

QT += xml
HEADERS += \
    fileshare.h \
    titlebarwidget.h \
    minimizebutton.h \
    customExitButton.h \
    buttonset.h \
    platform.h \
    profilepicture.h \
    home.h \
    NetworkSharing.h \
    WlanInterface.h \
    settings.h \
    clientviewmodel.h \
    hotspotbutton.h \
    historymodel.h \
    historyview.h \
    filewidget.h \
    clientinfo.h \
    sendfilebutton.h \
    netwokselectionbox.h \
    client.h \
    global.h \
    miniserver.h \
    writeprocess.h \
    windowsfirewall.h

SOURCES += \
    fileshare.cpp \
    titlebarwidget.cpp \
    main.cpp \
    minimizebutton.cpp \
    customExitButton.cpp \
    buttonset.cpp \
    platform.cpp \
    profilepicture.cpp \
    home.cpp \
    NetworkSharing.cpp \
    settings.cpp \
    wlaninterface.cpp \
    clientviewmodel.cpp \
    hotspotbutton.cpp \
    historymodel.cpp \
    historyview.cpp \
    filewidget.cpp \
    sendfilebutton.cpp \
    netwokselectionbox.cpp \
    client.cpp \
    miniserver.cpp \
    writeprocess.cpp \
    windowsfirewall.cpp

RESOURCES += \
    resources.qrc

FORMS += \
    home.ui \
    settings.ui

win32{
    RC_FILE = resource.rc
}
QMAKE_
DISTFILES += \
    resource.rc
