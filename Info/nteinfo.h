#ifndef NTEINFO_H
#define NTEINFO_H

#include "ontinfo.h"

class NteInfo : public OntInfo
{
public:
    NteInfo();
    void Reconfigure();

    typedef std::shared_ptr<NteInfo> Ptr;
};

#endif // NTEINFO_H
