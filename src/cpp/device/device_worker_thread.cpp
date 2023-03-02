#include "device/device_worker_thread.h"
#include "device/abstract_device_worker.h"
#include "service/command_queue.h"

DeviceWorkerThread::DeviceWorkerThread(DeviceWorker *pWorker) //
    : m_pWorker(pWorker)
{
    qDebug("Create DeviceWorkerThread");
}

DeviceWorkerThread::~DeviceWorkerThread()
{
    qDebug("Destroy DeviceWorkerThread");
}

void DeviceWorkerThread::run()
{
    qDebug("Running... DeviceWorkerThread");
    CommandQueue *l_pCommandQueue = m_pWorker->commandQueue();
    while (true) {
        // qDebug("##### Waiting a COMMAND from service");
        XFSIoTCommandEvent *l_pCommandEvent = l_pCommandQueue->dequeueCommand(5000);
        if (l_pCommandEvent == nullptr) {
            // qDebug("==== No COMMAND, IDLE Processing");
            m_pWorker->idleProcess();
        } else {
            m_pWorker->setDeviceStatus(AbstractDeviceWorker::ST_deviceBusy);
            m_pWorker->executeCommand(l_pCommandEvent);
            l_pCommandQueue->completedExecute();
            m_pWorker->setDeviceStatus(AbstractDeviceWorker::ST_online);
        }
    }
}
