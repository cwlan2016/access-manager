#ifndef DSLAM_H
#define DSLAM_H

#include "device.h"

class BoardTableModel;

class Dslam : public Device
{
    Q_OBJECT
public:
    Dslam(QObject *parent = 0);
    Dslam(QString name, QString ip, QObject *parent = 0);
    Dslam(Device *source, QObject *parent = 0);
    //TODO: Assign only 2 values: 0 or 1. Replace using type on bool
    short autoFill() const;
    void setAutoFill(short autoFill);

    short autoNumeringBoard() const;
    void setAutoNumeringBoard(short autoNumeringBoard);

    DeviceType::Enum deviceType() const;
    virtual int countPorts(BoardType::Enum boardType);
    virtual long snmpInterfaceIndex(BoardType::Enum boardType, int slot, int port);

    BoardTableModel *boardTableModel();

    bool getServiceDataFromDevice();

    typedef QSharedPointer<Dslam> Ptr;

private:
    void InitializeBoardTableModel();

    short mAutoFill;
    short mAutoNumeringBoard;
    BoardTableModel *mBoardListModel;
};

#endif // DSLAM_H
