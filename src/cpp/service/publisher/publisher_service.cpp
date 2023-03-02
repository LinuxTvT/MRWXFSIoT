#include "publisher_service.h"
#include "qeventloop.h"
#include "qglobal.h"
#include "xfs_iot_standard.h"

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
    foreach (QString item, m_lUriServices) {
        QJsonObject l_jsObject;
        l_jsObject[XFSIoTStandard::JK_SEVICE_URI] = item;
        jsArray.append(l_jsObject);
    }
}

AbstractDeviceWorker *PublisherService::loadDeviceWorker(const QJsonValue &joProperties)
{
    return nullptr;
}

void PublisherService::Common_Status(XFSIoTCommandEvent *pEvent)
{
    error(QString("Unimplement command [%1]").arg(pEvent->commandName()));
}
