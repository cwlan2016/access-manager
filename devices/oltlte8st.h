#ifndef OLTLTE8ST_H
#define OLTLTE8ST_H

#include "olt.h"

class OltLte8st : public Olt
{
    Q_OBJECT
public:
    explicit OltLte8st(QObject *parent = 0);
    OltLte8st(QString name, QString ip, QObject *parent = 0);
    explicit OltLte8st(Device *source, QObject *parent = 0);

    DeviceModel::Enum deviceModel() const;
};

#endif // OLTLTE8ST_H
