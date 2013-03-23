#ifndef DSLAMINFOMXA32_H
#define DSLAMINFOMXA32_H

#include "dslaminfo.h"

class DslamInfoMxa32 : public DslamInfo
{
    Q_OBJECT
public:
    explicit DslamInfoMxa32(QObject *parent = 0);
    DslamInfoMxa32(QString name, QString ip, QObject *parent = 0);

    DeviceModel::Enum deviceModel() const;
};

#endif // DSLAMINFOMXA32_H
