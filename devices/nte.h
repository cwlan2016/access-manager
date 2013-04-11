#ifndef NTE_H
#define NTE_H

#include "ont.h"

class Nte : public Ont
{
    Q_OBJECT
public:
    Nte(QObject *parent);

    void Reconfigure();

    typedef Nte *Ptr;
};

#endif // NTE_H
