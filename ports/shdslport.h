#ifndef SHDSLPORTINFO_H
#define SHDSLPORTINFO_H

#include "xdslportinfo.h"

class ShdslPort : public XdslPort
{
    Q_OBJECT
public:
    ShdslPort(QObject *parent);

    typedef ShdslPort *Ptr;
};

#endif // SHDSLPORTINFO_H
