#include "devicetablemodel.h"

#ifdef _MSC_VER
#include "../basicdialogs.h"
#include "../config.h"
#include "../constant.h"
#include "../converters.h"
#include "../snmpclient.h"
#include "../Info/boardinfo.h"
#include "../Info/dslaminfoma5300.h"
#include "../Info/dslaminfoma5600.h"
#include "../Info/dslaminfomxa32.h"
#include "../Info/dslaminfomxa64.h"
#include "../Info/oltinfolte8st.h"
#include "../Info/oltinfoltp8x.h"
#include "../Info/switchinfodes3526.h"
#include "../Info/switchinfodes3528.h"
#include "../Info/switchinfodes3550.h"
#include "../Models/boardtablemodel.h"
#else
#include "basicdialogs.h"
#include "config.h"
#include "constant.h"
#include "converters.h"
#include "snmpclient.h"
#include "Info/boardinfo.h"
#include "Info/dslaminfoma5300.h"
#include "Info/dslaminfoma5600.h"
#include "Info/dslaminfomxa32.h"
#include "Info/dslaminfomxa64.h"
#include "Info/oltinfolte8st.h"
#include "Info/oltinfoltp8x.h"
#include "Info/switchinfodes3526.h"
#include "Info/switchinfodes3528.h"
#include "Info/switchinfodes3550.h"
#include "Models/boardtablemodel.h"
#endif

// Columns
// DisplayRole, EditRole
// 0 - name
// 1 - model_device
// 2 - ip
// 3 - type_device
DeviceTableModel::DeviceTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    mDeviceListPath = Config::path() % "devicelist.xml";
    mDeviceListBackupPath = Config::path() % "devicelist.bak";
    mModified = false;
}

int DeviceTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mList.size();
}

int DeviceTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4;
}

QVariant DeviceTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole) {
        if (index.column() == 0) {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        } else {
            return int(Qt::AlignCenter | Qt::AlignVCenter);
        }
    } else if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (index.column() == 0) {
            return mList.at(index.row())->name();
        } else if (index.column() == 1) {
            return DeviceModel::toString(mList.at(index.row())->deviceModel());
        }  else if (index.column() == 2) {
            return mList.at(index.row())->ip();
        } else if (index.column() == 3) {
            return DeviceType::toString(mList.at(index.row())->deviceType());
        }
    }

    return QVariant();
}

bool DeviceTableModel::setData(const QModelIndex &index, const QVariant &value,
                               int role)
{
    if (!index.isValid() || (role != Qt::EditRole))
        return false;

    if (role == Qt::EditRole) {
        if (index.column() == 0) {
            mList[index.row()]->setName(value.toString().trimmed());
            emit dataChanged(index, index);
            mModified = true;
            return true;
        } else if (index.column() == 1) {
            DeviceModel::Enum newModel = DeviceModel::from(value.toString());
            DeviceType::Enum newType = DeviceType::from(newModel);

            QModelIndex deviceTypeIndex = this->index(index.row(), 3);

            if (mList.at(index.row())->deviceType() == DeviceType::Other) {
                //device only now added
                DeviceInfo::Ptr deviceInfo;

                if (newType == DeviceType::Switch) {
                    deviceInfo = DeviceInfo::Ptr(new SwitchInfo(this));
                } else if (newType == DeviceType::Dslam) {
                    deviceInfo = DeviceInfo::Ptr(new DslamInfo(this));
                    //FIXME: Setup new type created device.
                    //DslamInfo::Ptr dslamInfo = deviceInfo.objectCast<DslamInfo>();
                    //dslamInfo->boardTableModel()->setParentDevice(dslamInfo);
                } else if (newType == DeviceType::Olt) {
                    deviceInfo = DeviceInfo::Ptr(new OltInfo(this));
                } else {
                    deviceInfo = DeviceInfo::Ptr(new DeviceInfo(this));
                }

                deviceInfo->setName(mList[index.row()]->name());
                deviceInfo->setIp(mList[index.row()]->ip());
                //deviceInfo->setDeviceModel(newModel);

                mList[index.row()] = deviceInfo;
            } else if (mList[index.row()]->deviceType() != newType) {
                BasicDialogs::information(0, BasicDialogStrings::Info,
                                          QString::fromUtf8("Запрещено менять модель с одного типа устройства на другое."));
                return false;
            } else {
                //mList[index.row()]->setDeviceModel(newModel);
            }

            emit dataChanged(index, index);
            emit dataChanged(deviceTypeIndex, deviceTypeIndex);
            mModified = true;

            return true;
        } else if (index.column() == 2) {
            if (!validIpAddress(value.toString())) {
                BasicDialogs::information(0, trUtf8("Редактирование IP адреса"),
                                          trUtf8("Некорректный IP адрес."));
                return false;
            }

            mList[index.row()]->setIp(value.toString().trimmed());
            emit dataChanged(index, index);
            mModified = true;

            return true;
        }
    }

    return false;
}

