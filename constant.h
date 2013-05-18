#ifndef CONSTANT_H
#define CONSTANT_H

#include "stdafx.h"

const quint64 invalidParentIndex = 111111111;

const QString LtpOntState[] =   { "Free", "Allocated", "AuthInProgress", "AuthFailed", "AuthOk", "CfgInProgress", "CfgFailed", "Ok", "Failed", "Blocked", "Mibreset", "Preconfig", "FwUpdating", "Unactivated", "Redundant", "Disabled", "Unknown" };
const QString LteOntState[] =   { "Free", "Allocated", "AuthInProgress", "CfgInProgress", "AuthFailed", "CfgFailed", "ReportTimeout", "Ok", "AuthOk", "ResetInProgress", "ResetOk", "Discovered", "Blocked" };
const QString LteOntType[] =    { "", "", "Nte-2c", "Nte-rg-1400f", "Nte-rg-1400g", "Nte-rg-1400f-w", "Nte-rg-1400g-w", "Nte-rg-1400fc", "Nte-rg-1400gc", "Nte-rg-1400fc-w", "Nte-rg-1400gc-w", "Nte-rg-1402f", "Nte-rg-1402g", "Nte-rg-1402f-w", "Nte-rg-1402g-w", "Nte-rg-1402fc", "Nte-rg-1402gc", "Nte-rg-1402fc-w", "Nte-rg-1402gc-w" };

