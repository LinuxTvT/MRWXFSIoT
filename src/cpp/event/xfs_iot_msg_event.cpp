#include "xfs_iot_msg_event.h"
#include "xfs_iot_standard.h"

XFSIoTMsgEvent::XFSIoTMsgEvent(Type type, uint uiClientId, int iReqeustId, const QString &strCommandName,
                               const QJsonObject &joPayload)
    : XFSIoTEvent(type),
      m_uiClientId(uiClientId),
      m_iReqeustId(iReqeustId),
      m_strCommandName(strCommandName),
      m_joPayload(joPayload)
{
}

bool XFSIoTMsgEvent::is(uint uiClientID, int iRequestID)
{
    if (m_uiClientId == uiClientID) {
        if (iRequestID == -1) {
            return true;
        } else {
            return (m_iReqeustId == iRequestID);
        }
    } else {
        return false;
    }
}

bool XFSIoTMsgEvent::is(uint uiClientID, const QSet<int> &iRequestIDs)
{
    if (m_uiClientId == uiClientID) {
        return iRequestIDs.contains(m_iReqeustId);
    } else {
        return false;
    }
}

const QString &XFSIoTMsgEvent::xfsIotType() const
{
    if (type() == XFSIoTEvent::CommandEvent) {
        return XFSIoTStandard::JV_TYPE_COMMAND;
    } else if (type() == XFSIoTEvent::Acknowledge) {
        return XFSIoTStandard::JV_TYPE_ACKNOWLEDGE;
    } else if (type() == XFSIoTEvent::EventEvent) {
        return XFSIoTStandard::JV_TYPE_EVENT;
    } else if (type() == XFSIoTEvent::CompletionEvent) {
        return XFSIoTStandard::JV_TYPE_COMPLETION;
    } else if (type() == XFSIoTEvent::Unsolicited) {
        return XFSIoTStandard::JV_TYPE_UNSOLICITED;
    } else {
        return XFSIoTStandard::JV_UNDEFINE;
    }
}

QJsonObject XFSIoTMsgEvent::header() const
{
    QJsonObject l_joHeader;
    l_joHeader[XFSIoTStandard::JK_TYPE] = xfsIotType();
    l_joHeader[XFSIoTStandard::JK_NAME] = m_strCommandName;
    l_joHeader[XFSIoTStandard::JK_REQUEST_ID] = m_iReqeustId;
    return l_joHeader;
}

XFSIoTCommandEvent::XFSIoTCommandEvent(uint uiClientId, //
                                       int iReqeustId, //
                                       const QString &strCommandName, //
                                       const QJsonObject &joPayload) //
    : XFSIoTMsgEvent(XFSIoTEvent::CommandEvent,
                     uiClientId, //
                     iReqeustId, //
                     strCommandName, //
                     joPayload)
{
    m_requestTime = QDateTime::currentDateTimeUtc();
    qDebug() << "Create Command Event:" << m_requestTime;
}

XFSIoTCommandEvent *XFSIoTCommandEvent::clone() const
{
    XFSIoTCommandEvent *l_pRet = new XFSIoTCommandEvent(clientId(), //
                                                        requestId(), //
                                                        commandName(), //
                                                        payLoad());
    l_pRet->m_requestTime = m_requestTime;
    return l_pRet;
}

void XFSIoTCommandEvent::cancel()
{
    setStaus(CANCELED);
}

bool XFSIoTCommandEvent::isCanceled()
{
    return CANCELED == status();
}

bool XFSIoTCommandEvent::isTimeOut()
{
    int l_iCommandTimeOut = payLoad()["timeout"].toInt();
    if (l_iCommandTimeOut > 0) {
        QDateTime l_curTime = QDateTime::currentDateTimeUtc();
        qDebug() << "Request Time" << m_requestTime;
        if (m_requestTime.addMSecs(l_iCommandTimeOut) < l_curTime) {
            qDebug() << "Request Time Add" << m_requestTime.addMSecs(l_iCommandTimeOut);
            qDebug() << "Current Time" << m_requestTime;
            qDebug() << "Time out " << l_iCommandTimeOut;
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void XFSIoTCommandEvent::setStaus(EStatus eStatus)
{
    m_muxState.lock();
    m_eStatus = eStatus;
    m_muxState.unlock();
}

XFSIoTCommandEvent::EStatus XFSIoTCommandEvent::status()
{
    m_muxState.lock();
    EStatus l_eTemp = m_eStatus;
    m_muxState.unlock();
    return l_eTemp;
}

XFSIoTCompletionEvent::XFSIoTCompletionEvent(const XFSIoTCommandEvent *pCommandEvent, //
                                             const QJsonObject &joPayload)
    : XFSIoTMsgEvent{ XFSIoTEvent::CompletionEvent, //
                      pCommandEvent->clientId(), //
                      pCommandEvent->requestId(), //
                      pCommandEvent->commandName(), //
                      joPayload }
{
}

XFSIoTEventEvent::XFSIoTEventEvent(const XFSIoTCommandEvent *pCommandEvent, //
                                   const QString &strEventName, //
                                   const QJsonObject &joPayload)
    : XFSIoTMsgEvent{ XFSIoTEvent::EventEvent, //
                      pCommandEvent->clientId(), //
                      pCommandEvent->requestId(), //
                      strEventName, //
                      joPayload }
{
}
