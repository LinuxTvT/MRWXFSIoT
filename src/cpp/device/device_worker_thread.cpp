#include "device/device_worker_thread.h"
#include "device/abstract_device_worker.h"
#include "service/command_queue.h"

DeviceWorkerThread::DeviceWorkerThread(DeviceWorker *pWorker) : QThread{ pWorker }, m_pWorker(pWorker) { }

void DeviceWorkerThread::run()
{
    CommandQueue *l_pCommandQueue = m_pWorker->commandQueue();
    while (true) {
        qDebug("############################ Wait printe command");
        XFSIoTCommandEvent *l_pCommandEvent = l_pCommandQueue->executeCommand();
        qDebug("============================> Have command");
        if (l_pCommandEvent == nullptr) {
            qDebug("============================> NULLLLLLLLLLLLLLLLLLL");
            return;
        } else {
            m_pWorker->setDeviceStatus(AbstractDeviceWorker::ST_deviceBusy);
            m_pWorker->doCommand(l_pCommandEvent);
            l_pCommandQueue->completedExecute();
            m_pWorker->setDeviceStatus(AbstractDeviceWorker::ST_online);
        }
    }
}
