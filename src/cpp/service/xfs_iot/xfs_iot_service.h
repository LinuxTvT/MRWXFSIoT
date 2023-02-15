#ifndef XFSIOTSERVICE_H
#define XFSIOTSERVICE_H

#include "service/abstract_service.h"
#include <QObject>

class XFSIoTService : public AbstractService
{
    Q_OBJECT
public:
    explicit XFSIoTService(const QString &strFileConfig);
    virtual ~XFSIoTService();
};

#endif // XFSIOTSERVICE_H
