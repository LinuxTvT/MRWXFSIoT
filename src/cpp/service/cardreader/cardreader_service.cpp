#include "cardreader_service.h"
#include "qeventloop.h"

CardreaderService::CardreaderService(const QString &strName, //
                                     const QString &strFileConfig)
    : AbstractService{ strName, strFileConfig }
{
}

AbstractDeviceWorker *CardreaderService::loadDeviceWorker(const QJsonValue &joProperties)
{
    return nullptr;
}

void CardreaderService::Common_Status(XFSIoTCommandEvent *pEvent)
{
    error(QString("Unimplement command [%1]").arg(pEvent->commandName()));
}
