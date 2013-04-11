#ifndef OLTLTP8X_H
#define OLTLTP8X_H

#include "olt.h"

class OltLtp8x : public Olt
{
    Q_OBJECT
public:
    explicit OltLtp8x(QObject *parent = 0);
    OltLtp8x(QString name, QString ip, QObject *parent = 0);
    OltLtp8x(Device *source, QObject *parent = 0);

    DeviceModel::Enum deviceModel() const;
};

#endif // OLTLTP8X_H
