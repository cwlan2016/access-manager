#include "dslaminfo.h"

#ifdef _MSC_VER
#include "../Models/boardtablemodel.h"
#else
#include "Models/boardtablemodel.h"
#endif

DslamInfo::DslamInfo(QObject *parent) :
    DeviceInfo(parent)
{
    //setDeviceModel(DeviceModel::MA5600);
    InitializeBoardTableModel();
}

DslamInfo::DslamInfo(QString name, QString ip, QObject *parent) :
    DeviceInfo(name, ip, parent)
{
    InitializeBoardTableModel();
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
