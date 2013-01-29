#ifndef CONVERTERS_H
#define CONVERTERS_H

#include <QtCore/QBitArray>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include "customtypes.h"

int         snmpInterfaceNumber(DeviceModel::Enum model, int slot, int port);
QString     dslamStatePortString(long state);
QString     switchStatePortString(long state);
QString     speedDuplexString(DeviceModel::Enum deviceModel, long speedDuplex);
BoardType::Enum boardTypeFromBoardName(QString boardName);
int         countPorts(DeviceModel::Enum deviceModel, BoardType::Enum boardType);
int         countPorts(DeviceModel::Enum deviceModel);
QString     decMacAddressToHex(oid *macAddressOid, int length);
bool        validIpAddress(QString ip);
QBitArray   ucharToQBitArray(DeviceModel::Enum deviceModel, uchar *str);
QString     qbitArrayToHexString(DeviceModel::Enum switchType, const QBitArray &array, bool ismv);
QString     typeLineString(int type);
QString     codingString(int coding);
QString     profileExtName(DeviceModel::Enum deviceModel, QString profile);
QString     displayNameProfileToDslamName(DeviceModel::Enum deviceModel, QString profile);
QString     toQString(u_char *string, int str_len);
#endif // CONVERTERS_H
