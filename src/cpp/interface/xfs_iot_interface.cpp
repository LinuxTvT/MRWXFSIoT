#include "xfs_iot_interface.h"
#include "qjsonarray.h"
#include "service/abstract_service.h"
#include "utils/json_utils.h"
#include "xfs_iot_standard.h"

char *stringDup(const QString &str)
{
    QByteArray l_baStr = str.toLocal8Bit();
    return qstrdup(l_baStr);
}

XFSIoTInterface::XFSIoTInterface(const QString &strName) : m_strName(strName), m_strJKName(strName)
{
    m_strJKName.replace(0, 1, m_strJKName[0].toLower());
}

XFSIoTInterface::~XFSIoTInterface()
{
    for (auto it = m_hFunctionsList.begin(); it != m_hFunctionsList.end(); it++) {
        delete (*it);
    }
    m_hFunctionsList.clear();
}

bool XFSIoTInterface::load(const QJsonArray &jaFunctionsList)
{
    for (auto it = jaFunctionsList.begin(); it != jaFunctionsList.end(); it++) {
        if (it->isString()) {
            addFunction(new XFSIoTFunction(it->toString(), this));
        } else {
            //            error(QString("Config interface error, value of function name not a string, it is [%1]")
            //                          .arg(JSonUtils::typeOfValue(*it)));
            return false;
        }
    }
    return true;
}

bool XFSIoTInterface::invokeServiceFunction(const QString &strFunctionName, AbstractService *pService,
                                            XFSIoTCommandEvent *pEvent) const
{
    if (m_hFunctionsList.contains(strFunctionName)) {
        return m_hFunctionsList.value(strFunctionName)->invoke(pService, pEvent);
    } else {
        return false;
    }
}

void XFSIoTInterface::addFunction(XFSIoTFunction *pFunction)
{
    if (m_hFunctionsList.contains(pFunction->name())) {
        delete m_hFunctionsList.take(pFunction->name());
    }
    m_hFunctionsList.insert(pFunction->name(), pFunction);
}
