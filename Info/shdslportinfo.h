#ifndef SHDSLPORTINFO_H
#define SHDSLPORTINFO_H

#include "xdslportinfo.h"

class ShdslPortInfo : public XdslPortInfo
{
public:
    ShdslPortInfo();

    typedef std::shared_ptr<ShdslPortInfo> Ptr;
};



#endif // SHDSLPORTINFO_H
