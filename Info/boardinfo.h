#ifndef BOARDINFO_H
#define BOARDINFO_H

#include "converters.h"

class BoardInfo
{
public:
    BoardInfo();
    BoardType type() const;
    int number() const;
    int firstPair() const;
    void setType(BoardType type);
    void setNumber(int number);
    void setFirstPair(int firstPair);
private:
    BoardType mType;
    int mNumber;
    int mFirstPair;
};

#endif // BOARDINFO_H
