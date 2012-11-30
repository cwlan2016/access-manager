#include "stdafx.h"
#include "boardinfo.h"

BoardInfo::BoardInfo()
{
    mType = BoardType::AnnexA;
    mFirstPair = 1;
}

BoardType BoardInfo::type() const
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

void BoardInfo::setType(BoardType type)
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

