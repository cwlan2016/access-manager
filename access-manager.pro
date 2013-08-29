#-------------------------------------------------
#
# Project created by QtCreator 2012-04-27T16:06:38
#
#-------------------------------------------------

QT       += core gui xmlpatterns widgets

TARGET = access-manager
TEMPLATE = app

PRECOMPILED_HEADER = stdafx.h

precompile_header:!isEmpty(PRECOMPILED_HEADER) {
 DEFINES += USING_PCH
}

gcc*:QMAKE_CXXFLAGS+="-std=c++11"
clang*:QMAKE_CXXFLAGS+="-std=c++11"

RC_FILE = access-manager.rc

INCLUDEPATH += $$PWD

SOURCES += main.cpp \    
    basicdialogs.cpp \
    config.cpp \
    constant.cpp \
    converters.cpp \
    customtypes.cpp \
    customsnmpfunctions.cpp \        
    portlistvalidator.cpp \    
    snmpclient.cpp \
    devices/device.cpp \
    devices/dslam.cpp \
    devices/switch.cpp \
    devices/olt.cpp \    
    devices/ont.cpp \
    devices/nte.cpp \
    devices/ntp.cpp \
    devices/switchdes3526.cpp \
    devices/switchdes3528.cpp \
    devices/switchdes3550.cpp \
    devices/dslamma5600.cpp \
    devices/dslamma5300.cpp \
    devices/dslammxa32.cpp \
    devices/dslammxa64.cpp \
    devices/oltlte8st.cpp \
    devices/oltltp8x.cpp \
    ports/switchportdes3526.cpp \
    ports/switchportdes3528.cpp \
    ports/switchportdes3550.cpp \
    ports/switchport.cpp \    
    ports/xdslport.cpp \
    ports/shdslport.cpp \
    ports/adslport.cpp \
    ports/adslportma5300.cpp \
    ports/adslportma5600.cpp \
    ports/adslportmxa32.cpp \
    ports/adslportmxa64.cpp \    
    other-data/dslamboard.cpp \
    other-data/mac.cpp \
    models/boardtabledelegate.cpp \
    models/boardtablemodel.cpp \
    models/devicetabledelegate.cpp \
    models/devicetablemodel.cpp \
    models/dslamporttablemodel.cpp \
    models/mactablemodel.cpp \
    models/onttablemodel.cpp \
    models/switchporttablemodel.cpp \
    configs/switchconfig.cpp \
    configs/snmpconfig.cpp \
    gui/mainwindow.cpp \
    gui/lineedit.cpp \
    gui/dslampagewidget.cpp \
    gui/pagewidget.cpp \
    gui/switchpagewidget.cpp \
    gui/oltpagewidget.cpp \    
    gui/aboutpagewidget.cpp \
    gui/settingspagewidget.cpp \
    gui/devicetablepagewidget.cpp \
    configs/dslamprofileconfig.cpp \
    models/dslprofiletablemodel.cpp \
    ports/shdslportma5300.cpp \
    ports/shdslportma5600.cpp \
    gui/devicepagewidget.cpp \
    models/dslamporttabledelegate.cpp \
    models/switchporttabledelegate.cpp

HEADERS  += basicdialogs.h \
    config.h \
    constant.h \
    converters.h \
    customsnmpfunctions.h \
    customtypes.h \
    portlistvalidator.h \
    snmpclient.h \   
    devices/ont.h \
    devices/nte.h \
    devices/ntp.h \
    devices/switchdes3526.h \
    devices/switchdes3528.h \
    devices/switchdes3550.h \
    devices/dslamma5600.h \
    devices/dslamma5300.h \
    devices/dslammxa32.h \
    devices/dslammxa64.h \
    devices/oltlte8st.h \
    devices/oltltp8x.h \
    devices/switch.h \
    devices/device.h \
    devices/dslam.h \
    devices/olt.h \
    ports/xdslport.h \
    ports/shdslport.h \
    ports/adslport.h \  
    ports/switchport.h \
    ports/switchportdes3526.h \
    ports/switchportdes3528.h \
    ports/switchportdes3550.h \
    ports/adslportma5300.h \
    ports/adslportma5600.h \
    ports/adslportmxa32.h \
    ports/adslportmxa64.h \
    other-data/dslamboard.h \
    other-data/mac.h \
    models/devicetablemodel.h \
    models/devicetabledelegate.h \
    models/boardtablemodel.h \
    models/mactablemodel.h \	
    models/switchporttablemodel.h \
    models/boardtabledelegate.h \
    models/dslamporttablemodel.h \
    models/onttablemodel.h \
    configs/snmpconfig.h \
    configs/switchconfig.h \
    gui/mainwindow.h \
    gui/lineedit.h \
    gui/dslampagewidget.h \
    gui/pagewidget.h \
    gui/switchpagewidget.h \
    gui/oltpagewidget.h \
    gui/aboutpagewidget.h \
    gui/settingspagewidget.h \
    gui/devicetablepagewidget.h \ 
    lineedit.h \
    configs/dslamprofileconfig.h \
    models/dslprofiletablemodel.h \
    ports/shdslportma5300.h \
    ports/shdslportma5600.h \
    gui/devicepagewidget.h \
    models/dslamporttabledelegate.h \
    models/switchporttabledelegate.h
    
FORMS += gui/mainwindow.ui \
    gui/dslampagewidget.ui \
    gui/switchpagewidget.ui \
    gui/oltpagewidget.ui \
    gui/aboutpagewidget.ui \
    gui/settingspagewidget.ui \
    gui/devicetablepagewidget.ui

RESOURCES += \
    resource.qrc

win32:debug:LIBS += -lnetsnmpd
win32:release:LIBS += -lnetsnmp
!win32:LIBS += -lnetsnmp

win32:LIBS+= -lwsock32
win32:LIBS+= -ladvapi32

OTHER_FILES += \
    images/up16.png \
    images/info16.png \
    images/yes16.png \
    images/tv.png \
    images/splash.png \
    images/save.png \
    images/remove16.png \
    images/refresh.png \
    images/prev16.png \
    images/open16.png \
    images/no16.png \
    images/edit16.png \
    images/down16.png \
    images/configure.png \
    images/add16.png \
    images/clear_left.png \
    data.ico \
    access-manager.rc \
    schemadevicelist.xsd \
    images/edit2.png \
    images/node.png \
    images/profile.png

HEADERS += \
    stdafx.h

SOURCES += \
    stdafx.cpp
