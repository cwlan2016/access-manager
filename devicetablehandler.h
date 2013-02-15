#ifndef DEVICETABLEPARSER_H
#define DEVICETABLEPARSER_H

#include <QtXml/QXmlDefaultHandler>
#include "Info/boardinfo.h"
#include "Info/deviceinfo.h"

class DeviceTableHandler : public QXmlDefaultHandler
{
public:
    DeviceTableHandler();

    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    bool fatalError(const QXmlParseException &exception);

    QString errorString() const;

    QVector<DeviceInfo::Ptr> &deviceList();

private:
    void parseSwitchElement(const QXmlAttributes &attributes);
    void parseDslamElement(const QXmlAttributes &attributes);
    void parseBoardElement(const QXmlAttributes &attributes);
    void parseOltElement(const QXmlAttributes &attributes);
    void parseUniOltProfileElement(const QXmlAttributes &attributes);
    void parseMultOltProfileElement(const QXmlAttributes &attributes);

    QString m_error;
    DeviceInfo::Ptr mCurrDeviceInfo;
    QHash<int, BoardInfo> mBoardList;
    QVector<DeviceInfo::Ptr> mDeviceList;
};

#endif // DEVICETABLEPARSER_H
