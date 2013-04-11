#include "boardinfo.h"

DslamBoard::DslamBoard(QObject *parent) : QObject(parent),
    mFirstPair(1),
    mType(BoardType::AnnexA)
{
}

int DslamBoard::index() const
{
    return mNumber;
}

void DslamBoard::setIndex(int number)
{
    mNumber = number;
}

BoardType::Enum DslamBoard::type() const
{
    return mType;
}

void DslamBoard::setType(BoardType::Enum type)
{
    mType = type;
}

int DslamBoard::firstPair() const
{
    return mFirstPair;
}

void DslamBoard::setFirstPair(int firstPair)
{
    mFirstPair = firstPair;
}
