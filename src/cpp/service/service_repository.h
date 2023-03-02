#ifndef SERVICEREPOSITORY_H
#define SERVICEREPOSITORY_H

#include "interface/interface_repository.h"
#include "selfservice_object.h"
#include "service/abstract_service.h"
#include <QObject>

class ServiceRepository : public SelfServiceObject
{
    Q_OBJECT
public:
    explicit ServiceRepository(QObject *parent = nullptr);
    virtual ~ServiceRepository();
    void addServiceClass(const QMetaObject *pMetaObject);
    virtual bool load(const QJsonArray &jaServices);
    AbstractService *getService(const QString &strName);

private:
    QHash<QString, AbstractService *> m_hServicesList;
    QHash<QString, const QMetaObject *> m_hMetaObjects;
    void addService(AbstractService *pInterface);
    AbstractService *createService(const QString &strClassName, //
                                   const QString &strName, //
                                   const QString &strFileConfig);
};

#endif // SERVICEREPOSITORY_H
