#ifndef DSLAMINFO_H
#define DSLAMINFO_H

#ifdef _MSC_VER
#include "../Info/deviceinfo.h"
#else
#include "Info/deviceinfo.h"
#endif

class BoardTableModel;

class DslamInfo : public DeviceInfo
{
    Q_OBJECT
public:
    DslamInfo(QObject *parent = 0);
    DslamInfo(QString name, QString ip, QObject *parent = 0);
    DslamInfo(DeviceInfo *source, QObject *parent = 0);
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

    typedef QSharedPointer<DslamInfo> Ptr;

private:
    void InitializeBoardTableModel();

    short mAutoFill;
    short mAutoNumeringBoard;
    BoardTableModel *mBoardListModel;
};

#endif // DSLAMINFO_H
