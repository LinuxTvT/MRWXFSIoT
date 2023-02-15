#include "printer_service_v3x.h"
#include "XFSAPI.H"
#include "XFSPTR.H"
#include "device/v3x/device_worker_v3x.h"
#include "qjsonvalue.h"
#include "service/printer/form/xfs_field.h"
#include "service/printer/form/xfs_form.h"
#include "service/printer/masung/masung_printer_worker.h"
#include "utils/xfs_utils.h"
#include "xfs_iot_standard.h"
#include <QJsonArray>
#include <QJsonObject>
#include <winnt.h>

PrinterServiceV3x::PrinterServiceV3x(const QString &strFileConfig) : AbstractPrinterService{ strFileConfig }
{
    m_pDeviceWorkerWrap = new DeviceWorkerV3x(QString{}, this);
    setWorker(m_pDeviceWorkerWrap);
}

PrinterServiceV3x::~PrinterServiceV3x() { }

void PrinterServiceV3x::dumpField(LPWFSFRMFIELD lpWfsFrmField, QJsonObject &jvFields)
{
    if (lpWfsFrmField != NULL) {
        QJsonObject l_joFieldInfo;
        l_joFieldInfo["type"] = FIELD_TYPE_HASH.value(lpWfsFrmField->fwType);
        l_joFieldInfo["indexCount"] = lpWfsFrmField->wIndexCount;
        l_joFieldInfo["class"] = FIELD_CLASS_HASH.value(lpWfsFrmField->fwClass);
        l_joFieldInfo["access"] = FIELD_ACCESS_HASH.value(lpWfsFrmField->fwAccess);
        l_joFieldInfo["overflow"] = FIELD_OVERFLOW_HASH.value(lpWfsFrmField->fwOverflow);
        l_joFieldInfo["initialValue"] = QString(lpWfsFrmField->lpszInitialValue);
        jvFields[lpWfsFrmField->lpszFieldName] = l_joFieldInfo;
    }
}

void PrinterServiceV3x::Update_Status_Printer(QJsonObject &joStatus)
{
    qDebug() << "joStatus = m_joPrinterStatus";
}

void PrinterServiceV3x::Printer_GetFormList(XFSIoTCommandEvent *pCommandEvent)
{
    DWORD l_dwTimeout = pCommandEvent->payLoad()["timeout"].toInt();
    LPWFSRESULT l_lpWfsResult;
    HRESULT l_hResult = m_pDeviceWorkerWrap->wfsGetInfo(WFS_INF_PTR_FORM_LIST, //
                                                        NULL, //
                                                        l_dwTimeout, //
                                                        &l_lpWfsResult);
    if (l_hResult == WFS_SUCCESS) {
        LPSTR l_lpszFormList = (LPSTR)l_lpWfsResult->lpBuffer;
        QJsonObject l_joPayload;
        QJsonArray l_jaForms;
        XfsUtils::parseLPSTR(l_lpszFormList, l_jaForms);
        l_joPayload["formList"] = l_jaForms;
        notifyCompletion(pCommandEvent, l_joPayload);
    } else {
        notifyCompletionError(pCommandEvent, //
                              XFSIoTStandard::JV_UNSUPPORTED_COMMAND, //
                              QString("Do command ERROR  [%1]").arg(pCommandEvent->commandName()));
    }
    WFSFreeResult(l_lpWfsResult);
}

void PrinterServiceV3x::Printer_GetQueryForm(XFSIoTCommandEvent *pCommandEvent)
{
    QString l_strFormName = pCommandEvent->payLoad()["formName"].toString();
    DWORD l_dwTimeout = pCommandEvent->payLoad()["timeout"].toInt();
    LPWFSRESULT l_lpWfsResult;
    HRESULT l_hResult = m_pDeviceWorkerWrap->wfsGetInfo(WFS_INF_PTR_QUERY_FORM, //
                                                        (LPVOID)l_strFormName.toUtf8().constData(), //
                                                        l_dwTimeout, //
                                                        &l_lpWfsResult);
    if (l_hResult == WFS_SUCCESS) {
        LPWFSFRMHEADER l_lpWfsForm = (LPWFSFRMHEADER)l_lpWfsResult->lpBuffer;
        QJsonObject l_joPayload;
        QJsonArray l_jaForms;
        XfsUtils::parseLPSTR(l_lpWfsForm->lpszFields, l_jaForms);
        l_joPayload["formList"] = l_jaForms;
        notifyCompletion(pCommandEvent, l_joPayload);
    } else {
        notifyCompletionError(pCommandEvent, //
                              XFSIoTStandard::JV_UNSUPPORTED_COMMAND, //
                              QString("Do command ERROR  [%1]").arg(pCommandEvent->commandName()));
    }
    WFSFreeResult(l_lpWfsResult);
}

void PrinterServiceV3x::Printer_GetQueryField(XFSIoTCommandEvent *pCommandEvent)
{
    WFSPTRQUERYFIELD l_wfsPtrQueryField;
    QString l_strFormName = pCommandEvent->payLoad()["formName"].toString();
    QJsonValue l_jvFieldName = pCommandEvent->payLoad()["fieldName"];

    l_wfsPtrQueryField.lpszFormName = SelfServiceObject::stringDup(l_strFormName);
    if (l_jvFieldName.isUndefined()) {
        l_wfsPtrQueryField.lpszFieldName = NULL;
    } else {
        l_wfsPtrQueryField.lpszFieldName = SelfServiceObject::stringDup(l_jvFieldName.toString());
    }

    QJsonObject l_joPayload;
    QJsonObject l_joFields;
    DWORD l_dwTimeout = pCommandEvent->payLoad()["timeout"].toInt();
    LPWFSRESULT l_lpWfsResult;
    HRESULT l_hResult = m_pDeviceWorkerWrap->wfsGetInfo(WFS_INF_PTR_QUERY_FIELD, //
                                                        (LPVOID)&l_wfsPtrQueryField, //
                                                        l_dwTimeout, //
                                                        &l_lpWfsResult);
    if (l_hResult == WFS_SUCCESS) {
        LPWFSFRMFIELD *l_ppFields = (LPWFSFRMFIELD *)l_lpWfsResult->lpBuffer;
        while ((*l_ppFields) != NULL) {
            dumpField(*l_ppFields, l_joFields);
            l_ppFields++;
        }
        l_joPayload["fields"] = l_joFields;
        notifyCompletion(pCommandEvent, l_joPayload);
    } else {
        notifyCompletionError(pCommandEvent, //
                              XFSIoTStandard::JV_UNSUPPORTED_COMMAND, //
                              QString("Do command ERROR  [%1]").arg(pCommandEvent->commandName()));
    }
    WFSFreeResult(l_lpWfsResult);
}

void PrinterServiceV3x::Printer_PrintForm(XFSIoTCommandEvent *pEvent)
{
    //    QString l_strFormName = pEvent->payLoad()["formName"].toString();

    //    if (m_pXFSFormRepository->form(l_strFormName) == nullptr) {
    //        notifyCompletionError(pEvent, "commandErrorCode", QString("Form [%1] can't found").arg(l_strFormName));
    //    } else {
    //        this->commandQueue().pushCommand(new XFSIoTCommandEvent(pEvent));
    //        log(QString("Print From [%1] is queued to device worker").arg(l_strFormName));
    //    }
}
