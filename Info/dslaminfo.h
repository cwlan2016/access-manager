#ifndef DSLAMINFO_H
#define DSLAMINFO_H

#ifdef _MSC_VER
#include "../Models/boardlistmodel.h"
#else
#include "Models/boardlistmodel.h"
#endif

class DslamInfo : public DeviceInfo
{
public:
    DslamInfo();
    DslamInfo(QString name, QString ip, DeviceModel::Enum deviceModel);
    virtual ~DslamInfo();
    short autoFill() const;
    short autoNumeringBoard() const;
    void setAutoFill(short autoFill);
    void setAutoNumeringBoard(short autoNumeringBoard);
    BoardListModel *boardListModel();
    virtual bool getServiceDataFromDevice();

    typedef std::shared_ptr<DslamInfo> Ptr;
private:
    BoardListModel *mBoardListModel;
};



#endif // DSLAMINFO_H
