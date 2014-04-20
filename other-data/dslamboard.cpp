#include "dslamboard.h"

DslamBoard::DslamBoard(QObject *parent) : QObject(parent),
    mIndex(0),
    mFirstPair(1),
    mType(BoardType::AnnexA)
{
}

int DslamBoard::index() const
{
    return mIndex;
}

void DslamBoard::setIndex(int number)
{
    mIndex = number;
}

BoardType::Enum DslamBoard::type() const
{
    return mType;
}

void DslamBoard::setType(BoardType::Enum type)
{
    if (mType != type) {
        mType = type;
        emit modified();
    }
}

int DslamBoard::firstPair() const
{
    return mFirstPair;
}

void DslamBoard::setFirstPair(int firstPair)
{
    if (mFirstPair != firstPair) {
        mFirstPair = firstPair;
        emit modified();
    }
}
