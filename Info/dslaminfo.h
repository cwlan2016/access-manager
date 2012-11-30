#ifndef DSLAMINFO_H
#define DSLAMINFO_H

#include "stdafx.h"
#include "deviceinfo.h"
#include "Models/boardlistmodel.h"

class DslamInfo : public DeviceInfo
{
public:
    DslamInfo();
    DslamInfo(QString name, QString ip, DeviceModel deviceModel);
    virtual ~DslamInfo();
    short autoFill() const;
    short autoNumeringBoard() const;
    void setAutoFill(short autoFill);
    void setAutoNumeringBoard(short autoNumeringBoard);
    BoardListModel* boardListModel();
    virtual bool getServiceDataFromDevice();

    typedef std::shared_ptr<DslamInfo> Ptr;
private:
    BoardListModel* mBoardListModel;
};



#endif // DSLAMINFO_H
