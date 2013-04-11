#ifndef NTEINFO_H
#define NTEINFO_H

#include "ontinfo.h"

class NteInfo : public OntInfo
{
    Q_OBJECT
public:
    NteInfo(QObject *parent);

    void Reconfigure();

    typedef NteInfo *Ptr;
};

#endif // NTEINFO_H
