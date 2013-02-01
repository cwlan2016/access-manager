#include "dslaminfo.h"

DslamInfo::DslamInfo(QObject *parent) :
    DeviceInfo(parent)
{
    setDeviceModel(DeviceModel::MA5600);
    mBoardListModel = new BoardTableModel(this);
    mBoardListModel->setAutoFill(1);
    mBoardListModel->setAutoNumeringBoard(1);
}

DslamInfo::DslamInfo(QString name, QString ip, DeviceModel::Enum deviceModel, QObject *parent) :
    DeviceInfo(name, ip, deviceModel, parent)
{
    mBoardListModel = new BoardTableModel(this);
    mBoardListModel->setAutoFill(1);
    mBoardListModel->setAutoNumeringBoard(1);
}

short DslamInfo::autoFill() const
{
    return mBoardListModel->autoFill();
}

short DslamInfo::autoNumeringBoard() const
{
    return mBoardListModel->autoNumeringBoard();
}

void DslamInfo::setAutoFill(short autoFill)
{
    mBoardListModel->setAutoFill(autoFill);
}

void DslamInfo::setAutoNumeringBoard(short autoNumeringBoard)
{
    mBoardListModel->setAutoNumeringBoard(autoNumeringBoard);
}

BoardTableModel *DslamInfo::boardListModel()
{
    return mBoardListModel;
}

bool DslamInfo::getServiceDataFromDevice()
{
    if (mBoardListModel == nullptr)
        return false;

    bool result = mBoardListModel->getBoardListFromDevice();

    if (!result)
        mError = mBoardListModel->error();

    return result;
}
