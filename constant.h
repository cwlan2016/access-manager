#ifndef CONSTANT_H
#define CONSTANT_H

#include "QtCore/QString"
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

const QString VlanStateName[] =     { "Untag", "Tag", "None" };
const QString LtpOntState[] =       { "Free", "Allocated", "AuthInProgress", "AuthFailed", "AuthOk", "CfgInProgress", "CfgFailed", "Ok", "Failed", "Blocked", "Mibreset", "Preconfig", "FwUpdating", "Unactivated", "Redundant", "Disabled", "Unknown" };
const QString LteOntState[] =       { "Free", "Allocated", "AuthInProgress", "CfgInProgress", "AuthFailed", "CfgFailed", "ReportTimeout", "Ok", "AuthOk", "ResetInProgress", "ResetOk", "Discovered", "Blocked" };
const QString LteOntType[] =        { "", "", "Nte-2c", "Nte-rg-1400f", "Nte-rg-1400g", "Nte-rg-1400f-w", "Nte-rg-1400g-w", "Nte-rg-1400fc", "Nte-rg-1400gc", "Nte-rg-1400fc-w", "Nte-rg-1400gc-w", "Nte-rg-1402f", "Nte-rg-1402g", "Nte-rg-1402f-w", "Nte-rg-1402g-w", "Nte-rg-1402fc", "Nte-rg-1402gc", "Nte-rg-1402fc-w", "Nte-rg-1402gc-w" };

