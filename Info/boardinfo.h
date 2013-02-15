#ifndef BOARDINFO_H
#define BOARDINFO_H

#ifdef _MSC_VER
#include "../customtypes.h"
#else
#include "customtypes.h"
#endif

class BoardInfo
{
public:
    BoardInfo();

    int index() const;
    void setIndex(int index);

    BoardType::Enum type() const;
    void setType(BoardType::Enum type);

    int firstPair() const;
    void setFirstPair(int firstPair);

private:
    int mNumber;
    int mFirstPair;
    BoardType::Enum mType;
};

#endif // BOARDINFO_H
