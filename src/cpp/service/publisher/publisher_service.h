#ifndef PUBLISHERSERVICE_H
#define PUBLISHERSERVICE_H

#include "service/abstract_service.h"
#include "qjsonarray.h"
#include <QObject>

class PublisherService : public AbstractService
{
    Q_OBJECT
public:
    inline static const QString PUBLISHER_SERVICE_NAME = QStringLiteral("Publisher");
    Q_INVOKABLE explicit PublisherService(const QString &strFileConfig) : AbstractService(strFileConfig) { }
    virtual ~PublisherService() { }
    void addUriServices(const QString &strUri);

    SERVICE_FUNCTION(ServicePublisher, GetServices);
    // Q_INVOKABLE void ServicePublisher_GetServices(XFSIoTCommandEvent *pEvent);

protected:
    virtual void buildJsonServices(QJsonArray &jsArray) const;

private:
    QList<QString> m_lUriServices;
};

#endif // PUBLISHERSERVICE_H
