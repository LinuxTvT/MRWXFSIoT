#ifndef CLIENTHANDLE_H
#define CLIENTHANDLE_H

#include <QObject>
#include "interface/xfs_iot_function.h"
#include "qglobal.h"
#include "qwebsocket.h"
#include "selfservice_object.h"
#include "service/abstract_service.h"

#define MAX_CLIENT_ID 9999

class ClientHandle : public SelfServiceObject
{
    Q_OBJECT

private:
    inline static uint s_uiClientID = 1;

public:
    explicit ClientHandle(QWebSocket *pWebSocket, AbstractService *pService, ServiceEndpoint *parent);
    virtual ~ClientHandle();
    inline uint id() const { return m_uiID; }
    inline QWebSocket *webSocket() const { return m_pWebsocket; }
    inline AbstractService *service() const { return m_pService; }
    inline int curRequestId() const { return m_iCurRequestId; }
    inline int setCurRequestId(int iNewId)
    {
        m_iCurRequestId = iNewId;
        return m_iCurRequestId;
    }
    bool sendMessage(const QString message);
    bool sendMessage(const QJsonObject &joMessage);
    bool sendAckMessage(const QJsonObject &joHeader, const QString &strStatus = QStringLiteral("ok"),
                        const QString &strErr = QString());
    bool sendAckMessage(const QJsonValue &joHeader, const QString &strStatus = QStringLiteral("ok"),
                        const QString &strErr = QString());
    bool sendCompletionMessage(const QJsonValue &joHeader, const QJsonObject &joPayload,
                               const QString &strCompletionCode = QStringLiteral("success"),
                               const QString &strErr = QString());
    bool sendCompletionMessage(const QJsonObject &joHeader, const QJsonObject &joPayload,
                               const QString &strCompletionCode = QStringLiteral("success"),
                               const QString &strErr = QString());

protected slots:
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();

protected:
    bool handleClientCommand(int iRequetId, const QString &strCommandName, const QJsonObject &jsObject);
    void postEvent2Service(QEvent *pEvent) const;

private:
    QWebSocket *m_pWebsocket = nullptr;
    AbstractService *m_pService = nullptr;
    uint m_uiID = 0;
    int m_iCurRequestId = 1;
};

#endif // CLIENTHANDLE_H
