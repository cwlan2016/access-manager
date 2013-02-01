#ifndef SHDSLPORTINFO_H
#define SHDSLPORTINFO_H

#include "xdslportinfo.h"

class ShdslPortInfo : public XdslPortInfo
{
    Q_OBJECT
public:
    ShdslPortInfo(QObject *parent);

    typedef ShdslPortInfo *Ptr;
};



#endif // SHDSLPORTINFO_H