QVariant DeviceTableModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const
{
    if (orientation == Qt::Vertical)
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (section == 0) {
            return DeviceTableModelStrings::Name;
        } else if (section == 1) {
            return DeviceTableModelStrings::DeviceModel;
        } else if (section == 2) {
            return DeviceTableModelStrings::IP;
        } else if (section == 3) {
            return DeviceTableModelStrings::DeviceType;
        }
    } else if (role == Qt::TextAlignmentRole) {
        return int(Qt::AlignCenter | Qt::AlignVCenter);
    } else if (role == Qt::FontRole) {
        QFont font(qApp->font());
        font.setBold(true);
        return font;
    }

    return QVariant();
}

Qt::ItemFlags DeviceTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (index.column() != 3)
        flags |= Qt::ItemIsEditable;

    return flags;
}

bool DeviceTableModel::insertRow(int row, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row);
    DeviceInfo::Ptr element = DeviceInfo::Ptr(new DeviceInfo(this));
    mList.push_back(element);
    endInsertRows();

    mModified = true;

    return true;
}

bool DeviceTableModel::removeRow(int row, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row);
    mList.erase(mList.begin() + row);
    endRemoveRows();

    mModified = true;

    return true;
}

bool DeviceTableModel::isModified()
{
    return mModified;
}

void DeviceTableModel::setModified(bool state)
{
    mModified = state;
}

bool DeviceTableModel::load()
{
    beginResetModel();

    mList.clear();

    if (!exist()) {
        endResetModel();

        return true;
    }

    QFile file(mDeviceListPath);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        mError = QString::fromUtf8("Ошибка: невозможно открыть файл %1: %2")
                 .arg(file.fileName())
                 .arg(file.errorString());
        endResetModel();

        return false;
    }

    QXmlSchema schema;
    QFile schemaFile(QString::fromUtf8(":/xsd/devicelist.xsd"));
    schemaFile.open(QFile::ReadOnly);
    schema.load(&schemaFile);

    if (!schema.isValid()) {
        mError = QString::fromUtf8("Файл схемы содержит ошибки.");
        file.close();
        endResetModel();

        return false;
    }

    QXmlSchemaValidator validator(schema);

    if (!validator.validate(&file)) {
        mError = QString::fromUtf8("Файл списка устройств содержит ошибки.");
        file.close();

        return false;
    }

    file.seek(0);

    QXmlStreamReader reader(&file);

    reader.readNextStartElement(); //read root
    reader.readNextStartElement(); //read first element

    while (!reader.atEnd()) {
        if (reader.name() == "dslam") {
            parseDslamElement(reader);
            continue;
        } else if (reader.name() == "switch") {
            parseSwitchElement(reader);
            continue;
        } else if (reader.name() == "olt") {
            parseOltElement(reader);
            continue;
        } else {
            reader.skipCurrentElement();
            continue;
        }

        readNextElement(reader);
    }

    file.close();

    endResetModel();
    mModified = false;

    return true;
}


