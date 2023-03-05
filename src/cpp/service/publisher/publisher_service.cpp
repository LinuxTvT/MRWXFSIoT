#include "publisher_service.h"
#include "xfs_iot_standard.h"
#include <QJsonObject>

PublisherService::PublisherService(const QString &strName, const QString &strFileConfig)
    : AbstractService(strName, strFileConfig)
{
}

PublisherService::~PublisherService() { }

void PublisherService::addUriServices(const QString &strUri)
{
    m_lUriServices.append(strUri);
}

void PublisherService::ServicePublisher_GetServices(XFSIoTCommandEvent *pEvent)
{
    QJsonObject l_joPayload;
    QJsonArray l_jsArrServices;
    XFSIoTStandard::buildPayloadCompletion(l_joPayload);
    buildJsonServices(l_jsArrServices);
    l_joPayload[XFSIoTStandard::JK_SERVICES] = l_jsArrServices;
    notifyCompletion(pEvent, l_joPayload);
}

void PublisherService::buildJsonServices(QJsonArray &jsArray) const
{
    for (auto itr = m_lUriServices.constBegin(); itr != m_lUriServices.constEnd(); itr++) {
        QJsonObject l_jsObject;
        l_jsObject[XFSIoTStandard::JK_SEVICE_URI] = (*itr);
        jsArray.append(l_jsObject);
    }
}

AbstractDeviceWorker *PublisherService::loadDeviceWorker(const QJsonValue &joProperties)
{
    error(QString("Publisher Service not need WORKER [%1]").arg(joProperties.toString()));
    return nullptr;
}

void PublisherService::Common_Status(XFSIoTCommandEvent *pEvent)
{
    error(QString("Unimplement command [%1]").arg(pEvent->commandName()));
}