namespace Mib
{
const QString ifName =                                  ".1.3.6.1.2.1.31.1.1.1.1.";
const QString ifDescr =                                 ".1.3.6.1.2.1.2.2.1.2.";                 // .index = Description
const QString ifAdminStatus =                           ".1.3.6.1.2.1.2.2.1.7.";
const QString ifOperStatus =                            ".1.3.6.1.2.1.2.2.1.8.";
const QString ifLastChange =                            ".1.3.6.1.2.1.2.2.1.9.";
const QString ifAlias =                                 ".1.3.6.1.2.1.31.1.1.1.18.";             // .index = Alias
const QString swL2PortInfoNwayStatusDES3526 =           ".1.3.6.1.4.1.171.11.64.1.2.4.1.1.5.";   // .index = Speed/Duplex
const QString swL2PortInfoNwayStatusDES3550 =           ".1.3.6.1.4.1.171.11.64.2.2.4.1.1.5.";   // .index = Speed/Duplex
const QString swL2PortInfoNwayStatusDES3528 =           ".1.3.6.1.4.1.171.11.105.1.2.3.1.1.6.";  // .index = Speed/Duplex
const QString dot1qTpFdbPort =                          ".1.3.6.1.2.1.17.7.1.2.2.1.2.";          // .vlanid.mac = portNumber
const QString swL2IGMPMulticastVlanMemberPortDES3526 =  ".1.3.6.1.4.1.171.11.64.1.2.10.6.1.4.";  // .vlanid = mask for ports
const QString swL2IGMPMulticastVlanMemberPortDES3550 =  ".1.3.6.1.4.1.171.11.64.2.2.10.6.1.4.";  // .vlanid = mask for ports
const QString swL2IGMPMulticastVlanMemberPortDES3528 =  ".1.3.6.1.4.1.171.12.64.3.1.1.4.";       // .vlanid = mask for ports
const oid   dot1qVlanStaticName[] =                     { 1, 3, 6, 1, 2, 1, 17, 7, 1, 4, 3, 1, 1 };          // .index = VlanName
const QString dot1qVlanStaticEgressPorts =              ".1.3.6.1.2.1.17.7.1.4.3.1.2.";          // .vlanid = mask for ports in vlan
const QString dot1qVlanStaticUntaggedPorts =            ".1.3.6.1.2.1.17.7.1.4.3.1.4.";          // .vlanid = mask for ports untag ports in vlan
const QString agentStatusSaveCfg =                      ".1.3.6.1.4.1.171.12.1.1.4.0";           // value proceeding(2) - current make. completed(3) failed (4)
// DES-3526 value 3
// DES-3528 value 5
const QString agentSaveCfg =                            ".1.3.6.1.4.1.171.12.1.2.6.0";           // value set(3) for save switch config
//информация по Dslam'у MA5600
const QString dslam_board_name =                        ".1.3.6.1.4.1.2011.2.6.7.1.1.2.1.7";
//MIBs для MXA-64
const QString mxa64DslPortName =                        ".1.3.6.1.4.1.35265.1.33.10.2.1.2.";
const QString mxa64DslPortActiveProfile =               ".1.3.6.1.4.1.35265.1.33.10.2.1.6.";
const QString mxa64DslPortOperStatus =                  ".1.3.6.1.4.1.35265.1.33.10.2.1.3.";
const QString mxa64DslPortAdminStatus =                 ".1.3.6.1.4.1.35265.1.33.10.2.1.4.";
const QString mxa64DslBandActualRateTx =                ".1.3.6.1.4.1.35265.1.33.10.3.1.3.2.";
const QString mxa64DslBandActualRateRx =                ".1.3.6.1.4.1.35265.1.33.10.3.1.3.1.";
const QString mxa64DslBandLineAttenuationTx =           ".1.3.6.1.4.1.35265.1.33.10.3.1.6.2.";
const QString mxa64DslBandLineAttenuationRx =           ".1.3.6.1.4.1.35265.1.33.10.3.1.6.1.";
//MIBs для MXA-32
const QString mxa32DslPortName =                        ".1.3.6.1.4.1.35265.1.28.10.2.1.2.";
const QString mxa32DslPortActiveProfile =               ".1.3.6.1.4.1.35265.1.28.10.2.1.6.";
const QString mxa32DslPortOperStatus =                  ".1.3.6.1.4.1.35265.1.28.10.2.1.3.";
const QString mxa32DslPortAdminStatus =                 ".1.3.6.1.4.1.35265.1.28.10.2.1.4.";
const QString mxa32DslBandActualRateTx =                ".1.3.6.1.4.1.35265.1.28.10.3.1.3.2.";
const QString mxa32DslBandActualRateRx =                ".1.3.6.1.4.1.35265.1.28.10.3.1.3.1.";
const QString mxa32DslBandLineAttenuationTx =           ".1.3.6.1.4.1.35265.1.28.10.3.1.6.2.";
const QString mxa32DslBandLineAttenuationRx =           ".1.3.6.1.4.1.35265.1.28.10.3.1.6.1.";
//параметры линии
const QString adslLineCoding =                          ".1.3.6.1.2.1.10.94.1.1.1.1.1.";
const QString adslLineType =                            ".1.3.6.1.2.1.10.94.1.1.1.1.2.";
const QString adslLineConfProfile =                     ".1.3.6.1.2.1.10.94.1.1.1.1.4.";
const QString adslAtucChanCurrTxRate =                  ".1.3.6.1.2.1.10.94.1.1.4.1.2."; //скорость нисходящего канала
const QString adslAturChanCurrTxRate =                  ".1.3.6.1.2.1.10.94.1.1.5.1.2."; //скорость восходящего канала
const QString adslAtucCurrAtn =                         ".1.3.6.1.2.1.10.94.1.1.2.1.5."; //затухание нисходящего канала
const QString adslAturCurrAtn =                         ".1.3.6.1.2.1.10.94.1.1.3.1.5."; //затухание восходящего канала
//MIBs для LTP-8X
//last 8 numbers is serial of device in decimal representation
const oid ltp8xONTConfigSerial[] =                      { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 4, 1, 2, 1, 8, 0, 0, 0, 0, 0, 0, 0, 0};
const oid ltp8xONTStateEquipmentID[] =                  { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 1, 1, 12, 1, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTACSUserID[] =                         { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 15, 1, 2, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTStateState[] =                        { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 1, 1, 5, 1, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTConfigServicesProfile[] =             { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 4, 1, 5, 1, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTConfigMulticastProfile[] =            { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 4, 1, 10, 1, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTACSConfigPPPLogin[] =                 { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 15, 1, 11, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTACSConfigPPPPassword[] =              { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 15, 1, 12, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTACSConfigVoice1Enable[] =             { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 15, 1, 4, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTACSConfigVoice1Number[] =             { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 15, 1, 5, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTACSConfigVoice1Password[] =           { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3 , 15, 1, 6, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTACSConfigVoice2Enable[] =             { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 15, 1, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTACSConfigVoice2Number[] =             { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 15, 1, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTACSConfigVoice2Password[] =           { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 15, 1, 9, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTACSConfigSIPProxy[] =                 { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 15, 1, 10, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTStateReconfigure[] =                  { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 1, 1, 20, 1, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTStateReset[] =                        { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 1, 1, 22, 1, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTStateResetToDefaults[] =              { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 1, 1, 23, 1, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
//last 1 number is index
const oid ltp8xONTServicesName[] =                      { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 8, 1, 3, 0 };
const oid ltp8xONTMulticastName[] =                     { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 7, 1, 3, 0 };
//MIBs для LTE-8ST
//last 6 numbers is mac-address of device in decimal representation
const oid lte8stONTConfigMAC[] =                        { 1, 3, 6, 1, 4, 1, 35265, 1, 21, 16, 1, 1, 1, 6, 0, 0, 0, 0, 0, 0 };
const oid lte8stONTConfigType[] =                       { 1, 3, 6, 1, 4, 1, 35265, 1, 21, 16, 1, 1, 10, 6, 0, 0, 0, 0, 0, 0 };
const oid lte8stONTConfigDescription[] =                { 1, 3, 6, 1, 4, 1, 35265, 1, 21, 16, 1, 1, 8, 6, 0, 0, 0, 0, 0, 0 };
const oid lte8stONTStateState[] =                       { 1, 3, 6, 1, 4, 1, 35265, 1, 21, 6, 1, 1, 6, 6, 0, 0, 0, 0, 0, 0 };
const oid lte8stONTConfigRulesProfile[] =               { 1, 3, 6, 1, 4, 1, 35265, 1, 21, 16, 1, 1, 3, 6, 0, 0, 0, 0, 0, 0 };
const oid lte8stONTConfigIpMulticastProfile[] =         { 1, 3, 6, 1, 4, 1, 35265, 1, 21, 16, 1, 1, 4, 6, 0, 0, 0, 0, 0, 0 };
const oid lte8stONTStateReconfigure[] =                 { 1, 3, 6, 1, 4, 1, 35265, 1, 21, 6, 1, 1, 9, 6, 0, 0, 0, 0, 0, 0 };
//last 1 number is index
const oid lte8stProfilesRulesDescription[] =            { 1, 3, 6, 1, 4, 1, 35265, 1, 21, 20, 3, 1, 2, 0 };
const oid lte8stProfilesIpMulticastDescription[] =      { 1, 3, 6, 1, 4, 1, 35265, 1, 21, 20, 4, 1, 2, 0 };
}

namespace LoadProgramString
{
const QString LoadLocale =              QString::fromUtf8("Загрузка локализации...");
const QString CreateWindow =            QString::fromUtf8("Создание окна программы...");
const QString CreateDeviceListPage =    QString::fromUtf8("Создание вкладки оборудования...");
const QString LoadList =                QString::fromUtf8("Загрузка данных...");
const QString LoadConfig =              QString::fromUtf8("Загрузка конфигурации...");
}

namespace BasicDialogTitle
{
const QString Error =           QString::fromUtf8("Ошибка");
const QString Info =            QString::fromUtf8("Информация");
const QString Warning =         QString::fromUtf8("Предупреждение");
const QString Question =        QString::fromUtf8("Подтверждение");
}

namespace DeviceListModelColumn
{
const QString Name =            QString::fromUtf8("Название");
const QString DeviceModel =     QString::fromUtf8("Модель");
const QString IP =              QString::fromUtf8("IP");
const QString DeviceType =      QString::fromUtf8("Тип устройства");
}

namespace SwitchPortListModelColumn
{
const QString Number =          QString::fromUtf8("№");
const QString State =           QString::fromUtf8("Статус");
const QString SpeedDuplex =     QString::fromUtf8("Скорость/Дуплекс");
const QString Desc =            QString::fromUtf8("Описание");
}

namespace DslamPortListModelColumn
{
const QString Pair =            QString::fromUtf8("Пара");
const QString Port =            QString::fromUtf8("Порт");
const QString State =           QString::fromUtf8("Статус");
const QString Desc =            QString::fromUtf8("Описание");
const QString Profile =         QString::fromUtf8("Профиль");
const QString LineType =        QString::fromUtf8("Тип линии");
const QString TxRate =          QString::fromUtf8("Tx уровень, kbps");
const QString RxRate =          QString::fromUtf8("Rx уровень, kbps");
const QString TxAttenuation =   QString::fromUtf8("Затухание Tx канала, dB");
const QString RxAttenuation =   QString::fromUtf8("Затухание Rx канала, dB");
const QString LastChange =      QString::fromUtf8("Последнее изменение");
const QString Coding =          QString::fromUtf8("Кодирование");
}

namespace MacListModelColumn
{
const QString Port =            QString::fromUtf8("Порт");
const QString Vlan =            QString::fromUtf8("Влан");
const QString MacAddress =      QString::fromUtf8("Mac-адрес");
}

namespace BoardListModelColumn
{
const QString Number =          QString::fromUtf8("№");
const QString TypeBoard =       QString::fromUtf8("Тип доски");
const QString Pairs =           QString::fromUtf8("Пары");
}

namespace OntTableModelColumn
{
const QString Id =                  QString::fromUtf8("Mac/Serial");
const QString Model =               QString::fromUtf8("Модель");
const QString State =               QString::fromUtf8("Статус");
const QString Description =         QString::fromUtf8("Описание");
const QString ServiceProfile =      QString::fromUtf8("Сервисный профиль");
const QString MulticastProfile =    QString::fromUtf8("Мультикастовый профиль");
}

namespace SnmpErrors
{
const QString SetupSession =    QString::fromUtf8("Ошибка: Не удалось настроить SNMP сессию.");
const QString OpenSession =     QString::fromUtf8("Ошибка: Не удалось открыть SNMP сессию.");
const QString GetInfo =         QString::fromUtf8("Ошибка: Не удалось получить информацию!");
}

namespace AdslProfileDisplayName
{
const QString AdslFast =        QString::fromUtf8("ADSL, Fast");
const QString AdslInterleave =  QString::fromUtf8("ADSL, Interleave");
const QString Adsl2Interleave = QString::fromUtf8("ADSL2+, Interleave");
const QString AdslGLite =       QString::fromUtf8("ADSL, G.Lite");
const QString AdslFast8Mb =     QString::fromUtf8("ADSL(8 mbps), Fast");
const QString Adsl2Fast10Mb =   QString::fromUtf8("ADSL2+(10 mbps), Fast");
const QString Adsl2Fast14Mb =   QString::fromUtf8("ADSL2+(14 mbps), Fast");
const QString Adsl2Fast18Mb =   QString::fromUtf8("ADSL2+(18 mbps), Fast");
}

namespace AdslProfileDslamName
{
namespace MA5600
{
const QString AdslFast =        QString::fromUtf8("ADSL LINE PROFILE 1000");
const QString AdslInterleave =  QString::fromUtf8("ADSL LINE PROFILE 1001");
const QString Adsl2Interleave = QString::fromUtf8("ADSL LINE PROFILE 1002");
const QString AdslGLite =       QString::fromUtf8("ADSL G.Lite");
const QString AdslFast8Mb =     QString::fromUtf8("ADSL (8mb)");
const QString Adsl2Fast10Mb =   QString::fromUtf8("ADSL 2+ (10mb)");
const QString Adsl2Fast14Mb =   QString::fromUtf8("ADSL 2+ (14mb)");
const QString Adsl2Fast18Mb =   QString::fromUtf8("ADSL 2+ (18mb)");
}

namespace MA5300
{
const QString AdslFast =        QString::fromUtf8("LINE-PROFILE-1");
const QString AdslInterleave =  QString::fromUtf8("LINE-PROFILE-2");
const QString Adsl2Interleave = QString::fromUtf8("LINE-PROFILE-100");
}

namespace MXA
{
const QString AdslInterleave =  QString::fromUtf8("1");
const QString Adsl2Interleave = QString::fromUtf8("3");
const QString AdslGLite =       QString::fromUtf8("2");
}
}

namespace BatchUpdateLabel
{
const QString Dslam =       QString::fromUtf8("Обновление данных о досках дсламов");
const QString Switch =      QString::fromUtf8("Обновление данных о вланах коммутатора");
const QString Olt =         QString::fromUtf8("Обновление данных о профилях olt");
const QString AllDevices =  QString::fromUtf8("Обновление данных о всех устройствах");
}

namespace SnmpSettingsString
{
const QString port =                QString::fromUtf8("port");
const QString readCommunity =       QString::fromUtf8("readCommunity");
const QString writeCommunity =      QString::fromUtf8("writeCommunity");
const QString timeout =             QString::fromUtf8("timeout");
const QString saveConfigTimeout =   QString::fromUtf8("saveConfigTimeout");
const QString retries =             QString::fromUtf8("retries");
}
#endif // CONSTANT_H
