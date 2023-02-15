#include "interface/interface_repository.h"
#include "qjsonvalue.h"
#include "utils/json_utils.h"

InterfaceRepository::InterfaceRepository(QObject *parent) : SelfServiceObject{ parent } { }

bool InterfaceRepository::load(const QJsonArray &jaInterfaces)
{
    for (auto it = jaInterfaces.begin(); it != jaInterfaces.end(); it++) {
        if (it->isObject()) {
            const QJsonValue l_jvName = (*it)["name"];
            const QJsonValue l_jvFunctionArray = (*it)["functions"];
            if (l_jvName.isString()) {
                if (l_jvFunctionArray.isArray()) {
                    XFSIoTInterface *l_pNewInterface = new XFSIoTInterface(l_jvName.toString());
                    if (l_pNewInterface->load(l_jvFunctionArray.toArray())) {
                        debug(QString("Load interface [%1] success!").arg(l_pNewInterface->name()));
                        addInterface(l_pNewInterface);
                    } else {
                        error(QString("Load interface [%1] ERROR!").arg(l_pNewInterface->name()));
                        delete l_pNewInterface;
                        return false;
                    }
                } else {
                    error(QString("Config [%1] interface error, list function not array, it is [%2]")
                                  .arg(l_jvName.toString(), JSonUtils::typeOfValue(*it)));
                    return false;
                }
            } else {
                error(QString("Config interface error, name of interace not a object, it is [%1]")
                              .arg(JSonUtils::typeOfValue(*it)));
                return false;
            }
        } else {
            error(QString("Config interface error, value of Interface not a object, it is [%1]")
                          .arg(JSonUtils::typeOfValue(*it)));
            return false;
        }
    }
    return true;
}

InterfaceRepository::~InterfaceRepository()
{
    for (auto it = m_hInterfacesList.begin(); it != m_hInterfacesList.end(); it++) {
        delete it.value();
    }
    m_hInterfacesList.clear();
}

XFSIoTInterface *InterfaceRepository::getInterface(const QString &strName) const
{
    if (m_hInterfacesList.contains(strName)) {
        return m_hInterfacesList.value(strName);
    } else {
        return nullptr;
    }
}

void InterfaceRepository::addInterface(XFSIoTInterface *pInterface)
{
    if (m_hInterfacesList.contains(pInterface->name())) {
        delete m_hInterfacesList.take(pInterface->name());
    }
    m_hInterfacesList.insert(pInterface->name(), pInterface);
}
