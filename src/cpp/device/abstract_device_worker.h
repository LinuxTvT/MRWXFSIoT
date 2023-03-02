#ifndef ABSTRACTDEVICEWORKER_H
#define ABSTRACTDEVICEWORKER_H

#include "event/xfs_iot_msg_event.h"
#include "runable_object.h"
#include <QJsonObject>
#include <QMutex>
#include <QObject>
#include <QThread>

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
    explicit AbstractDeviceWorker(const QString &strName, //
                                  const QString &strFileConfig, //
                                  AbstractService *pParentService, //
                                  QThread *pThread);
    virtual ~AbstractDeviceWorker();
    virtual bool cancelCommand(uint uiClientID, int iRequestID = -1) = 0;
    virtual bool cancelCommand(uint uiClientID, const QSet<int> &iSetRequestIds);
    bool isReady();
    void setReady(bool newVal = true);
    inline const AbstractService *service() const { return m_pService; }

private:
    AbstractService *m_pService;
    bool m_isReady = false;
    QMutex m_muxReady;

protected:
    virtual bool init() override;
};

#endif // ABSTRACTDEVICEWORKER_H
