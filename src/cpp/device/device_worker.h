#ifndef DEVICEWORKER_H
#define DEVICEWORKER_H

#include "abstract_device_worker.h"
#include "service/command_queue.h"
#include <QObject>

class AbstractService;

class DeviceWorker : public AbstractDeviceWorker
{
    Q_OBJECT
public:
    explicit DeviceWorker(const QString &strFileConfig, AbstractService *pParentService);
    virtual ~DeviceWorker();
    inline CommandQueue *commandQueue() { return m_pCommandQueue; }

private:
    CommandQueue *m_pCommandQueue = nullptr;

    // AbstractDeviceWorker interface
public:
    virtual bool cancelCommand(uint uiClientID, int iRequestID) override;
};

#endif // DEVICEWORKER_H
