#include "service_repository.h"
#include "interface/interface_repository.h"

ServiceRepository::ServiceRepository(QObject *parent) : SelfServiceObject{ parent } { }

ServiceRepository::~ServiceRepository()
{
    for (auto it = m_hServicesList.begin(); it != m_hServicesList.end(); it++) {
        it.value()->deleteLater();
    }
    m_hServicesList.clear();
}

void ServiceRepository::addServiceClass(const QMetaObject *pMetaObject)
{
    m_hMetaObjects.insert(pMetaObject->className(), pMetaObject);
}

bool ServiceRepository::load(const QJsonArray &jaServices)
{
    for (auto it = jaServices.begin(); it != jaServices.end(); it++) {
        if (it->isObject()) {
            const QJsonValue l_jvName = (*it)["name"];
            const QJsonValue l_jvClass = (*it)["class"];
            const QJsonValue l_jvInterfacesArray = (*it)["interfaces"];
            const QJsonValue l_jvConfigsFile = (*it)["configs"];
            if (l_jvName.isString()) {
                if (l_jvClass.isString()) {
                    AbstractService *l_pNewService = createService(l_jvClass.toString(), //
                                                                   l_jvName.toString(), //
                                                                   l_jvConfigsFile.toString());
                    if (l_pNewService != nullptr) {
                        if (l_pNewService->load(l_jvInterfacesArray.toArray())) {
                            log(QString("Load service [%1] success").arg(l_jvClass.toString()));
                            addService(l_pNewService);
                        } else {
                            error(QString("Load service [%1] ERROR").arg(l_jvClass.toString()));
                            return false;
                        }
                    } else {
                        error(QString("Unknow Class [%1]").arg(l_jvClass.toString()));
                        return false;
                    }
                } else {
                    error(QString("Config service error, [class] requide String"));
                    return false;
                }
            } else {
                error(QString("Config service error, [name] requide String"));
                return false;
            }
        } else {
            error(QString("Service item requide a Object"));
            return false;
        }
    }
    return true;
}

AbstractService *ServiceRepository::getService(const QString &strName)
{
    if (m_hServicesList.contains(strName)) {
        return m_hServicesList.value(strName);
    } else {
        return nullptr;
    }
}

void ServiceRepository::addService(AbstractService *pService)
{
    if (m_hServicesList.contains(pService->name())) {
        warn(QString("Service [%1] ready added").arg(pService->name()));
        m_hServicesList.take(pService->name())->deleteLater();
    }
    m_hServicesList.insert(pService->name(), pService);
}

AbstractService *ServiceRepository::createService(const QString &strClassName, //
                                                  const QString &strName, //
                                                  const QString &strFileConfig)
{
    const QMetaObject *l_pMetaObject = m_hMetaObjects.value(strClassName);
    if (l_pMetaObject == nullptr) {
        error(QString("Can't find QMetaObject for class name [%1]").arg(strClassName));
        return nullptr;
    }
    QObject *l_pObject = l_pMetaObject->newInstance(Q_ARG(QString, strName), Q_ARG(QString, strFileConfig));
    return (AbstractService *)l_pObject;
}
