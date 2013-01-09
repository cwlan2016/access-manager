#include "devicelistmodel.h"

#include <QtCore/QFileInfo>
#include <QtCore/QStringBuilder>
#include <QtXmlPatterns/QXmlSchema>
#include <QtXmlPatterns/QXmlSchemaValidator>
#include <QtWidgets/QApplication>
#ifdef _MSC_VER
#include "../basicdialogs.h"
#include "../config.h"
#include "../constant.h"
#include "../converters.h"
#include "../devicelisthandler.h"
#include "../snmpclient.h"
#include "../Info/boardinfo.h"
#else
#include "basicdialogs.h"
#include "config.h"
#include "constant.h"
#include "converters.h"
#include "devicelisthandler.h"
#include "snmpclient.h"
#include "Info/boardinfo.h"
#endif

// Columns
// DisplayRole, EditRole
// 0 - name
// 1 - model_device
// 2 - ip
// 3 - type_device
DeviceListModel::DeviceListModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    mDeviceListPath = Config::path() % "devicelist.xml";
    mDeviceListBackupPath = Config::path() % "devicelist.bak";
    mModified = false;
}

int DeviceListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mDeviceList.size();
}

int DeviceListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4;
}

QVariant DeviceListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole) {
        if (index.column() == 0)
            return int(Qt::AlignLeft | Qt::AlignVCenter); //имя слева
        else
            return int(Qt::AlignCenter | Qt::AlignVCenter); //ip и type в центре
    } else if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (index.column() == 0)
            return mDeviceList[index.row()]->name();
        else if (index.column() == 1)
            return DeviceModel::toString(mDeviceList[index.row()]->deviceModel());
        else if (index.column() == 2)
            return mDeviceList[index.row()]->ip();
        else if (index.column() == 3)
            return DeviceType::toString(mDeviceList[index.row()]->deviceType());
        else
            return QVariant();
    } else
        return QVariant();
}

bool DeviceListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || (role != Qt::EditRole))
        return false;


    if (role == Qt::EditRole) {
        if (index.column() == 0) {
            mDeviceList[index.row()]->setName(value.toString().trimmed());
            emit dataChanged(index, index);
            mModified = true;

            return true;
        } else if (index.column() == 1) {
            DeviceModel::Enum newModel = DeviceModel::from(value.toString().trimmed());
            DeviceType::Enum newType = DeviceType::from(newModel);

            QModelIndex deviceTypeIndex = this->index(index.row(), 3);

            if (mDeviceList[index.row()]->deviceType() == DeviceType::Other) {
                //устройство только что добавлено
                DeviceInfo::Ptr deviceInfo;

                if (newType == DeviceType::Switch) {
                    deviceInfo = std::make_shared<SwitchInfo>();
                } else if (newType == DeviceType::Dslam) {
                    deviceInfo = std::make_shared<DslamInfo>();
                    std::static_pointer_cast<DslamInfo>(deviceInfo)->boardListModel()->setParentDevice(deviceInfo);
                } else if (newType == DeviceType::Olt) {
                    deviceInfo = std::make_shared<OltInfo>();
                } else {
                    deviceInfo = std::make_shared<DeviceInfo>();
                }

                deviceInfo->setName(mDeviceList[index.row()]->name());
                deviceInfo->setIP(mDeviceList[index.row()]->ip());
                deviceInfo->setDeviceModel(newModel);

                mDeviceList[index.row()] = std::move(deviceInfo);
            } else if (mDeviceList[index.row()]->deviceType() != newType) {
                BasicDialogs::information(NULL, BasicDialogTitle::Info, QString::fromUtf8("Запрещено менять модель с одного типа устройства на другое."));
                return false;
            } else {
                mDeviceList[index.row()]->setDeviceModel(newModel);
            }

            emit dataChanged(index, index);
            emit dataChanged(deviceTypeIndex, deviceTypeIndex);
            mModified = true;

            return true;
        } else if (index.column() == 2) {
            if (!ValidIpAddress(value.toString())) {
                BasicDialogs::information(NULL, trUtf8("Редактирование IP адреса"), trUtf8("Некорректный IP адрес."));
                return false;
            }

            mDeviceList[index.row()]->setIP(value.toString().trimmed());
            emit dataChanged(index, index);
            mModified = true;

            return true;
        }
    }

    return false;
}

QVariant DeviceListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical)
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (section == 0)
            return DeviceListModelColumn::Name;
        else if (section == 1)
            return DeviceListModelColumn::DeviceModel;
        else if (section == 2)
            return DeviceListModelColumn::IP;
        else if (section == 3)
            return DeviceListModelColumn::DeviceType;
    } else if (role == Qt::TextAlignmentRole) {
        return int(Qt::AlignCenter | Qt::AlignVCenter);
    } else if (role == Qt::FontRole) {
        QFont font(qApp->font());
        font.setBold(true);
        return font;
    }

    return QVariant();
}

