#ifndef DSLAMINFOMA5600_H
#define DSLAMINFOMA5600_H

#include "dslaminfo.h"

class DslamInfoMa5600 : public DslamInfo
{
    Q_OBJECT
public:
    explicit DslamInfoMa5600(QObject *parent = 0);
    DslamInfoMa5600(QString name, QString ip, QObject *parent = 0);

    DeviceModel::Enum deviceModel() const;
};

#endif // DSLAMINFOMA5600_H