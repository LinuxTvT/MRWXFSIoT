#include "service_manager.h"
#include "device/v3x/device_worker_v3x.h"
#include "qglobal.h"
#include "qjsonarray.h"
#include "qjsonvalue.h"
#include "qthread.h"
#include "service/cardreader/cardreader_service.h"
#include "service/printer/printer_service.h"
#include "service/printer/printer_service_v3x.h"
#include "service/publisher/publisher_service.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QWebSocket>

ServiceManager::ServiceManager(QObject *parent) : SelfServiceObject{ parent }
{
    m_pServiceRepository = new ServiceRepository(this);
    m_pInterfaceRepository = new InterfaceRepository(this);
    m_pServiceRepository->addServiceClass(&CardreaderService::staticMetaObject);
    m_pServiceRepository->addServiceClass(&PrinterService::staticMetaObject);
    m_pServiceRepository->addServiceClass(&PublisherService::staticMetaObject);
    m_pServiceRepository->addServiceClass(&PrinterServiceV3x::staticMetaObject);
}

ServiceManager::~ServiceManager()
{
    log("Destroy ServiceManager");
}

bool ServiceManager::init()
{

    DeviceWorkerV3x::loadXFSSDK();
    PublisherService *l_pPublisherService =
            (PublisherService *)m_pServiceRepository->getService(PublisherService::PUBLISHER_SERVICE_NAME);

    if (l_pPublisherService != nullptr) {
        for (auto itr = m_lEndpointsList.constBegin(); itr != m_lEndpointsList.constEnd(); itr++) {
            QList<QString> l_servicesName = (*itr)->servicesName();
            for (auto itrSN = l_servicesName.constBegin(); itrSN != l_servicesName.constEnd(); itrSN++) {
                l_pPublisherService->addUriServices(buildServiceUri((*itr)->port(), (*itrSN)));
            }
        }
        return true;
    } else {
        error(QString("Init ServiceManager failed, find not found [%1] service ")
                      .arg(PublisherService ::PUBLISHER_SERVICE_NAME));
        return false;
    }
}

bool ServiceManager::start()
{
    for (auto itr = m_lEndpointsList.begin(); itr != m_lEndpointsList.end(); itr++) {
        debug(QString("##########> Starting endpoint [%1]").arg((*itr)->name()));
        startEndpoint(*itr);
    }
    return true;
}

QString ServiceManager::buildServiceUri(quint16 ui16Port, const QString &strServiceName)
{
    return QString("ws://localhost:%1/xfs4iot/v1.0/%2").arg(ui16Port).arg(strServiceName);
}

bool ServiceManager::startEndpoint(ServiceEndpoint *pEndpoint)
{
    log("Starts Endpoint");
    pEndpoint->run();
    log("Endpoint started");

    return true;
}

bool ServiceManager::loadEndpoint(const QJsonArray jaEnpointsList)
{
    for (auto it = jaEnpointsList.begin(); it != jaEnpointsList.end(); it++) {
        const QJsonValue &l_jsValService = (*it)[JK_SERVICES];
        if (l_jsValService.isArray()) {
            ServiceEndpoint *l_pNewEndpoint = new ServiceEndpoint((*it)[JK_PORT].toInt(), //
                                                                  (*it)["name"].toString(), //
                                                                  (*it)["configs"].toString(), //
                                                                  nullptr);
            m_lEndpointsList.append(l_pNewEndpoint);
            const QJsonArray l_jaService = l_jsValService.toArray();
            for (auto itService = l_jaService.begin(); itService != l_jaService.end(); itService++) {
                QString l_strServiceName = itService->toString();
                AbstractService *l_pService = m_pServiceRepository->getService(l_strServiceName);
                if (l_pService != nullptr) {
                    l_pNewEndpoint->addService(l_pService);
                } else {
                    error(QString("Can not find service, name [%1]").arg(l_strServiceName));
                    return false;
                }
            }
        } else {
            error(QString("Json of [%1] have to the array").arg(JK_SERVICES));
            return false;
        }
    }
    return true;
}

bool ServiceManager::loadConfig(const QJsonObject &config)
{
    SelfServiceObject::loadConfig(config);
    /*
     * Load interfaces XFSIoT Support
     * */
    const QJsonValue &l_jvInterfaces = config[JK_INTERFACES];
    if (l_jvInterfaces.isArray()) {
        if (m_pInterfaceRepository->load(l_jvInterfaces.toArray())) {
            log("Load XFSIoT interfaces success");
            AbstractService::setInterfaceRepository(m_pInterfaceRepository);
        } else {
            error("Load XFSIoT interfaces ERROR");
            return false;
        }
    } else {
        error(QString("Json of [%1] have to the array").arg(JK_INTERFACES));
        return false;
    }

    /*
     *
     * */

    const QJsonValue &l_jvServices = config[JK_SERVICES];
    ;
    if (l_jvServices.isArray()) {
        if (m_pServiceRepository->load(l_jvServices.toArray())) {
            log("Load Services success");
        } else {
            error("Load Services ERROR");
            return false;
        }
    } else {
        error(QString("Json of [%1] have to the array").arg(JK_SERVICES));
        return false;
    }

    const QJsonValue &l_jsValEndpoints = config[JK_ENDPOINTS];
    if (l_jsValEndpoints.isArray()) {
        if (loadEndpoint(l_jsValEndpoints.toArray())) {
            log("Load Endpoints success");
        } else {
            error("Load Endpoints ERROR");
            return false;
        }
    } else {
        error(QString("Json of [%1] have to the array").arg(JK_ENDPOINTS));
        return false;
    }
    return true;
}
