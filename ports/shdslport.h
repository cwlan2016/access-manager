#ifndef SHDSLPORT_H
#define SHDSLPORT_H

#include "xdslport.h"

class ShdslPort : public XdslPort
{
    Q_OBJECT
public:
    ShdslPort(long index, QObject *parent);

    typedef ShdslPort *Ptr;
};

#endif // SHDSLPORT_H
