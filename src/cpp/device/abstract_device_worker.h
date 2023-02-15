#ifndef ABSTRACTDEVICEWORKER_H
#define ABSTRACTDEVICEWORKER_H

#include "event/xfs_iot_msg_event.h"
#include "qjsonobject.h"
#include "qthread.h"
#include "runable_object.h"
#include <QMutex>
#include <QObject>

class AbstractService;

#define JS_ENUM(x, y) x##_##y
#define JS_VALUE(x, y)                                                                                                 \
    {                                                                                                                  \
        x##_##y, #y                                                                                                    \
    }

class AbstractDeviceWorker : public RunableObject
{
    Q_OBJECT
public:
    enum EDeviceStatus {
        JS_ENUM(ST, online),
        JS_ENUM(ST, offline),
        JS_ENUM(ST, powerOff),
        JS_ENUM(ST, noDevice),
        JS_ENUM(ST, hardwareError),
        JS_ENUM(ST, userError),
        JS_ENUM(ST, deviceBusy),
        JS_ENUM(ST, fraudAttempt),
        JS_ENUM(ST, potentialFraud),
        JS_ENUM(ST, starting)
    };

    inline static QHash<EDeviceStatus, QString> DEVICE_STATUS_MAP = { JS_VALUE(ST, online), //
                                                                      JS_VALUE(ST, offline), //
                                                                      JS_VALUE(ST, powerOff), //
                                                                      JS_VALUE(ST, noDevice), //
                                                                      JS_VALUE(ST, hardwareError), //
                                                                      JS_VALUE(ST, userError), //
                                                                      JS_VALUE(ST, deviceBusy), //
                                                                      JS_VALUE(ST, fraudAttempt), //
                                                                      JS_VALUE(ST, potentialFraud), //
                                                                      JS_VALUE(ST, starting) };
    explicit AbstractDeviceWorker(const QString &strFileConfig, //
                                  AbstractService *pParentService, //
                                  QThread *pThread);
    virtual ~AbstractDeviceWorker();
    virtual void commonStatus(QJsonObject &joStatus);
    virtual bool doCommand(XFSIoTCommandEvent *pCommandEvent);
    virtual bool cancelCommand(uint uiClientID, int iRequestID = -1) = 0;
    virtual bool cancelCommand(uint uiClientID, const QSet<int> &iSetRequestIds);
    inline const AbstractService *service() const { return m_pService; }
    void setDeviceStatus(EDeviceStatus eNewStatus);
    EDeviceStatus deviceStatus();

private:
    // QJsonObject m_joCommonStatus;
    QMutex m_mtCommonStatus;
    EDeviceStatus m_eDeviceStatus;
    AbstractService *m_pService;

protected:
    bool setCommonStatus(const QString &strKey, const QString &strValue);

    // RunableObject interface
protected:
    virtual bool init() override;
};

#endif // ABSTRACTDEVICEWORKER_H
