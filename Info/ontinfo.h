#ifndef ONTINFO_H
#define ONTINFO_H

#include <QtCore/QObject>
#ifdef _MSC_VER
#include "../customtypes.h"
#else
#include "customtypes.h"
#endif

class OntInfo : public QObject
{
    Q_OBJECT
public:
    OntInfo(QObject *parent);
    QString id();
    QString state();
    QString description();
    OntType::Enum type();
    QString model();
    int serviceProfile();
    int multicastProfile();
    void setId(QString id);
    void setState(QString state);
    void setDescription(QString description);
    void setType(OntType::Enum type);
    void setModel(QString model);
    void setServiceProfile(int index);
    void setMulticastProfile(int index);
    virtual void Reconfigure();

    typedef OntInfo *Ptr;
protected:
    QString mId;    //Mac or Serial
    QString mState;
    QString mDescription;
    OntType::Enum mType;
    QString mModel;
    int     mServiceProfile;
    int     mMulticastProfile;
};

#endif // ONTINFO_H
