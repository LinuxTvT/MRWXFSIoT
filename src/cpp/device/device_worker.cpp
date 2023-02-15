#include "device_worker.h"
#include "device/device_worker_thread.h"
#include "service/abstract_service.h"

DeviceWorker::DeviceWorker(const QString &strFileConfig, AbstractService *pParentService)
    : AbstractDeviceWorker{ strFileConfig, pParentService, new DeviceWorkerThread(this) }
{
    m_pCommandQueue = new CommandQueue();
}

DeviceWorker::~DeviceWorker()
{
    delete m_pCommandQueue;
}

bool DeviceWorker::cancelCommand(uint uiClientID, int iRequestID)
{
    m_pCommandQueue->cancelCommand(uiClientID, iRequestID);
    return true;
}