bool DeviceTableModel::save()
{
    if (exist())
        backup();

    QFile file(mDeviceListPath);
    QFileInfo fileInfo(mDeviceListPath);

    if (!fileInfo.dir().exists())
        fileInfo.dir().mkpath(fileInfo.dir().path());

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        mError = QString::fromUtf8("Ошибка: невозможно открыть файл на запись %1: %2")
                 .arg(file.fileName())
                 .arg(file.errorString());
        return false;
    }

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.setAutoFormattingIndent(4);
    writer.setCodec("UTF-8");
    writer.writeStartDocument();

    writer.writeStartElement("devicelist");

    auto it = mList.constBegin();
    auto end = mList.constEnd();
    for (; it != end; ++it) {
        if ((*it)->deviceType() == DeviceType::Switch) {
            createSwitchElement(writer, (*it).objectCast<SwitchInfo>());
        } else if ((*it)->deviceType() == DeviceType::Dslam) {
            createDslamElement(writer, (*it).objectCast<DslamInfo>());
        } else if ((*it)->deviceType() == DeviceType::Olt) {
            createOltElement(writer, (*it).objectCast<OltInfo>());
        }
    }

    writer.writeEndElement();
    writer.writeEndDocument();

    file.close();
    mModified = false;

    return true;
}

QVector<DeviceInfo::Ptr> &DeviceTableModel::deviceList()
{
    return mList;
}

BoardTableModel *DeviceTableModel::boardListModel(QModelIndex index)
{
    return mList.at(index.row()).objectCast<DslamInfo>()->boardTableModel();
}

int DeviceTableModel::inetVlan(QModelIndex index)
{
    return mList.at(index.row()).objectCast<SwitchInfo>()->inetVlanTag();
}

int DeviceTableModel::iptvVlan(QModelIndex index)
{
    return mList.at(index.row()).objectCast<SwitchInfo>()->iptvVlanTag();
}

QStringListModel *DeviceTableModel::serviceProfileOltListModel(QModelIndex index)
{
    return mList.at(index.row()).objectCast<OltInfo>()->serviceProfileListModel(this);
}

QStringListModel *DeviceTableModel::multicastProfileOltListModel(QModelIndex index)
{
    return mList.at(index.row()).objectCast<OltInfo>()->multicastProfileListModel(this);
}

bool DeviceTableModel::getVlanTagFromDevice(QModelIndex index)
{
    if (!index.isValid()) {
        mError = QString::fromUtf8("Неверный индекс.");
        return false;
    }

    if (mList[index.row()]->deviceType() != DeviceType::Switch) {
        mError = QString::fromUtf8("Обновлять информацию о вланах можно только на коммутаторе!");
        return false;
    }

    bool result = mList[index.row()]->getServiceDataFromDevice();

    if (!result) {
        mError = mList[index.row()]->error();
        return false;
    } else {
        mModified = true;
        return true;
    }
}

bool DeviceTableModel::getBoardListFromDevice(QModelIndex index)
{
    if (!index.isValid()) {
        mError = QString::fromUtf8("Неверный индекс.");
        return false;
    }

    if (mList.at(index.row())->deviceType() != DeviceType::Dslam) {
        mError = QString::fromUtf8("Обновлять информацию о досках можно только на дсламе!");
        return false;
    }

    DslamInfo::Ptr deviceInfo = mList.at(index.row()).objectCast<DslamInfo>();
    bool result = deviceInfo->boardTableModel()->getBoardListFromDevice();

    if (deviceInfo->autoNumeringBoard())
        deviceInfo->boardTableModel()->renumeringPairList();

    if (!result) {
        mError = deviceInfo->boardTableModel()->error();
        return false;
    } else {
        mModified = true;
        return true;
    }
}

bool DeviceTableModel::getProfilesFromDevice(QModelIndex index)
{
    if (!index.isValid()) {
        mError = QString::fromUtf8("Неверный индекс.");
        return false;
    }

    if (mList.at(index.row())->deviceType() != DeviceType::Olt) {
        mError = QString::fromUtf8("Обновлять информацию о профилях можно только на олт!");
        return false;
    }

    OltInfo::Ptr deviceInfo = mList.at(index.row()).objectCast<OltInfo>();
    bool result = deviceInfo->getServiceDataFromDevice();

    if (!result) {
        mError = deviceInfo->error();
        return false;
    } else {
        mModified = true;
        return true;
    }
}

QString DeviceTableModel::error() const
{
    return mError;
}

bool DeviceTableModel::exist()
{
    QFileInfo fileInfo(mDeviceListPath);

    return fileInfo.exists();
}

bool DeviceTableModel::backup()
{
    if (!exist())
        return false;

    QFile::remove(mDeviceListBackupPath);
    QFile::copy(mDeviceListPath, mDeviceListBackupPath);

    return true;
}

