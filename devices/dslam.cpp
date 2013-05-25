#include "dslam.h"

#include <models/boardtablemodel.h>

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
    if (mAutoFill != autoFill) {
        mAutoFill = autoFill;
        emit modified();
    }
}

short Dslam::autoNumeringBoard() const
{
    return mAutoNumeringBoard;
}

void Dslam::setAutoNumeringBoard(short autoNumeringBoard)
{
    if (mAutoNumeringBoard != autoNumeringBoard) {
        mAutoNumeringBoard = autoNumeringBoard;
        emit modified();
    }
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

long Dslam::snmpPortIndex(BoardType::Enum boardType, int slot,
                                   int port)
{
    Q_UNUSED(boardType)
    Q_UNUSED(slot)
    Q_UNUSED(port)

    return 0;
}

XdslPort::Ptr Dslam::createPort(BoardType::Enum boardType, int boardIndex,
                                int portIndex, QObject *parent)
{
    Q_UNUSED(boardType)
    Q_UNUSED(boardIndex)
    Q_UNUSED(portIndex)
    Q_UNUSED(parent)

    return 0;
}

QList<DslProfile> *Dslam::defaultAdslProfiles()
{
    return 0;
}

QList<DslProfile> *Dslam::defaultShdslProfiles()
{
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
    connect(mBoardListModel, &BoardTableModel::modified,
            this, &Dslam::boardListIsModified);

    mAutoFill = 1;
    mAutoNumeringBoard = 1;
}

void Dslam::boardListIsModified()
{
    emit modified();
}

