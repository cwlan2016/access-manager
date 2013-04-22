#include "switch.h"

#include <constant.h>
#include <converters.h>
#include <customsnmpfunctions.h>
#include <configs/switchconfig.h>

Switch::Switch(QObject *parent) :
    Device(parent),
    mInetVlanTag(0),
    mIptvVlanTag(0)
{
}

Switch::Switch(QString name, QString ip, QObject *parent) :
    Device(name, ip, parent),
    mInetVlanTag(0),
    mIptvVlanTag(0)
{
}

Switch::Switch(Device *source, QObject *parent) :
    Device(source, parent)
{
    if (source->deviceType() == DeviceType::Switch) {
        mInetVlanTag = static_cast<Switch *>(source)->inetVlanTag();
        mIptvVlanTag = static_cast<Switch *>(source)->iptvVlanTag();
    } else {
        mInetVlanTag = 0;
        mIptvVlanTag = 0;
    }
}

int Switch::inetVlanTag() const
{
    return mInetVlanTag;
}

void Switch::setInetVlanTag(int vlanTag)
{
    if (mInetVlanTag != vlanTag) {
        mInetVlanTag = vlanTag;
        emit modified();
    }
}

int Switch::iptvVlanTag() const
{
    return mIptvVlanTag;
}

void Switch::setIptvVlanTag(int vlanTag)
{
    if (mIptvVlanTag != vlanTag) {
        mIptvVlanTag = vlanTag;
        emit modified();
    }
}

DeviceType::Enum Switch::deviceType() const
{
    return DeviceType::Switch;
}

int Switch::countPorts()
{
    return 0;
}

bool Switch::getServiceDataFromDevice()
{
    mError.clear();

    QScopedPointer<SnmpClient> snmpClient(new SnmpClient());

    snmpClient->setIp(ip());

    if (!snmpClient->setupSession(SessionType::ReadSession)) {
        mError = SnmpErrorStrings::SetupSession;
        return false;
    }

    if (!snmpClient->openSession()) {
        mError = SnmpErrorStrings::OpenSession;
        return false;
    }

    snmpClient->createPdu(SNMP_MSG_GETNEXT);
    snmpClient->addOid(createOidPair(Mib::dot1qVlanStaticName, 13));

    bool findedInet, findedIptv;
    findedInet = false;
    findedIptv = false;

    while (snmpClient->sendRequest()) {
        netsnmp_variable_list *vars = snmpClient->varList();

        if (snmp_oid_ncompare(Mib::dot1qVlanStaticName, 13, vars->name,
                              vars->name_length, 13) != 0)
            break;

        QString vlanName = toString(vars->val.string, vars->val_len);

        if (vlanName == SwitchConfig::inetVlanName()) {
            findedInet = true;
            setInetVlanTag(vars->name[13]);

            if (findedIptv)
                break;
        } else if (vlanName == SwitchConfig::iptvVlanName()) {
            findedIptv = true;
            setIptvVlanTag(vars->name[13]);

            if (findedInet)
                break;
        }

        snmpClient->createPduFromResponse(SNMP_MSG_GETNEXT);
    }

    if (!findedInet && !findedInet) {
        mError = QString::fromUtf8("Вланы для интернета и iptv на коммутаторе %1 [%2] не найдены.").arg(name(), ip());
    } else if (!findedInet) {
        mError = QString::fromUtf8("Влан для интернета на коммутаторе %1 [%2] не найден.").arg(name(), ip());
    } else if (!findedIptv) {
        mError = QString::fromUtf8("Влан для iptv на коммутаторе %1 [%2] не найден.").arg(name(), ip());
    }

    return findedInet && findedIptv;
}

bool Switch::saveConfig()
{
    QScopedPointer<SnmpClient> snmp(new SnmpClient());

    snmp->setIp(ip());

    if ((deviceModel() == DeviceModel::DES3526)
            || (deviceModel() == DeviceModel::DES3550)) {
        if (!snmp->setupSession(SessionType::ReadSession)) {
            mError = SnmpErrorStrings::SetupSession;
            return false;
        }

        if (!snmp->openSession()) {
            mError = SnmpErrorStrings::OpenSession;
            return false;
        }

        snmp->createPdu(SNMP_MSG_GET);

        snmp->addOid(Mib::agentStatusSaveCfg, 12);

        if (snmp->sendRequest()) {
            netsnmp_variable_list *vars = snmp->varList();

            if (isValidSnmpValue(vars)) {
                if (*(vars->val.integer) == 2) {

                    mError = QString::fromUtf8("Ошибка: В данный момент уже идет сохранение конфигурации коммутатора.");
                    return false;
                }
            } else {
                mError = QString::fromUtf8("Ошибка: Запрошенная информация на коммутаторе не найдена.");
                return false;
            }
        } else {
            mError = QString::fromUtf8("Ошибка: Не удалось получить данные о текущем состоянии коммутатора.");
            return false;
        }
    }

    if (!snmp->setupSession(SessionType::WriteSession)) {
        mError = SnmpErrorStrings::SetupSession;
        return false;
    }

    snmp->setTimeoutSaveConfig();

    if (!snmp->openSession()) {
        mError = SnmpErrorStrings::OpenSession;
        return false;
    }

    snmp->createPdu(SNMP_MSG_SET);

    OidPair saveOid = createOidPair(Mib::agentSaveCfg, 12);

    if ((deviceModel() == DeviceModel::DES3526)
            || (deviceModel() == DeviceModel::DES3550)) {
        snmp->addOid(saveOid, "3", 'i');
    } else if (deviceModel() == DeviceModel::DES3528) {
        snmp->addOid(saveOid, "5", 'i');
    } else {
        mError = QString::fromUtf8("Неизвестная модель коммутатора.");
        return false;
    }

    if (!snmp->sendRequest()) {
        mError = QString::fromUtf8("Ошибка: Не удалось отправить запрос на сохранение конфигурации коммутатора.");
        return false;
    }

    return true;
}
