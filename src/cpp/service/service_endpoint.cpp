#include "service_endpoint.h"
#include "event/xfs_iot_msg_event.h"
#include "qcoreapplication.h"
#include "qglobal.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "qwebsocket.h"
#include "service/printer/masung/masung_printer_worker.h"
#include "service/publisher/publisher_service.h"
#include "xfs_iot_standard.h"
#include <QThread>

ServiceEndpoint::ServiceEndpoint(quint16 ui16Port, //
                                 const QString &strName, //
                                 const QString &strFileConfigs, //
                                 RunableObject *pParent)
    : RunableObject(strName, strFileConfigs, pParent), m_ui16Port(ui16Port)
{
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("Miraway XFSIoT"), //
                                              QWebSocketServer::NonSecureMode, //
                                              this);
}

ServiceEndpoint::~ServiceEndpoint() { }

QList<QString> ServiceEndpoint::servicesName() const
{
    return m_hServicesList.keys();
}

bool ServiceEndpoint::handleServiceMsg(XFSIoTMsgEvent *pEvent)
{
    ClientHandle *l_pClient = findClient(pEvent->clientId());
    if (l_pClient != nullptr) {
        QJsonObject l_joMessage;
        pEvent->buildJsonMsg(l_joMessage);
        return l_pClient->sendMessage(l_joMessage);
    } else {
        error(QString("Can't find client id [%1], Maybe It disconnected").arg(pEvent->clientId()));
        return false;
    }
}

bool ServiceEndpoint::startService(AbstractService *pService)
{
    log(QString("Strarting Service [%1]").arg(pService->name()));
    pService->run();
    log(QString("Started Service [%1]").arg(pService->name()));
    return true;
}

void ServiceEndpoint::addService(AbstractService *pService)
{
    m_hServicesList.insert(pService->name(), pService);
    pService->setEndpoint(this);
    pService->setParent(this);
}

void ServiceEndpoint::onNewConnection()
{
    log("Have client connect");
    log(QString("onNewConnection Thread ID [%1]")
                .arg(reinterpret_cast<quintptr>(QThread::currentThreadId()), QT_POINTER_SIZE * 2, 16, QChar('0')));
    QWebSocket *l_pSocket = m_pWebSocketServer->nextPendingConnection();
    QUrl l_url = l_pSocket->requestUrl();
    log(QString("Connect URL [%1]").arg(l_url.toString()));

    QString l_strServiceName;
    if (l_url.path() == "/xfs4iot/v1.0") {
        l_strServiceName = PublisherService::PUBLISHER_SERVICE_NAME;
    } else {
        l_strServiceName = l_url.fileName();
    }
    log(QString("Client connect to service [%1]").arg(l_strServiceName));
    if (m_hServicesList.contains(l_strServiceName)) {
        AbstractService *l_pService = m_hServicesList.value(l_strServiceName);
        if (addClient(l_pSocket, l_pService)) {
            log("Add client success");
        } else {
            QString l_strErr = QString("Can't add client for service [%1]").arg(l_strServiceName);
            error(l_strErr);
            l_pSocket->sendTextMessage(l_strErr);
            l_pSocket->close();
        }
    } else {
        QString l_strErr = QString("URL inavlid, can not find service [%1]").arg(l_strServiceName);
        error(l_strErr);
        l_pSocket->sendTextMessage(l_strErr);
        l_pSocket->close();
    }
}

bool ServiceEndpoint::addClient(QWebSocket *pSocket, AbstractService *pService)
{
    ClientHandle *l_pNewClient = new ClientHandle(pSocket, pService, this);
    m_hClientList.insert(l_pNewClient->id(), l_pNewClient);
    return true;
}

void ServiceEndpoint::delClient(uint uiID)
{
    if (m_hClientList.contains(uiID)) {
        m_hClientList.take(uiID)->deleteLater();
    } else {
        error(QString("Can't find client id [%1] to delete").arg(uiID));
    }
}

bool ServiceEndpoint::event(QEvent *event)
{
    if (event->type() == XFSIoTEvent::CompletionEvent) {
        return onCompetedEvent((XFSIoTMsgEvent *)event);
    } else if (event->type() == XFSIoTEvent::EventEvent) {
        return onEventEvent((XFSIoTMsgEvent *)event);
    }

    return SelfServiceObject::event(event);
}

bool ServiceEndpoint::onCompetedEvent(XFSIoTMsgEvent *pEvent)
{
    return handleServiceMsg(pEvent);
}

bool ServiceEndpoint::onEventEvent(XFSIoTMsgEvent *pEvent)
{
    return handleServiceMsg(pEvent);
}

bool ServiceEndpoint::init()
{
    log(QString("==> Service Endpoint , Port [%1] is started, thread id [%2]")
                .arg(this->port())
                .arg(reinterpret_cast<quintptr>(QThread::currentThreadId()), QT_POINTER_SIZE * 2, 16, QChar('0')));
    log(QString("Starting Endpoint : Port[%1]").arg(this->port()));
    for (auto itr = m_hServicesList.begin(); itr != m_hServicesList.end(); itr++) {
        log(QString("Strarting Service [%1]").arg((*itr)->name()));
        startService(*itr);
    }

    if (m_pWebSocketServer->listen(QHostAddress::Any, this->port())) {
        log("Endpoint is listen");
    } else {
        error("Endpoint listen ERROR");
    }
    connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &ServiceEndpoint::onNewConnection);

    return true;
}
