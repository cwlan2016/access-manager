#include "boardinfo.h"

BoardInfo::BoardInfo(QObject *parent) : QObject(parent),
    mFirstPair(1),
    mType(BoardType::AnnexA)
{
}

int BoardInfo::index() const
{
    return mNumber;
}

void BoardInfo::setIndex(int number)
{
    mNumber = number;
}

BoardType::Enum BoardInfo::type() const
{
    return mType;
}

void BoardInfo::setType(BoardType::Enum type)
{
    mType = type;
}

int BoardInfo::firstPair() const
{
    return mFirstPair;
}

void BoardInfo::setFirstPair(int firstPair)
{
    mFirstPair = firstPair;
}