Qt::ItemFlags DeviceListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (index.column() != 3)
        flags |= Qt::ItemIsEditable;

    return flags;
}

bool DeviceListModel::insertRow(int row, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row);
    DeviceInfo::Ptr element = std::make_shared<DeviceInfo>();
    mDeviceList.push_back(std::move(element));
    endInsertRows();

    mModified = true;

    return true;
}

bool DeviceListModel::removeRow(int row, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row);
    mDeviceList.erase(mDeviceList.begin() + row);
    endRemoveRows();

    mModified = true;

    return true;
}

std::vector<DeviceInfo::Ptr> &DeviceListModel::deviceList()
{
    return mDeviceList;
}

void DeviceListModel::setModified(bool state)
{
    mModified = state;
}

BoardListModel *DeviceListModel::boardListModel(QModelIndex index)
{
    return std::static_pointer_cast<DslamInfo>(mDeviceList[index.row()])->boardListModel();
}

bool DeviceListModel::load()
{
    beginResetModel();

    if (!exist()) {
        mDeviceList.clear();
        endResetModel();
        return true;
    }

    mDeviceList.clear();

    QFile file(mDeviceListPath);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        mError = QString::fromUtf8("Ошибка: невозможно открыть файл %1: %2")
                 .arg(file.fileName())
                 .arg(file.errorString());
        endResetModel();
        return false;
    }

    //проверка схемы списка устройств

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

    //парсинг списка устройств
    file.seek(0);
    DeviceListHandler handler;
    QXmlSimpleReader reader;
    reader.setContentHandler(&handler);
    reader.setErrorHandler(&handler);
    QXmlInputSource xmlInputSource(&file);

    if (!reader.parse(xmlInputSource)) {
        mError = reader.errorHandler()->errorString();
        file.close();

        return false;
    }

    file.close();

    mDeviceList = handler.deviceList();

    endResetModel();
    //reset();
    mModified = false;

    return true;
}

bool DeviceListModel::save()
{
    if (exist())
        backup();

    QFile file(mDeviceListPath);

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

    auto it = mDeviceList.begin();
    auto end = mDeviceList.end();
    for (; it != end; ++it) {
        if ((*it)->deviceType() == DeviceType::Switch) {
            createSwitchElement(writer, std::static_pointer_cast<SwitchInfo>(*it));
        } else if ((*it)->deviceType() == DeviceType::Dslam) {
            createDslamElement(writer, std::static_pointer_cast<DslamInfo>(*it));
        } else if ((*it)->deviceType() == DeviceType::Olt) {
            createOltElement(writer, std::static_pointer_cast<OltInfo>(*it));
        }
    }

    writer.writeEndElement();
    writer.writeEndDocument();

    file.close();
    mModified = false;

    return true;
}


bool DeviceListModel::isModified()
{
    return mModified;
}

int DeviceListModel::inetVlan(QModelIndex index)
{
    return std::static_pointer_cast<SwitchInfo>(mDeviceList[index.row()])->inetVlanTag();
}

int DeviceListModel::iptvVlan(QModelIndex index)
{
    return std::static_pointer_cast<SwitchInfo>(mDeviceList[index.row()])->iptvVlanTag();
}

QStringListModel *DeviceListModel::serviceProfileOltListModel(QModelIndex index)
{
    return std::static_pointer_cast<OltInfo>(mDeviceList[index.row()])->serviceProfileListModel(this);
}

QStringListModel *DeviceListModel::multicastProfileOltListModel(QModelIndex index)
{
    return std::static_pointer_cast<OltInfo>(mDeviceList[index.row()])->multicastProfileListModel(this);
}

bool DeviceListModel::getVlanTagFromDevice(QModelIndex index)
{
    if (!index.isValid()) {
        mError = QString::fromUtf8("Неверный индекс.");
        return false;
    }

    if (mDeviceList[index.row()]->deviceType() != DeviceType::Switch) {
        mError = QString::fromUtf8("Обновлять информацию о вланах можно только на коммутаторе!");
        return false;
    }

    bool result = mDeviceList[index.row()]->getServiceDataFromDevice();

    if (!result) {
        mError = mDeviceList[index.row()]->error();
        return false;
    } else {
        mModified = true;
        return true;
    }
}

