#include "xfs_iot_standard.h"

void XFSIoTStandard::buildPayloadCompletion(QJsonObject &joPayload, const QString &strCode, const QString &strDesc)
{
    joPayload[JK_COMPLETION_CODE] = strCode;
    joPayload[JK_ERROR_DESCRIPTION] = strDesc;
}
