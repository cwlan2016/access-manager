#include "dslaminfo.h"

DslamInfo::DslamInfo()
{
    setDeviceModel(DeviceModel::MA5600);
    mBoardListModel = new BoardListModel();
    mBoardListModel->setAutoFill(1);
    mBoardListModel->setAutoNumeringBoard(1);
}

DslamInfo::DslamInfo(QString name, QString ip, DeviceModel deviceModel) :
    DeviceInfo(name, ip, deviceModel)
{
    mBoardListModel = new BoardListModel();
    mBoardListModel->setAutoFill(1);
    mBoardListModel->setAutoNumeringBoard(1);
}

DslamInfo::~DslamInfo()
{
    delete mBoardListModel;
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

BoardListModel* DslamInfo::boardListModel()
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
