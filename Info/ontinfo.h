#ifndef ONTINFO_H
#define ONTINFO_H

#include "stdafx.h"
#include "customtypes.h"

class OntInfo
{
public:
    OntInfo();
    QString id();
    QString state();
    QString description();
    OntType type();
    QString model();
    int serviceProfile();
    int multicastProfile();
    void setId(QString id);
    void setState(QString state);
    void setDescription(QString description);
    void setType(OntType type);
    void setModel(QString model);
    void setServiceProfile(int index);
    void setMulticastProfile(int index);
    virtual void Reconfigure();

    typedef std::shared_ptr<OntInfo> Ptr;
protected:
    QString mId;    //Mac or Serial
    QString mState;
    QString mDescription;
    OntType mType;
    QString mModel;
    int     mServiceProfile;
    int     mMulticastProfile;
};

#endif // ONTINFO_H
