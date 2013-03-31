#ifndef OLTINFOLTE8ST_H
#define OLTINFOLTE8ST_H

#include "oltinfo.h"

class OltInfoLte8st : public OltInfo
{
    Q_OBJECT
public:
    explicit OltInfoLte8st(QObject *parent = 0);
    OltInfoLte8st(QString name, QString ip, QObject *parent = 0);
    OltInfoLte8st(DeviceInfo *source, QObject *parent = 0);

    DeviceModel::Enum deviceModel() const;
};

#endif // OLTINFOLTE8ST_H
