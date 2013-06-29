#ifndef SHDSLPORT_H
#define SHDSLPORT_H

#include "xdslport.h"

class ShdslPort : public XdslPort
{
    Q_OBJECT
public:
    ShdslPort(long index, QObject *parent);

    int actualRate() const;
    void setActualRate(int actualRate);

    int maxRate() const;
    void setMaxRate(int maxRate);

    int snrMargin() const;
    void setSnrMargin(int snrMargin);

    QString transmissonMode() const;
    void setTransmissonMode(QString transmissonMode);

    QString modemVendorId() const;
    void modemVendorId(QString modemVendorId);

    QString modemModel() const;
    void setModemModel(QString modemModel);

    QString modemSerial() const;
    void setModemSerial(QString modemSerial);

    QString modemFirmware() const;
    void setModemFirmware(QString modemFirmware);

    QString modemVendorOther() const;
    void setModemVendorOther(QString modemVendorOther);


    typedef ShdslPort *Ptr;

protected:
    QString transmissonModeString(int mode);

    int mActualRate;
    int mMaxRate;
    int mSnrMargin;
    QString mTransmissonMode;
    QString mModemVendorId;
    QString mModemModel;
    QString mModemSerial;
    QString mModemFirmware;
    QString mModemVendorOther;
};

#endif // SHDSLPORT_H
