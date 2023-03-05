#include "xfs_iot_function.h"
#include "interface/xfs_iot_interface.h"
#include "service/abstract_service.h"

XFSIoTFunction::XFSIoTFunction(const QString &strName, XFSIoTInterface *parent) : m_strName(strName)
{
    QString l_strInvokName = QString("%1_%2").arg(parent->name(), strName);
    QByteArray l_baName = l_strInvokName.toLocal8Bit();
    m_pCharInvokeName = qstrdup(l_baName.constData());
    QByteArray l_baSignature = QString("%1(%2)").arg(l_strInvokName, "XFSIoTCommandEvent*").toLocal8Bit();
    m_pCharMethodSignature = qstrdup(l_baSignature.constData());
}

XFSIoTFunction::~XFSIoTFunction()
{
    if (m_pCharInvokeName != nullptr) {
        delete[] m_pCharInvokeName;
    }
    if (m_pCharMethodSignature != nullptr) {
        delete[] m_pCharMethodSignature;
    }
}

bool XFSIoTFunction::invoke(AbstractService *pService, XFSIoTCommandEvent *pEvent) const
{

    return QMetaObject::invokeMethod(pService, //
                                     m_pCharInvokeName, //
                                     Qt::DirectConnection, //
                                     Q_ARG(XFSIoTCommandEvent *, pEvent));
}

bool XFSIoTFunction::isSupport(AbstractService *pService) const
{
    if (pService->metaObject()->indexOfMethod(m_pCharMethodSignature) == -1) {
        return false;
    } else {
        return true;
    }
}
