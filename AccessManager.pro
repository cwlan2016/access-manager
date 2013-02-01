#-------------------------------------------------
#
# Project created by QtCreator 2012-04-27T16:06:38
#
#-------------------------------------------------

QT       += core gui xml xmlpatterns widgets

TARGET = AccessManager
TEMPLATE = app

RC_FILE = AccessManager.rc

SOURCES += main.cpp\
    mainwindow.cpp \
    Models/mactablemodel.cpp \
    Info/macinfo.cpp \
    snmpclient.cpp \
    constant.cpp \
    config.cpp \
    basicdialogs.cpp \
    Models/devicetablemodel.cpp \
    Models/devicetabledelegate.cpp \
    Info/switchinfo.cpp \
    Info/snmpconfiginfo.cpp \
    converters.cpp \
    Info/deviceinfo.cpp \
    devicetablehandler.cpp \
    Info/dslaminfo.cpp \
    Info/boardinfo.cpp \
    Models/boardtablemodel.cpp \
    portlistvalidator.cpp \
    Models/switchporttablemodel.cpp \
    Info/switchportinfo.cpp \
    Models/boardtabledelegate.cpp \
    Models/dslamporttablemodel.cpp \
    Info/xdslportinfo.cpp \
    Info/shdslportinfo.cpp \
    Info/adslportinfo.cpp \
    customtypes.cpp \
    Info/oltinfo.cpp \
    Pages/lineedit.cpp \
    Pages/dslampagewidget.cpp \
    Pages/pagewidget.cpp \
    Pages/switchpagewidget.cpp \
    Pages/oltpagewidget.cpp \
    customsnmpfunctions.cpp \
    Pages/editdslamboardtablepagewidget.cpp \
    Pages/aboutpagewidget.cpp \
    Pages/settingspagewidget.cpp \
    Pages/devicetablepagewidget.cpp \
    Models/onttablemodel.cpp \
    Info/ontinfo.cpp \
    Info/nteinfo.cpp \
    Info/ntpinfo.cpp

HEADERS  += mainwindow.h \
    Models/mactablemodel.h \
    Info/macinfo.h \
    snmpclient.h \
    constant.h \
    config.h \
    basicdialogs.h \
    Models/devicetablemodel.h \
    Models/devicetabledelegate.h \
    Info/switchinfo.h \
    Info/snmpconfiginfo.h \
    converters.h \
    Info/deviceinfo.h \
    devicetablehandler.h \
    Info/dslaminfo.h \
    Info/boardinfo.h \
    Models/boardtablemodel.h \
    portlistvalidator.h \
    Models/switchporttablemodel.h \
    Info/switchportinfo.h \
    Models/boardtabledelegate.h \
    Models/dslamporttablemodel.h \
    Info/xdslportinfo.h \
    Info/shdslportinfo.h \
    Info/adslportinfo.h \
    customtypes.h \
    Info/oltinfo.h \
    Pages/lineedit.h \
    Pages/dslampagewidget.h \
    Pages/pagewidget.h \
    Pages/switchpagewidget.h \
    Pages/oltpagewidget.h \
    customsnmpfunctions.h \
    Pages/editdslamboardtablepagewidget.h \
    Pages/aboutpagewidget.h \
    Pages/settingspagewidget.h \
    Pages/devicetablepagewidget.h \
    Models/onttablemodel.h \
    Info/ontinfo.h \
    Info/nteinfo.h \
    Info/ntpinfo.h

FORMS    += mainwindow.ui \
    Pages/dslampagewidget.ui \
    Pages/switchpagewidget.ui \
    Pages/oltpagewidget.ui \
    Pages/editdslamboardtablepagewidget.ui \
    Pages/aboutpagewidget.ui \
    Pages/settingspagewidget.ui \
    Pages/devicetablepagewidget.ui

RESOURCES += \
    resource.qrc

win32
{
debug {
LIBS += -lnetsnmpd
}

release {
LIBS += -lnetsnmp
}
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

HEADERS += \
    lineedit.h
