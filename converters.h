#ifndef CONVERTERS_H
#define CONVERTERS_H

#include "stdafx.h"
#include "constant.h"
#include "customtypes.h"

int         snmpInterfaceNumber(DeviceModel model, int slot, int port);
QString     DslamStatePortString(long state);
QString     SwitchStatePortString(long state);
QString     SpeedDuplexString(DeviceModel deviceModel, long speedDuplex);
DeviceModel DeviceModelFromString(QString deviceModel);
DeviceType  DeviceTypeFromString(QString deviceType);
DeviceType  DeviceTypeFromDeviceModel(DeviceModel deviceModel);
BoardType   BoardTypeFromString(QString type);
BoardType   BoardTypeFromBoardName(QString boardName);
int         CountPorts(DeviceModel deviceModel, BoardType boardType);
int         CountPorts(DeviceModel deviceModel);
QString     DecMacAddressToHex(oid* macAddressOid, int length);
bool        ValidIpAddress(QString ip);
QBitArray   UCharToQBitArray(DeviceModel deviceModel, uchar* str);
QString     QBitArrayToHexString(DeviceModel switchType, const QBitArray& array, bool ismv);
QString     TypeLineString(int type);
QString     CodingString(int coding);
QString     ProfileExtName(DeviceModel deviceModel, QString profile);
QString     DisplayNameProfileToDslamName(DeviceModel deviceModel, QString profile);
QString     ToQString(u_char* string, int str_len);
#endif // CONVERTERS_H