bool DeviceListModel::getBoardListFromDevice(QModelIndex index)
{
    if (!index.isValid()) {
        mError = QString::fromUtf8("Неверный индекс.");
        return false;
    }

    if (mDeviceList[index.row()]->deviceType() != DeviceType::Dslam) {
        mError = QString::fromUtf8("Обновлять информацию о досках можно только на дсламе!");
        return false;
    }

    bool result = std::static_pointer_cast<DslamInfo>(mDeviceList[index.row()])->boardListModel()->getBoardListFromDevice();
    if (std::static_pointer_cast<DslamInfo>(mDeviceList[index.row()])->autoNumeringBoard())
        std::static_pointer_cast<DslamInfo>(mDeviceList[index.row()])->boardListModel()->renumeringPairList();

    if (!result) {
        mError = std::static_pointer_cast<DslamInfo>(mDeviceList[index.row()])->boardListModel()->error();
        return false;
    } else {
        mModified = true;
        return true;
    }
}

bool DeviceListModel::getProfilesFromDevice(QModelIndex index)
{
    if (!index.isValid()) {
        mError = QString::fromUtf8("Неверный индекс.");
        return false;
    }

    if (mDeviceList[index.row()]->deviceType() != DeviceType::Olt) {
        mError = QString::fromUtf8("Обновлять информацию о профилях можно только на олт!");
        return false;
    }

    bool result = std::static_pointer_cast<OltInfo>(mDeviceList[index.row()])->getServiceDataFromDevice();

    if (!result) {
        mError = std::static_pointer_cast<OltInfo>(mDeviceList[index.row()])->error();
        return false;
    } else {
        mModified = true;
        return true;
    }
}

QString DeviceListModel::error() const
{
    return mError;
}

bool DeviceListModel::exist()
{
    QFileInfo fileInfo(mDeviceListPath);

    return fileInfo.exists();
}

bool DeviceListModel::backup()
{
    if (!exist())
        return false;

    QFile::remove(mDeviceListBackupPath);
    QFile::copy(mDeviceListPath, mDeviceListBackupPath);

    return true;
}

void DeviceListModel::createSwitchElement(QXmlStreamWriter &writer, const SwitchInfo::Ptr &deviceInfo)
{
    writer.writeStartElement("switch");

    writer.writeAttribute("name", deviceInfo->name());
    writer.writeAttribute("ip", deviceInfo->ip());
    writer.writeAttribute("model", DeviceModel::toString(deviceInfo->deviceModel()));
    writer.writeAttribute("inetVlan", QString::number(deviceInfo->inetVlanTag()));
    writer.writeAttribute("iptvVlan", QString::number(deviceInfo->iptvVlanTag()));

    writer.writeEndElement();
}

void DeviceListModel::createDslamElement(QXmlStreamWriter &writer, const DslamInfo::Ptr &deviceInfo)
{
    writer.writeStartElement("dslam");

    writer.writeAttribute("name", deviceInfo->name());
    writer.writeAttribute("ip", deviceInfo->ip());
    writer.writeAttribute("model", DeviceModel::toString(deviceInfo->deviceModel()));
    writer.writeAttribute("autofill", QString::number(deviceInfo->autoFill()));
    writer.writeAttribute("autonumeringboard", QString::number(deviceInfo->autoNumeringBoard()));

    auto it = deviceInfo->boardListModel()->boardList().begin();
    auto end = deviceInfo->boardListModel()->boardList().end();
    for (; it != end; ++it) {
        writer.writeStartElement("board");

        writer.writeAttribute("number", QString::number((*it).number()));
        writer.writeAttribute("firstpair", QString::number((*it).firstPair()));
        writer.writeAttribute("type", BoardType::toString((*it).type()));

        writer.writeEndElement();
    }

    writer.writeEndElement();
}

void DeviceListModel::createOltElement(QXmlStreamWriter &writer, const OltInfo::Ptr &deviceInfo)
{
    writer.writeStartElement("olt");

    writer.writeAttribute("name", deviceInfo->name());
    writer.writeAttribute("ip", deviceInfo->ip());
    writer.writeAttribute("model", DeviceModel::toString(deviceInfo->deviceModel()));

    createOltProfileList(writer, deviceInfo->serviceProfileList(), "uniprofile");
    createOltProfileList(writer, deviceInfo->multicastProfileList(), "multprofile");

    writer.writeEndElement();
}

void DeviceListModel::createOltProfileList(QXmlStreamWriter &writer, const OltProfileMap &profileMap, QString typeElem)
{
    auto it = profileMap.begin();
    auto end = profileMap.end();
    for (; it != end; ++it) {
        writer.writeStartElement(typeElem);

        writer.writeAttribute("index", QString::number((*it).first));
        writer.writeAttribute("name", (*it).second);

        writer.writeEndElement();
    }
}

