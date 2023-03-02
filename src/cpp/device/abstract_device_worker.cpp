#include "abstract_device_worker.h"
#include "service/abstract_service.h"

#define jk_starting "starting"

AbstractDeviceWorker::AbstractDeviceWorker(const QString &strName, //
                                           const QString &strFileConfig, //
                                           AbstractService *pParentService, //
                                           QThread *pThread)
    : RunableObject{ strName, strFileConfig, pParentService, pThread }
{
    m_pService = pParentService;
}

AbstractDeviceWorker::~AbstractDeviceWorker() { }

bool AbstractDeviceWorker::cancelCommand(uint uiClientID, const QSet<int> &iSetRequestIds)
{
    for (auto itr = iSetRequestIds.constBegin(); itr != iSetRequestIds.constEnd(); itr++) {
        cancelCommand(uiClientID, *(itr));
    }
    return true;
}

bool AbstractDeviceWorker::isReady()
{
    QMutexLocker LOCKER(&m_muxReady);
    return m_isReady;
}
void AbstractDeviceWorker::setReady(bool newVal)
{
    QMutexLocker LOCKER(&m_muxReady);
    m_isReady = newVal;
}

bool AbstractDeviceWorker::init()
{
    return true;
}
