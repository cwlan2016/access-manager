#ifndef CONVERTERS_H
#define CONVERTERS_H

#include <QtCore/QBitArray>
#include "customtypes.h"
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

QString     SnmpInterfaceNumber(DeviceModel::Enum model, int slot, int port);
QString     DslamStatePortString(long state);
QString     SwitchStatePortString(long state);
QString     SpeedDuplexString(DeviceModel::Enum deviceModel, long speedDuplex);
DeviceModel::Enum   DeviceModelFromString(QString deviceModel);
DeviceType::Enum    DeviceTypeFromString(QString deviceType);
DeviceType::Enum    DeviceTypeFromDeviceModel(DeviceModel::Enum deviceModel);
BoardType::Enum     BoardTypeFromString(QString type);
BoardType::Enum     BoardTypeFromBoardName(QString boardName);
int         CountPorts(DeviceModel::Enum deviceModel, BoardType::Enum boardType);
int         CountPorts(DeviceModel::Enum deviceModel);
QString     DecMacAddressToHex(oid *macAddressOid, int length);
bool        ValidIpAddress(QString ip);
QBitArray   UCharToQBitArray(DeviceModel::Enum deviceModel, uchar *str);
QString     QBitArrayToHexString(DeviceModel::Enum switchType, const QBitArray &array, bool ismv);
QString     TypeLineString(int type);
QString     CodingString(int coding);
QString     ProfileExtName(DeviceModel::Enum deviceModel, QString profile);
QString     DisplayNameProfileToDslamName(DeviceModel::Enum deviceModel, QString profile);
QString     ToQString(u_char *string, int str_len);
#endif // CONVERTERS_H
