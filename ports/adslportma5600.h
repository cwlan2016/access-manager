#ifndef ADSLPORTMA5600_H
#define ADSLPORTMA5600_H

#include "adslport.h"

class AdslPortMa5600 : public AdslPort
{
    Q_OBJECT
public:
    explicit AdslPortMa5600(QObject *parent = 0);
};

#endif // ADSLPORTMA5600_H
