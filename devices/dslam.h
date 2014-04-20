#ifndef DSLAM_H
#define DSLAM_H

#include "device.h"

#include <ports/xdslport.h>

class BoardTableModel;

class Dslam : public Device
{
    Q_OBJECT
public:
    explicit Dslam(QObject *parent = 0);
    Dslam(QString name, QString ip, QObject *parent = 0);
    explicit Dslam(Device *source, QObject *parent = 0);
    //TODO: Assign only 2 values: 0 or 1. Replace using type on bool
    short autoFill() const;
    void setAutoFill(short autoFill);

    short autoNumeringBoard() const;
    void setAutoNumeringBoard(short autoNumeringBoard);

    DeviceType::Enum deviceType() const;
    virtual int countPorts(BoardType::Enum boardType);
    virtual long snmpPortIndex(BoardType::Enum boardType, int slot, int port);
    virtual XdslPort::Ptr createPort(BoardType::Enum boardType, int boardIndex,
                                     int portIndex, QObject *parent);

    static QList<DslProfile> *defaultAdslProfiles();
    static QList<DslProfile> *defaultShdslProfiles();

    BoardTableModel *boardTableModel();

    bool getServiceDataFromDevice();

    typedef QSharedPointer<Dslam> Ptr;

private:
    void InitializeBoardTableModel();
    void boardListIsModified();

    short mAutoFill;
    short mAutoNumeringBoard;
    BoardTableModel *mBoardListModel;
};

#endif // DSLAM_H
