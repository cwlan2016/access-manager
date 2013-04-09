#include "dslaminfo.h"

#ifdef _MSC_VER
#include "../Models/boardtablemodel.h"
#else
#include "Models/boardtablemodel.h"
#endif

DslamInfo::DslamInfo(QObject *parent) :
    DeviceInfo(parent)
{
    InitializeBoardTableModel();
}

DslamInfo::DslamInfo(QString name, QString ip, QObject *parent) :
    DeviceInfo(name, ip, parent)
{
    InitializeBoardTableModel();
}

DslamInfo::DslamInfo(DeviceInfo *source, QObject *parent) :
    DeviceInfo(source, parent)
{
    InitializeBoardTableModel();

    if (source->deviceType() == DeviceType::Dslam) {
        mAutoFill = static_cast<DslamInfo *>(source)->autoFill();
        mAutoNumeringBoard = static_cast<DslamInfo *>(source)->autoNumeringBoard();
    }

}

short DslamInfo::autoFill() const
{
    return mAutoFill;
}

void DslamInfo::setAutoFill(short autoFill)
{
    mAutoFill = autoFill;
}

short DslamInfo::autoNumeringBoard() const
{
    return mAutoNumeringBoard;
}

void DslamInfo::setAutoNumeringBoard(short autoNumeringBoard)
{
    mAutoNumeringBoard = autoNumeringBoard;
}

DeviceType::Enum DslamInfo::deviceType() const
{
    return DeviceType::Dslam;
}

int DslamInfo::countPorts(BoardType::Enum boardType)
{
    Q_UNUSED(boardType)

    return 0;
}

long DslamInfo::snmpInterfaceIndex(BoardType::Enum boardType, int slot,
                                   int port)
{
    Q_UNUSED(boardType)
    Q_UNUSED(slot)
    Q_UNUSED(port)

    return 0;
}

BoardTableModel *DslamInfo::boardTableModel()
{
    return mBoardListModel;
}

bool DslamInfo::getServiceDataFromDevice()
{
    if (mBoardListModel == 0)
        return false;

    bool result = mBoardListModel->getBoardListFromDevice();

    if (!result)
        mError = mBoardListModel->error();

    return result;
}

void DslamInfo::InitializeBoardTableModel()
{
    mBoardListModel = new BoardTableModel(this, this);
    setAutoFill(1);
    setAutoNumeringBoard(1);
}
