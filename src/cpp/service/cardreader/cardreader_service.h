#ifndef CARDREADER_SERVICE_H
#define CARDREADER_SERVICE_H

#include "service/abstract_service.h"
#include <QObject>

class CardreaderService : public AbstractService
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit CardreaderService(const QString &strName, //
                                           const QString &strFileConfig);
    virtual ~CardreaderService() { }

    SERVICE_FUNCTION(Common, Status) override;

    // AbstractService interface
protected:
    virtual AbstractDeviceWorker *loadDeviceWorker(const QJsonValue &joProperties) override;
};

#endif // CARDREADER_SERVICE_H
