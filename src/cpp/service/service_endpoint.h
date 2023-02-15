#ifndef _SERVICEENDPOINT_H
#define _SERVICEENDPOINT_H

#include <QObject>
#include <QUrl>
#include "qeventloop.h"
#include "qglobal.h"
#include "qjsonvalue.h"
#include "qwebsocket.h"
#include "qwebsocketserver.h"
#include "selfservice_object.h"
#include "service/abstract_service.h"
#include "service/client_handle.h"
#include "thread_facotry.h"

class ServiceEndpoint : public RunableObject
{
    Q_OBJECT

public:
    explicit ServiceEndpoint(quint16 ui16Port, RunableObject *pParent);
    virtual ~ServiceEndpoint();
    inline const QHash<QString, AbstractService *> &servicesList() const { return m_hServicesList; }
    inline quint16 port() const { return m_ui16Port; };
    QList<QString> servicesName() const;
    // Services
    void addService(AbstractService *pService);
    void delClient(uint uiID);

protected:
    virtual bool handleServiceMsg(XFSIoTMsgEvent *pEvent);
    virtual bool startService(AbstractService *pService);
    virtual bool onCompetedEvent(XFSIoTMsgEvent *pEvent);
    virtual bool onEventEvent(XFSIoTMsgEvent *pEvent);
    virtual bool event(QEvent *event) override;

    // public slots:
    //    void start();

private slots:
    void onNewConnection();

private:
    inline ClientHandle *findClient(uint uiID) const { return m_hClientList.value(uiID, nullptr); }
    bool addClient(QWebSocket *pSocket, AbstractService *pService);

private:
    // ThreadFacotry *m_pThreadFactory = nullptr;
    QWebSocketServer *m_pWebSocketServer = nullptr;
    quint16 m_ui16Port;
    QHash<uint, ClientHandle *> m_hClientList;
    QHash<QString, AbstractService *> m_hServicesList;
    bool m_isCheckRequestID = false;

    // RunableObject interface
protected:
    virtual bool init() override;
};

#endif // _SERVICEENDPOINT_H
