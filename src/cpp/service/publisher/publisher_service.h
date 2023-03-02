#ifndef PUBLISHERSERVICE_H
#define PUBLISHERSERVICE_H

#include "qjsonarray.h"
#include "service/abstract_service.h"
#include <QObject>

class PublisherService : public AbstractService
{
    Q_OBJECT
public:
    inline static const QString PUBLISHER_SERVICE_NAME = QStringLiteral("Publisher");
    Q_INVOKABLE explicit PublisherService(const QString &strName, const QString &strFileConfig)
        : AbstractService(strName, strFileConfig)
    {
    }
    virtual ~PublisherService() { }
    void addUriServices(const QString &strUri);

    SERVICE_FUNCTION(Common, Status) override;
    SERVICE_FUNCTION(ServicePublisher, GetServices);

protected:
    virtual void buildJsonServices(QJsonArray &jsArray) const;

private:
    QList<QString> m_lUriServices;

    // AbstractService interface
protected:
    virtual AbstractDeviceWorker *loadDeviceWorker(const QJsonValue &joProperties) override;
};

#endif // PUBLISHERSERVICE_H
