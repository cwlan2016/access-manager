#ifndef DSLAMINFO_H
#define DSLAMINFO_H

#ifdef _MSC_VER
#include "../Models/boardtablemodel.h"
#else
#include "Models/boardtablemodel.h"
#endif

class DslamInfo : public DeviceInfo
{
    Q_OBJECT
public:
    DslamInfo(QObject *parent = 0);
    DslamInfo(QString name, QString ip, DeviceModel::Enum deviceModel, QObject *parent = 0);
    short autoFill() const;
    short autoNumeringBoard() const;
    void setAutoFill(short autoFill);
    void setAutoNumeringBoard(short autoNumeringBoard);
    BoardTableModel *boardListModel();
    bool getServiceDataFromDevice();

    typedef QSharedPointer<DslamInfo> Ptr;
private:
    BoardTableModel *mBoardListModel;
};



#endif // DSLAMINFO_H
