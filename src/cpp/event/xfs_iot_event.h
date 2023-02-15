#ifndef XFSIOTEVENT_H
#define XFSIOTEVENT_H

#include <QEvent>
#include <QObject>

class XFSIoTEvent : public QEvent
{
public:
    inline static const QEvent::Type CommandEvent = (QEvent::Type)QEvent::registerEventType();
    inline static const QEvent::Type Acknowledge = (QEvent::Type)QEvent::registerEventType();
    inline static const QEvent::Type CompletionEvent = (QEvent::Type)QEvent::registerEventType();
    inline static const QEvent::Type EventEvent = (QEvent::Type)QEvent::registerEventType();
    inline static const QEvent::Type Unsolicited = (QEvent::Type)QEvent::registerEventType();
    inline static const QEvent::Type ClientConnected = (QEvent::Type)QEvent::registerEventType();
    inline static const QEvent::Type ClientDisconnect = (QEvent::Type)QEvent::registerEventType();
    explicit XFSIoTEvent(Type type);
    virtual ~XFSIoTEvent() { }
};

class XFSIoTClientEvent : public XFSIoTEvent
{
public:
    explicit XFSIoTClientEvent(Type type, int iClientID) : XFSIoTEvent{ type }, m_iClientID(iClientID) { }
    virtual ~XFSIoTClientEvent() { }
    inline int clientID() const { return m_iClientID; }

private:
    int m_iClientID;
};

#endif // XFSIOTEVENT_H
