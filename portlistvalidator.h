#ifndef PORTLISTVALIDATOR_H
#define PORTLISTVALIDATOR_H


#include "customtypes.h"

class PortListValidator : public QValidator
{
    Q_OBJECT
public:
    explicit PortListValidator(int countPorts);

    State validate(QString &input, int &pos) const;

private:
    int mCountPorts;
};

#endif // PORTLISTVALIDATOR_H
