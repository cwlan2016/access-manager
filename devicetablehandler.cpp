#include "devicetablehandler.h"

#include "converters.h"
#include "Info/dslaminfo.h"
#include "Info/oltinfo.h"
#include "Info/switchinfo.h"
#include "Models/boardtablemodel.h"

DeviceTableHandler::DeviceTableHandler() :
    QXmlDefaultHandler()
{
}

bool DeviceTableHandler::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attributes)
{
    Q_UNUSED(namespaceURI);
    Q_UNUSED(localName);

    if (qName == "switch") {
        parseSwitchElement(attributes);
    } else if (qName == "dslam") {
        parseDslamElement(attributes);
    } else if (qName == "board") {
        parseBoardElement(attributes);
    } else if (qName == "olt") {
        parseOltElement(attributes);
    } else if (qName == "uniprofile") {
        parseUniOltProfileElement(attributes);
    } else if (qName == "multprofile") {
        parseMultOltProfileElement(attributes);
    }

    return true;
}

bool DeviceTableHandler::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    Q_UNUSED(namespaceURI);
    Q_UNUSED(localName);

    if ((qName == "switch")
            || (qName == "olt")) {
        mDeviceList.push_back(mCurrDeviceInfo);
    } else if (qName == "dslam") {
        DslamInfo::Ptr dslamInfo = mCurrDeviceInfo.objectCast<DslamInfo>();
        BoardTableModel *boardTableModel = dslamInfo->boardTableModel();
        boardTableModel->setBoardList(mBoardList);
        boardTableModel->setParentDevice(dslamInfo);
        mDeviceList.push_back(mCurrDeviceInfo);
    }
//    else if(qName == "board")
//    {
//        ничего не надо делать. добавление досок в лист
//        идет при начале парсинга елемента board
//        добавление листа в модель идет при окончании парсинга
//        элемента dslam. см. выше.
//    }

    return true;
}

bool DeviceTableHandler::fatalError(const QXmlParseException &exception)
{
    m_error = QString::fromUtf8("Ошибка при разборе списка устройств в строке %1, позиции %2:\n%3")
              .arg(exception.lineNumber())
              .arg(exception.columnNumber())
              .arg(exception.message());
    return false;
}

QString DeviceTableHandler::errorString() const
{
    return m_error;
}

QVector<DeviceInfo::Ptr> &DeviceTableHandler::deviceList()
{
    return mDeviceList;
}

void DeviceTableHandler::parseSwitchElement(const QXmlAttributes &attributes)
{
    QString name = attributes.value("name");
    QString ip = attributes.value("ip");
    DeviceModel::Enum deviceModel = DeviceModel::from(attributes.value("model"));
    mCurrDeviceInfo = DeviceInfo::Ptr(new SwitchInfo(name, ip, deviceModel));
    mCurrDeviceInfo.objectCast<SwitchInfo>()->setInetVlanTag(attributes.value("inetVlan").toUInt());
    mCurrDeviceInfo.objectCast<SwitchInfo>()->setIptvVlanTag(attributes.value("iptvVlan").toUInt());
}

void DeviceTableHandler::parseDslamElement(const QXmlAttributes &attributes)
{
    QString name = attributes.value("name");
    QString ip = attributes.value("ip");
    DeviceModel::Enum deviceModel = DeviceModel::from(attributes.value("model"));

    mCurrDeviceInfo = DeviceInfo::Ptr(new DslamInfo(name, ip, deviceModel));
    DslamInfo::Ptr dslamInfo = mCurrDeviceInfo.objectCast<DslamInfo>();
    dslamInfo->boardTableModel()->setParentDevice(dslamInfo);
    mBoardList.clear();

    dslamInfo->setAutoFill(attributes.value("autofill").toUInt());
    dslamInfo->setAutoNumeringBoard(attributes.value("autonumeringboard").toUInt());
}

void DeviceTableHandler::parseBoardElement(const QXmlAttributes &attributes)
{
    BoardInfo info;
    info.setType(BoardType::from(attributes.value("type")));
    info.setFirstPair(attributes.value("firstpair").toUInt());
    int number = attributes.value("number").toUInt();
    info.setIndex(number);
    mBoardList.insert(number, info);
}

void DeviceTableHandler::parseOltElement(const QXmlAttributes &attributes)
{
    QString name = attributes.value("name");
    QString ip = attributes.value("ip");
    DeviceModel::Enum deviceModel = DeviceModel::from(attributes.value("model"));

    mCurrDeviceInfo = DeviceInfo::Ptr(new OltInfo(name, ip, deviceModel));
}

void DeviceTableHandler::parseUniOltProfileElement(const QXmlAttributes &attributes)
{
    int index = attributes.value("index").toInt();
    QString name = attributes.value("name");

    mCurrDeviceInfo.objectCast<OltInfo>()->addServiceProfile(index, name);
}

void DeviceTableHandler::parseMultOltProfileElement(const QXmlAttributes &attributes)
{
    int index = attributes.value("index").toInt();
    QString name = attributes.value("name");

    mCurrDeviceInfo.objectCast<OltInfo>()->addMulticastProfile(index, name);
}
