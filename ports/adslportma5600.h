#ifndef ADSLPORTINFOMA5600_H
#define ADSLPORTINFOMA5600_H

#include "adslportinfo.h"

class AdslPortMa5600 : public AdslPort
{
    Q_OBJECT
public:
    explicit AdslPortMa5600(QObject *parent = 0);
};

#endif // ADSLPORTINFOMA5600_H
