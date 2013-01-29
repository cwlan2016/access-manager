#include "switchinfo.h"

#include <QtCore/QScopedPointer>
#ifdef _MSC_VER
#include "../constant.h"
#include "../customsnmpfunctions.h"
#include "../snmpclient.h"
#else
#include "constant.h"
#include "customsnmpfunctions.h"
#include "snmpclient.h"
#endif

SwitchInfo::SwitchInfo(QObject *parent) :
    DeviceInfo(parent)
{
    mInetVlanTag = 0;
    mIptvVlanTag = 0;
}

SwitchInfo::SwitchInfo(QString name, QString ip, DeviceModel::Enum deviceModel, QObject *parent) :
    DeviceInfo(name, ip, deviceModel, parent)
{
}

int SwitchInfo::inetVlanTag() const
{
    return mInetVlanTag;
}

int SwitchInfo::iptvVlanTag() const
{
    return mIptvVlanTag;
}

void SwitchInfo::setInetVlanTag(int vlanTag)
{
    mInetVlanTag = vlanTag;
}

void SwitchInfo::setIptvVlanTag(int vlanTag)
{
    mIptvVlanTag = vlanTag;
}

bool SwitchInfo::getServiceDataFromDevice()
{
    mError.clear();

    QScopedPointer<SnmpClient> snmp(new SnmpClient());

    snmp->setIp(mIp);

    if (!snmp->setupSession(SessionType::ReadSession)) {
        mError = SnmpErrors::SetupSession;
        return false;
    }

    if (!snmp->openSession()) {
        mError = SnmpErrors::OpenSession;
        return false;
    }

    oid vlanNameOid[13];
    size_t vlanNameOidLen = 13;
    memcpy(&vlanNameOid, Mib::dot1qVlanStaticName, 13 * sizeof(oid));

    oid *nextOid = new oid[13];
    size_t nextOidLen = 13;
    memcpy(nextOid, Mib::dot1qVlanStaticName, 13 * sizeof(oid));

    bool findedInet, findedIptv;
    findedInet = findedIptv = false;

    while (true) {
        snmp->createPdu(SNMP_MSG_GETNEXT);
        snmp->addOid(nextOid, nextOidLen);

        if (!snmp->sendRequest())
            break;

        netsnmp_variable_list *vars = snmp->varList();

        if (snmp_oid_ncompare(vlanNameOid, vlanNameOidLen, vars->name, vars->name_length, 13) != 0)
            break;

        QString vlanName = QString::fromLatin1((char *)vars->val.string, vars->val_len);

        if (vlanName == "Inet") {
            findedInet = true;
            mInetVlanTag = vars->name[13];

            if (findedIptv)
                break;
        } else if (vlanName == "IPTV_Unicast") {
            findedIptv = true;
            mIptvVlanTag = vars->name[13];

            if (findedInet)
                break;
        }

        delete[] nextOid;
        nextOid = new oid[vars->name_length];

        memcpy(nextOid, vars->name, vars->name_length * sizeof(oid));
        nextOidLen = vars->name_length;

        snmp->clearResponsePdu();
    }

    delete[] nextOid;

    if (!findedInet && !findedInet)
        mError = QString::fromUtf8("Ошибка: вланы для интернета и iptv на коммутаторе %1 [%2] не найдены.").arg(mName, mIp);
    else if (!findedInet)
        mError = QString::fromUtf8("Ошибка: влан для интернета на коммутаторе %1 [%2] не найден.").arg(mName, mIp);
    else if (!findedIptv)
        mError = QString::fromUtf8("Ошибка: влан для iptv на коммутаторе %1 [%2] не найден.").arg(mName, mIp);

    return findedInet && findedIptv;
}

bool SwitchInfo::saveConfig()
{
    QScopedPointer<SnmpClient> snmp(new SnmpClient());

    snmp->setIp(mIp);

    if ((mDeviceModel == DeviceModel::DES3526)
            || (mDeviceModel == DeviceModel::DES3550)) {
        if (!snmp->setupSession(SessionType::ReadSession)) {
            mError = SnmpErrors::SetupSession;
            return false;
        }

        if (!snmp->openSession()) {
            mError = SnmpErrors::OpenSession;
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
        mError = SnmpErrors::SetupSession;
        return false;
    }

    snmp->setTimeoutSaveConfig(); //устанавливаем длинный таймаут

    if (!snmp->openSession()) {
        mError = SnmpErrors::OpenSession;
        return false;
    }

    snmp->createPdu(SNMP_MSG_SET);

    if ((mDeviceModel == DeviceModel::DES3526)
            || (mDeviceModel == DeviceModel::DES3550)) {
        snmp->addOid(createOid(Mib::agentSaveCfg, 12), 12, "3", 'i');
    } else if (mDeviceModel == DeviceModel::DES3528) {
        snmp->addOid(createOid(Mib::agentSaveCfg, 12), 12, "5", 'i');
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
