#include "abstract_device_worker.h"
#include "service/abstract_service.h"

#define jk_starting "starting"

AbstractDeviceWorker::AbstractDeviceWorker(const QString &strFileConfig, //
                                           AbstractService *pParentService, //
                                           QThread *pThread)
    : RunableObject{ strFileConfig, pParentService, pThread }
{
    m_pService = pParentService;
}

AbstractDeviceWorker::~AbstractDeviceWorker() { }

void AbstractDeviceWorker::setDeviceStatus(EDeviceStatus eNewStatus)
{
    m_mtCommonStatus.lock();
    m_eDeviceStatus = eNewStatus;
    m_mtCommonStatus.unlock();
}

AbstractDeviceWorker::EDeviceStatus AbstractDeviceWorker::deviceStatus()
{
    m_mtCommonStatus.lock();
    EDeviceStatus l_eTmpRet = m_eDeviceStatus;
    m_mtCommonStatus.unlock();
    return l_eTmpRet;
}

void AbstractDeviceWorker::commonStatus(QJsonObject &joStatus)
{
    joStatus["device"] = DEVICE_STATUS_MAP[deviceStatus()];
}

bool AbstractDeviceWorker::cancelCommand(uint uiClientID, const QSet<int> &iSetRequestIds)
{
    for (auto itr = iSetRequestIds.constBegin(); itr != iSetRequestIds.constEnd(); itr++) {
        cancelCommand(uiClientID, *(itr));
    }
    return true;
}

bool AbstractDeviceWorker::doCommand(XFSIoTCommandEvent *pCommandEvent)
{
    debug(QString("DO COMMAND [%1]").arg(pCommandEvent->commandName()));
    if (pCommandEvent->isCanceled()) {
        warn("Command was canceled");
        this->service()->notifyCanceled(pCommandEvent);
        return true;
    } else {
        return false;
    }
}

bool AbstractDeviceWorker::init()
{
    setDeviceStatus(ST_starting);
    return true;
}
