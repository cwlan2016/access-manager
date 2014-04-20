#ifndef DSLAMBOARD_H
#define DSLAMBOARD_H

#include <stdafx.h>
#include <customtypes.h>

class DslamBoard : public QObject
{
    Q_OBJECT
public:
    explicit DslamBoard(QObject *parent = 0);

    int index() const;
    void setIndex(int index);

    BoardType::Enum type() const;
    void setType(BoardType::Enum type);

    int firstPair() const;
    void setFirstPair(int firstPair);

    typedef DslamBoard *Ptr;
signals:
    void modified();
private:
    int mIndex;
    int mFirstPair;
    BoardType::Enum mType;
};

#endif // DSLAMBOARD_H
