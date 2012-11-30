#-------------------------------------------------
#
# Project created by QtCreator 2012-04-27T16:06:38
#
#-------------------------------------------------

QT       += core gui xml xmlpatterns

TARGET = AccessManager
TEMPLATE = app

RC_FILE = AccessManager.rc

PRECOMPILED_HEADER = stdafx.h

QMAKE_CXXFLAGS+="-std=c++11"

QMAKE_CXXFLAGS_RELEASE+="-O2"

SOURCES += main.cpp\
    mainwindow.cpp \
    Models/maclistmodel.cpp \
    Info/macinfo.cpp \
    stdafx.cpp \
    snmpclient.cpp \
    constant.cpp \
    config.cpp \
    basicdialogs.cpp \
    Models/devicelistmodel.cpp \
    Models/devicelistdelegate.cpp \
    Info/switchinfo.cpp \
    Info/snmpconfiginfo.cpp \
    converters.cpp \
    Info/deviceinfo.cpp \
    devicelisthandler.cpp \
    Info/dslaminfo.cpp \
    Info/boardinfo.cpp \
    Models/boardlistmodel.cpp \
    portlistvalidator.cpp \
    Models/switchportlistmodel.cpp \
    Info/switchportinfo.cpp \
    Models/boardlistdelegate.cpp \
    Models/dslamportlistmodel.cpp \
    Info/xdslportinfo.cpp \
    Info/shdslportinfo.cpp \
    Info/adslportinfo.cpp \
    customtypes.cpp \
    Info/oltinfo.cpp \
    lineedit.cpp \
    Pages/dslampagewidget.cpp \
    Pages/pagewidget.cpp \
    Pages/switchpagewidget.cpp \
    Pages/oltpagewidget.cpp \
    customsnmpfunctions.cpp \
    Pages/editdslamboardlistpagewidget.cpp \
    Pages/aboutpagewidget.cpp \
    Pages/settingspagewidget.cpp \
    Pages/devicelistpagewidget.cpp

HEADERS  += mainwindow.h \
    Models/maclistmodel.h \
    Info/macinfo.h \
    stdafx.h \
    snmpclient.h \
    constant.h \
    config.h \
    basicdialogs.h \
    Models/devicelistmodel.h \
    Models/devicelistdelegate.h \
    Info/switchinfo.h \
    Info/snmpconfiginfo.h \
    converters.h \
    Info/deviceinfo.h \
    devicelisthandler.h \
    Info/dslaminfo.h \
    Info/boardinfo.h \
    Models/boardlistmodel.h \
    portlistvalidator.h \
    Models/switchportlistmodel.h \
    Info/switchportinfo.h \
    Models/boardlistdelegate.h \
    Models/dslamportlistmodel.h \
    Info/xdslportinfo.h \
    Info/shdslportinfo.h \
    Info/adslportinfo.h \
    customtypes.h \
    Info/oltinfo.h \
    lineedit.h \
    Pages/dslampagewidget.h \
    Pages/pagewidget.h \
    Pages/switchpagewidget.h \
    Pages/oltpagewidget.h \
    customsnmpfunctions.h \
    Pages/editdslamboardlistpagewidget.h \
    Pages/aboutpagewidget.h \
    Pages/settingspagewidget.h \
    Pages/devicelistpagewidget.h

FORMS    += mainwindow.ui \
    Pages/dslampagewidget.ui \
    Pages/switchpagewidget.ui \
    Pages/oltpagewidget.ui \
    Pages/editdslamboardlistpagewidget.ui \
    Pages/aboutpagewidget.ui \
    Pages/settingspagewidget.ui \
    Pages/devicelistpagewidget.ui

RESOURCES += \
    resource.qrc

win32:debug {
LIBS += -lnetsnmpd
}

win32:release {
LIBS += -lnetsnmp
}

win32 {
LIBS += -lwsock32
LIBS += -ladvapi32
}

OTHER_FILES += \
    images/up16.png \
    images/table16.png \
    images/splash.png \
    images/save.png \
    images/remove16.png \
    images/refresh.png \
    images/info16.png \
    images/edit16.png \
    images/down16.png \
    images/configure.png \
    images/add16.png \
    images/yes16.png \
    images/up16.png \
    images/table16.png \
    images/splash.png \
    images/save.png \
    images/remove16.png \
    images/refresh.png \
    images/prev16.png \
    images/no16.png \
    images/info16.png \
    images/edit16.png \
    images/down16.png \
    images/configure.png \
    images/add16.png \
    images/open16.png \
    images/yes16.png \
    images/up16.png \
    images/tv.png \
    images/table16.png \
    images/splash.png \
    images/save.png \
    images/remove16.png \
    images/refresh.png \
    images/prev16.png \
    images/open16.png \
    images/no16.png \
    images/info16.png \
    images/edit16.png \
    images/down16.png \
    images/configure.png \
    images/add16.png \
    Data.ico \
    AccessManager.rc \
    images/clear_left.png \
    SchemaDeviceList.xsd
