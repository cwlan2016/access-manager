#ifndef PORTLISTVALIDATOR_H
#define PORTLISTVALIDATOR_H

#include <QtGui/QValidator>
#include "customtypes.h"

class PortListValidator : public QValidator
{
    Q_OBJECT
public:
    explicit PortListValidator(DeviceModel::Enum deviceModel);
    State validate(QString &input, int &pos) const;
private:
    int count;
};

#endif // PORTLISTVALIDATOR_H
