#include "dslam.h"

#ifdef _MSC_VER
#include "../models/boardtablemodel.h"
#else
#include "models/boardtablemodel.h"
#endif

Dslam::Dslam(QObject *parent) :
    Device(parent)
{
    InitializeBoardTableModel();
}

Dslam::Dslam(QString name, QString ip, QObject *parent) :
    Device(name, ip, parent)
{
    InitializeBoardTableModel();
}

Dslam::Dslam(Device *source, QObject *parent) :
    Device(source, parent)
{
    InitializeBoardTableModel();

    if (source->deviceType() == DeviceType::Dslam) {
        mAutoFill = static_cast<Dslam *>(source)->autoFill();
        mAutoNumeringBoard = static_cast<Dslam *>(source)->autoNumeringBoard();
    }

}

short Dslam::autoFill() const
{
    return mAutoFill;
}

void Dslam::setAutoFill(short autoFill)
{
    mAutoFill = autoFill;
}

short Dslam::autoNumeringBoard() const
{
    return mAutoNumeringBoard;
}

void Dslam::setAutoNumeringBoard(short autoNumeringBoard)
{
    mAutoNumeringBoard = autoNumeringBoard;
}

DeviceType::Enum Dslam::deviceType() const
{
    return DeviceType::Dslam;
}

int Dslam::countPorts(BoardType::Enum boardType)
{
    Q_UNUSED(boardType)

    return 0;
}

long Dslam::snmpInterfaceIndex(BoardType::Enum boardType, int slot,
                                   int port)
{
    Q_UNUSED(boardType)
    Q_UNUSED(slot)
    Q_UNUSED(port)

    return 0;
}

BoardTableModel *Dslam::boardTableModel()
{
    return mBoardListModel;
}

bool Dslam::getServiceDataFromDevice()
{
    if (mBoardListModel == 0)
        return false;

    bool result = mBoardListModel->getBoardListFromDevice();

    if (!result)
        mError = mBoardListModel->error();

    return result;
}

void Dslam::InitializeBoardTableModel()
{
    mBoardListModel = new BoardTableModel(this, this);
    setAutoFill(1);
    setAutoNumeringBoard(1);
}