void DeviceTableModel::readNextElement(QXmlStreamReader &reader)
{
    while (!reader.atEnd()) {
        reader.readNext();

        if (reader.isStartElement())
            return;
    }
}

void DeviceTableModel::parseSwitchElement(QXmlStreamReader &reader)
{
    QString modelString = reader.attributes().value("model").toString();
    DeviceModel::Enum switchModel = DeviceModel::from(modelString);
    DeviceInfo::Ptr switchInfo;

    switch (switchModel)
    {
    case DeviceModel::DES3526:
        switchInfo = DeviceInfo::Ptr(new SwitchInfoDes3526(this));
        break;
    case DeviceModel::DES3528:
        switchInfo = DeviceInfo::Ptr(new SwitchInfoDes3528(this));
        break;
    case DeviceModel::DES3550:
        switchInfo = DeviceInfo::Ptr(new SwitchInfoDes3550(this));
        break;
    default:
        reader.skipCurrentElement();
        return;
    }

    if (switchInfo.isNull())
        return;

    int inetVlan = reader.attributes().value("inetVlan").toString().toInt();
    int iptvVlan = reader.attributes().value("iptvVlan").toString().toInt();

    switchInfo->setName(reader.attributes().value("name").toString());
    switchInfo->setIp(reader.attributes().value("ip").toString());
    switchInfo.objectCast<SwitchInfo>()->setInetVlanTag(inetVlan);
    switchInfo.objectCast<SwitchInfo>()->setIptvVlanTag(iptvVlan);

    readNextElement(reader);

    mList.push_back(switchInfo);
}

void DeviceTableModel::parseDslamElement(QXmlStreamReader &reader)
{
    QString modelString = reader.attributes().value("model").toString();
    DeviceModel::Enum dslamModel = DeviceModel::from(modelString);
    DeviceInfo::Ptr dslamInfo;

    switch (dslamModel)
    {
    case DeviceModel::MA5600:
        dslamInfo = DeviceInfo::Ptr(new DslamInfoMa5600(this));
        break;
    case DeviceModel::MA5300:
        dslamInfo = DeviceInfo::Ptr(new DslamInfoMa5300(this));
        break;
    case DeviceModel::MXA32:
        dslamInfo = DeviceInfo::Ptr(new DslamInfoMxa32(this));
        break;
    case DeviceModel::MXA64:
        dslamInfo = DeviceInfo::Ptr(new DslamInfoMxa64(this));
        break;
    default:
        reader.skipCurrentElement();
        return;
    }

    if (dslamInfo.isNull())
        return;

    short autoFill = reader.attributes().value("autofill").toString().toInt();
    short autoNumering = reader.attributes().value("autonumeringboard").toString().toInt();

    dslamInfo->setName(reader.attributes().value("name").toString());
    dslamInfo->setIp(reader.attributes().value("ip").toString());
    dslamInfo.objectCast<DslamInfo>()->setAutoFill(autoFill);
    dslamInfo.objectCast<DslamInfo>()->setAutoNumeringBoard(autoNumering);

    readNextElement(reader);

    if (reader.atEnd())
        return;

    if (reader.name() == "board")
        parseDslamBoardList(reader, dslamInfo);

    mList.push_back(dslamInfo);
}

void DeviceTableModel::parseDslamBoardList(QXmlStreamReader &reader,
                                           DeviceInfo::Ptr deviceInfo)
{
    DslamInfo::Ptr dslamInfo = deviceInfo.objectCast<DslamInfo>();

    int index;
    int firstPair;
    BoardType::Enum type;

    while (reader.name() == "board") {
        index = reader.attributes().value("number").toString().toInt();
        firstPair = reader.attributes().value("firstpair").toString().toInt();
        type = BoardType::from(reader.attributes().value("type").toString());
        dslamInfo->boardTableModel()->addBoard(index, type, firstPair);

        readNextElement(reader);

        if (reader.atEnd())
            return;
    }
}

