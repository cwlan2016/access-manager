#include "devicelisthandler.h"

#include "converters.h"

DeviceListHandler::DeviceListHandler() :
    QXmlDefaultHandler()
{
}

bool DeviceListHandler::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &attributes)
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

bool DeviceListHandler::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    Q_UNUSED(namespaceURI);
    Q_UNUSED(localName);

    if ((qName == "switch")
            || (qName == "olt")) {
        m_deviceList.push_back(std::move(m_currDeviceInfoElement));
    } else if (qName == "dslam") {
        std::static_pointer_cast<DslamInfo>(m_currDeviceInfoElement)->boardListModel()->setBoardList(m_boardList);
        std::static_pointer_cast<DslamInfo>(m_currDeviceInfoElement)->boardListModel()->setParentDevice(m_currDeviceInfoElement);
        m_deviceList.push_back(std::move(m_currDeviceInfoElement));
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

bool DeviceListHandler::fatalError(const QXmlParseException &exception)
{
    m_error = QString::fromUtf8("Ошибка при разборе списка устройств в строке %1, позиции %2:\n%3")
              .arg(exception.lineNumber())
              .arg(exception.columnNumber())
              .arg(exception.message());
    return false;
}

QString DeviceListHandler::errorString() const
{
    return m_error;
}

std::vector<DeviceInfo::Ptr> &DeviceListHandler::deviceList()
{
    return m_deviceList;
}

void DeviceListHandler::parseSwitchElement(const QXmlAttributes &attributes)
{
    QString name = attributes.value("name");
    QString ip = attributes.value("ip");
    DeviceModel::Enum deviceModel = DeviceModel::from(attributes.value("model"));
    m_currDeviceInfoElement = std::make_shared<SwitchInfo>(name, ip, deviceModel);
    std::static_pointer_cast<SwitchInfo>(m_currDeviceInfoElement)->setInetVlanTag(attributes.value("inetVlan").toUInt());
    std::static_pointer_cast<SwitchInfo>(m_currDeviceInfoElement)->setIptvVlanTag(attributes.value("iptvVlan").toUInt());
}

void DeviceListHandler::parseDslamElement(const QXmlAttributes &attributes)
{
    QString name = attributes.value("name");
    QString ip = attributes.value("ip");
    DeviceModel::Enum deviceModel = DeviceModel::from(attributes.value("model"));

    m_currDeviceInfoElement = std::make_shared<DslamInfo>(name, ip, deviceModel);
    std::static_pointer_cast<DslamInfo>(m_currDeviceInfoElement)->boardListModel()->setParentDevice(m_currDeviceInfoElement);
    m_boardList.clear();

    std::static_pointer_cast<DslamInfo>(m_currDeviceInfoElement)->setAutoFill(attributes.value("autofill").toUInt());
    std::static_pointer_cast<DslamInfo>(m_currDeviceInfoElement)->setAutoNumeringBoard(attributes.value("autonumeringboard").toUInt());
}

void DeviceListHandler::parseBoardElement(const QXmlAttributes &attributes)
{
    BoardInfo info;
    info.setType(BoardType::from(attributes.value("type")));
    info.setFirstPair(attributes.value("firstpair").toUInt());
    int number = attributes.value("number").toUInt();
    info.setNumber(number);
    m_boardList.insert(number, info);
}

void DeviceListHandler::parseOltElement(const QXmlAttributes &attributes)
{
    QString name = attributes.value("name");
    QString ip = attributes.value("ip");
    DeviceModel::Enum deviceModel = DeviceModel::from(attributes.value("model"));

    m_currDeviceInfoElement = std::make_shared<OltInfo>(name, ip, deviceModel);
}

void DeviceListHandler::parseUniOltProfileElement(const QXmlAttributes &attributes)
{
    int index = attributes.value("index").toInt();
    QString name = attributes.value("name");

    std::static_pointer_cast<OltInfo>(m_currDeviceInfoElement)->addServiceProfile(index, name);
}

void DeviceListHandler::parseMultOltProfileElement(const QXmlAttributes &attributes)
{
    int index = attributes.value("index").toInt();
    QString name = attributes.value("name");

    std::static_pointer_cast<OltInfo>(m_currDeviceInfoElement)->addMulticastProfile(index, name);
}
