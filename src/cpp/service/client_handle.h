#ifndef CLIENTHANDLE_H
#define CLIENTHANDLE_H

#include "interface/xfs_iot_function.h"
#include "qglobal.h"
#include "qwebsocket.h"
#include "selfservice_object.h"
#include "service/abstract_service.h"
#include <QObject>

#define MAX_CLIENT_ID 9999

class ClientHandle : public SelfServiceObject
{
    Q_OBJECT

private:
    inline static uint CLIENT_INDEX = 1;

public:
    explicit ClientHandle(QWebSocket *pWebSocket, //
                          AbstractService *pService, //
                          ServiceEndpoint *parent);
    virtual ~ClientHandle();

    bool sendMessage(const QJsonObject &joHeader, const QJsonObject &joPayload);
    inline uint id() const { return m_uiID; }

private:
    bool sendMessage(const QString message);
    bool sendMessage(const QJsonObject &joMessage);
    bool sendAckMessage(const QJsonValue &joHeader, //
                        const QString &strStatus = QStringLiteral("ok"), //
                        const QString &strErr = QString());
    bool handleClientCommand(int iRequetId, //
                             const QString &strCommandName, //
                             const QJsonObject &jsObject);
    void postEvent2Service(QEvent *pEvent) const;

private slots:
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();

private:
    QWebSocket *m_pWebsocket = nullptr;
    AbstractService *m_pService = nullptr;
    uint m_uiID = 0;
    int m_iCurRequestId = 1;
};

#endif // CLIENTHANDLE_H
