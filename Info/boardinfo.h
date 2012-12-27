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
    BoardType::Enum type() const;
    int number() const;
    int firstPair() const;
    void setType(BoardType::Enum type);
    void setNumber(int number);
    void setFirstPair(int firstPair);
private:
    BoardType::Enum mType;
    int mNumber;
    int mFirstPair;
};

#endif // BOARDINFO_H
