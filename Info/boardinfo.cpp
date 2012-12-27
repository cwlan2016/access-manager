#include "boardinfo.h"

BoardInfo::BoardInfo()
{
    mType = BoardType::AnnexA;
    mFirstPair = 1;
}

BoardType::Enum BoardInfo::type() const
{
    return mType;
}

int BoardInfo::number() const
{
    return mNumber;
}

int BoardInfo::firstPair() const
{
    return mFirstPair;
}

void BoardInfo::setType(BoardType::Enum type)
{
    mType = type;
}

void BoardInfo::setNumber(int number)
{
    mNumber = number;
}

void BoardInfo::setFirstPair(int firstPair)
{
    mFirstPair = firstPair;
}

