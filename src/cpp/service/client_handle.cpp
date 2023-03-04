#include "client_handle.h"
#include "qglobal.h"
#include "qjsondocument.h"
#include "service_endpoint.h"
#include "xfs_iot_standard.h"
#include <QCoreApplication>

ClientHandle::ClientHandle(QWebSocket *pWebSocket, AbstractService *pService, ServiceEndpoint *parent)
    : SelfServiceObject{ parent }, m_pWebsocket(pWebSocket), m_pService(pService)
{
    CLIENT_INDEX++;
    if (CLIENT_INDEX == MAX_CLIENT_ID) {
        CLIENT_INDEX = 1;
    }
    m_uiID = CLIENT_INDEX;

    setName(QString("%1_%2_%3") //
                    .arg(pWebSocket->peerAddress().toString())
                    .arg(pWebSocket->peerPort())
                    .arg(m_uiID));

    connect(m_pWebsocket, &QWebSocket::textMessageReceived, this, &ClientHandle::processTextMessage);
    connect(m_pWebsocket, &QWebSocket::binaryMessageReceived, this, &ClientHandle::processBinaryMessage);
    connect(m_pWebsocket, &QWebSocket::disconnected, this, &ClientHandle::socketDisconnected);
    postEvent2Service(new XFSIoTClientEvent(XFSIoTMsgEvent::ClientConnected, m_uiID));
    log(QString("Client [%1] was created").arg(name()));
}

ClientHandle::~ClientHandle()
{
    if (m_pWebsocket != nullptr) {
        m_pWebsocket->close();
        m_pWebsocket->deleteLater();
    }
    log(QString("Client [%1] deleted").arg(name()));
}

bool ClientHandle::sendMessage(const QJsonObject &joHeader, const QJsonObject &joPayload)
{
    QJsonObject l_joMessage;
    l_joMessage[XFSIoTStandard::JK_HEADER] = joHeader;
    l_joMessage[XFSIoTStandard::JK_PAYLOAD] = joPayload;
    return sendMessage(l_joMessage);
}

bool ClientHandle::sendMessage(const QString message)
{
    if (m_pWebsocket != nullptr && m_pWebsocket->state() == QAbstractSocket::ConnectedState) {
        qDebug().noquote() << "Send message:" << message;
        return m_pWebsocket->sendTextMessage(message);
    } else {
        return false;
    }
}

bool ClientHandle::sendMessage(const QJsonObject &joMessage)
{
    QJsonDocument l_jsonDoc = QJsonDocument();
    l_jsonDoc.setObject(joMessage);
    return sendMessage(l_jsonDoc.toJson());
}

bool ClientHandle::sendAckMessage(const QJsonValue &joHeader, const QString &strStatus, const QString &strErr)
{
    // Logging error if have
    if (!strErr.isEmpty()) {
        error(strErr);
    }
    QJsonObject l_joPayload;
    l_joPayload[XFSIoTStandard::JK_STATUS] = strStatus;
    l_joPayload[XFSIoTStandard::JK_ERROR_DESCRIPTION] = strErr;
    QJsonObject l_joHeader = joHeader.toObject();
    l_joHeader[XFSIoTStandard::JK_TYPE] = XFSIoTStandard::JV_TYPE_ACKNOWLEDGE;
    return sendMessage(l_joHeader, l_joPayload);
}

void ClientHandle::processTextMessage(QString message)
{
    qDebug().noquote() << "Message received:" << message;

    // Parse text message -> JsonObject
    QJsonParseError l_jsonParseError;
    QJsonDocument l_jsonDocument = QJsonDocument::fromJson(message.toUtf8(), &l_jsonParseError);
    if (l_jsonParseError.error != QJsonParseError::NoError) {
        // Parse message error, Send invalid to client
        sendAckMessage(QJsonObject(), //
                       XFSIoTStandard::JV_INVALID_MESSAGE,
                       QString("Parse JSON message error [%1]").arg(l_jsonParseError.errorString()));
    } else {
        // Parse message success
        const QJsonValue l_jsvHeader = l_jsonDocument[XFSIoTStandard::JK_HEADER];
        const QJsonValue l_jsvPayload = l_jsonDocument[XFSIoTStandard::JK_PAYLOAD];
        if (l_jsvHeader.isObject()) {
            QString l_strType = l_jsvHeader[XFSIoTStandard::JK_TYPE].toString();
            // Service only support <command> message type
            if (l_strType != XFSIoTStandard::JV_TYPE_COMMAND) {
                // Send error ack to client
                sendAckMessage(l_jsvHeader, //
                               XFSIoTStandard::JV_INVALID_MESSAGE, //
                               QString("Invalid request type [%1]").arg(l_strType));
                return;
            } else {
                int l_iRequetId = l_jsvHeader[XFSIoTStandard::JK_REQUEST_ID].toInt();
                QString l_strName = l_jsvHeader[XFSIoTStandard::JK_NAME].toString();
                QStringList l_strSplitName = l_strName.split('.');
                if (l_strSplitName.size() != 2) {
                    sendAckMessage(l_jsvHeader, //
                                   XFSIoTStandard::JV_INVALID_MESSAGE, //
                                   QString("Invalid command name, " //
                                           "it have to [pattern: ^[0-9A-Za-z]*\\.[0-9A-Za-z]*$]")
                                           .arg(l_strName));
                } else {
                    if (handleClientCommand(l_iRequetId, l_strName, l_jsvPayload.toObject())) {
                        sendAckMessage(l_jsvHeader);
                    } else {
                        // Send error ack to client
                        sendAckMessage(l_jsvHeader, //
                                       XFSIoTStandard::JV_INVALID_MESSAGE, //
                                       QString("Can't handle command [%1]").arg(l_strName));
                    }
                }
            }
        } else {
            sendAckMessage(QJsonValue(), //
                           XFSIoTStandard::JV_INVALID_MESSAGE, //
                           QStringLiteral("Header not as JSON object"));
        }
    }
}

bool ClientHandle::handleClientCommand(int iRequetId, //
                                       const QString &strCommandName, //
                                       const QJsonObject &jsObject)
{
    postEvent2Service(new XFSIoTCommandEvent(id(), iRequetId, strCommandName, jsObject));
    return true;
}

void ClientHandle::postEvent2Service(QEvent *pEvent) const
{
    QCoreApplication::postEvent(m_pService, pEvent);
}

void ClientHandle::processBinaryMessage(QByteArray message)
{
    Q_UNUSED(message)
    QString l_strErr = QStringLiteral("Don't support binary message");
    error(l_strErr);
    sendAckMessage(QJsonValue(), XFSIoTStandard::JV_INVALID_MESSAGE, l_strErr);
}

void ClientHandle::socketDisconnected()
{
    postEvent2Service(new XFSIoTClientEvent(XFSIoTMsgEvent::ClientDisconnect, m_uiID));
    ServiceEndpoint *l_pEndpoint = (ServiceEndpoint *)parent();
    l_pEndpoint->delClient(m_uiID);
}
