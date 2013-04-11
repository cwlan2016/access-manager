#ifndef ONT_H
#define ONT_H

#ifdef _MSC_VER
#include "../customtypes.h"
#else
#include "customtypes.h"
#endif

class Ont : public QObject
{
    Q_OBJECT
public:
    Ont(QObject *parent);

    QString id();
    void setId(QString id);

    QString state();
    void setState(QString state);

    QString description();
    void setDescription(QString description);

    QString model();
    void setModel(QString model);

    OntType::Enum type();
    void setType(OntType::Enum type);

    int serviceProfile();
    void setServiceProfile(int index);

    int multicastProfile();
    void setMulticastProfile(int index);

    virtual void Reconfigure();

    typedef Ont *Ptr;

protected:
    QString mId;    //Mac or Serial
    QString mState;
    QString mModel;
    QString mDescription;
    OntType::Enum mType;
    int     mServiceProfile;
    int     mMulticastProfile;
};

#endif // ONT_H
