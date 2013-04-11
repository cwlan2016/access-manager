#ifndef DSLAMBOARD_H
#define DSLAMBOARD_H

#ifdef _MSC_VER
#include "../stdafx.h"
#include "../customtypes.h"
#else
#include "stdafx.h"
#include "customtypes.h"
#endif

class DslamBoard : public QObject
{
    Q_OBJECT
public:
    DslamBoard(QObject *parent = 0);

    int index() const;
    void setIndex(int index);

    BoardType::Enum type() const;
    void setType(BoardType::Enum type);

    int firstPair() const;
    void setFirstPair(int firstPair);

    typedef DslamBoard *Ptr;

private:
    int mNumber;
    int mFirstPair;
    BoardType::Enum mType;
};

#endif // DSLAMBOARD_H
