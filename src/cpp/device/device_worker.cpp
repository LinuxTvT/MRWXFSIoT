#include "device_worker.h"
#include "device/abstract_device_worker.h"
#include "device/device_worker_thread.h"
#include "service/abstract_service.h"

DeviceWorker::DeviceWorker(const QString &strName, //
                           const QString &strFileConfig, //
                           AbstractService *pParentService)
    : AbstractDeviceWorker{ strName, //
                            strFileConfig, //
                            pParentService, //
                            new DeviceWorkerThread(this) }
{
    m_pCommandQueue = new CommandQueue();
}

DeviceWorker::~DeviceWorker()
{
    delete m_pCommandQueue;
}

void DeviceWorker::setDeviceStatus(EDeviceStatus eNewStatus)
{
    QMutexLocker LOCKER(&m_mtDeviceStatus);
    m_eDeviceStatus = eNewStatus;
    setCommonStatus("device", DEVICE_STATUS_MAP.value(m_eDeviceStatus));
}

AbstractDeviceWorker::EDeviceStatus DeviceWorker::deviceStatus()
{
    QMutexLocker LOCKER(&m_mtDeviceStatus);
    return m_eDeviceStatus;
}

void DeviceWorker::commonStatus(QJsonObject &joPayload)
{
    m_mtCommonStatus.lock();
    joPayload["common"] = m_joCommonStatus;
    m_mtCommonStatus.unlock();
}

void DeviceWorker::setCommonStatus(const QString &strKey, const QString &strValue)
{
    m_mtCommonStatus.lock();
    m_joCommonStatus[strKey] = strValue;
    m_mtCommonStatus.unlock();
}

bool DeviceWorker::executeCommand(XFSIoTCommandEvent *pCommandEvent)
{
    debug(QString("DO COMMAND [%1]").arg(pCommandEvent->commandName()));
    if (pCommandEvent->isCanceled()) {
        warn("Command was canceled");
        service()->notifyCanceled(pCommandEvent);
        return true;
    } else if (pCommandEvent->isTimeOut()) {
        warn("Command was time out");
        service()->notifyTimeOut(pCommandEvent);
        return true;
    } else {
        return false;
    }
}

bool DeviceWorker::isBusy()
{
    return (ST_deviceBusy == deviceStatus());
}

bool DeviceWorker::queueCommand(const XFSIoTCommandEvent *pCommandEvent)
{
    return m_pCommandQueue->enqueueCommand(pCommandEvent->clone());
}

bool DeviceWorker::cancelCommand(uint uiClientID, int iRequestID)
{
    m_pCommandQueue->cancelCommand(uiClientID, iRequestID);
    return true;
}

void DeviceWorker::idleProcess()
{
    debug("Idle proccessing");
}

bool DeviceWorker::loadConfig(const QJsonObject &config)
{
    AbstractDeviceWorker::loadConfig(config);
    m_joCommonStatus = config["initCommonStatus"].toObject();
    return true;
}
