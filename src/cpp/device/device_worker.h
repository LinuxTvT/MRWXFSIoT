#ifndef DEVICEWORKER_H
#define DEVICEWORKER_H

#include "abstract_device_worker.h"
#include "qjsonobject.h"
#include "service/command_queue.h"
#include <QObject>

class AbstractService;

class DeviceWorker : public AbstractDeviceWorker
{
    Q_OBJECT
public:
    explicit DeviceWorker(const QString &strName, //
                          const QString &strFileConfig, //
                          AbstractService *pParentService);
    virtual ~DeviceWorker();

    inline CommandQueue *commandQueue() { return m_pCommandQueue; }
    bool queueCommand(const XFSIoTCommandEvent *pCommandEvent);
    virtual bool executeCommand(XFSIoTCommandEvent *pCommandEvent);
    virtual bool isBusy();

public:
    virtual bool cancelCommand(uint uiClientID, int iRequestID) override;
    virtual void idleProcess();

    void setDeviceStatus(EDeviceStatus eNewStatus);
    EDeviceStatus deviceStatus();
    void commonStatus(QJsonObject &joPayload);
    void setCommonStatus(const QString &strKey, const QString &strValue);

private:
    CommandQueue *m_pCommandQueue = nullptr;
    QJsonObject m_joCommonStatus;
    QMutex m_mtCommonStatus;
    QMutex m_mtDeviceStatus;
    EDeviceStatus m_eDeviceStatus;

    // SelfServiceObject interface
public:
    virtual bool loadConfig(const QJsonObject &config) override;
};

#endif // DEVICEWORKER_H
