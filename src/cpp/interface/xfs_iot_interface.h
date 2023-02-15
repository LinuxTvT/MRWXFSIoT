#ifndef _XFSIOT_INTERFACE_H
#define _XFSIOT_INTERFACE_H

#include "event/xfs_iot_msg_event.h"
#include "interface/xfs_iot_function.h"
#include "selfservice_object.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QObject>

#define UPDATE_STATUS_FUNC_NAME_FORMAT QStringLiteral("Update_Status_%1")
#define UPDATE_CAPABILITIES_FUNC_NAME_FORMAT QStringLiteral("Update_Capabilities_%1")

#define STATUS_FUNCTION(i_name) Q_INVOKABLE void Update_Status_##i_name(QJsonObject &joStatus)
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

    // invoke
    bool invokeUpdateStatus(AbstractService *pService, QJsonObject &joStatus) const;
    bool invokeServiceFunction(const QString &strFunctionName, //
                               AbstractService *pService, //
                               XFSIoTCommandEvent *pEvent) const;

    inline const QString &name() const { return m_strName; }

private:
    QHash<QString, XFSIoTFunction *> m_hFunctionsList;
    void addFunction(XFSIoTFunction *pFunction);
    QString m_strName;
    QString m_strJKName;
    char *m_pCharStatusInvokeName = nullptr;
    char *m_pCharCapabilitiesInvokeName = nullptr;
};

#endif // _XFSIOT_INTERFACE_H
