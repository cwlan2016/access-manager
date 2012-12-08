#ifndef NTEINFO_H
#define NTEINFO_H

#include "stdafx.h"
#include "ontinfo.h"
#include "customtypes.h"

class NteInfo : public OntInfo
{
public:
    NteInfo();
    virtual void Reconfigure();

    typedef std::shared_ptr<NteInfo> Ptr;
};

#endif // NTEINFO_H
