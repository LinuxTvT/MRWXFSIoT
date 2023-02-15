#ifndef SERVICEMANAGER_H
#define SERVICEMANAGER_H

#include <QObject>
#include <QWebSocketServer>
#include "qglobal.h"
#include "qjsonarray.h"
#include "selfservice_object.h"
#include "service/abstract_service.h"
#include "service/service_endpoint.h"
#include "interface/interface_repository.h"
#include "service/service_repository.h"

#define STR_DEFAULT_ROOT_PATH QStringLiteral("/xfs4iot/v1.0")

class ServiceManager : public SelfServiceObject
{
    Q_OBJECT
public:
    inline static const QString JK_PAYLOAD = QStringLiteral("payload");
    inline static const QString JK_SERVICES = QStringLiteral("services");
    inline static const QString JK_INTERFACES = QStringLiteral("interfaces");

    inline static const QString JK_SERVICE_URI = QStringLiteral("serviceURI");
    inline static const QString JK_PUBLISHER_URI = QStringLiteral("publisherURI");
    inline static const QString JK_ENDPOINTS = QStringLiteral("endpoints");
    inline static const QString JK_PORT = QStringLiteral("port");

    explicit ServiceManager(QObject *parent = nullptr);
    virtual ~ServiceManager();
    bool init();
    bool start();

protected:
    static QString buildServiceUri(quint16 ui16Port, const QString &strServiceName);
    bool startEndpoint(ServiceEndpoint *pEndpoint);
    bool loadEndpoint(const QJsonArray jaEnpointsList);

private:
    ServiceRepository *m_pServiceRepository = nullptr;
    InterfaceRepository *m_pInterfaceRepository = nullptr;
    QList<ServiceEndpoint *> m_lEndpointsList;
    QString m_strPublisherUri = QStringLiteral("ws://localhost:5846/xfs4iot/v1.0");

    // SelfServiceObject interface
public:
    virtual bool loadConfig(const QJsonObject &config) override;
};

#endif // SERVICEMANAGER_H