void DeviceTableModel::parseOltElement(QXmlStreamReader &reader)
{
    QString modelString = reader.attributes().value("model").toString();
    DeviceModel::Enum oltModel = DeviceModel::from(modelString);
    DeviceInfo::Ptr oltInfo;

    switch (oltModel)
    {
    case DeviceModel::LTE8ST:
        oltInfo = DeviceInfo::Ptr(new OltInfoLte8st(this));
        break;
    case DeviceModel::LTP8X:
        oltInfo = DeviceInfo::Ptr(new OltInfoLtp8x(this));
        break;
    default:
        reader.skipCurrentElement();
        return;
    }

    if (oltInfo.isNull())
        return;

    oltInfo->setName(reader.attributes().value("name").toString());
    oltInfo->setIp(reader.attributes().value("ip").toString());

    readNextElement(reader);

    if (reader.atEnd())
        return;

    if ((reader.name() == "uniprofile") || (reader.name() == "multprofile"))
        parseOltProfileList(reader, oltInfo);


    mList.push_back(oltInfo);
}

void DeviceTableModel::parseOltProfileList(QXmlStreamReader &reader,
                                           DeviceInfo::Ptr deviceInfo)
{
    OltInfo::Ptr oltInfo = deviceInfo.objectCast<OltInfo>();

    int index;
    QString name;

    while (true) {
        if (reader.name() == "uniprofile") {
            index = reader.attributes().value("index").toString().toInt();
            name = reader.attributes().value("name").toString();

            oltInfo->addServiceProfile(index, name);
        } else if (reader.name() == "multprofile") {
            index = reader.attributes().value("index").toString().toInt();
            name = reader.attributes().value("name").toString();

            oltInfo->addMulticastProfile(index, name);
        } else {
            break;
        }

        readNextElement(reader);

        if (reader.atEnd())
            return;
    }
}

void DeviceTableModel::createSwitchElement(QXmlStreamWriter &writer,
        const SwitchInfo::Ptr &deviceInfo)
{
    writer.writeStartElement("switch");

    writer.writeAttribute("name", deviceInfo->name());
    writer.writeAttribute("ip", deviceInfo->ip());
    writer.writeAttribute("model", DeviceModel::toString(deviceInfo->deviceModel()));
    writer.writeAttribute("inetVlan", QString::number(deviceInfo->inetVlanTag()));
    writer.writeAttribute("iptvVlan", QString::number(deviceInfo->iptvVlanTag()));

    writer.writeEndElement();
}

void DeviceTableModel::createDslamElement(QXmlStreamWriter &writer,
                                          const DslamInfo::Ptr &deviceInfo)
{
    writer.writeStartElement("dslam");

    writer.writeAttribute("name", deviceInfo->name());
    writer.writeAttribute("ip", deviceInfo->ip());
    writer.writeAttribute("model", DeviceModel::toString(deviceInfo->deviceModel()));
    writer.writeAttribute("autofill", QString::number(deviceInfo->autoFill()));
    writer.writeAttribute("autonumeringboard", QString::number(deviceInfo->autoNumeringBoard()));

    auto it = deviceInfo->boardTableModel()->boardList().constBegin();
    auto end = deviceInfo->boardTableModel()->boardList().constEnd();
    for (; it != end; ++it) {
        writer.writeStartElement("board");

        writer.writeAttribute("number", QString::number((*it)->index()));
        writer.writeAttribute("firstpair", QString::number((*it)->firstPair()));
        writer.writeAttribute("type", BoardType::toString((*it)->type()));

        writer.writeEndElement();
    }

    writer.writeEndElement();
}

void DeviceTableModel::createOltElement(QXmlStreamWriter &writer,
                                        const OltInfo::Ptr &deviceInfo)
{
    writer.writeStartElement("olt");

    writer.writeAttribute("name", deviceInfo->name());
    writer.writeAttribute("ip", deviceInfo->ip());
    writer.writeAttribute("model", DeviceModel::toString(deviceInfo->deviceModel()));

    createOltProfileList(writer, deviceInfo->serviceProfileList(), "uniprofile");
    createOltProfileList(writer, deviceInfo->multicastProfileList(), "multprofile");

    writer.writeEndElement();
}

void DeviceTableModel::createOltProfileList(QXmlStreamWriter &writer,
                                            const OltProfileMap &profileMap,
                                            QString typeElem)
{
    auto it = profileMap.constBegin();
    auto end = profileMap.constEnd();
    for (; it != end; ++it) {
        writer.writeStartElement(typeElem);

        writer.writeAttribute("index", QString::number(it.key()));
        writer.writeAttribute("name", it.value());

        writer.writeEndElement();
    }
}