namespace Mib
{
const oid ifName[] =                                    { 1, 3, 6, 1, 2, 1, 31, 1, 1, 1, 1 };  // .index = Description_new
const oid ifDescr[] =                                   { 1, 3, 6, 1, 2, 1, 2, 2, 1, 2 };    // .index = Description_new
const oid ifAdminStatus[] =                             { 1, 3, 6, 1, 2, 1, 2, 2, 1, 7 };
const oid ifOperStatus[] =                              { 1, 3, 6, 1, 2, 1, 2, 2, 1, 8 };
const oid ifLastChange[] =                              { 1, 3, 6, 1, 2, 1, 2, 2, 1, 9 };
const oid ifAlias[] =                                   { 1, 3, 6, 1, 2, 1, 31, 1, 1, 1, 18 };  // .index = Alias_new
const oid swL2PortInfoNwayStatusDES3526[] =             { 1, 3, 6, 1, 4, 1, 171, 11, 64, 1, 2, 4, 1, 1, 5 }; // .index = Speed/Duplex_new
const oid swL2PortInfoNwayStatusDES3550[] =             { 1, 3, 6, 1, 4, 1, 171, 11, 64, 2, 2, 4, 1, 1, 5 };   // .index = Speed/Duplex_new
const oid swL2PortInfoNwayStatusDES3528[] =             { 1, 3, 6, 1, 4, 1, 171, 11, 105, 1, 2, 3, 1, 1, 6 };  // .index = Speed/Duplex_new

const oid dot1qTpFdbPort[] =                            { 1, 3, 6, 1, 2, 1, 17, 7, 1, 2, 2, 1, 2 };          // .vlanid.mac = portNumber

const oid swL2IGMPMulticastVlanMemberPortDES3526[] =    { 1, 3, 6, 1, 4, 1, 171, 11, 64, 1, 2, 10, 6, 1, 4 };  // .vlanid = mask for ports
const oid swL2IGMPMulticastVlanMemberPortDES3550[] =    { 1, 3, 6, 1, 4, 1, 171, 11, 64, 2, 2, 10, 6, 1, 4 };  // .vlanid = mask for ports
const oid swL2IGMPMulticastVlanMemberPortDES3528[] =    { 1, 3, 6, 1, 4, 1, 171, 12, 64, 3, 1, 1, 4 };       // .vlanid = mask for ports

const oid dot1qVlanStaticName[] =                       { 1, 3, 6, 1, 2, 1, 17, 7, 1, 4, 3, 1, 1 };          // .index = VlanName
const oid dot1qVlanStaticEgressPorts[] =                { 1, 3, 6, 1, 2, 1, 17, 7, 1, 4, 3, 1, 2 };          // .vlanid = mask for ports in vlan
const oid dot1qVlanStaticUntaggedPorts[] =              { 1, 3, 6, 1, 2, 1, 17, 7, 1, 4, 3, 1, 4 };          // .vlanid = mask for ports untag ports in vlan
const oid agentStatusSaveCfg[] =                        { 1, 3, 6, 1, 4, 1, 171, 12, 1, 1, 4, 0 };           // value proceeding(2) - current make. completed(3) failed (4)
// DES-3526 value 3
// DES-3528 value 5
const oid agentSaveCfg[] =                              { 1, 3, 6, 1, 4, 1, 171, 12, 1, 2, 6, 0 };           // value set(3) for save switch config
//информация по Dslam'у MA5600
const oid dslamBoardName[] =                            { 1, 3, 6, 1, 4, 1, 2011, 2, 6, 7, 1, 1, 2, 1, 7, 0 };
//MIBs для MXA-64
const oid mxa64DslPortName[] =                          { 1, 3, 6, 1, 4, 1, 35265, 1, 33, 10, 2, 1, 2 };
const oid mxa64DslPortActiveProfile[] =                 { 1, 3, 6, 1, 4, 1, 35265, 1, 33, 10, 2, 1, 6 };
const oid mxa64DslPortOperStatus[] =                    { 1, 3, 6, 1, 4, 1, 35265, 1, 33, 10, 2, 1, 3 };
const oid mxa64DslPortAdminStatus[] =                   { 1, 3, 6, 1, 4, 1, 35265, 1, 33, 10, 2, 1, 4 };
const oid mxa64DslBandActualRateTx[] =                  { 1, 3, 6, 1, 4, 1, 35265, 1, 33, 10, 3, 1, 3, 2 };
const oid mxa64DslBandActualRateRx[] =                  { 1, 3, 6, 1, 4, 1, 35265, 1, 33, 10, 3, 1, 3, 1 };
const oid mxa64DslBandLineAttenuationTx[] =             { 1, 3, 6, 1, 4, 1, 35265, 1, 33, 10, 3, 1, 6, 2 };
const oid mxa64DslBandLineAttenuationRx[] =             { 1, 3, 6, 1, 4, 1, 35265, 1, 33, 10, 3, 1, 6, 1 };
//MIBs для MXA-32
const oid mxa32DslPortName[] =                          { 1, 3, 6, 1, 4, 1, 35265, 1, 28, 10, 2, 1, 2 };
const oid mxa32DslPortActiveProfile[] =                 { 1, 3, 6, 1, 4, 1, 35265, 1, 28, 10, 2, 1, 6 };
const oid mxa32DslPortOperStatus[] =                    { 1, 3, 6, 1, 4, 1, 35265, 1, 28, 10, 2, 1, 3 };
const oid mxa32DslPortAdminStatus[] =                   { 1, 3, 6, 1, 4, 1, 35265, 1, 28, 10, 2, 1, 4 };
const oid mxa32DslBandActualRateTx[] =                  { 1, 3, 6, 1, 4, 1, 35265, 1, 28, 10, 3, 1, 3, 2 };
const oid mxa32DslBandActualRateRx[] =                  { 1, 3, 6, 1, 4, 1, 35265, 1, 28, 10, 3, 1, 3, 1 };
const oid mxa32DslBandLineAttenuationTx[] =             { 1, 3, 6, 1, 4, 1, 35265, 1, 28, 10, 3, 1, 6, 2 };
const oid mxa32DslBandLineAttenuationRx[] =             { 1, 3, 6, 1, 4, 1, 35265, 1, 28, 10, 3, 1, 6, 1 };
//параметры линии
const oid adslLineCoding[] =                            { 1, 3, 6, 1, 2, 1, 10, 94, 1, 1, 1, 1, 1 };
const oid adslLineType[] =                              { 1, 3, 6, 1, 2, 1, 10, 94, 1, 1, 1, 1, 2 };
const oid adslLineConfProfile[] =                       { 1, 3, 6, 1, 2, 1, 10, 94, 1, 1, 1, 1, 4 };
const oid adslAtucChanCurrTxRate[] =                    { 1, 3, 6, 1, 2, 1, 10, 94, 1, 1, 4, 1, 2 }; //скорость нисходящего канала
const oid adslAturChanCurrTxRate[] =                    { 1, 3, 6, 1, 2, 1, 10, 94, 1, 1, 5, 1, 2 }; //скорость восходящего канала
const oid adslAtucCurrAtn[] =                           { 1, 3, 6, 1, 2, 1, 10, 94, 1, 1, 2, 1, 5 }; //затухание нисходящего канала
const oid adslAturCurrAtn[] =                           { 1, 3, 6, 1, 2, 1, 10, 94, 1, 1, 3, 1, 5 }; //затухание восходящего канала
//MIBs для LTP-8X
//last 8 numbers is serial of device in decimal representation
const oid ltp8xONTConfigSerial[] =                      { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 4, 1, 2, 1, 8, 0, 0, 0, 0, 0, 0, 0, 0};
const oid ltp8xONTStateEquipmentID[] =                  { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 1, 1, 12, 1, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTACSUserID[] =                         { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 15, 1, 2, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTStateState[] =                        { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 1, 1, 5, 1, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTConfigServicesProfile[] =             { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 4, 1, 5, 1, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTConfigMulticastProfile[] =            { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 4, 1, 10, 1, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTACSConfigPPPLogin[] =                 { 1, 3, 6, 1, 4 ,1, 35265 ,1, 22, 3, 15, 1, 11, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTACSConfigPPPPassword[] =              { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 15, 1, 12, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTACSConfigVoice1Enable[] =             { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 15, 1, 4, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTACSConfigVoice1Number[] =             { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 15, 1, 5, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTACSConfigVoice1Password[] =           { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3 ,15, 1, 6, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTACSConfigVoice2Enable[] =             { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 15, 1, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTACSConfigVoice2Number[] =             { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 15, 1, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTACSConfigVoice2Password[] =           { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 15, 1, 9, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTACSConfigSIPProxy[] =                 { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 15, 1, 10, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTStateReconfigure[] =                  { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 1, 1, 20, 1, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTStateReset[] =                        { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 1, 1, 22, 1, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTStateResetToDefaults[] =              { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 1, 1, 23, 1, 8, 0, 0, 0, 0, 0, 0, 0, 0 };
const oid ltp8xONTServicesName[] =                      { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 8, 1, 3 };
const oid ltp8xONTMulticastName[] =                     { 1, 3, 6, 1, 4, 1, 35265, 1, 22, 3, 7, 1, 3 };
//MIBs для LTE-8ST
//last 6 numbers is mac-address of device in decimal representation
const oid lte8stONTConfigMAC[] =                        { 1, 3, 6, 1, 4, 1, 35265, 1, 21, 16, 1, 1, 1, 6, 0, 0, 0, 0, 0, 0 };
const oid lte8stONTConfigType[] =                       { 1, 3, 6, 1, 4, 1, 35265, 1, 21, 16, 1, 1, 10, 6, 0, 0, 0, 0, 0, 0 };
const oid lte8stONTConfigDescription[] =                { 1, 3, 6, 1, 4, 1, 35265, 1, 21, 16, 1, 1, 8, 6, 0, 0, 0, 0, 0, 0 };
const oid lte8stONTStateState[] =                       { 1, 3, 6, 1, 4, 1, 35265, 1, 21, 6, 1, 1, 6, 6, 0, 0, 0, 0, 0, 0 };
const oid lte8stONTConfigRulesProfile[] =               { 1, 3, 6, 1, 4, 1, 35265, 1, 21, 16, 1, 1, 3, 6, 0, 0, 0, 0, 0, 0 };
const oid lte8stONTConfigIpMulticastProfile[] =         { 1, 3, 6, 1, 4, 1, 35265, 1, 21, 16, 1, 1, 4, 6, 0, 0, 0, 0, 0, 0 };
const oid lte8stONTStateReconfigure[] =                 { 1, 3, 6, 1, 4, 1, 35265, 1, 21, 6, 1, 1, 9, 6, 0, 0, 0, 0, 0, 0 };
const oid lte8stProfilesRulesDescription[] =            { 1, 3, 6, 1, 4, 1, 35265, 1, 21, 20, 3, 1, 2 };
const oid lte8stProfilesIpMulticastDescription[] =      { 1, 3, 6, 1, 4, 1, 35265, 1, 21, 20, 4, 1, 2 };
}

namespace LoadProgramStrings
{
const QString LoadLocale =              QString::fromUtf8("Загрузка локализации...");
const QString CreateWindow =            QString::fromUtf8("Создание окна программы...");
const QString CreateDeviceListPage =    QString::fromUtf8("Создание вкладки оборудования...");
const QString LoadList =                QString::fromUtf8("Загрузка данных...");
const QString LoadConfig =              QString::fromUtf8("Загрузка конфигурации...");
}

namespace BasicDialogStrings
{
const QString Error =           QString::fromUtf8("Ошибка");
const QString Info =            QString::fromUtf8("Информация");
const QString Warning =         QString::fromUtf8("Предупреждение");
const QString Question =        QString::fromUtf8("Подтверждение");
}

namespace DeviceTableModelStrings
{
const QString Name =            QString::fromUtf8("Название");
const QString DeviceModel =     QString::fromUtf8("Модель");
const QString IP =              QString::fromUtf8("IP");
const QString DeviceType =      QString::fromUtf8("Тип устройства");
}

namespace SwitchPortTableModelStrings
{
const QString Number =          QString::fromUtf8("№");
const QString State =           QString::fromUtf8("Статус");
const QString SpeedDuplex =     QString::fromUtf8("Скорость/Дуплекс");
const QString Desc =            QString::fromUtf8("Описание");
}

namespace DslamPortTableModelStrings
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

namespace MacTableModelStrings
{
const QString Port =            QString::fromUtf8("Порт");
const QString Vlan =            QString::fromUtf8("Влан");
const QString MacAddress =      QString::fromUtf8("Mac-адрес");
}

namespace BoardTableModelStrings
{
const QString Number =          QString::fromUtf8("№");
const QString TypeBoard =       QString::fromUtf8("Тип доски");
const QString Pairs =           QString::fromUtf8("Пары");
}

namespace OntTableModelStrings
{
const QString Id =                  QString::fromUtf8("Mac/Serial");
const QString Model =               QString::fromUtf8("Модель");
const QString State =               QString::fromUtf8("Статус");
const QString Description =         QString::fromUtf8("Описание");
const QString ServiceProfile =      QString::fromUtf8("Сервисный профиль");
const QString MulticastProfile =    QString::fromUtf8("Мультикастовый профиль");
}

namespace SnmpErrorStrings
{
const QString SetupSession =    QString::fromUtf8("Ошибка: Не удалось настроить SNMP сессию.");
const QString OpenSession =     QString::fromUtf8("Ошибка: Не удалось открыть SNMP сессию.");
const QString GetInfo =         QString::fromUtf8("Ошибка: Не удалось получить информацию!");
}

namespace BatchUpdateStrings
{
const QString Dslam =           QString::fromUtf8("Обновление данных о досках дсламов");
const QString Switch =          QString::fromUtf8("Обновление данных о вланах коммутатора");
const QString Olt =             QString::fromUtf8("Обновление данных о профилях olt");
const QString AllDevices =      QString::fromUtf8("Обновление данных о всех устройствах");
}

namespace SnmpSettingsStrings
{
const QString port =                QString::fromUtf8("port");
const QString readCommunity =       QString::fromUtf8("readCommunity");
const QString writeCommunity =      QString::fromUtf8("writeCommunity");
const QString timeout =             QString::fromUtf8("timeout");
const QString saveConfigTimeout =   QString::fromUtf8("saveConfigTimeout");
const QString retries =             QString::fromUtf8("retries");
}

namespace SwitchSettingsStrings
{
const QString inetVlanName =        QString::fromUtf8("inetVlanName");
const QString iptvUnicastVlanName = QString::fromUtf8("iptvUnicastVlanName");
}

namespace DslProfileTableModelStrings
{
const QString DisplayName = QString::fromUtf8("Отображаемое имя профиля");
const QString DslamName = QString::fromUtf8("Имя профиля на дсламе");
}

#endif // CONSTANT_H
