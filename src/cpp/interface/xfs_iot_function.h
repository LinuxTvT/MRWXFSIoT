#ifndef XFSIOTFUNCTION_H
#define XFSIOTFUNCTION_H

#include "event/xfs_iot_msg_event.h"
#include <QObject>

class AbstractService;
class XFSIoTInterface;

class XFSIoTFunction
{

public:
    explicit XFSIoTFunction(const QString &strName, XFSIoTInterface *parent);
    virtual ~XFSIoTFunction();

    bool invoke(AbstractService *pService, XFSIoTCommandEvent *pEvent) const;
    bool isSupport(AbstractService *pService) const;

    inline const QString &name() const { return m_strName; }

    // const XFSIoTInterface *xfsInterface() const;

private:
    char *m_pCharInvokeName = nullptr;
    char *m_pCharMethodSignature = nullptr;
    QString m_strName;
    // XFSIoTInterface *m_pInterface = nullptr;
};

#endif // XFSIOTFUNCTION_H
