project(access-manager)
cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#Use C++11 instructions
if ((${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    OR (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang"))
    add_definitions("-Wall -pedantic -std=c++0x")
endif ()

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5XmlPatterns REQUIRED)


#Project implementation files
set(SRC_FILES
    main.cpp
    basicdialogs.cpp
    config.cpp
    constant.cpp
    converters.cpp
    customtypes.cpp
    customsnmpfunctions.cpp
    portlistvalidator.cpp
    snmpclient.cpp
    devices/device.cpp
    devices/dslam.cpp
    devices/switch.cpp
    devices/olt.cpp
    devices/ont.cpp
    devices/nte.cpp
    devices/ntp.cpp
    devices/switchdes3526.cpp
    devices/switchdes3528.cpp
    devices/switchdes3550.cpp
    devices/dslamma5600.cpp
    devices/dslamma5300.cpp
    devices/dslammxa32.cpp
    devices/dslammxa64.cpp
    devices/oltlte8st.cpp
    devices/oltltp8x.cpp
    ports/switchportdes3526.cpp
    ports/switchportdes3528.cpp
    ports/switchportdes3550.cpp
    ports/switchport.cpp
    ports/xdslport.cpp
    ports/shdslport.cpp
    ports/adslport.cpp
    ports/adslportma5300.cpp
    ports/adslportma5600.cpp
    ports/adslportmxa32.cpp
    ports/adslportmxa64.cpp
    other-data/dslamboard.cpp
    other-data/mac.cpp
    models/boardtabledelegate.cpp
    models/boardtablemodel.cpp
    models/devicetabledelegate.cpp
    models/devicetablemodel.cpp
    models/dslamporttablemodel.cpp
    models/mactablemodel.cpp
    models/onttablemodel.cpp
    models/switchporttablemodel.cpp
    configs/switchconfig.cpp
    configs/snmpconfig.cpp
    gui/mainwindow.cpp
    gui/dslampagewidget.cpp
    gui/pagewidget.cpp
    gui/switchpagewidget.cpp
    gui/oltpagewidget.cpp
    gui/aboutpagewidget.cpp
    gui/settingspagewidget.cpp
    gui/devicetablepagewidget.cpp
    configs/dslamprofileconfig.cpp
    models/dslprofiletablemodel.cpp
    ports/shdslportma5300.cpp
    ports/shdslportma5600.cpp
    gui/devicepagewidget.cpp
    models/dslamporttabledelegate.cpp
    models/switchporttabledelegate.cpp
    improvedwidgets/improvedmessagewidget.cpp
    improvedwidgets/improvedlineedit.cpp
    improvedwidgets/improvedbusyindicator.cpp
    gui/xdslportpvcwidget.cpp
)

#Project headers files
set(HEADER_FILES
    config.h
    constant.h
    converters.h
    customsnmpfunctions.h
    customtypes.h
    portlistvalidator.h
    snmpclient.h
    devices/ont.h
    devices/nte.h
    devices/ntp.h
    devices/switchdes3526.h
    devices/switchdes3528.h
    devices/switchdes3550.h
    devices/dslamma5600.h
    devices/dslamma5300.h
    devices/dslammxa32.h
    devices/dslammxa64.h
    devices/oltlte8st.h
    devices/oltltp8x.h
    devices/switch.h
    devices/device.h
    devices/dslam.h
    devices/olt.h
    ports/xdslport.h
    ports/shdslport.h
    ports/adslport.h
    ports/switchport.h
    ports/switchportdes3526.h
    ports/switchportdes3528.h
    ports/switchportdes3550.h
    ports/adslportma5300.h
    ports/adslportma5600.h
    ports/adslportmxa32.h
    ports/adslportmxa64.h
    other-data/dslamboard.h
    other-data/mac.h
    models/devicetablemodel.h
    models/devicetabledelegate.h
    models/boardtablemodel.h
    models/mactablemodel.h
    models/switchporttablemodel.h
    models/boardtabledelegate.h
    models/dslamporttablemodel.h
    models/onttablemodel.h
    configs/snmpconfig.h
    configs/switchconfig.h
    gui/mainwindow.h
    gui/dslampagewidget.h
    gui/pagewidget.h
    gui/switchpagewidget.h
    gui/oltpagewidget.h
    gui/aboutpagewidget.h
    gui/settingspagewidget.h
    gui/devicetablepagewidget.h
    configs/dslamprofileconfig.h
    models/dslprofiletablemodel.h
    ports/shdslportma5300.h
    ports/shdslportma5600.h
    gui/devicepagewidget.h
    models/dslamporttabledelegate.h
    models/switchporttabledelegate.h
    improvedwidgets/improvedmessagewidget.h
    improvedwidgets/improvedlineedit.h
    improvedwidgets/improvedbusyindicator.h
    gui/xdslportpvcwidget.h
)

#Qt Resource files list
set(QRC_FILES
  resource.qrc
  improvedwidgets/improvedwidgets.qrc
)

#UI Interface files list
set(UI_FILES
  gui/aboutpagewidget.ui
  gui/devicetablepagewidget.ui
  gui/dslampagewidget.ui
  gui/mainwindow.ui
  gui/oltpagewidget.ui
  gui/settingspagewidget.ui
  gui/switchpagewidget.ui
  gui/xdslportpvcwidget.ui
)

#Add directory with custom components
include_directories(improvedwidgets)

#Generate Resource Headers
QT5_ADD_RESOURCES(RCC_HEADERS ${QRC_FILES})

#Generate UI Headers
QT5_WRAP_UI(UI_HEADERS ${UI_FILES})

#Add icon and program info in Windows build
if(WIN32)
    set(SRC_FILES ${SRC_FILES} access-manager.rc)
endif()

#Create project exe
add_executable(access-manager ${SRC_FILES} ${UI_HEADERS} ${RCC_HEADERS})

#Binding uses Qt Modules
QT5_USE_MODULES(access-manager Widgets Concurrent XmlPatterns)

#Binding uses libraries
target_link_libraries(access-manager netsnmp)

#Binding additional uses libraries in Windows
if(WIN32)
  target_link_libraries(access-manager ws2_32 advapi32)
endif()

install(TARGETS access-manager RUNTIME DESTINATION bin)
