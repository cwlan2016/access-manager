#ifndef PORTLISTVALIDATOR_H
#define PORTLISTVALIDATOR_H

#include "stdafx.h"

#include "converters.h"
#include "customtypes.h"

class PortListValidator : public QValidator
{
    Q_OBJECT
public:
    explicit PortListValidator(DeviceModel deviceModel);
    State validate(QString& input, int& pos) const;
signals:

public slots:
private:
    int countPorts;
};

#endif // PORTLISTVALIDATOR_H
