#ifndef PUBLISHERSERVICE_H
#define PUBLISHERSERVICE_H

#include "service/abstract_service.h"
#include <QJsonArray>
#include <QObject>

class PublisherService : public AbstractService
{
    Q_OBJECT
public:
    inline static const QString PUBLISHER_SERVICE_NAME = QStringLiteral("Publisher");
    Q_INVOKABLE explicit PublisherService(const QString &strName, const QString &strFileConfig);
    virtual ~PublisherService();
    void addUriServices(const QString &strUri);

    SERVICE_FUNCTION(Common, Status) override;
    SERVICE_FUNCTION(ServicePublisher, GetServices);

private:
    void buildJsonServices(QJsonArray &jsArray) const;
    AbstractDeviceWorker *loadDeviceWorker(const QJsonValue &joProperties) override;

private:
    QList<QString> m_lUriServices;
};

#endif // PUBLISHERSERVICE_H
