#ifndef DEVICE_WORKER_THREAD_H
#define DEVICE_WORKER_THREAD_H

#include "device/abstract_device_worker.h"
#include "device/device_worker.h"
#include "service/command_queue.h"
#include <QObject>
#include <QThread>

class DeviceWorkerThread : public QThread
{
    Q_OBJECT
public:
    explicit DeviceWorkerThread(DeviceWorker *pWorker); //

private:
    // CommandQueue *m_pCommandQueue;
    DeviceWorker *m_pWorker;

    // QThread interface
protected:
    virtual void run() override;
};

#endif // DEVICE_WORKER_THREAD_H
