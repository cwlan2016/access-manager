#include "shdslport.h"

ShdslPort::ShdslPort(int index, long snmpIndex, QObject *parent) :
    XdslPort(index, snmpIndex, parent),
    mActualRate(0),
    mMaxRate(0),
    mSnrMargin(0)
{
}

int ShdslPort::actualRate() const
{
    return mActualRate;
}

void ShdslPort::setActualRate(int actualRate)
{
    mActualRate = actualRate;
}

int ShdslPort::maxRate() const
{
    return mMaxRate;
}

void ShdslPort::setMaxRate(int maxRate)
{
    mMaxRate = maxRate;
}

int ShdslPort::snrMargin() const
{
    return mSnrMargin;
}

void ShdslPort::setSnrMargin(int snrMargin)
{
    mSnrMargin = snrMargin;
}

QString ShdslPort::transmissonMode() const
{
    return mTransmissonMode;
}

void ShdslPort::setTransmissonMode(QString transmissonMode)
{
    mTransmissonMode = transmissonMode;
}

QString ShdslPort::modemVendorId() const
{
    return mModemVendorId;
}

void ShdslPort::modemVendorId(QString modemVendorId)
{
    mModemVendorId = modemVendorId;
}

QString ShdslPort::modemModel() const
{
    return mModemModel;
}

void ShdslPort::setModemModel(QString modemModel)
{
    mModemModel = modemModel;
}

QString ShdslPort::modemSerial() const
{
    return mModemSerial;
}

void ShdslPort::setModemSerial(QString modemSerial)
{
    mModemSerial = modemSerial;
}

QString ShdslPort::modemFirmware() const
{
    return mModemFirmware;
}

void ShdslPort::setModemFirmware(QString modemFirmware)
{
    mModemFirmware = modemFirmware;
}

QString ShdslPort::modemVendorOther() const
{
    return mModemVendorOther;
}

void ShdslPort::setModemVendorOther(QString modemVendorOther)
{
    mModemVendorOther = modemVendorOther;
}

QString ShdslPort::transmissonModeString(int mode)
{
    if (mode == 64) {
        return "Annex B";
    } else if (mode == 128) {
        return "Annex A";
    } else {
        return "Other";
    }
}
