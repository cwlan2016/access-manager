#include "devicetablemodel.h"

#ifdef _MSC_VER
#include "../basicdialogs.h"
#include "../config.h"
#include "../constant.h"
#include "../converters.h"
#include "../devicetablehandler.h"
#include "../snmpclient.h"
#include "../Info/boardinfo.h"
#include "../Models/boardtablemodel.h"
#else
#include "basicdialogs.h"
#include "config.h"
#include "constant.h"
#include "converters.h"
#include "devicetablehandler.h"
#include "snmpclient.h"
#include "Info/boardinfo.h"
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
                    DslamInfo::Ptr dslamInfo = deviceInfo.objectCast<DslamInfo>();
                    dslamInfo->boardTableModel()->setParentDevice(dslamInfo);
                } else if (newType == DeviceType::Olt) {
                    deviceInfo = DeviceInfo::Ptr(new OltInfo(this));
                } else {
                    deviceInfo = DeviceInfo::Ptr(new DeviceInfo(this));
                }

                deviceInfo->setName(mList[index.row()]->name());
                deviceInfo->setIp(mList[index.row()]->ip());
                deviceInfo->setDeviceModel(newModel);

                mList[index.row()] = deviceInfo;
            } else if (mList[index.row()]->deviceType() != newType) {
                BasicDialogs::information(0, BasicDialogStrings::Info,
                                          QString::fromUtf8("Запрещено менять модель с одного типа устройства на другое."));
                return false;
            } else {
                mList[index.row()]->setDeviceModel(newModel);
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
    DeviceTableHandler handler;
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

    mList = handler.deviceList();

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

        writer.writeAttribute("number", QString::number((*it).index()));
        writer.writeAttribute("firstpair", QString::number((*it).firstPair()));
        writer.writeAttribute("type", BoardType::toString((*it).type()));

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
