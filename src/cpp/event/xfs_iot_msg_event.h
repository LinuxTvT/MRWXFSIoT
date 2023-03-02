#ifndef _CMD_MESSAGEEVENT_H
#define _CMD_MESSAGEEVENT_H

#include "event/xfs_iot_event.h"
#include "qjsonobject.h"
#include <QEvent>
#include <QMutex>
#include <QObject>

class XFSIoTMsgEvent : public XFSIoTEvent
{

protected:
    XFSIoTMsgEvent(Type type, //
                   uint uiClientId, //
                   int iReqeustId, //
                   const QString &strCommandName, //
                   const QJsonObject &joPayload);

public:
    virtual ~XFSIoTMsgEvent() { }

    inline uint clientId() const { return m_uiClientId; }
    inline int requestId() const { return m_iReqeustId; }
    inline const QJsonObject &payLoad() const { return m_joPayload; }
    inline const QString &commandName() const { return m_strCommandName; }
    bool is(uint uiClientID, int iRequestID = -1);
    bool is(uint uiClientID, const QSet<int> &iRequestIDs);
    const QString &xfsIotType() const;
    bool buildJsonMsg(QJsonObject &joBuilder);

private:
    uint m_uiClientId;
    int m_iReqeustId;
    QString m_strCommandName;
    QJsonObject m_joPayload;
};

class XFSIoTCommandEvent : public XFSIoTMsgEvent
{

public:
    enum EStatus { INIT, QUEUED, EXECUTING, CANCELED };
    XFSIoTCommandEvent(uint uiClientId, //
                       int iReqeustId, //
                       const QString &strCommandName, //
                       const QJsonObject &joPayload);

    XFSIoTCommandEvent *clone() const;
    virtual ~XFSIoTCommandEvent() { }

    inline const QDateTime &requestTime() const { return m_requestTime; }

    void cancel();
    bool isCanceled();
    bool isTimeOut();
    void setStaus(EStatus eStatus);
    EStatus status();

private:
    EStatus m_eStatus = INIT;
    QMutex m_muxState;
    QDateTime m_requestTime;
};

class XFSIoTCompletionEvent : public XFSIoTMsgEvent
{

public:
    XFSIoTCompletionEvent(const XFSIoTCommandEvent *pCommandEvent, //
                          const QJsonObject &joPayload);
    virtual ~XFSIoTCompletionEvent() { }
};

class XFSIoTEventEvent : public XFSIoTMsgEvent
{

public:
    XFSIoTEventEvent(const XFSIoTCommandEvent *pCommandEvent, //
                     const QString &strEventName, //
                     const QJsonObject &joPayload);
    virtual ~XFSIoTEventEvent() { }
};

#endif // _CMD_MESSAGEEVENT_H
