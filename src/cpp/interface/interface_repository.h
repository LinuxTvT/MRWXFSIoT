#ifndef INTERFACE_REPOSITORY_H
#define INTERFACE_REPOSITORY_H

#include "interface/xfs_iot_interface.h"
#include "selfservice_object.h"
#include <QJsonArray>
#include <QObject>

class InterfaceRepository : public SelfServiceObject
{
    Q_OBJECT
public:
    explicit InterfaceRepository(QObject *parent = nullptr);
    virtual ~InterfaceRepository();
    virtual bool load(const QJsonArray &jaInterfaces);
    virtual XFSIoTInterface *getInterface(const QString &strName) const;

private:
    QHash<QString, XFSIoTInterface *> m_hInterfacesList;
    void addInterface(XFSIoTInterface *pInterface);
};

#endif // INTERFACE_REPOSITORY_H
