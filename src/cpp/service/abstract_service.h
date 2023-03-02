#ifndef ABSTRACTSERVICE_H
#define ABSTRACTSERVICE_H

#include "device/abstract_device_worker.h"
#include "event/xfs_iot_msg_event.h"
#include "interface/interface_repository.h"
#include "interface/xfs_iot_interface.h"
#include "qjsonarray.h"
#include "qjsonobject.h"
#include "qjsonvalue.h"
#include "runable_object.h"
#include "selfservice_object.h"
#include "service/command_queue.h"
#include <QObject>

class ServiceEndpoint;
class InterfaceRepository;

class AbstractService : public RunableObject
{
    Q_OBJECT
public:
    inline static const QString JK_ITEM_STATUS_COMMON = QStringLiteral("Status.common");
    static void setInterfaceRepository(InterfaceRepository *pInterfaceRepository);

private:
    static InterfaceRepository *s_pInterfaceRepository;

public:
    explicit AbstractService(const QString &strName, const QString &strFileConfig);
    virtual ~AbstractService();
    bool load(const QJsonArray &jaInterfacesList);
    inline void setEndpoint(ServiceEndpoint *pEndpoint) { m_pEndpoint = pEndpoint; }
    inline AbstractDeviceWorker *deviceWorker() { return m_pDeviceWorker; }
    void addInterface(XFSIoTInterface *pInterface);
    bool isSupportInterface(const QString &strInterfaceName) const;

    // QObject interface
    virtual bool event(QEvent *event) override;

    // SelfServiceObject interface
    virtual bool loadConfig(const QJsonObject &config) override;
    const QJsonValue payloadItem(const QString &strName) const;
    using SelfServiceObject::loadConfig;

    virtual SERVICE_FUNCTION(Common, Status) = 0;
    SERVICE_FUNCTION(Common, Capabilities);
    SERVICE_FUNCTION(Common, SetVersions);
    SERVICE_FUNCTION(Common, Cancel);

    // Client Handle
    bool onCommand(XFSIoTCommandEvent *pEvent);
    bool onClientConnected(int iClientID);
    bool onClientDisconnected(int iClientID);

    // Notify completed message to client
public:
    void notifyEvent(XFSIoTMsgEvent *event) const;
    void notifyCompletion(const XFSIoTCommandEvent *pEventCmd, // Event of command
                          const QString &strCompletionCode, //
                          const QString &strErrorDescription = QString()) const;
    void notifyCompletionErrorCode(const XFSIoTCommandEvent *pEventCmd, // Event of command
                                   const QString &strErrorCode, //
                                   const QString &strErrorDescription = QString()) const;
    void notifyInvalidCommand(const XFSIoTCommandEvent *pEventCmd, //
                              const QString &strErrorDescription) const;
    void notifyCompletion(const XFSIoTCommandEvent *pEventCmd, const QJsonObject &joPayload) const;
    void notifyCanceled(const XFSIoTCommandEvent *pEventCmd) const;
    void notifyTimeOut(const XFSIoTCommandEvent *pEventCmd) const;

    // Event message
    void notifyEventEvent(const XFSIoTCommandEvent *pEventCmd, //
                          const QString &strEventName, //
                          const QJsonObject &joPayload) const;

protected:
    virtual bool init() override;
    bool setCommandVersion(int iClientID, const QString &strCommandName, int iVersion);

    virtual bool updateStatus();
    inline QJsonObject &commonStatus() { return m_joStatusCommon; }

    virtual AbstractDeviceWorker *loadDeviceWorker(const QJsonValue &joProperties) = 0;

    void queueCommand(const XFSIoTCommandEvent *pEventCmd) const;

private:
    ServiceEndpoint *m_pEndpoint = nullptr;
    AbstractDeviceWorker *m_pDeviceWorker = nullptr;
    QHash<QString, XFSIoTInterface *> m_hInterfacesList;
    QHash<int, QHash<QString, int>> m_hhVersionSelected;
    QString m_strFileConfig;
    QJsonObject m_joPayloadItems;
    QJsonObject m_joStatusCommon;
};

#endif // ABSTRACTSERVICE_H
