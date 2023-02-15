#ifndef V3XSERVICE_H
#define V3XSERVICE_H

#include "service/abstract_service.h"
#include <QObject>

class V3xService : public AbstractService
{
    Q_OBJECT
public:
    explicit V3xService(const QString &strFileConfig);
    virtual ~V3xService();
};

#endif // V3XSERVICE_H
