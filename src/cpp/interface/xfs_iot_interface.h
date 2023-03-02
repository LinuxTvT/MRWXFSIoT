#ifndef _XFSIOT_INTERFACE_H
#define _XFSIOT_INTERFACE_H

#include "event/xfs_iot_msg_event.h"
#include "interface/xfs_iot_function.h"
#include "selfservice_object.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QObject>

#define SERVICE_FUNCTION(i_name, f_name) Q_INVOKABLE void i_name##_##f_name(XFSIoTCommandEvent *pEvent)

class AbstractService;

class XFSIoTInterface
{

public:
    explicit XFSIoTInterface(const QString &strName);
    virtual ~XFSIoTInterface();
    bool load(const QJsonArray &jaFunctionsList);
    inline const QString &jsonKeyName() const { return m_strJKName; };
    inline const QHash<QString, XFSIoTFunction *> &functionsList() const { return m_hFunctionsList; }

    bool invokeServiceFunction(const QString &strFunctionName, //
                               AbstractService *pService, //
                               XFSIoTCommandEvent *pEvent) const;

    inline const QString &name() const { return m_strName; }

private:
    QHash<QString, XFSIoTFunction *> m_hFunctionsList;
    void addFunction(XFSIoTFunction *pFunction);
    QString m_strName;
    QString m_strJKName;
};

#endif // _XFSIOT_INTERFACE_H
